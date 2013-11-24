var Os = require('os');
var Fs = require('fs');
var Spawn = require('child_process').spawn;
var nThen = require('nthen');
//var Extend = require('node.extend');
//var AbiName = require('./AbiName');
//var PlanRunner = require('./PlanRunner');
//var TestRunner = require('./TestRunner');
//var RandomBytes = require('./RandomBytes');
//var Common = require('./Common');

var WORKERS = Os.cpus().length;

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
            callback(error("\n" + err));
        }
        callback(undefined, out);
    });
    gcc.stdin.write(content, function (err) {
        if (err) { throw err; }
        gcc.stdin.end();
    });
};

var execJs = function (js, fileObj, callback) {
    var x;
    var err;
    try {
        var func = eval('func = function(file) { ' + js + ' };');
        x = func(fileObj) || '';
    } catch (e) {
        err = e;
        err.message += "Content: [" + js + "]";
    }
    process.nextTick(function() { callback(err, x); });
};

var debug = console.log;

var preprocess = function (content, fileObj, callback) {
    var captures = [];
    nThen(function (waitFor) {
        content = content.replace(/<\?js(.*)\?>/g, function (x, capture) {
            captures.push(capture);
            return '<?js?>';
        });
        captures.forEach(function (capture, i) {
            execJs(capture, fileObj, waitFor(function (err, ret) {
                if (err) {
                    callback(err);
                    callback = function() {};
                    return;
                }
                debug('[' + capture + '] --> [' + ret + ']');
                captures[i] = ret;
            }));
        });
    }).nThen(function (waitFor) {
        content = content.replace(/<\?js\?>/g, function (x) {
            return captures.shift();
        });

        callback(undefined, content);
    });
};

var Fs_readFile = Fs.readFile;
var Fs_stat = Fs.stat;



var getFile = function ()
{
    return {
        includes: [],
        links: [],
        oldmtime: 0
    };
};

var compile = function (fileName, state, callback)
{
    if (typeof(state.files[fileName]) !== 'undefined') {
        callback(undefined);
        return;
    }
    debug('compile('+fileName+')');
    var outFile = state.buildDir+'/'+fileName.replace(/[^a-zA-Z0-9_-]/g, '_')+'.o';
    state.files[fileName] = getFile();
    var fileContent;
    nThen(function (waitFor) {

        debug("Load file");
        Fs_readFile(fileName, waitFor(function (err, ret) {
            if (err) {
                waitFor.abort();
                callback(err);
                return;
            }
            fileContent = ret.toString('utf8');
        }));

    }).nThen(function (waitFor) {

        debug("Preprocess 1");
        preprocess(fileContent, state.files[fileName], waitFor(function (err, output) {
            if (err) { throw err; }
            fileContent = output;
        }));

    }).nThen(function (waitFor) {
        debug("CPP -MM");
        var flags = ['-E', '-MM'];
        flags.push.apply(flags, state.cflags);
        flags.push('-');
        cc(fileContent, fileName, flags, waitFor(function (err, output) {
            if (err) { throw err; }
            // replace the escapes and newlines
            output = output.replace(/ \\|\n/g, '').split(' ');
            // first 2 entries are crap
            output.splice(0,2);
            state.files[fileName].includes = output;
            //debug(state.files[fileName]);
        }));

    }).nThen(function (waitFor) {
        debug("CPP");
        var flags = ['-E'];
        flags.push.apply(flags, state.cflags);
        flags.push('-');
        cc(fileContent, fileName, flags, waitFor(function (err, output) {
            if (err) { throw err; }
            fileContent = output;
        }));

    }).nThen(function (waitFor) {
        debug("Preprocess 2");
        preprocess(fileContent, state.files[fileName], waitFor(function (err, output) {
            if (err) { throw err; }
            fileContent = output;
//debug(output);
        }));

        Fs.exists(outFile, waitFor(function (exists) {
            if (exists) {
                Fs.unlink(outFile, waitFor(function (err) {
                    if (err) { throw err; }
                }));
            }
        }));

    }).nThen(function (waitFor) {

        debug("CC");
        var flags = ['-c','-x','cpp-output','-o',outFile,'-'];
        flags.push.apply(flags, state.cflags);
        cc(fileContent, fileName, flags, waitFor(function (err) {
            if (err) { throw err; }
        }));

    }).nThen(function (waitFor) {
        debug("Compiling " + fileName + " --> " + outFile + " complete");
        state.files[fileName].obj = outFile;
        callback();
    });
};

/**
 * @param files state.files
 * @param mtimes a mapping of files to times for files for which the times are known
 * @param callback when done.
 */
