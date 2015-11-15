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

var UDP = require('dgram');
var Bencode = require('./bencode');
var Crypto = require('crypto');
var Fs = require('fs');
var nThen = require('nthen');
var Semaphore = require('../Semaphore.js');

var TIMEOUT_MILLISECONDS = 10000;

var sendmsg = function (sock, addr, port, msg, txid, callback) {
    var to = setTimeout(function () {
        callback(new Error("timeout after " + TIMEOUT_MILLISECONDS + "ms"));
        delete sock.handlers[txid];
    }, TIMEOUT_MILLISECONDS);
    sock.handlers[txid] = {
        callback: callback,
        timeout: to
    };

    sock.send(msg, 0, msg.length, port, addr, function(err, bytes) {
        if (err) {
            clearTimeout(to);
            delete sock.handlers[txid];
            callback(err);
        }
    });
};

var callFunc = function (sock, addr, port, pass, func, args, callback) {
    var cookieTxid = String(sock.counter++);
    var cookieMsg = new Buffer(Bencode.encode({'q':'cookie','txid':cookieTxid}));
    sendmsg(sock, addr, port, cookieMsg, cookieTxid, function (err, ret) {
        if (err) { callback(err); return; }
        var cookie = ret.cookie;
        if (typeof(cookie) !== 'string') { throw new Error("invalid cookie in [" + ret + "]"); }
        var json = {
            txid: String(sock.counter++),
            q: func,
            args: {}
        };
        Object.keys(args).forEach(function (arg) {
            json.args[arg] = args[arg];
        });
        if (pass) {
            json.aq = json.q;
            json.q = 'auth';

            json.cookie = cookie;
            json.hash = Crypto.createHash('sha256').update(pass + cookie).digest('hex');
            json.hash = Crypto.createHash('sha256').update(Bencode.encode(json)).digest('hex');
        }
        sendmsg(sock, addr, port, new Buffer(Bencode.encode(json)), json.txid, callback);
    });
};

var getArgs = function (func) {
    var rArgs = [];
    Object.keys(func).forEach(function (name) {
        if (func[name].required === '1') {
            rArgs.push({ name: name, type: func[name].type, required: true });
        }
    });
    // be sure that the arguments are always in the same order
    rArgs.sort(function (a,b) { a = a.name; b = b.name; return (a !== b) ? (a < b) ? 1 : -1 : 0; });
    var oArgs = [];
    Object.keys(func).forEach(function (name) {
        if (func[name].required === '0') {
            oArgs.push({ name: name, type: func[name].type, required: false });
        }
    });
    oArgs.sort(function (a,b) { a = a.name; b = b.name; return (a !== b) ? (a < b) ? 1 : -1 : 0; });
    rArgs.push.apply(rArgs, oArgs);
    return rArgs;
};

var makeFunctionDescription = function (funcName, func) {
    var args = getArgs(func);
    var outArgs = [];
    args.forEach(function (arg) {
        outArgs.push( ((arg.required) ? 'required ' : '') + arg.type + ' ' + arg.name );
    });
    return funcName + "(" + outArgs.join(', ') + ")";
};

var compatibleType = function (typeName, obj) {
    switch (typeName) {
        case 'Int': return (typeof(obj) === 'number' && Math.floor(obj) === obj);
        case 'String': return (typeof(obj) === 'string');
        case 'Dict': return (typeof(obj) === 'object');
        case 'List': return Array.isArray(obj);
        default: throw new Error();
    }
};

var makeFunction = function (sock, addr, port, pass, funcName, func) {
    var args = getArgs(func);
    return function () {
        var i;
        var argsOut = {};
        for (i = 0; i < arguments.length-1; i++) {
            var arg = arguments[i];
            if (!args[i].required && (arg === null || arg === undefined)) { continue; }
            if (!compatibleType(args[i].type, arg)) {
                throw new Error("argument [" + i + "] ([" + arguments[i] + "]) [" +
                                args[i].type + " " + args[i].name + "]" +
                                " is of type [" + typeof(arg) + "] which is not compatible with " +
                                "required type " + args[i].type);
            }
            argsOut[args[i].name] = arg;
        }
        if (args.length > i && args[i].required) {
            throw new Error("argument [" + i + "] [" + args[i].type + " " + args[i].name + "] is " +
                            "required and is not specified");
        }

        var callback = arguments[arguments.length-1];
        if (typeof(callback) !== 'function') {
            throw new Error("callback is unspecified");
        }

        sock.semaphore.take(function (returnAfter) {
            callFunc(sock, addr, port, pass, funcName, argsOut, returnAfter(callback));
        });
    };
};

