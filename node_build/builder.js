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
var Os = require('os');
var Fs = require('fs');
var Spawn = require('child_process').spawn;
var nThen = require('nthen');
var Crypto = require('crypto');
var Semaphore = require('../tools/lib/Semaphore');
var GetVersion = require('./GetVersion');

/*
 * Why hello dear packager,
 *
 * I suppose you have found this place as you are trying to figure out how to work this into your
 * build system. You're probably faced with a decision between getting node.js into your build and
 * "fixing" this build process so it doesn't need such a silly thing. A 500 line script is certainly
 * not unapproachable, right?
 * The reason why I am speaking to you now is because I care about you. I want you to be happy
 * and live a carefree life, and because you are standing on the precipice of a cavern so dark and
 * deep that while you may well make it out alive, your personal pride and innocence almost
 * certainly will not. Imagine yourself after months of sleepless nights wallowing in the quicksand,
 * forever trying to slay the dragon which is always so close yet and so far away. Imagine the deep
 * hatred you will have for humanity, code, and the creator of this doomsday machine. I beg you to
 * turn back now while there is still hope. You need not die here, your life is important, and
 * whether you close this file now or not, in the end you will still end up including node.js in
 * your build.
 *
 * The Creator
 */

// Since many of the compile operations are short, the best
// performance seems to be when running 1.25x the number of jobs as
// cpu cores. On BSD and iphone systems, os.cpus() is not reliable so
// if it returns undefined let's just assume 1
var cpus = Os.cpus(); // workaround, nodejs seems to be broken on openbsd (undefined result after second call)
var PROCESSORS = Math.floor((typeof cpus === 'undefined' ? 1 : cpus.length) * 1.25);

var error = function (message)
{
    try {
        throw new Error(message);
    } catch (e) {
        return e;
    }
};

var throwIfErr = function(err) {
    if (err) {
        throw new Error(err);
    }
};

var expandArgs = function (args) {
    var out = [];
    for (var i = 0; i < args.length; i++) {
        if (typeof(args[i]) === 'object') {
            if (Array.isArray(args[i])) {
                out.push.apply(out, expandArgs(args[i]));
            } else {
                throw new Error("object in arguments [" + args + "]");
            }
        } else {
            out.push(args[i]);
        }
    }
    return out;
};

var sema = Semaphore.create(PROCESSORS);
var compiler = function (compilerPath, args, callback, content) {
    args = expandArgs(args);
    sema.take(function (returnAfter) {
        var gcc = Spawn(compilerPath, args);
        var err = '';
        var out = '';

        gcc.stdout.on('data', function (dat) { out += dat.toString(); });
        gcc.stderr.on('data', function (dat) { err += dat.toString(); });
        gcc.on('close', returnAfter(function (ret) {
            callback(ret, out, err);
        }));

        gcc.on('error', function (err) {
            if (err.code === 'ENOENT') {
                console.error('\033[1;31mError: ' + compilerPath + ' is required!\033[0m');
            } else {
                console.error(
                    '\033[1;31mFail run ' + process.cwd() + ': ' + compilerPath + ' '
                    + args.join(' ') + '\033[0m'
                );
                console.error('Message:' + err);
            }

            // handle the error safely
            console.log(args);
        });

        if (content) {
            gcc.stdin.write(content, function (err) {
                if (err) { throw err; }
                gcc.stdin.end();
            });
        }
    });
};

var cc = function (gcc, args, callback, content) {
    compiler(gcc, args, function (ret, out, err) {
        if (ret) {
            callback(error("gcc " + args.join(' ') + "\n\n" + err));
        }

        if (err !== '') {
            debug(err);
        }

        callback(undefined, out);
    }, content);
};

var tmpFile = function (state, name) {
    name = name || '';
    return state.tempDir + '/jsmake-' + name + Crypto.pseudoRandomBytes(10).toString('hex');
};