var getMTimes = function (files, mtimes, callback)
{
    nThen(function (waitFor) {
        Object.keys(files).forEach(function (fileName) {
            mtimes[fileName] = mtimes[fileName] || 0;
            files[fileName].includes.forEach(function (incl) {
                mtimes[incl] = mtimes[incl] || 0;
            });
        });
        Object.keys(mtimes).forEach(function (fileName) {
            if (mtimes[fileName] !== 0) { return; }
            Fs_stat(fileName, waitFor(function (err, stat) {
                if (err) {
                    waitFor.abort();
                    callback(err);
                    return;
                }
                mtimes[fileName] = stat.mtime.getTime();
            }));
        });
    }).nThen(function (waitFor) {
        callback(undefined, mtimes);
    });
};

var CONFIG = {
    cflags: [
        '-std=c99',
        '-Wall',
        '-Wextra',
        '-Werror',
        '-Wno-pointer-sign',
        '-pedantic',
        '-D','Linux=1',
        '-D','HAS_ETH_INTERFACE=1',
        '-Wno-unused-parameter',
        '-Wno-unused-result',
        '-fno-stack-protector',
        '-fstack-protector-all',
        '-Wstack-protector',
        '-D','HAS_BUILTIN_CONSTANT_P',
        '-fPIE',
        '-g',
        '-D','Log_DEBUG',
        '-D','CJDNS_MAX_PEERS=256',
        '-D','Identity_CHECK=1',
        '-D','PARANOIA=1',
        '-D','HAS_JS_PREPROCESSOR'
    ],
    includeDirs: [
        'build/nacl_build/include/'
    ]
};

var removeFile = function (state, fileName, callback)
{
    nThen(function (waitFor) {
        var f = state.files[fileName];
        if (typeof(f.obj) === 'string') {
            Fs.unlink(f.obj, waitFor(function (err) {
                if (err) { throw err; }
            }));
        }
        delete state.files[fileName];
    }).nThen(function (waitFor) {
        callback();
    });
};

var setUp = function (callback) {
    var state = CONFIG;
    state.includeDirs = state.includeDirs || [];
    state.files = state.files || {};
    state.includeDirs.unshift('.');
    state.buildDir = 'jsbuild';

    for (var i = 0; i < state.includeDirs.length; i++) {
        state.cflags.push('-I');
        state.cflags.push(state.includeDirs[i]);
    }

    nThen(function(waitFor) {
        // make the build directory
        Fs.exists(state.buildDir, waitFor(function (exists) {
            if (exists) { return; }
            Fs.mkdir('jsbuild', waitFor(function (err) {
                if (err) { throw err; }
            }));
        }));

    }).nThen(function(waitFor) {

        // read out the state if it exists
        Fs.exists(state.buildDir + '/state.json', waitFor(function (exists) {
            if (!exists) { return; }
            Fs_readFile('jsbuild/state.json', waitFor(function (err, ret) {
                if (err) { throw err; }
                state = JSON.parse(ret);
            }));
        }));

    }).nThen(function(waitFor) {

        // Move the mtimes to the old mtimes
        var oldmtimes = state.mtimes;

        // Get the new timestamps.
        getMTimes(state.files, {}, waitFor(function (err, mtimes) {
            if (err) { throw err; }
            state.mtimes = mtimes;

            // For anything which is out of date, remove it and it will be recompiled
            Object.keys(state.files).forEach(function (name) {
                if (oldmtimes[name] !== mtimes[name]) {
                    removeFile(state, name, waitFor());
                    return;
                }
                var fileObj = state.files[name];
                for (incl in fileObj.includes) {
                    if (oldmtimes[incl] !== mtimes[incl]) {
                        removeFile(state, name, waitFor());
                        return;
                    }
                }
            });

        }));

    }).nThen(function(waitFor) {
        callback(state);
    });
};

var main = function() {

    var state;
    var file = './admin/angel/cjdroute2.c';

    nThen(function (waitFor) {

        setUp(waitFor(function (s) { state = s; }));

    }).nThen(function(waitFor) {

        // Get the thing we mean to build
        console.log("compiling " + file);
        compile(file, state, waitFor(function (err) {
debug('done');
            if (err) { throw err; }
        }));

    }).nThen(function(waitFor) {

        // After preprocess2 is complete, we know the linker dependencies
        // and can begin compiling those
        state.files[file].links.forEach(function (file) {
            compile(file, state, waitFor(function (err) {
                if (err) { throw err; }
            }));
        });

    }).nThen(function(waitFor) {

        getMTimes(state.files, state.mtimes, waitFor(function (err, mtimes) {
            if (err) { throw err; }
            state.mtimes = mtimes;
        }));

    }).nThen(function(waitFor) {

        //compile(file, state.files[file], 'jsbuild', waitFor());

    }).nThen(function(waitFor) {

        console.log('done');
        console.log(JSON.stringify(state, null, '  '));

    });

    //compile('publictoip6.c');
};

main();
