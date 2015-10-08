var Os = require('os');
var Fs = require('fs');
var Spawn = require('child_process').spawn;
var JobQueue = require('./JobQueue');
var Common = require('./Common');

var BUILD_DIR = 'jsbuild';
var OBJ_DIR = BUILD_DIR + '/objects_internal';

var getCompiler = function(cc, config) {
    return function(compileCall, onComplete) {
        console.log('\033[2;32mCompiling Test ' + compileCall.outFile + '\033[0m');
        /*console.log('cc -o ' + compileCall.outFile + ' -c ' + compileCall.inFile + ' ' +
                      compileCall.args.join(' ')); */

        var args = [];
        args.push.apply(args, compileCall.args);
        args.push(config.flag.outputExe + compileCall.outFile, compileCall.inFile);
        args.push(BUILD_DIR + '/' + 'libnacl' + config.ext.lib);

        cflags = process.env['CFLAGS'];
        if (cflags) {
            flags = cflags.split(' ');
            flags.forEach(function(flag) {
                args.push(flag);
            });
        }
        cc(args, function(retCode, stdout, stderr) {
            if (stderr !== '') {
                console.log(stderr);
            }
            if (retCode !== 0) {
                throw new Error('failed to compile');
            }
            onComplete();
        });
    };
};

var buildQueue = function(plan, config, onComplete) {
    var workers = [0];
    var primitivesByOp = {};
    plan.PLAN_IMPLEMENTATIONS.forEach(function(opi) {
        primitivesByOp[opi[0]] = primitivesByOp[opi[0]] || [];
        primitivesByOp[opi[0]].push(opi[1]);
    });

    Fs.readdir('tests', function(err, tests) {
        if (err) {
            throw err;
        }
        var queue = [];
        tests.forEach(function(test) {
            if (!test.match(/\.c$/)) {
                return;
            }
            var longName = 'tests/' + test;
            var args = [
                config.flag.include + Common.INCLUDE,
                config.flag.include + Common.INCLUDE_INTERNAL
            ];
            // Search for #include <$operation> with primitive unspecified.
            // Tests like this will be run for every primitive of that operation.
            workers[0]++;
            Fs.readFile(longName, function(err, dat) {
                if (err) {
                    throw err;
                }
                var includes = dat.toString().match(/#include [^\n]*\n/g);
                var added = false;
                var outName = Common.TESTS + '/' + test.replace(/\..*$/, '');
                includes.forEach(function(include) {
                    include = include.replace(/^.*<|>.*\n$/g, '');
                    include = include.replace(/^[^"]*\"|\"[^"]*/g, '');
                    include = include.replace('.h', '');
                    if (include in primitivesByOp) {
                        primitivesByOp[include].forEach(function(prim) {
                            var a = [];
                            a.push.apply(a, args);
                            a.push(config.flag.include + Common.INCLUDE_INTERNAL + '/' + include + '_' + prim);
                            queue.push({
                                inFile: longName,
                                outFile: outName + '_' + prim,
                                args: a
                            });
                            added = true;
                        });
                        //console.log(longName + ': ' + include + ' -> ' + primitivesByOp[include]);
                    }
                    //console.log(include);
                });
                if (!added) {
                    queue.push({
                        inFile: longName,
                        outFile: outName,
                        args: args
                    });
                }
                Common.done(workers, function() {
                    onComplete(queue);
                });
            });
        });
    });
};

var getRunner = function() {
    return function(entry, onComplete) {
        var expectedFile = 'tests/' + entry.outFile.replace(/^.*\//, '') + '.out';
        Fs.exists(expectedFile, function(exists) {
            if (!exists) {
                expectedFile = expectedFile.replace(/_[^_]*/, '') + '.out';
            }
            console.log('\033[2;36mRunning Test ' + expectedFile + '\033[0m');
            Fs.readFile(expectedFile, function(err, expected) {
                if (err) {
                    throw err;
                }
                expected = expected.toString();

                var exe = Spawn(entry.outFile);
                var out = '';
                exe.stderr.on('data', function(dat) {
                    out += dat.toString();
                });
                exe.stdout.on('data', function(dat) {
                    out += dat.toString();
                });
                exe.on('close', function(ret) {
                    var fail = false;
                    if (out !== expected) {
                        console.log("mismatch:\nexpected: [" + expected + "]\ngot: [" + out + "]");
                        fail = true;
                    }
                    if (ret !== 0) {
                        console.log("test returned non-zero");
                        fail = true;
                    }
                    if (fail) {
                        throw new Error(entry.inFile + " failed");
                    }
                    onComplete();
                });
            });
        });
    };
};

var cpus = Os.cpus(); // workaround, nodejs seems to be broken on openbsd (undefined result after second call)
var PROCESSORS = Math.floor((typeof cpus === 'undefined' ? 1 : cpus.length) * 1.25);

var PROCESSORS_MAX_16 = (PROCESSORS > 16) ? 16 : PROCESSORS;

var compileTests = function(cc, config, queue, onComplete) {
    JobQueue.run(queue, getCompiler(cc, config), PROCESSORS_MAX_16, onComplete);
};

var runTests = function(queue, onComplete) {
    JobQueue.run(queue, getRunner(), PROCESSORS_MAX_16, onComplete);
};

module.exports.run = function(cc, config, plan, onComplete) {
    buildQueue(plan, config, function(queue) {
        compileTests(cc, config, queue, function() {
            onComplete();
            //runTests(queue, onComplete);
        });
    });
};