var mkBuilder = function (state) {
    var builder = {
        cc: function (args, callback) {
            compiler(builder.config.gcc, args, callback);
        },

        buildExecutable: function (cFile, outputFile) {
            if (!outputFile) {
                outputFile = cFile.replace(/^.*\/([^\/\.]*).*$/, function (a, b) { return b; });
            }

            if (state.systemName === 'win32' && !(/\.exe$/.test(outputFile))) {
                outputFile += '.exe';
            }

            var temp = state.buildDir + '/' + getExecutableFile(cFile);
            compile(cFile, temp, builder, builder.waitFor());
            builder.executables.push([temp, outputFile]);

            return temp;
        },

        buildTest: function (cFile) {
            return builder.buildExecutable(cFile, state.buildDir + '/' + getExecutableFile(cFile));
        },

        runTest: function (outFile, testRunner) {
            builder.tests.push(function (cb) { testRunner(outFile, cb); });
        },

        lintFiles: function (linter) {
            builder.linters.push(linter);
        },

        config: state,

        tmpFile: function (name) {
            return tmpFile(state, name);
        },

        rebuiltFiles: [],

        // Test executables (arrays containing name in build dir and test runner)
        tests: [],

        // Executables (arrays containing name in build dir and final name)
        executables: [],

        linters: [],

        // Concurrency...
        processors: PROCESSORS
    };
    return builder;
};