var getFunctions = function (sock, addr, port, pass, callback) {
    var funcs = {};
    nThen(function (waitFor) {
        var next = function (i) {
            callFunc(sock, addr, port, pass, 'Admin_availableFunctions', {page:i},
                waitFor(function (err, ret) {
                    if (err) { throw err; }
                    Object.keys(ret.availableFunctions).forEach(function (funcName) {
                        funcs[funcName] = ret.availableFunctions[funcName];
                    });
                    if (Object.keys(ret.availableFunctions).length > 0) {
                        next(i+1);
                    }
                })
            );
        };
        next(0);

    }).nThen(function (waitFor) {
        var funcDescriptions = [];
        var cjdns = {};
        Object.keys(funcs).forEach(function (name) {
            cjdns[name] = makeFunction(sock, addr, port, pass, name, funcs[name]);
            funcDescriptions.push(makeFunctionDescription(name, funcs[name]));
        });
        cjdns.functions = function (cb) { cb(undefined, funcDescriptions); };
        callback(cjdns);
    });
};

var connect = module.exports.connect = function (addr, port, pass, callback) {
    var sock = UDP.createSocket((addr.indexOf(':') !== -1) ? 'udp6' : 'udp4');
    sock.semaphore = Semaphore.create(4);
    sock.handlers = {};
    sock.counter = Math.floor(Math.random() * 4000000000);
    sock.on('message', function (msg) {
        var response = Bencode.decode(msg.toString('utf8'));
        if (!response.txid) {
            throw new Error("Response [" + msg + "] with no txid");
        }
        var handler = sock.handlers[response.txid];
        if (!handler) {
            if (sock.defaultHandler) {
                sock.defaultHandler(undefined, response);
            }
            return;
        }
        clearTimeout(handler.timeout);
        delete sock.handlers[response.txid];
        handler.callback(undefined, response);
    });

    nThen(function (waitFor) {
        callFunc(sock, addr, port, pass, 'ping', {}, waitFor(function (err, ret) {
            if (err) { throw err; }
            //console.log("got pong! [" + JSON.stringify(ret) + "]");
        }));
    }).nThen(function (waitFor) {
        getFunctions(sock, addr, port, pass, function (cjdns) {
            cjdns.disconnect = function () { sock.close(); };
            cjdns.setDefaultHandler = function (handler) { sock.defaultHandler = handler; };
            callback(cjdns);
        });
    });
};

var connectWithAdminInfo = module.exports.connectWithAdminInfo = function (callback) {
    var cjdnsAdmin = {'addr': '127.0.0.1', 'port': 11234, 'password': 'NONE'};
    nThen(function (waitFor) {
        Fs.readFile(process.env.HOME + '/.cjdnsadmin', waitFor(function (err, ret) {
            if (err && err.code != 'ENOENT') { throw err; }
            if (!err) { cjdnsAdmin = JSON.parse(String(ret)); }
        }));
    }).nThen(function (waitFor) {
        connect(cjdnsAdmin.addr, cjdnsAdmin.port, cjdnsAdmin.password, callback);
    });
};

var connectAsAnon = module.exports.connectAsAnon = function (callback, addr, port) {
    var cjdnsAdmin = {'addr': addr || '127.0.0.1', 'port': port || 11234, 'password': 'NONE'};
    nThen(function (waitFor) {
        Fs.readFile(process.env.HOME + '/.cjdnsadmin', waitFor(function (err, ret) {
            if (!err) { cjdnsAdmin = JSON.parse(String(ret)); }
        }));
    }).nThen(function (waitFor) {
        connect(cjdnsAdmin.addr, cjdnsAdmin.port, null, callback);
    });
};
