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
var Spawn = require('child_process').spawn;

var DEFAULT_BCONF = '/etc/cjdroute.bconf';

var usage = function () {
    console.log("Usage: cjdnsctl [ OPTIONS ] OBJECT { COMMAND | help }");
    console.log("where  OBJECT := { iptun | conf }");
    console.log("       OPTIONS := { -f { cjdns-binary-config-file }");
};

var confHelp = function () {
    console.log("Usage: cjdnsctl conf gen [ B_LOCATION ] [ yes ]");
    console.log("       cjdnsctl conf convert INPUT [ B_LOCATION ] [ yes ]");
    console.log("       cjdnsctl conf json [ B_LOCATION ]");
    console.log("where  B_LOCATION := binary file path, default: " + DEFAULT_BCONF);
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
            console.log("Unrecognized argument: " + process.argv[i+1]);
            iptunHelp();
            cb();
        }
    }
};

var runCjdroute = function (args, stdin, cb) {
    var cjdroute = 'cjdroute';
    nThen(function (waitFor) {
        Fs.exists(process.cwd() + '/cjdroute', waitFor(function (ex) {
            if (ex) { cjdroute = process.cwd() + '/cjdroute'; }
        }));
    }).nThen(function (waitFor) {
        var proc = Spawn(cjdroute, args);
        var err = '';
        var out = '';
        proc.stderr.on('data', function (dat) { err += dat.toString(); });
        proc.stdout.on('data', function (dat) { out += dat.toString(); });
        proc.on('close', waitFor(function (ret) {
            cb(ret, out, err);
        }));
        proc.on('error', function (err) { throw err; });
        if (stdin) {
            proc.stdin.write(stdin, function (err) {
                if (err) { throw err; }
                proc.stdin.end();
            });
        }
    });
};

var parseConf = function (conf) {
    var out = [];
    for (;;) {
        var idx = conf.match(/\/\/|\/\*|"/);
        if (!idx) {
            out.push(conf);
            return JSON.parse(out.join(''));
        }
        out.push(conf.substring(0,idx.index));
        switch (idx[0]) {
            case '"': {
                var ni = conf.indexOf('"', idx.index+1)+1;
                out.push(conf.substring(idx.index, ni));
                conf = conf.substring(ni);
                break;
            }
            case '//': {
                out.push('\n');
                var ni = conf.indexOf('\n', idx.index+1)+2;
                conf = conf.substring(ni);
                break;
            }
            case '/*': {
                out.push('\n');
                var ni = conf.indexOf('*/', idx.index+1)+2;
                conf = conf.substring(ni);
                break;
            }
            default: throw new Error();
        }
    }
};

var lockConfFile = function (confFile, cb) {
    var uid;
    var mode;
    nThen(function (waitFor) {
        Fs.stat(confFile, waitFor(function (err, stat) {
            if (err) { throw err; }
            mode = stat.mode;
            uid = stat.uid;
        }));
    }).nThen(function (waitFor) {
        if (uid === 0) { return; }
        Fs.chown(confFile, process.getuid(), process.getgid(), waitFor(function (err) {
            if (err) { throw err; }
        }));
    }).nThen(function (waitFor) {
        if (mode.toString(8).slice(-3) === '600') { return; }
        Fs.chmod(confFile, parseInt('600',8), waitFor(function (err) {
            if (err) { throw err; }
        }));
    }).nThen(cb);
};

var confGen = function (i) {
    var confFile = DEFAULT_BCONF;
    var force = false;
    var conf;
    if (process.argv.length > i+3) {
        console.log("ERROR: `cjdnsctl conf gen` requires maximum 2 arguments");
        return;
    }
    if (process.argv[i+2] === 'yes') { force = true; }
    if (process.argv.length > i+1) {
        if (force || process.argv[i+1] !== 'yes') {
            confFile = process.argv[i+1];
        } else {
            force = true;
        }
    }

    nThen(function (waitFor) {
        Fs.exists(confFile, waitFor(function (ex) {
            if (ex && !force) {
                console.log("ERROR: `cjdnsctl conf gen` output file [" + confFile + "] exists.");
                var cf = (confFile === DEFAULT_BCONF) ? "" : (confFile + " ");
                console.log("       use `cjdnsctl conf gen " + cf + "yes` to overwrite.");
                confFile = null;
            }
        }));
    }).nThen(function (waitFor) {
        if (!confFile) { return; }
        runCjdroute(['--genconf'], undefined, waitFor(function (ret, out, err) {
            conf = parseConf(out);
            // Don't need default auth'd passwords.
            conf.authorizedPasswords = [];
        }));
    }).nThen(function (waitFor) {
        if (!confFile) { return; }
        var bstr = Benc.encode(conf)
        if (bstr !== Benc.encode(Benc.decode(bstr))) { throw new Error(); }
        Fs.writeFile(confFile, bstr, waitFor(function (err, ret) {
            if (err) { throw err; }
        }));
    }).nThen(function (waitFor) {
        if (!confFile) { return; }
        lockConfFile(confFile, waitFor());
    });
};

var confConvert = function (i) {
    var confFile = DEFAULT_BCONF;
    var force = false;
    var conf;
    if (process.argv.length > i+4 || process.argv.length <= i+1) {
        console.log("ERROR: `cjdnsctl conf convert` requires between 1 and 3 arguments");
        return;
    }

    var inputFile = process.argv[i+1];
    if (process.argv.length > i+2 && process.argv[process.argv.length-1] === 'yes') {
        force = true;
        process.argv.pop();
    }
    if (process.argv.length > i+2) {
        confFile = process.argv.pop();
    }

    var inputContent;
    nThen(function (waitFor) {
        Fs.exists(confFile, waitFor(function (ex) {
            if (ex && !force) {
                console.log("ERROR: `cjdnsctl conf convert` output file [" + confFile + "] exists");
                var cf = (confFile === DEFAULT_BCONF) ? "" : (confFile + " ");
                console.log("       use `cjdnsctl conf convert " + cf + "yes` to overwrite.");
                confFile = null;
            }
        }));
    }).nThen(function (waitFor) {
        Fs.readFile(inputFile, waitFor(function (err, ret) {
            if (err) { throw err; }
            inputContent = ret.toString('utf8');
        }));
    }).nThen(function (waitFor) {
        if (!confFile) { return; }
        runCjdroute(['--cleanconf'], inputContent, waitFor(function (ret, out, err) {
            conf = parseConf(out);
        }));
    }).nThen(function (waitFor) {
        if (!confFile) { return; }
        var bstr = Benc.encode(conf)
        if (bstr !== Benc.encode(Benc.decode(bstr))) { throw new Error(); }
        Fs.writeFile(confFile, bstr, waitFor(function (err, ret) {
            if (err) { throw err; }
        }));
    }).nThen(function (waitFor) {
        if (!confFile) { return; }
        lockConfFile(confFile, waitFor());
    });
};

var confJson = function (i) {
    var confFile = DEFAULT_BCONF;
    if (process.argv.length > i+2) {
        console.log("ERROR: `cjdnsctl conf json` takes at most, 1 argument");
        return;
    }
    if (process.argv.length > i+1) {
        confFile = process.argv[i+1];
    }
    Fs.readFile(confFile, function (err, ret) {
        if (err) { throw err; }
        var conf = Benc.decode(ret.toString('utf8'));
        if (!conf) {
            console.log("ERROR: failed to parse bconf file [" + confFile + "]");
        }
        process.stdout.write(JSON.stringify(conf, null, '  '));
    });
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