// You Were Warned
var execJs = function (js, builder, file, fileName, callback) {
    var res;
    var x;
    var err;

    // # 74 "./wire/Message.h"
    js = js.replace(/\n#.*\n/g, '');
    var to = setTimeout(function () {
        throw new Error("Inline JS did not return after 120 seconds [" + js + "]");
    }, 120000);

    var REQUIRE = function (str) {
        if (typeof(str) !== 'string') {
            throw new Error("must be a string");
        }

        try { return require(str); } catch (e) { }

        return require(process.cwd() + '/' + str);
    };

    nThen(function (waitFor) {

        try {
            /* jshint -W054 */ // Suppress jshint warning on Function being a form of eval
            var func = new Function('file', 'require', 'fileName', 'console', 'builder', js);

            func.async = function () {
                return waitFor(function (result) {
                    res = result;
                });
            };

            x = func.call(func,
                          file,
                          REQUIRE,
                          fileName,
                          console,
                          builder);
        } catch (e) {
            err = e;
            err.message += "\nContent: [" + js + "]";
            clearTimeout(to);
            throw err;
        }

    }).nThen(function (waitFor) {

        if (err) { return; }
        res = res || x || '';
        clearTimeout(to);
        process.nextTick(function () { callback(undefined, res); });

    });
};

var debug = console.log;

var preprocessBlock = function (block, builder, fileObj, fileName, callback) {
    // a block is an array of strings and arrays, any inside arrays must be
    // preprocessed first. deep first top to bottom.

    var error = false;
    var nt = nThen;

    block.forEach(function (elem, i) {
        if (typeof(elem) === 'string') { return; }

        nt = nt(function (waitFor) {
            preprocessBlock(elem, builder, fileObj, fileName, waitFor(function (err, ret) {
                if (err) { throw err; }

                block[i] = ret;
            }));
        }).nThen;
    });

    nt(function (waitFor) {

        if (error) { return; }
        var capture = block.join('');
        execJs(capture, builder, fileObj, fileName, waitFor(function (err, ret) {
            if (err) { throw err; }
            callback(undefined, ret);
        }));

    });
};

var preprocess = function (content, builder, fileObj, fileName, callback) {
    // <?js file.Test_mainFunc = "<?js return 'RootTest_'+file.RootTest_mainFunc; ?>" ?>
    // worse:
    // <?js file.Test_mainFunc = "<?js var done = this.async(); process.nextTick(done); ?>" ?>

    var flatArray = content.split(/(<\?js|\?>)/);
    var elems = [];
    var unflatten = function (array, startAt, out) {
        for (var i = startAt; i < array.length; i++) {
            /* jshint -W018 */ // Suppress jshint warning on ! being confusing
            if (!((i - startAt) % 2)) {
                out.push(array[i]);
            } else if (array[i] === '<?js') {
                var next = [];
                out.push(next);
                i = unflatten(array, i+1, next);
            } else if (array[i] === '?>') {
                return i;
            }
        }

        return i;
    };

    if (unflatten(flatArray, 0, elems) !== flatArray.length) {
        throw new Error();
    }

    var nt = nThen;
    elems.forEach(function (elem, i) {
        if (typeof(elem) === 'string') { return; }

        nt = nt(function (waitFor) {
            preprocessBlock(elem, builder, fileObj, fileName, waitFor(function (err, ret) {
                if (err) { throw err; }

                elems[i] = ret;
            }));
        }).nThen;
    });

    nt(function (waitFor) {
        callback(undefined, elems.join(''));
    });
};

var getFile = function ()
{
    return {
        includes: [],
        links: [],
        cflags: [],
        oldmtime: 0
    };
};

var getObjectFile = function (cFile) {
    return cFile.replace(/[^a-zA-Z0-9_-]/g, '_') + '.o';
};

var getExecutableFile = function (cFile) {
    return cFile.replace(/[^a-zA-Z0-9_-]/g, '_');
};

var getFlags = function (state, fileName, includeDirs) {
    var flags = [];
    flags.push.apply(flags, state.cflags);
    flags.push.apply(flags, state['cflags'+fileName]);

    if (includeDirs) {
        for (var i = 0; i < state.includeDirs.length; i++) {
            if (flags[flags.indexOf(state.includeDirs[i])-1] === '-I') {
                continue;
            }

            flags.push('-I');
            flags.push(state.includeDirs[i]);
        }
    }

    for (var ii = flags.length-1; ii >= 0; ii--) {
        // might be undefined because splicing causes us to be off the end of the array
        if (typeof(flags[ii]) === 'string' && flags[ii][0] === '!') {
            var f = flags[ii].substring(1);
            flags.splice(ii, 1);

            var index;
            while ((index = flags.indexOf(f)) > -1) {
                flags.splice(index, 1);
            }
        }
    }

    return flags;
};

var currentlyCompiling = {};
var compileFile = function (fileName, builder, tempDir, callback)
{
    var state = builder.config;
    if (typeof(state.files[fileName]) !== 'undefined') {
        callback();
        return;
    }

    if (typeof(currentlyCompiling[fileName]) !== 'undefined') {
        currentlyCompiling[fileName].push(callback);
        return;
    } else {
        currentlyCompiling[fileName] = [];
    }
    currentlyCompiling[fileName].push(callback);

    //debug('\033[2;32mCompiling ' + fileName + '\033[0m');

    var preprocessed = state.buildDir + '/' + getObjectFile(fileName) + '.i';
    var outFile = state.buildDir + '/' + getObjectFile(fileName);
    var fileContent;
    var fileObj = getFile();

    nThen(function (waitFor) {

        (function () {
            //debug("CPP -MM");
            var flags = ['-E', '-MM'];
            flags.push.apply(flags, getFlags(state, fileName, true));
            flags.push(fileName);

            cc(state.gcc, flags, waitFor(function (err, output) {
                if (err) { throw err; }

                // replace the escapes and newlines
                output = output.replace(/ \\|\n/g, '').split(' ');

                // first 2 entries are crap
                output.splice(0, 2);

                for (var i = output.length-1; i >= 0; i--) {
                    //console.log('Removing empty dependency [' +
                    //    state.gcc + ' ' + flags.join(' ') + ']');
                    if (output[i] === '') {
                        output.splice(i, 1);
                    }
                }

                fileObj.includes = output;
            }));
        })();

        (function () {
            //debug("CPP");
            var flags = ['-E'];
            flags.push.apply(flags, getFlags(state, fileName, true));
            flags.push(fileName);

            cc(state.gcc, flags, waitFor(function (err, output) {
                if (err) { throw err; }
                fileContent = output;
            }));
        })();

    }).nThen(function (waitFor) {

        Fs.exists(preprocessed, waitFor(function (exists) {
            if (!exists) { return; }

            Fs.unlink(preprocessed, waitFor(function (err) {
                if (err) { throw err; }
            }));
        }));

    }).nThen(function (waitFor) {

        //debug("Preprocess");
        preprocess(fileContent, builder, fileObj, fileName, waitFor(function (err, output) {
            if (err) { throw err; }

            Fs.writeFile(preprocessed, output, waitFor(function (err) {
                if (err) { throw err; }
            }));

            // important, this will prevent the file from also being piped to gcc.
            fileContent = undefined;
        }));

        Fs.exists(outFile, waitFor(function (exists) {
            if (!exists) { return; }

            Fs.unlink(outFile, waitFor(function (err) {
                if (err) { throw err; }
            }));
        }));

    }).nThen(function (waitFor) {

        //debug("CC");
        var flags = ['-c', '-x', 'cpp-output', '-o', outFile];
        flags.push.apply(flags, getFlags(state, fileName, false));
        flags.push(preprocessed);

        cc(state.gcc, flags, waitFor(function (err) {
            if (err) { throw err; }
            fileObj.obj = outFile;
        }), fileContent);

    }).nThen(function (waitFor) {

        debug('\033[2;32mBuilding C object ' + fileName + ' complete\033[0m');

        state.files[fileName] = fileObj;
        var callbacks = currentlyCompiling[fileName];
        delete currentlyCompiling[fileName];

        callbacks.forEach(function (cb) { cb(); });

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

            Fs.stat(fileName, waitFor(function (err, stat) {
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

var removeFile = function (state, fileName, callback)
{
    //debug("remove " + fileName);
    nThen(function (waitFor) {

        // And every file which includes it
        Object.keys(state.files).forEach(function (file) {
            // recursion could remove it
            if (typeof(state.files[file]) === 'undefined') {
                return;
            }

            if (state.files[file].includes.indexOf(fileName) !== -1) {
                removeFile(state, file, waitFor());
            }
        });

        // we'll set the oldmtime on the file to 0 since it's getting rebuilt.
        state.oldmtimes[fileName] = 0;
        var f = state.files[fileName];

        if (typeof(f) === 'undefined') {
            return;
        }

        delete state.files[fileName];

        if (typeof(f.obj) === 'string') {
            Fs.unlink(f.obj, waitFor(function (err) {
                if (err && err.code !== 'ENOENT') {
                    throw err;
                }
            }));
        }

    }).nThen(function (waitFor) {
        callback();
    });
};

var recursiveCompile = function (fileName, builder, tempDir, callback)
{
    // Recursive compilation
    var state = builder.config;
    var doCycle = function (toCompile, parentStack, callback) {
        if (toCompile.length === 0) {
            callback();
            return;
        }

        nThen(function (waitFor) {

            var filefunc = function (file) {
                var stack = [];
                stack.push.apply(stack, parentStack);
                //debug("compiling " + file);
                stack.push(file);

                if (stack.indexOf(file) !== stack.length - 1) {
                    throw new Error("Dependency loops are bad and you should feel bad\n" +
                                    "Dependency stack:\n" + stack.reverse().join('\n'));
                }

                compileFile(file, builder, tempDir, waitFor(function () {
                    var toCompile = [];

                    state.files[file].links.forEach(function (link) {
                        if (link === file) {
                            return;
                        }

                        toCompile.push(link);
                    });

                    doCycle(toCompile, stack, waitFor(function () {
                        if (stack[stack.length - 1] !== file) {
                            throw new Error();
                        }

                        stack.pop();
                    }));
                }));
            };

            for (var file = toCompile.pop(); file; file = toCompile.pop()) {
                filefunc(file);
            }

        }).nThen(function (waitFor) {
            callback();
        });
    };

    doCycle([fileName], [], callback);
};

var getLinkOrder = function (fileName, files) {
    var completeFiles = [];

    var getFile = function (name) {
        var f = files[name];
        //debug('Resolving links for ' + name + ' ' + f);

        for (var i = 0; i < f.links.length; i++) {
            if (f.links[i] === name) {
                continue;
            }

            if (completeFiles.indexOf(f.links[i]) > -1) {
                continue;
            }

            getFile(f.links[i]);
        }

        completeFiles.push(name);
    };

    getFile(fileName);

    return completeFiles;
};

var needsToLink = function (fileName, state) {
    if (typeof(state.oldmtimes[fileName]) !== 'number') {
        return true;
    }

    if (state.oldmtimes[fileName] !== state.mtimes[fileName]) {
        return true;
    }

    var links = state.files[fileName].links;
    for (var i = 0; i < links.length; i++) {
        if (links[i] !== fileName && needsToLink(links[i], state)) {
            return true;
        }
    }

    return false;
};

var makeTime = function () {
    return function () {
        var oldTime = this.time || 0;
        var newTime = this.time = new Date().getTime();
        return newTime - oldTime;
    };
};

var compile = function (file, outputFile, builder, callback) {
    var state = builder.config;
    var tempDir;

    if (!needsToLink(file, state)) {
        process.nextTick(callback);
        return;
    }

    nThen(function (waitFor) {

        tempDir = tmpFile(state);
        Fs.mkdir(tempDir, waitFor(function (err) {
            if (err) { throw err; }
        }));

    }).nThen(function (waitFor) {

        recursiveCompile(file, builder, tempDir, waitFor());

    }).nThen(function (waitFor) {

        var linkOrder = getLinkOrder(file, state.files);
        for (var i = 0; i < linkOrder.length; i++) {
            linkOrder[i] = state.buildDir + '/' + getObjectFile(linkOrder[i]);
        }

        var ldArgs = [state.ldflags, '-o', outputFile, linkOrder, state.libs];
        debug('\033[1;31mLinking C executable ' + file + '\033[0m');

        cc(state.gcc, ldArgs, waitFor(function (err, ret) {
            if (err) { throw err; }
        }));

    }).nThen(function (waitFor) {

        Fs.readdir(tempDir, waitFor(function (err, files) {
            if (err) { throw err; }

            files.forEach(function (file) {
                Fs.unlink(tempDir + '/' + file, waitFor(function (err) {
                    if (err) { throw err; }
                }));
            });
        }));

    }).nThen(function (waitFor) {

        Fs.rmdir(tempDir, waitFor(function (err) {
            if (err) { throw err; }
        }));

    }).nThen(function (waitFor) {

        if (callback) {
            callback();
        }

    });
};

var getStatePrototype = function (params) {
    var base = {
        includeDirs: ['.'],
        files: {},
        mtimes: {},

        cflags: [],
        ldflags: [],
        libs: [],

        rebuildIfChanges: [],
        rebuildIfChangesHash: undefined,

        tempDir: '/tmp',

        systemName: 'linux'
    };

    for (var key in params) {
        if (params.hasOwnProperty(key)) {
            if (typeof params[key] !== 'object') {
                base[key] = params[key];
            }
        }
    }

    return base;
};

/**
 * Get a copy of process.env with a few entries which are constantly changing removed.
 * This prevents isStaleState from returning true every time one builds in a different
 * window.
 */
var normalizedProcessEnv = function () {
    var out = process.env;
    delete out.WINDOWID;
    delete out.OLDPWD;
    return out;
};

var getRebuildIfChangesHash = function (rebuildIfChanges, callback) {
    var ret = false;
    var hash = Crypto.createHash('sha256');
    var rebIfChg = [];

    nThen(function (waitFor) {

        rebuildIfChanges.forEach(function (fileName, i) {
            Fs.readFile(fileName, waitFor(function (err, ret) {
                if (err) { throw err; }

                rebIfChg[i] = ret;
            }));
        });

        hash.update(JSON.stringify(normalizedProcessEnv()));

    }).nThen(function (waitFor) {

        rebIfChg.forEach(function (data) {
            hash.update(data);
        });

        callback(hash.digest('hex'));

    });
};

var throwIfErr = function (err) { if (err) { throw err; } };

var probeCompiler = function (state, callback) {
    nThen(function (waitFor) {
        var compilerType = state.compilerType = {
            isLLVM: false,
            isClang: false,
            isGCC: false,
            version: undefined
        };
        compiler(state.gcc, ['-v'], waitFor(function (ret, out, err) {
            if (ret !== 0) { throw new Error("Failed to probe compiler ret[" + ret + "]\n" + err); }
            if (/Apple LLVM version /.test(err)) {
                compilerType.isLLVM = true;
                if (/clang/.test(err)) {
                    // Apple LLVM version 6.1.0 (clang-602.0.53) (based on LLVM 3.6.0svn)
                    // Target: x86_64-apple-darwin14.4.0
                    // Thread model: posix
                    compilerType.isClang = true;
                    compilerType.version = err.match(/Apple LLVM version ([^ ]+) /)[1];
                } else if (/gcc version /.test(err)) {
                    // Using built-in specs.
                    // Target: i686-apple-darwin11
                    // Configured with: /private/var/tmp/llvmgcc42/llvmgcc42.......
                    // Thread model: posix
                    // gcc version 4.2.1 (Based on Apple Inc. build 5658) (LLVM build 2336.11.00)
                    compilerType.isGCC = true;
                    compilerType.version = err.match(/gcc version ([^ ]+) /)[1];
                }
            } else if (/clang version /.test(err)) {
                // FreeBSD clang version 3.0 (tags/RELEASE_30/final 145349) 20111210
                // Target: x86_64-unknown-freebsd10.0
                // Thread model: posix

                // clang version 3.2 (trunk)
                // Target: x86_64-unknown-linux-gnu
                // Thread model: posix
                compilerType.isLLVM = true;
                compilerType.isClang = true;
                compilerType.version = err.match(/clang version ([^ ]+) /)[1];
            } else if (/gcc version /.test(err)) {
                compilerType.isGCC = true;
                compilerType.version = err.match(/gcc version ([^ ]+) /)[1];
            }
            console.log(JSON.stringify(compilerType));
        }));
    }).nThen(callback);
};

process.on('exit', function () {
    console.log("Total build time: " + Math.floor(process.uptime() * 1000) + "ms.");
});

var stage = function (st, builder, waitFor) {
    builder.waitFor = waitFor;
    st(builder, waitFor);
};

var configure = module.exports.configure = function (params, configFunc) {

    // Track time taken for various steps
    var time = makeTime();
    time();

    if (typeof(params.systemName) !== 'string') {
        throw new Error("system not specified");
    }

    params.buildDir = params.buildDir || 'build_' + params.systemName;

    var version;
    var state;
    var builder;
    var buildStage = function () {};
    var testStage = function () {};
    var packStage = function () {};
    var successStage = function () {};
    var failureStage = function () {};
    var completeStage = function () {};

    nThen(function (waitFor) {

        // make the build directory
        Fs.exists(params.buildDir, waitFor(function (exists) {
            if (exists) { return; }

            Fs.mkdir(params.buildDir, waitFor(function (err) {
                if (err) { throw err; }
            }));
        }));

    }).nThen(function (waitFor) {

        // read out the state if it exists
        Fs.exists(params.buildDir + '/state.json', waitFor(function (exists) {
            if (!exists) { return; }

            Fs.readFile(params.buildDir + '/state.json', waitFor(function (err, ret) {
                if (err) { throw err; }

                state = JSON.parse(ret);
            }));
        }));

    }).nThen(function (waitFor) {
        if (process.env["CJDNS_RELEASE_VERSION"]) {
            version = '' + process.env["CJDNS_RELEASE_VERSION"];
        } else {
            GetVersion(waitFor(function(err, data) {
                if (err === null) {
                    version = '' + data;
                    version = version.replace(/(\r\n|\n|\r)/gm, "");
                } else {
                    version = 'unknown';
                }
            }));
        }
    }).nThen(function (waitFor) {

        if (!state || !state.rebuildIfChanges) {
            // no state
            state = undefined;
        } else {
            getRebuildIfChangesHash(state.rebuildIfChanges, waitFor(function (rich) {
                if (rich !== state.rebuildIfChangesHash) {
                    debug("rebuildIfChanges changed, rebuilding");
                    state = undefined;
                }
            }));
        }

    }).nThen(function (waitFor) {

        debug("Initialize " + time() + "ms");

        // Do the configuration step
        if (state) {
            builder = mkBuilder(state);
            builder.config.version = version;
            return;
        }

        state = getStatePrototype(params);
        builder = mkBuilder(state);
        builder.config.version = version;
        probeCompiler(state, waitFor());

    }).nThen(function (waitFor) {

        configFunc(builder, waitFor);

    }).nThen(function (waitFor) {

        debug("Configure " + time() + "ms");

        if (state.rebuildIfChangesHash) {
            return;
        }

        if (state.rebuildIfChanges.indexOf(module.parent.filename) === -1) {
            // Always always rebuild if the makefile was changed.
            state.rebuildIfChanges.push(module.parent.filename);
        }

        getRebuildIfChangesHash(state.rebuildIfChanges, waitFor(function (rich) {
            state.rebuildIfChangesHash = rich;
        }));

    }).nThen(function (waitFor) {

        state.oldmtimes = state.mtimes;
        state.mtimes = {};

        Object.keys(state.oldmtimes).forEach(function (fileName) {
            Fs.stat(fileName, waitFor(function (err, stat) {
                if (err) {
                    if (err.code === 'ENOENT') {
                        // Doesn't matter as long as it's not referenced...
                        debug("File [" + fileName + "] was removed");
                        delete state.files[fileName];

                        return;
                    } else {
                        throw err;
                    }
                }

                state.mtimes[fileName] = stat.mtime.getTime();
                if (state.oldmtimes[fileName] !== stat.mtime.getTime()) {
                    debug(fileName + ' is out of date, rebuilding');
                    removeFile(state, fileName, waitFor());
                }
            }));
        });

    }).nThen(function (waitFor) {

        debug("Scan for out of date files " + time() + "ms");

    }).nThen(function (waitFor) {

        stage(buildStage, builder, waitFor);

    }).nThen(function (waitFor) {

        debug("Compile " + time() + "ms");

        var allFiles = {};

        Object.keys(state.files).forEach(function (fileName) {
            allFiles[fileName] = 1;

            state.files[fileName].includes.forEach(function (fileName) {
                allFiles[fileName] = 1;
            });
        });

        Object.keys(allFiles).forEach(function (fileName) {
            var omt = state.oldmtimes[fileName];
            if (omt > 0 && omt === state.mtimes[fileName]) {
                return;
            }

            builder.rebuiltFiles.push(fileName);
        });

    }).nThen(function (waitFor) {

        builder.tests.forEach(function (test) {
            test(waitFor(function (output, failure) {
                debug(output);

                if (failure) {
                    builder.failure = true;
                }
            }));
        });

    }).nThen(function (waitFor) {

        if (builder.linters.length === 0) {
            return;
        }

        debug("Checking codestyle");

        var sema = Semaphore.create(64);

        builder.rebuiltFiles.forEach(function (fileName) {
            sema.take(waitFor(function (returnAfter) {
                Fs.readFile(fileName, waitFor(function (err, ret) {
                    if (err) { throw err; }

                    ret = ret.toString('utf8');
                    nThen(function (waitFor) {

                        builder.linters.forEach(function (linter) {
                            linter(fileName, ret, waitFor(function (out, isErr) {
                                if (isErr) {
                                    debug("\033[1;31m" + out + "\033[0m");
                                    builder.failure = true;
                                }
                            }));
                        });

                    }).nThen(returnAfter(waitFor()));
                }));
            }));
        });

    }).nThen(function (waitFor) {

        stage(testStage, builder, waitFor);

    }).nThen(function (waitFor) {

        if (builder.failure) { return; }

        debug("Test " + time() + "ms");

        builder.executables.forEach(function (array) {
            if (array[1] === array[0]) { return; }
            Fs.rename(array[0], array[1], waitFor(function (err) {
                // TODO(cjd): It would be better to know in advance whether to expect the file.
                if (err && err.code !== 'ENOENT') {
                    throw err;
                }
            }));
        });

    }).nThen(function (waitFor) {

        if (builder.failure) { return; }

        stage(packStage, builder, waitFor);

    }).nThen(function (waitFor) {

        if (builder.failure) { return; }

        debug("Pack " + time() + "ms");

        getMTimes(state.files, state.mtimes, waitFor(function (err, mtimes) {
            if (err) { throw err; }

            state.mtimes = mtimes;
            debug("Get mtimes " + time() + "ms");
        }));

    }).nThen(function (waitFor) {

        if (builder.failure) { return; }

        // save state
        var stateJson = JSON.stringify(state, null, '  ');
        Fs.writeFile(state.buildDir + '/state.json', stateJson, waitFor(function (err) {
            if (err) { throw err; }

            debug("Save State " + time() + "ms");
        }));

    }).nThen(function (waitFor) {

        if (builder.failure) { return; }

        stage(successStage, builder, waitFor);

    }).nThen(function (waitFor) {

        if (!builder.failure) { return; }

        stage(failureStage, builder, waitFor);

    }).nThen(function (waitFor) {

        stage(completeStage, builder, waitFor);

    });

    var out = {
        build:    function (x) { buildStage = x;    return out; },
        test:     function (x) { testStage = x;     return out; },
        pack:     function (x) { packStage = x;     return out; },
        failure:  function (x) { failureStage = x;  return out; },
        success:  function (x) { successStage = x;  return out; },
        complete: function (x) { completeStage = x; return out; },
    };

    return out;
};
