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
(function () { 'use strict'
let Cjdns = require('./cjdnsadmin/cjdnsadmin');
let Benc = require('./cjdnsadmin/bencode');
let nThen = require('nthen');
let Fs = require('fs');
let Spawn = require('child_process').spawn;

let DEFAULT_BCONF = '/etc/cjdroute.bconf';

let usage = function () {
    console.log("Usage: cjdnsctl [ OPTIONS ] OBJECT { COMMAND | help }");
    console.log("where  OBJECT := { iptun | conf | peers }");
    console.log("       OPTIONS := { -f { cjdns-binary-config-file }");
};

let confHelp = function () {
    console.log("Usage: cjdnsctl conf gen [ B_LOCATION ] [ yes ]");
    console.log("       cjdnsctl conf convert INPUT [ B_LOCATION ] [ yes ]");
    console.log("       cjdnsctl conf json [ B_LOCATION ]");
    console.log("       cjdnsctl conf get PATH");
    console.log("       cjdnsctl conf set PATH VALUE");
    console.log("where  B_LOCATION := binary file path, default: " + DEFAULT_BCONF);
    console.log("       INPUT := a legacy format cjdroute.conf file");
    console.log("       PATH := a json path for example authorizedPasswords[0].password");
    console.log("       VALUE := any valid json");
};

let iptunHelp = function () {
    console.log("Usage: cjdnsctl iptun add KEY APA [ APA ] [ comment COMMENT ]");
    console.log("       cjdnsctl iptun rm KEY");
    console.log("       cjdnsctl iptun conn KEY");
    console.log("       cjdnsctl iptun disconn KEY");
    console.log("       cjdnsctl iptun ls");
    console.log("where  KEY := base-32 encoded string ending in .k");
    console.log("       APA := ADDRESS[ [ /PREFIX ] /ALLOCATION ]");
    console.log("       PREFIX := 0-32 for IPv4, 0-128 for IPv6 (announced to client)");
    console.log("       ALLOCATION := 0-32 for IPv4, 0-128 for IPv6 (issued to client)");
    console.log("       COMMENT := a string, never sent, shown in `cjdnsctl iptun ls`");
};

let peersHelp = function () {
    console.log("Usage: cjdnsctl peers conn ADDR KEY PASSWD [ user LOGIN ] [ dev DEV ] " +
        "[ comment COMMENT ]");
    console.log("       cjdnsctl peers disconn KEY");
    console.log("       cjdnsctl peers auth PASSWD LOGIN [ comment COMMENT ]");
    console.log("       cjdnsctl peers deauth LOGIN");
    console.log("       cjdnsctl peers ls");
    console.log("       cjdnsctl peers lsdev");
    console.log("       cjdnsctl peers lsauth");
    console.log("where  KEY := base-32 encoded string ending in .k");
    console.log("       ADDR := a string representation of a Sockaddr for connecting to the peer");
    console.log("       PASSWD := a string");
    console.log("       LOGIN := a string");
    console.log("       COMMENT := a string, never sent, shown in `cjdnsctl peers ls`");
}

let syncConf = function (ctx) {
    let bstr = Benc.encode(ctx.conf);
    //console.log('>' + ctx.confFile);
    Fs.writeFile(ctx.confFile, bstr, function (err, ret) {
        if (err) {
            console.error("Failed to sync changes to cjdns config file [" + ctx.confFile + "]");
            throw err;
        }
    });
};

let assertNum = function (str, lowerBound, upperBound)
{
    let num = Number(str);
    if (num <= upperBound && num >= lowerBound) { return num; }
    throw new Error("[" + str + "] must be a number between [" + lowerBound + "] and [" +
        upperBound + "]");
};

let iptunAdd = function (ctx, argIndex, cb) {
    let ip6;
    let prefix6;
    let alloc6;
    let ip4;
    let prefix4;
    let alloc4;
    let key;
    let comment;

    if ((/[a-z0-9]+\.k/).test(process.argv[argIndex+1])) {
        key = process.argv[argIndex+1];
    } else {
        console.error("ERROR: `cjdnsctl iptun add` requires the public key of the connecting node");
        return cb();
    }

    for (let i = argIndex+2; i < process.argv.length; i++) {
        if ((/[0-9a-fA-F]+:/).test(process.argv[i])) {
            let arr = process.argv[i].split('/');
            ip6 = arr[0];
            prefix6 = arr[1] && assertNum(arr[1], 0, 128);
            alloc6 = arr[2] && assertNum(arr[2], 0, 128);
        }
        if ((/[0-9]+\.[0-9]+\.[0-9]+\.[0-9]+/).test(process.argv[i])) {
            let arr = process.argv[i].split('/');
            ip4 = arr[0];
            prefix4 = arr[1] && assertNum(arr[1], 0, 32);
            alloc4 = arr[2] && assertNum(arr[2], 0, 32);
        }
        if (process.argv[i] === 'comment') {
            if (!process.argv[i+1]) {
                console.error("ERROR: `comment` must be followed by a string");
                return cb();
            }
            comment = process.argv[i+1];
        }
    }

    if (!ip4 && !ip6) {
        console.error("ERROR: `cjdnsctl iptun add` requires an ipv4 and/or ipv6 address to issue");
        return cb();
    }

    for (let i = 0; i < ctx.conf.router.ipTunnel.allowedConnections.length; i++) {
        if (ctx.conf.router.ipTunnel.allowedConnections[i].publicKey === key) {
            console.error("ERROR: public key [" + key  + "] is already authorized");
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
        let conn = {
            publicKey: key
        };
        if (ip4 !== undefined) { conn.ip4Address = ip4; }
        if (prefix4 !== undefined) { conn.ip4Prefix = prefix4; }
        if (alloc4 !== undefined) { conn.ip4Alloc = alloc4; }

        if (ip6 !== undefined) { conn.ip6Address = ip6; }
        if (prefix6 !== undefined) { conn.ip6Prefix = prefix6; }
        if (alloc6 !== undefined) { conn.ip6Alloc = alloc6; }

        if (comment !== undefined) { conn.comment = comment; }

        ctx.conf.router.ipTunnel.allowedConnections.push(conn);
        syncConf(ctx);
        cb();
    });
};

let getIptunConnections = function (ctx, cb) {
    let connTable;
    nThen(function (waitFor) {
        ctx.cjdns.IpTunnel_listConnections(waitFor(function (err, ret) {
            if (err) { throw err; }
            if (ret.error && ret.error !== 'none') {
                throw new Error("Response from cjdns: " + ret.error);
            }
            connTable = ret.connections;
        }));
    }).nThen(function (waitFor) {
        let nt = nThen;
        for (let i = 0; i < connTable.length; i++) {
            (function(i) {
                nt = nt(function (waitFor) {
                    ctx.cjdns.IpTunnel_showConnection(i, waitFor(function (err, ret) {
                        if (err) { throw err; }
                        if (ret.error && ret.error !== 'none') {
                            throw new Error("Response from cjdns: " + ret.error);
                        }
                        ret._connNum = connTable[i];
                        connTable[i] = ret;
                    }));
                }).nThen;
            })(i);
        }
        nt(waitFor());
    }).nThen(function (waitFor) {
        cb(connTable);
    });
};

let iptunRm = function (ctx, argIndex, cb) {
    if (argIndex+2 !== process.argv.length) {
        console.error("ERROR: `cjdnsctl iptun rm` takes one argument");
        iptunHelp();
        return cb();
    }
    let publicKey = process.argv[argIndex+1];
    let index = -1;
    let entry;
    for (let i = 0; i < ctx.conf.router.ipTunnel.allowedConnections.length; i++) {
        if (ctx.conf.router.ipTunnel.allowedConnections[i].publicKey === publicKey) {
            index = i;
            entry = ctx.conf.router.ipTunnel.allowedConnections[i];
            break;
        }
    }
    if (index === -1) {
        console.error("ERROR: no connection found in conf matching key [" + publicKey + "]");
        return cb();
    }
    ctx.conf.router.ipTunnel.allowedConnections.splice(index, 1);

    let connTable;
    nThen(function (waitFor) {
        getIptunConnections(waitFor(function (ct) { connTable = ct; }));
    }).nThen(function (waitFor) {
        let found = false;
        for (let i = 0; i < connTable.length; i++) {
            if (Number(connTable[i].outgoing)) {
                // fallthrough
            } else if (connTable[i].key === publicKey) {
                found = true;
                ctx.cjdns.IpTunnel_removeConnection(Number(connTable[i]._connNum),
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

let iptunLs = function (ctx, argIndex, cb) {
    if (argIndex+1 !== process.argv.length) {
        console.error("ERROR: `cjdnsctl iptun ls` takes no arguments");
        iptunHelp();
        return cb();
    }
    console.log(JSON.stringify(ctx.conf.router.ipTunnel.allowedConnections, null, '  '));
    cb();
};

let iptunConnDisconn = function (ctx, i, cb) {
    if (i+2 !== process.argv.length) {
        console.error("ERROR: `cjdnsctl iptun " + process.argv[i] + "` takes exactly one argument");
        return cb();
    }
    if (process.argv[i] === 'conn') {
        if (ctx.conf.router.ipTunnel.outgoingConnections.length > 0) {
            console.error("ERROR: already connected to an IPTunnel (multiple is not well tested)");
            return cb();
        }
        ctx.conf.router.ipTunnel.outgoingConnections.push(process.argv[i+1]);
        ctx.cjdns.IpTunnel_connectTo(process.argv[i+1], function (err, ret) {
            if (err) { throw err; }
            if (ret.error !== 'none') { throw new Error("Response from cjdns: " + ret.error); }
            syncConf(ctx);
            cb();
        });
    } else if (process.argv[i] !== 'disconn') {
        throw new Error();
    }
    if (ctx.conf.router.ipTunnel.outgoingConnections.length === 0) {
        console.error("ERROR: not connected to any IPTunnel");
        return cb();
    }
    let idx = ctx.conf.router.ipTunnel.outgoingConnections.indexOf(process.argv[i+1]);
    if (idx === -1) {
        console.error("ERROR: no connection for given key");
        return cb();
    }
    ctx.conf.router.ipTunnel.outgoingConnections.splice(idx, 1);

    let connTable;
    nThen(function (waitFor) {
        getIptunConnections(waitFor(function (ct) { connTable = ct; }));
    }).nThen(function (waitFor) {
        let found = false;
        for (let i = 0; i < connTable.length; i++) {
            if (!connTable[i].outgoing) {
                // fallthrough
            } else if (connTable[i].key === process.argv[i+1]) {
                found = true;
                ctx.cjdns.IpTunnel_removeConnection(connTable[i]._connNum,
                    waitFor(function (err, ret) {
                        if (err) { throw err; }
                        if (ret.error && ret.error !== 'none') {
                            throw new Error("Response from cjdns: " + ret.error);
                        }
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

let iptun = function (ctx, i, cb) {
    switch (process.argv[i+1]) {
        case 'add': return iptunAdd(ctx, i+1, cb);
        case 'rm': return iptunRm(ctx, i+1, cb);
        case 'ls': return iptunLs(ctx, i+1, cb);
        case 'conn':
        case 'disconn': return iptunConnDisconn(ctx, i+1, cb);
        case undefined:
        case 'help': iptunHelp(); cb(); return;
        default: {
            console.error("Unrecognized argument: " + process.argv[i+1]);
            iptunHelp();
            cb();
        }
    }
};

let runCjdroute = function (args, stdin, cb) {
    let cjdroute = 'cjdroute';
    nThen(function (waitFor) {
        Fs.exists(process.cwd() + '/cjdroute', waitFor(function (ex) {
            if (ex) { cjdroute = process.cwd() + '/cjdroute'; }
        }));
    }).nThen(function (waitFor) {
        let proc = Spawn(cjdroute, args);
        let err = '';
        let out = '';
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

let lockConfFile = function (confFile, cb) {
    let uid;
    let mode;
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

let confGen = function (i) {
    let confFile = DEFAULT_BCONF;
    let force = false;
    let conf;
    let confStr;
    if (process.argv.length > i+3) {
        console.error("ERROR: `cjdnsctl conf gen` requires maximum 2 arguments");
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
                console.error("ERROR: `cjdnsctl conf gen` output file [" + confFile + "] exists.");
                let cf = (confFile === DEFAULT_BCONF) ? "" : (confFile + " ");
                console.error("       use `cjdnsctl conf gen " + cf + "yes` to overwrite.");
                confFile = null;
            }
        }));
    }).nThen(function (waitFor) {
        if (!confFile) { return; }
        runCjdroute(['--genconf'], undefined, waitFor(function (ret, out, err) {
            confStr = out;
        }));
    }).nThen(function (waitFor) {
        if (!confFile) { return; }
        runCjdroute(['--cleanconf'], confStr, waitFor(function (ret, out, err) {
            conf = JSON.parse(out);
            // Don't need default auth'd passwords.
            conf.authorizedPasswords = [];
        }));
    }).nThen(function (waitFor) {
        if (!confFile) { return; }
        let bstr = Benc.encode(conf)
        if (bstr !== Benc.encode(Benc.decode(bstr))) { throw new Error(); }
        Fs.writeFile(confFile, bstr, waitFor(function (err, ret) {
            if (err) { throw err; }
        }));
    }).nThen(function (waitFor) {
        if (!confFile) { return; }
        lockConfFile(confFile, waitFor());
    });
};

let fixConf = function (conf) {
    conf.authorizedPasswords = conf.authorizedPasswords.map(function (pwd, i) {
        let out = JSON.parse(JSON.stringify(pwd));
        if (!('user' in out)) {
            // This is synchronized with Configurator.c
            out.user = '_noname_' + i;
        }
        return out;
    });

    Object.keys(conf.interfaces).forEach(function (iface) {
        if (!Array.isArray(conf.interfaces[iface])) {
            conf.interfaces[iface] = [ conf.interfaces[iface] ];
        }
        conf.interfaces[iface].forEach(function (ifaceB) {
            if (!('bind' in ifaceB)) {
                if (iface === 'ETHInterface') {
                    ifaceB.bind = 'all';
                } else {
                    ifaceB.bind = '0.0.0.0/' + Math.floor(Math.random() * (65535 - 1025)) + 1025;
                }
            }
        });
    });
};

let confConvert = function (i) {
    let confFile = DEFAULT_BCONF;
    let force = false;
    let conf;
    if (process.argv.length > i+4 || process.argv.length <= i+1) {
        console.error("ERROR: `cjdnsctl conf convert` requires between 1 and 3 arguments");
        return;
    }

    let inputFile = process.argv[i+1];
    if (process.argv.length > i+2 && process.argv[process.argv.length-1] === 'yes') {
        force = true;
        process.argv.pop();
    }
    if (process.argv.length > i+2) {
        confFile = process.argv.pop();
    }

    let inputContent;
    nThen(function (waitFor) {
        Fs.exists(confFile, waitFor(function (ex) {
            if (ex && !force) {
                console.error("ERROR: `cjdnsctl conf convert` output file [" + confFile + "] exists");
                let cf = (confFile === DEFAULT_BCONF) ? "" : (confFile + " ");
                console.error("       use `cjdnsctl conf convert " + cf + "yes` to overwrite.");
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
            conf = JSON.parse(out);
        }));
    }).nThen(function (waitFor) {
        if (!confFile) { return; }
        fixConf(conf);
        let bstr = Benc.encode(conf)
        if (bstr !== Benc.encode(Benc.decode(bstr))) { throw new Error(); }
        Fs.writeFile(confFile, bstr, waitFor(function (err, ret) {
            if (err) { throw err; }
        }));
    }).nThen(function (waitFor) {
        if (!confFile) { return; }
        lockConfFile(confFile, waitFor());
    });
};

let confJson = function (i) {
    let confFile = DEFAULT_BCONF;
    if (process.argv.length > i+2) {
        console.error("ERROR: `cjdnsctl conf json` takes at most, 1 argument");
        return;
    }
    if (process.argv.length > i+1) {
        confFile = process.argv[i+1];
    }
    Fs.readFile(confFile, function (err, ret) {
        if (err) { throw err; }
        let conf = Benc.decode(ret.toString('utf8'));
        if (!conf) {
            console.error("ERROR: failed to parse bconf file [" + confFile + "]");
        }
        process.stdout.write(JSON.stringify(conf, null, '  '));
    });
};

let confGet = function (ctx, i, cb) {
    if (process.argv.length !== i+2) {
        console.error("ERROR: `cjdnsctl conf get` requires exactly one argument");
        cb();
        return;
    }
    console.log(JSON.stringify(eval('ctx.conf.' + process.argv[i+1]), null, '  '));
    cb();
};

let confSet = function (ctx, i, cb) {
    if (process.argv.length !== i+3) {
        console.error("ERROR: `cjdnsctl conf set` requires exactly two arguments");
        cb();
        return;
    }
    if (process.argv[i+2] === 'undefined') {
        eval('delete ctx.conf.' + process.argv[i+1] + ';');
    } else {
        let val = JSON.parse(process.argv[i+2]);
        eval('ctx.conf.' + process.argv[i+1] + ' = val;');
    }
    syncConf(ctx);
    cb();
};

let conf = function (ctx, i, cb) {
    switch (process.argv[i+1]) {
        case 'gen': return confGen(i+1);
        case 'convert': return confConvert(i+1);
        case 'json': return confJson(i+1);
        case 'get': return confGet(ctx, i+1, cb);
        case 'set': return confSet(ctx, i+1, cb);
        case undefined:
        case 'help': confHelp(); return;
        default: {
            console.error("Unrecognized argument: " + process.argv[i+1]);
            confHelp();
        }
    }
};

let getAllConnectBlocks = function (ctx) {
    let connectBlocks = {};
    [ "UDPInterface", "ETHInterface" ].forEach(function (ifaceName) {
        if (!(ifaceName in ctx.conf.interfaces)) { return; }
        let iface = ctx.conf.interfaces[ifaceName];
        for (let i = 0; i < iface.length; i++) {
            for (let ipPort in iface[i].connectTo) {
                connectBlocks[iface[i].connectTo[ipPort].publicKey] = {
                    ipPort: ipPort,
                    block: iface[i].connectTo[ipPort],
                    iface: ifaceName.substring(0,3) + '/' + iface[i].bind
                };
            }
        }
    });
    return connectBlocks;
};

let getDevices = function (ctx, cb) {
    let ifaces = [];
    let page = 0;
    let handler = function (err, ret) {
        if (err) { throw err; }
        if (ret.error && ret.error !== 'none') {
            throw new Error("Response from cjdns: " + ret.error);
        }
        if (!ret.ifaces) { throw new Error("response missing ifaces"); }
        ifaces.push.apply(ifaces, ret.ifaces);
        if (ret.more) {
            page++;
            ctx.cjdns.InterfaceController_getInterfaces(page, handler);
        } else {
            ifaces.sort();
            cb(ifaces);
        }
    };
    ctx.cjdns.InterfaceController_getInterfaces(page, handler);
};

let peersConn = function (ctx, i, cb) {
    if (process.argv.length < i+4) {
        console.error("ERROR: `cjdnsctl peers conn` requires at least 3 arguments");
        cb();
        return;
    }
    for (let ii = 0; ii <= i; ii++) { process.argv.shift(); }
    let values = {
        addr: process.argv.shift(),
        key: process.argv.shift(),
        pass: process.argv.shift(),
        user: null,
        dev: null,
        comment: null
    };

    let opts = ['user','dev','comment'];
    while (process.argv.length) {
        let nextName = process.argv.shift();
        let nextVal = process.argv.shift();
        let idx = opts.indexOf(nextName);
        if (idx === -1) {
            console.error("ERROR: `cjdnsctl peers conn` extra parameters include " +
                JSON.stringify(opts) + " argument " + JSON.stringify(nextName) + " unhandled");
            cb();
            return;
        }
        if (typeof(nextVal) === 'undefined') {
            console.error("ERROR: `cjdnsctl peers conn` extra parameter " +
                JSON.stringify(nextName) + " requires a value");
            cb();
            return;
        }
        values[nextName] = nextVal;
    }

    if (values.key in getAllConnectBlocks(ctx)) {
        console.error("ERROR: node " + JSON.stringify(values.key) + " already in a connectTo block");
        cb();
        return;
    }

    let connectBlock = null;
    let ifaces;
    nThen(function (waitFor) {
        getDevices(ctx, waitFor(function (ifs) { ifaces = ifs; }));
    }).nThen(function (waitFor) {
        if (values.dev === null) {
            // No device specified, we need to guess.
            let matcher;
            let type;
            if (/^([a-fA-F0-9]{2}:){5}[a-fA-F0-9]{2}$/.test(values.addr)) {
                // It's ethernet, pick the first eth device
                matcher = /^ETH\//;
                type = "Ethernet/MAC";
            } else if (/^\[[a-fA-F0-9:]+\]:[0-9]+$/.test(values.addr)) {
                // It's ipv6
                matcher = /^UDP\/\[/;
                type = "UDP/IPv6";
            } else if (/^([0-9]{1,3}\.){3}[0-9]{1,3}:[0-9]+$/.test(values.addr)) {
                // It's ipv4
                matcher = /^UDP\/[0-9]+/;
                type = "UDP/IPv4";
            } else {
                console.error("ERROR: address " + JSON.stringify(values.addr) + " is not " +
                    "recognized as Ethernet/MAC, UDP/IPv6 or UDP/IPv4");
                return;
            }
            for (let i = 0; i < ifaces.length; i++) {
                if (matcher.test(ifaces[i].name)) {
                    values.dev = ifaces[i].name;
                    console.log("Using iface " + JSON.stringify(values.dev));
                    break;
                }
            }
            if (values.dev === null) {
                console.error("ERROR: address " + JSON.stringify(values.addr) + " is recognized " +
                    "as type " + JSON.stringify(type) + " and there is no existing interface " +
                    "for this address type.\nTry `cjdnsctl peers lsdev` for a list");
                return;
            }
        } else {
            let found = false;
            for (let i = 0; i < ifaces.length; i++) {
                if (ifaces[i].name === values.dev) { found = true; break; }
            }
            if (!found) {
                console.error("ERROR: dev " + JSON.stringify(values.dev) + " does not exist.\n" +
                    "Try `cjdnsctl peers lsdev` for a list");
                return;
            }
        }

        // Now we have to guess the correct block in the conf to insert the connect block into
        Object.keys(ctx.conf.interfaces).some(function (iface) {
            // Check iface type (UDP|ETH)
            if (iface.indexOf(values.dev.split('/')[0]) !== 0) { return; }
            ctx.conf.interfaces[iface].some(function (ifaceB, i) {
                if (ifaceB.bind === values.dev.split('/')[1]) {
                    values.confBlock = ctx.conf.interfaces[iface][i].connectTo;
                    return true;
                }
            });
            return "confBlock" in values;
        });
        if (!("confBlock" in values)) {
            console.error("ERROR: dev " + JSON.stringify(values.dev) + " cannot be matched " +
                "to a block in the configuration db");
            return;
        }

        let block = {
            password: values.pass,
            publicKey: values.key
        };
        if (values.user) { block.login = values.user; }
        if (values.comment) { block.comment = values.comment; }
        values.confBlock[values.addr] = block;

        ctx.cjdns.InterfaceController_connectTo(
            values.key, values.pass, values.dev, values.addr, values.user,
            waitFor(function (err, ret) {
                if (err) { throw err; }
                if (ret.error !== 'none') { throw new Error("Response from cjdns: " + ret.error); }
                syncConf(ctx);
            }));
    }).nThen(function (waitFor) {
        if (values.dev.indexOf('UDP') !== 0) { return; }
        ctx.cjdns.RouteGen_addException(values.addr.replace(/:[0-9]+$/, ''),
            waitFor(function (err, ret) {
                if (err) { throw err; }
                if (ret.error !== 'none') { throw new Error("Response from cjdns: " + ret.error); }
            }));
    }).nThen(function (waitFor) {
        if (values.dev.indexOf('UDP') !== 0) { return; }
        ctx.cjdns.RouteGen_commit(waitFor(function (err, ret) {
            if (err) { throw err; }
            if (ret.error !== 'none') { throw new Error("Response from cjdns: " + ret.error); }
        }));
    }).nThen(function (waitFor) {
        cb();
    });
};

let peersDisconn = function (ctx, i, cb) {
    if (process.argv.length !== i+2) {
        console.error("ERROR: `cjdnsctl peers disconn` requires exactly one argument");
        cb();
        return;
    }
    let key = process.argv[i+1];

    let addr;
    let dev;
    Object.keys(ctx.conf.interfaces).forEach(function (iface) {
        ctx.conf.interfaces[iface].forEach(function (ifaceB) {
            Object.keys(ifaceB.connectTo).forEach(function (ip) {
                if (ifaceB.connectTo[ip].publicKey === key) {
                    addr = ip;
                    dev = iface;
                    delete ifaceB.connectTo[ip];
                }
            });
        });
    });
    if (!addr) {
        console.error("not found");
        cb();
        return;
    }
    nThen(function (waitFor) {
        ctx.cjdns.InterfaceController_disconnectPeer(key, function (err, ret) {
            if (err) { throw err; }
            if (ret.error !== 'none') { throw new Error("Response from cjdns: " + ret.error); }
            syncConf(ctx);
        });
    }).nThen(function (waitFor) {
        if (dev.indexOf('UDP') !== 0) { return; }
        ctx.cjdns.RouteGen_removeException(addr.replace(/:[0-9]+$/, ''),
            waitFor(function (err, ret) {
                if (err) { throw err; }
                if (ret.error !== 'none') { throw new Error("Response from cjdns: " + ret.error); }
            }));
    }).nThen(function (waitFor) {
        if (dev.indexOf('UDP') !== 0) { return; }
        ctx.cjdns.RouteGen_commit(waitFor(function (err, ret) {
            if (err) { throw err; }
            if (ret.error !== 'none') { throw new Error("Response from cjdns: " + ret.error); }
        }));
    }).nThen(function (waitFor) {
        cb();
    });
};

let peersAuth = function (ctx, i, cb) {
    let comment;
    if (process.argv.length !== i+3) {
        if (process.argv.length === i+5 && process.argv[i+3] === 'comment') {
            comment = process.argv[i+4];
        } else {
            console.err("ERROR: `cjdnsctl peers auth` requires 2 arguments plus optional comment");
            cb();
            return;
        }
    }
    let block = { password: process.argv[i+1], user: process.argv[i+2] };
    if (comment) { block.comment = comment; }
    ctx.conf.authorizedPasswords.push(block);
    ctx.cjdns.AuthorizedPasswords_add(block.password, block.user, function (err, ret) {
        if (err) { throw err; }
        if (ret.error !== 'none') {
            throw new Error("Response from cjdns: " + ret.error);
        }
        syncConf(ctx);
        cb();
    });
};

let peersDeauth = function (ctx, i, cb) {
    if (process.argv.length !== i+2) {
        console.error("ERROR: `cjdnsctl peers deauth` requires exactly one argument");
        cb();
        return;
    }
    let login = process.argv[i+1];
    let found = false;
    for (let i = 0; i < ctx.conf.authorizedPasswords.length; i++) {
        if (ctx.conf.authorizedPasswords[i].user === login) {
            ctx.conf.authorizedPasswords.splice(i, 1);
            found = true;
            break;
        }
    }
    if (!found) {
        console.error("not found");
        cb();
        return;
    }
    ctx.cjdns.AuthorizedPasswords_remove(login, function (err, ret) {
        if (err) { throw err; }
        if (ret.error !== 'none') {
            throw new Error("Response from cjdns: " + ret.error);
        }
        syncConf(ctx);
        cb();
    });
};

let peersLsAuth = function (ctx, i, cb) {
    ctx.conf.authorizedPasswords.forEach(function (pwd, i) {
        let out = pwd.user;
        if ('comment' in pwd) { out += ' comment ' + JSON.stringify(pwd.comment); }
        console.log(out);
    });
    cb();
};

let parseAddr = function (addr) {
    let out;
    addr.replace(/^(v[0-9]+)((\.[0-9a-f]{4}){4})\.([a-z0-9]{52}\.k)$/, function (all, v, l, x, k) {
        out = { ver: v, path: l.substring(1), key: k };
    });
    if (!out) { throw new Error("failed to parse address [" + addr + "]"); }
    return out;
};

let formatPeerData = function (peer) {
    let p = peer.linkAddr + '\t' + peer.addr + ' ' + peer.state +
        ' in ' + peer.recvKbps + 'kb/s' + ' out ' + peer.sendKbps + 'kb/s';

    if (peer.duplicates !== 0) {
        p += ' ' + ' DUP ' + peer.duplicates;
    }
    if (peer.lostPackets !== 0) {
        p += ' ' + ' LOS ' + peer.lostPackets;
    }
    if (peer.receivedOutOfRange !== 0) {
        p += ' ' + ' OOR ' + peer.receivedOutOfRange;
    }
    if ('user' in peer) {
        p += ' ' + JSON.stringify(peer.user);
    }
    ['_passwdBlock', '_connectBlock'].forEach(function (blk) {
        if (blk in peer && 'comment' in peer[blk].block) {
            p += ' comment ' + JSON.stringify(peer[blk].block.comment);
        }
    });
    return p;
};

let peersLs = function (ctx, _i, cb) {
    let peerStats = [];
    let connectBlocks = getAllConnectBlocks(ctx);
    let passwords = {};
    ctx.conf.authorizedPasswords.forEach(function (pwd, i) { passwords[pwd.user] = pwd; });

    nThen(function (waitFor) {
        let again = function (i) {
            ctx.cjdns.InterfaceController_peerStats(i, waitFor(function (err, ret) {
                if (err) { throw err; }
                peerStats.push.apply(peerStats, ret.peers);
                if ('more' in ret) {
                    again(i+1);
                } else {
                    cb(peers);
                }
            }));
        };
        again(0);
    }).nThen(function (waitFor) {
        let ifaces = {};
        for (let i = 0; i < peerStats.length; i++) {
            peerStats[i]._parsedAddr = parseAddr(peerStats[i].addr);
            if (peerStats[i]._parsedAddr.key in connectBlocks) {
                peerStats[i]._connectBlock = connectBlocks[peerStats[i]._parsedAddr.key];
                delete connectBlocks[peerStats[i]._parsedAddr.key];
            }
            if ('user' in peerStats[i] && peerStats[i].user in passwords) {
                peerStats[i]._passwdBlock = passwords[peerStats[i].user];
                delete passwords[peerStats[i].user];
            }
            (ifaces[peerStats[i].ifName] = ifaces[peerStats[i].ifName] || []).push(peerStats[i]);
        }

        for (let iface in ifaces) {
            console.log("iface: " + iface);
            let pc = ifaces[iface];
            for (let i = 0; i < pc.length; i++) {
                console.log('  ' + formatPeerData(pc[i]));
            }
        }

        /*if (Object.keys(passwords).length) {
            console.log("unconnected authorized passwords:");
            for (let uname in passwords) {
                let out = uname;
                if ('comment' in passwords[uname]) {
                    out += ' comment ' + passwords[uname].comment;
                }
                console.log('  ' + out);
            }
        }*/

        if (Object.keys(connectBlocks).length) {
            console.log("unsynced connect blocks:");
            console.log(JSON.stringify(connectBlocks, null, '  '));
        }

        cb();
        return;
    });

};

let peersLsdev = function (ctx, i, cb) {
    let ifaces;
    nThen(function (waitFor) {
        getDevices(ctx, waitFor(function (ifs) { ifaces = ifs; }));
    }).nThen(function (waitFor) {
        for (let i = 0; i < ifaces.length; i++) {
            console.log(ifaces[i].name);
        }
        cb();
    });
};

let peers = function (ctx, i, cb) {
    switch (process.argv[i+1]) {
        case 'conn': return peersConn(ctx, i+1, cb);
        case 'disconn': return peersDisconn(ctx, i+1, cb);
        case 'auth': return peersAuth(ctx, i+1, cb);
        case 'deauth': return peersDeauth(ctx, i+1, cb);
        case 'lsauth': return peersLsAuth(ctx, i+1, cb);
        case 'ls': return peersLs(ctx, i+1, cb);
        case 'lsdev': return peersLsdev(ctx, i+1, cb);
        case undefined:
        case 'help': peersHelp(); cb(); return;
        default: {
            console.error("Unrecognized argument: " + process.argv[i+1]);
            peersHelp();
            cb();
        }
    }
};

let funMap = {
    'iptun': iptun,
    'conf': conf,
    'peers': peers
};

let main = module.exports.main = function () {
    let confFile = DEFAULT_BCONF;
    let func;
    let i = 0;
    for (; i < process.argv.length; i++) {
        if (funMap[process.argv[i]]) {
            func = funMap[process.argv[i]];
            break;
        }
        if (process.argv[i] === '-f') {
            confFile = process.argv[++i];
        }
    }
    if (func === conf && ['get','set'].indexOf(process.argv[i+1]) === -1) {
        conf(null, i);
        return;
    }
    if (confFile && func) {
        let out = {
            confFile: confFile
        };
        nThen(function (waitFor) {
            Fs.readFile(confFile, waitFor(function (err, ret) {
                if (err) {
                    console.error("Need access to the cjdns configuration db [" + confFile + "]");
                    throw err;
                }
                let str = ret.toString('utf8');
                out.conf = Benc.decode(str);
                if (!out.conf) { throw new Error("Failed to parse [" + confFile + "]"); }
                if (Benc.encode(out.conf) !== str) { throw new Error("bencode roundtrip failed"); }
                fixConf(out.conf);
            }));
        }).nThen(function (waitFor) {
            let addrPort = out.conf.admin.bind;
            let lColon = addrPort.lastIndexOf(':');
            let addr = addrPort.substring(0, lColon);
            let port = Number(addrPort.substring(lColon+1));
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
}());
