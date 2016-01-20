/* vim: set expandtab ts=4 sw=4: */
/*
 * You may redistribute this program and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
var Cjdns = require('./cjdnsadmin/cjdnsadmin');
var Benc = require('./cjdnsadmin/bencode');
var nThen = require('nthen');
var Fs = require('fs');

var DEFAULT_BCONF = '/etc/cjdroute.bconf';

var usage = function () {
    console.log("Usage: cjdnsctl [ OPTIONS ] OBJECT { COMMAND | help }");
    console.log("where  OBJECT := { iptun | conf }");
    console.log("       OPTIONS := { -f { cjdns-binary-config-file }");
};

var confHelp = function () {
    console.log("Usage: cjdnsctl conf gen [ B_LOCATION ] [ yes ]");
    console.log("       cjdnsctl conf convert [ B_LOCATION ] [ INPUT ] [ yes ]");
    console.log("       cjdnsctl conf json [ B_LOCATION ]");
    console.log("where  B+LOCATION := binary file path, default: " + DEFAULT_BCONF);
    console.log("       INPUT := a legacy format cjdroute.conf file");
};

var iptunHelp = function () {
    console.log("Usage: cjdnsctl iptun add KEY APA [ APA ] [ comment COMMENT ]");
    console.log("       cjdnsctl iptun rm KEY");
    console.log("       cjdnsctl iptun ls");
    console.log("       cjdnsctl iptun help");
    console.log("where  KEY := base-32 encoded string ending in .k");
    console.log("       APA := ADDRESS[ [ /PREFIX ] /ALLOCATION ]");
    console.log("       PREFIX := 0-32 for IPv4, 0-128 for IPv6 (announced to client)");
    console.log("       ALLOCATION := 0-32 for IPv4, 0-129 for IPv6 (issued to client)");
    console.log("       COMMENT := any string");
};

var syncConf = function (ctx) {
    var bstr = Benc.encode(ctx.conf);
    console.log('>' + ctx.confFile);
    Fs.writeFile(ctx.confFile, bstr, function (err, ret) {
        if (err) {
            console.log("Failed to sync changes to cjdns configuration db [" + ctx.confFile + "]");
            throw err;
        }
    });
};

var assertNum = function (str, lowerBound, upperBound)
{
    var num = Number(str);
    if (num <= upperBound && num >= lowerBound) { return num; }
    throw new Error("[" + str + "] must be a number between [" + lowerBound + "] and [" +
        upperBound + "]");
};

var iptunAdd = function (ctx, argIndex, cb) {
    var ip6;
    var prefix6;
    var alloc6;
    var ip4;
    var prefix4;
    var alloc4;
    var key;
    var comment;

    if ((/[a-z0-9]+\.k/).test(process.argv[argIndex+1])) {
        key = process.argv[argIndex+1];
    } else {
        console.log("ERROR: `cjdnsctl iptun add` requires the public key of the connecting node");
        return cb();
    }

    for (var i = argIndex+2; i < process.argv.length; i++) {
        if ((/[0-9a-fA-F]+:/).test(process.argv[i])) {
            var arr = process.argv[i].split('/');
            ip6 = arr[0];
            prefix6 = arr[1] && assertNum(arr[1], 0, 128);
            alloc6 = arr[2] && assertNum(arr[2], 0, 128);
        }
        if ((/[0-9]+\.[0-9]+\.[0-9]+\.[0-9]+/).test(process.argv[i])) {
            var arr = process.argv[i].split('/');
            ip4 = arr[0];
            prefix4 = arr[1] && assertNum(arr[1], 0, 32);
            alloc4 = arr[2] && assertNum(arr[2], 0, 32);
        }
        if (process.argv[i] === 'comment') {
            if (!process.argv[i+1]) {
                console.log("ERROR: `comment` must be followed by a string");
                return cb();
            }
            comment = process.argv[i+1];
        }
    }

    if (!ip4 && !ip6) {
        console.log("ERROR: `cjdnsctl iptun add` requires an ipv4 and/or ipv6 address to issue");
        return cb();
    }

    for (var i = 0; i < ctx.conf.router.ipTunnel.allowedConnections.length; i++) {
        if (ctx.conf.router.ipTunnel.allowedConnections[i].publicKey === key) {
            console.log("ERROR: public key [" + key  + "] is already authorized");
            return cb();
        }
    }

    nThen(function (waitFor) {
        ctx.cjdns.IpTunnel_allowConnection(key, prefix6, alloc6, ip6, prefix4, alloc4, ip4,
            waitFor(function (err, ret) {
                if (err) { throw err; }
                if (ret.error && ret.error !== 'none') {
                    throw new Error("Response from cjdns: " + ret.error);
                }
            }));
    }).nThen(function (waitFor) {
        var conn = {
            publicKey: key
        };
        if (ip4 !== undefined) { conn.ip4Address = ip4; }
        if (prefix4 !== undefined) { conn.ip4Prefix = prefix4; }
        if (alloc4 !== undefined) { conn.ip4Alloc = alloc4; }

        if (ip6 !== undefined) { conn.ip6Address = ip6; }
        if (prefix6 !== undefined) { conn.ip6Prefix = prefix6; }
        if (alloc6 !== undefined) { conn.ip6Alloc = alloc6; }

        if (comment !== undefined) { conn._comment = comment; }

        ctx.conf.router.ipTunnel.allowedConnections.push(conn);
        syncConf(ctx);
        cb();
    });
};

var iptunRm = function (ctx, argIndex, cb) {
    if (argIndex+2 !== process.argv.length) {
        console.log("ERROR: `cjdnsctl iptun rm` takes one argument");
        iptunHelp();
        return cb();
    }
    var publicKey = process.argv[argIndex+1];
    var index = -1;
    var entry;
    for (var i = 0; i < ctx.conf.router.ipTunnel.allowedConnections.length; i++) {
        if (ctx.conf.router.ipTunnel.allowedConnections[i].publicKey === publicKey) {
            index = i;
            entry = ctx.conf.router.ipTunnel.allowedConnections[i];
            break;
        }
    }
    if (index === -1) {
        console.log("ERROR: no connection found in conf matching key [" + publicKey + "]");
        return cb();
    }
    ctx.conf.router.ipTunnel.allowedConnections.splice(index, 1);

    var connTable;
    nThen(function (waitFor) {
        ctx.cjdns.IpTunnel_listConnections(waitFor(function (err, ret) {
            if (err) { throw err; }
            if (ret.error && ret.error !== 'none') {
                throw new Error("Response from cjdns: " + ret.error);
            }
            connTable = ret.connections;
        }));
    }).nThen(function (waitFor) {
        var nt = nThen;
        for (var i = 0; i < connTable.length; i++) {
            (function(i) {
                nt = nt(function (waitFor) {
                    ctx.cjdns.IpTunnel_showConnection(i, waitFor(function (err, ret) {
                        if (err) { throw err; }
                        if (ret.error && ret.error !== 'none') {
                            throw new Error("Response from cjdns: " + ret.error);
                        }
                        ret.__connNum = connTable[i];
                        connTable[i] = ret;
                    }));
                }).nThen;
            })(i);
        }
        nt(waitFor());
    }).nThen(function (waitFor) {
        var found = false;
        for (var i = 0; i < connTable.length; i++) {
            if (Number(connTable[i].outgoing)) {
                // fallthrough
            } else if (connTable[i].key === publicKey) {
                found = true;
                ctx.cjdns.IpTunnel_removeConnection(Number(connTable[i].__connNum),
                    waitFor(function (err, ret) {
                        if (err) { throw err; }
                        if (ret.error && ret.error !== 'none') {
                            throw new Error("Response from cjdns: " + ret.error);
                        }
                        cb();
                    }));
            }
        }
        if (!found) {
            throw new Error("no such connection inside of running cjdns");
        }
    }).nThen(function (waitFor) {
        syncConf(ctx);
        cb();
    });
};

var iptunLs = function (ctx, argIndex, cb) {
    if (argIndex+1 !== process.argv.length) {
        console.log("ERROR: `cjdnsctl iptun ls` takes no arguments");
        iptunHelp();
        return cb();
    }
    console.log(JSON.stringify(ctx.conf.router.ipTunnel.allowedConnections, null, '  '));
    cb();
};

var iptun = function (ctx, i, cb) {
    switch (process.argv[i+1]) {
        case 'add': return iptunAdd(ctx, i+1, cb);
        case 'rm': return iptunRm(ctx, i+1, cb);
        case 'ls': return iptunLs(ctx, i+1, cb);
        case undefined:
        case 'help': iptunHelp(); cb(); return;
        default: {
            console.log("Unrecognized argument: " + process.argv[i+!]);
            iptunHelp();
            cb();
        }
    }
};

var confGen = function (i) {

};

var confConvert = function (i) {
};

var confJson = function (i) {
};

var conf = function (i) {
    switch (process.argv[i+1]) {
        case 'gen': return confGen(i+1);
        case 'convert': return confConvert(i+1);
        case 'json': return confJson(i+1);
        case undefined:
        case 'help': confHelp(); return;
        default: {
            console.log("Unrecognized argument: " + process.argv[i+1]);
            confHelp();
        }
    }
};

var funMap = {
    'iptun': iptun,
    'conf': conf
};

var main = module.exports.main = function () {
    var confFile = DEFAULT_BCONF;
    var func;
    var i = 0;
    for (; i < process.argv.length; i++) {
        if (funMap[process.argv[i]]) {
            func = funMap[process.argv[i]];
            break;
        }
        if (process.argv[i] === '-f') {
            confFile = process.argv[++i];
        }
    }
    if (func === conf) {
        conf(i);
        return;
    }
    if (confFile && func) {
        var out = {
            confFile: confFile
        };
        nThen(function (waitFor) {
            Fs.readFile(confFile, waitFor(function (err, ret) {
                if (err) {
                    console.log("Need access to the cjdns configuration db [" + confFile + "]");
                    throw err;
                }
                var str = ret.toString('utf8');
                out.conf = Benc.decode(str);
                if (!out.conf) { throw new Error("Failed to parse [" + confFile + "]"); }
                if (Benc.encode(out.conf) !== str) { throw new Error("bencode roundtrip failed"); }
            }));
        }).nThen(function (waitFor) {
            var addrPort = out.conf.admin.bind;
            var lColon = addrPort.lastIndexOf(':');
            var addr = addrPort.substring(0, lColon);
            var port = Number(addrPort.substring(lColon+1));
            Cjdns.connect(addr, port, out.conf.admin.password, waitFor(function (cjdns) {
                out.cjdns = cjdns;
            }));
        }).nThen(function (waitFor) {
            func(out, i, waitFor());
        }).nThen(function (waitFor) {
            out.cjdns.disconnect();
        });
    } else {
        return usage();
    }
};
