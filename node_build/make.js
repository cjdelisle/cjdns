var Os = require('os');
var Fs = require('fs');
var Spawn = require('child_process').spawn;
var nThen = require('nthen');
//var AbiName = require('./AbiName');
//var PlanRunner = require('./PlanRunner');
//var TestRunner = require('./TestRunner');
//var RandomBytes = require('./RandomBytes');
//var Common = require('./Common');

var WORKERS = Os.cpus().length;

var cc = function(args, onComplete, noArg) {
  if (noArg) { throw new Error(); }
  var gcc = Spawn('gcc', args);
  var err = '';
  var out = '';
  gcc.stdout.on('data', function(dat) { out += dat.toString() ; });
  gcc.stderr.on('data', function(dat) { err += dat.toString() ; });
  gcc.on('close', function(ret) { onComplete(err, out, ret); });
};

var ar = function(args, onComplete) {
  var ar = Spawn('ar', args);
  var out = '';
  ar.stderr.on('data', function(dat) { out += dat.toString(); });
  ar.stdout.on('data', function(dat) { out += dat.toString(); });
  ar.on('close', function(ret) {
    onComplete(ret, out);
  });
};

var ranlib = function(args, onComplete) {
  var rl = Spawn('ranlib', args);
  var out = '';
  rl.stderr.on('data', function(dat) { out += dat.toString(); });
  rl.on('close', function(ret) {
    onComplete(ret, out);
  });
};


var error = function (message)
{
    try {
        throw new Error(message);
    } catch (e) {
        return e;
    }
};

var cc = function (content, fileName, args, callback, noArg) {
    if (noArg) { throw new Error(); }
    //args = args || [];
    //args.push.apply(args, ['-I', '.', '-E', '-x', 'c', '-']);
    var gcc = Spawn('gcc', args);
    var err = '';
    var out = '';
    gcc.stdout.on('data', function(dat) { out += dat.toString() ; });
    gcc.stderr.on('data', function(dat) { err += dat.toString() ; });
    gcc.on('close', function(ret) {
        if (ret) {
            err = err.replace(/<stdin>:/, fileName+":");
            err = err.replace(/\n<stdin>:/g, '\n'+fileName+":");
            callback(error("GCC failed with [" + ret + "] error:\n" + err));
        }
        callback(undefined, out);
    });
    gcc.stdin.write(content, function (err) {
        if (err) { throw err; }
        gcc.stdin.end();
    });
};

var execJs = function (js, callback) {
    var x;
    var err;
    try {
        x = eval('(function() { ' + js + ' })();');
    } catch (e) {
        err = e;
        err.message += "Content: [" + js + "]";
    }
    process.nextTick(function() { callback(err, x); });
};

var preprocess = function (content, callback) {
    var out = [];
    nThen(function (waitFor) {
        var elements = content.split('<?js');
        elements.forEach(function (elem, i) {
            if (!(i % 2)) { out[i] = elements[i]; return; }
            var jsAndNon = elements[i].split('?>');
            execJs(jsAndNon[0], waitFor(function (err, ret) {
                if (err) {
                    callback(err);
                    callback = function() {};
                    return;
                }
                out[i] = ret + jsAndNon[1];
            }));
        });
    }).nThen(function (waitFor) {
        callback(undefined, out.join(''));
    });
};

var Fs_readFile = Fs.readFile;

var debug = console.log;

var compile = function(fileName, callback) {
    var fileContent;
    nThen(function (waitFor) {
        debug("Read file");
        Fs_readFile(fileName, waitFor(function (err, content) {
            if (err) { throw err; }
            fileContent = content.toString('utf8');
        }));

    }).nThen(function (waitFor) {
        debug("Preprocess 1");
        preprocess(fileContent, waitFor(function (err, output) {
            if (err) { throw err; }
            fileContent = output;
        }));

    }).nThen(function (waitFor) {
        debug("CPP");
        cc(fileContent, fileName, ['-I', '.', '-E', '-x', 'c', '-'], waitFor(function (err, output) {
            if (err) { throw err; }
            fileContent = output;
        }));

    }).nThen(function (waitFor) {
        debug("Preprocess 2");
        preprocess(fileContent, waitFor(function (err, output) {
            if (err) { throw err; }
            fileContent = output;
        }));

    }).nThen(function (waitFor) {
        debug("CC");
        cc(fileContent, fileName, ['-c', '-std=c99', '-x', 'cpp-output', '-', '-o', fileName + '.o'], waitFor(function (err) {
            if (err) { throw err; }
        }));

    }).nThen(function (waitFor) {
        console.log('done');

    });
};

var SYSTEM_DEPS = {
    'stdio.h':[],
    'stdint.h':[],
    'stdbool.h':[],
    'unistd.h':[],
    'stddef.h':[],
    'string.h':[],
    'libkern/OSByteOrder.h': [],

    'inttypes.h':[],
    'crypto_scalarmult_curve25519.h':[]
};

var getIncludes = function (fileName, deps, callback)
{
    if (typeof(deps[fileName]) !== 'undefined') {
        callback(undefined);
        return;
    }
    //debug('getIncludes('+fileName+')');
    deps[fileName] = [];
    var fileContent;
    nThen(function (waitFor) {
        Fs_readFile(fileName, waitFor(function (err, ret) {
            if (err) {
                waitFor.abort();
                callback(err);
            }
            fileContent = ret.toString('utf8');
        }));

    }).nThen(function (waitFor) {
        // don't worry about all of the other possible ways to say #      include\n<file.h>
        var includes = fileContent.split('#include ');
        for (var i = 0; i < includes.length; i++) {
            if (!(i % 2)) { continue; }
            var incl;
            includes[i].replace(/[\"<]([a-zA-Z0-9_\/\.\-]*)[\">]/, function (x,y) { incl = y; });
            if (typeof(incl) === 'undefined') {
                waitFor.abort();
                callback(error("malformed include in line ["+fileName+":"+i+"]\n"+includes[i]));
                return;
            }
            deps[fileName].push(incl);
            debug(fileName + ' -> ' + incl);
            getIncludes(incl, deps, waitFor(function (err) {
                if (err) {
                    waitFor.abort();
                    debug(err.stack + '\n' + fileName);
                    callback(err);
                }
            }));
        }
    }).nThen(function (waitFor) {
        callback();
    });
};

var main = function() {
    console.log("Building publictoip6.c");
    //compile('publictoip6.c');
    getIncludes('./admin/angel/cjdroute2.c', SYSTEM_DEPS, function (err) {
        if (err) { throw err; }
        console.log(SYSTEM_DEPS);
    });
};

main();
