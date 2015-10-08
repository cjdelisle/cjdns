var Os = require('os');
var Fs = require('fs');
var JobQueue = require('./JobQueue');

var BUILD_DIR = 'jsbuild';
var OBJ_DIR = BUILD_DIR + '/objects_internal';

var done = function(workers, onComplete) {
    if (!workers[0]) {
        throw new Error();
    }
    if (!--workers[0]) {
        onComplete();
    }
};

/** Replace all instances of op in prototype file. */
var replaceOp = function(proto, op, func) {
    proto.forEach(function(line) {
        if ((typeof(line) !== "undefined") && (line !== "") && (line.indexOf(op) !== -1)) {
            func(line.replace(/(\r\n|\n|\r)/gm, ""));
        }
    });
};

var writeOPIHeader = function(protos, opi, onComplete) {
    var op = opi[0].replace(/(\r\n|\n|\r)/gm, "");
    var prim = opi[1].replace(/(\r\n|\n|\r)/gm, "");
    var impl = opi[2].replace(/(\r\n|\n|\r)/gm, "");

    var out = [
        "#ifndef " + op + "_" + prim + "_H",
        "#define " + op + "_" + prim + "_H",
        ""
    ];

    var path = opi.join('/');
    var o_p_i = path.replace(/[\.-\/]/g, '_').replace(/(\r\n|\n|\r)/gm, "");
    Fs.readFile(path + '/api.h', function(err, ret) {
        if (err) {
            throw err;
        }
        ret = ret.toString();
        out.push(ret.replace(/[ \\t]CRYPTO_/g, ' ' + o_p_i + '_'));

        var f = function(line) {
            out.push(line.replace(op, o_p_i));
        };

        // C++
        out.push("#ifdef __cplusplus");
        out.push("#include <string>");
        replaceOp(protos['PROTOTYPES.cpp'], op, f);
        out.push("extern \"C\" {");
        out.push("#endif");
        replaceOp(protos['PROTOTYPES.c'], op, f);
        out.push("#ifdef __cplusplus");
        out.push("}");
        out.push("#endif");
        out.push("");

        replaceOp(protos.MACROS, op, function(line) {
            var mopi = line.replace(op, o_p_i);
            var mop = line.replace(op, op + "_" + prim);
            out.push("#define" + " " + mop + " " + mopi);
        });

        out.push("#define " + op + "_" + prim + "_IMPLEMENTATION " + "\"" + path + "\"");
        out.push("#ifndef " + o_p_i + "_VERSION");
        out.push("#define " + o_p_i + "_VERSION " + "\"-\"");
        out.push("#endif");
        out.push("#define " + op + "_" + prim + "_VERSION " + o_p_i + "_VERSION");
        out.push("");
        out.push("#endif");

        var outPath = BUILD_DIR + "/include/" + op + "_" + prim + ".h";
        var outText = out.join("\n");

        Fs.writeFile(outPath, outText, function(err) {
            if (err) {
                throw err;
            }
            onComplete();
        });
    });
};

/**
 * Write the operation header for this OPI.
 * Because an OPI is not aware of it's own implementation or
 * primitive names, we need a special header which will be used
 * so that it's generic names can be mapped to names based on the
 * primitive and implementation.
 */
var writeOPHeader = function(protos, opi, onComplete) {
    var o_p = (opi[0] + "_" + opi[1]).replace(/(\r\n|\n|\r)/gm, "");
    var out = [
        "#ifndef " + opi[0] + "_H",
        "#define " + opi[0] + "_H",
        "",
        '#include "' + o_p + '.h"',
        ""
    ];
    replaceOp(protos.MACROS, opi[0], function(line) {
        out.push("#define " + line + " " + o_p + line.replace(opi[0], ""));
    });
    out.push("#define " + opi[0] + "_PRIMITIVE " + "\"" + opi[1] + "\"");
    out.push("#define " + opi[0] + "_IMPLEMENTATION " + o_p + "_IMPLEMENTATION");
    out.push("#define " + opi[0] + "_VERSION " + o_p + "_VERSION");
    out.push("");
    out.push("#endif");

    var dir = BUILD_DIR + "/include_internal/" + o_p;
    var path = dir + "/" + opi[0] + ".h";
    var write = function() {
        Fs.writeFile(path, out.join("\n"), function(err) {
            if (err) {
                throw err;
            }
            onComplete();
        });
    };

    Fs.exists(dir, function(ret) {
        if (ret) {
            write();
            return;
        }
        Fs.mkdir(dir, function(err) {
            if (err) {
                throw err;
            }
            write();
        });
    });
};

var allOpPrims = [];
var runOPI = function(protos, opi, onComplete) {
    var workers = [0];

    workers[0]++;
    writeOPIHeader(protos, opi, function() {
        done(workers, onComplete);
    });

    var o_p = opi[0] + "_" + opi[1];
    if (allOpPrims.indexOf(o_p) === -1) {
        allOpPrims.push(o_p);
        workers[0]++;
        writeOPHeader(protos, opi, function() {
            done(workers, onComplete);
        });
    }
};

var genIncludes = function(protos, opis, onComplete) {
    var workers = [0];
    opis.forEach(function(opi) {
        workers[0]++;
        runOPI(protos, opi, function() {
            done(workers, onComplete);
        });
    });
};

var getPrototypes = function(onComplete) {
    var workers = [0];
    var protos = {};
    var getProto = function(name) {
        workers[0]++;
        Fs.readFile(name, function(err, ret) {
            if (err) {
                throw err;
            }
            protos[name] = ret.toString().split('\n');
            done(workers, function() {
                onComplete(protos);
            });
        });
    };
    getProto('MACROS');
    getProto('PROTOTYPES.cpp');
    getProto('PROTOTYPES.c');
};

var getCompiler = function(cc, config) {
    return function(compileCall, onComplete) {
        console.log('\033[2;32mCompiling ' + compileCall.outFile + '\033[0m');
        /*console.log('cc -o ' + compileCall.outFile + ' -c ' + compileCall.inFile + ' ' +
                      compileCall.args.join(' ')); */
        var args = [];
        args.push.apply(args, compileCall.args);
        args.push(config.flag.compileOnly, config.flag.outputObj + compileCall.outFile, compileCall.inFile);
        cflags = process.env['CFLAGS'];
        if (cflags) {
            flags = cflags.split(' ');
            flags.forEach(function(flag) {
                args.push(flag);
            });
        }
        cc(args, function(retCode, stdout, stderr) {
            if (stdout !== '') {
                console.log(stdout);
            }
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

var cpus = Os.cpus(); // workaround, nodejs seems to be broken on openbsd (undefined result after second call)
var PROCESSORS = Math.floor((typeof cpus === 'undefined' ? 1 : cpus.length) * 1.25);

var PROCESSORS_MAX_16 = (PROCESSORS > 16) ? 16 : PROCESSORS;

var compileFiles = function(compileQueue, cc, config, onComplete) {
    JobQueue.run(compileQueue, getCompiler(cc, config), PROCESSORS_MAX_16, onComplete);
};

var buildCompileQueue = function(impls, config, onComplete) {
    var compileQueue = [];
    var i = 0;
    impls.forEach(function(impl) {
        var path = impl.join('/');
        Fs.readdir(path, function(err, files) {
            if (err) {
                throw err;
            }
            files.forEach(function(file) {
                if (!file.match(/\.[csS]$/)) {
                    return;
                }
                var dotO = OBJ_DIR + '/' + impl.join('_').replace(/\//g, '_') + '_' + file + config.ext.obj;
                var args = [
                    config.flag.include + BUILD_DIR + '/include',
                    config.flag.include + BUILD_DIR + '/include_internal',
                    config.flag.include + BUILD_DIR + '/include_internal/' + impl[0] + '_' + impl[1]
                ];
                compileQueue.push({
                    args: args,
                    inFile: path + '/' + file,
                    outFile: dotO
                });
            });
            if (++i === impls.length) {
                onComplete(compileQueue);
            }
        });
    });
};

var compile = function(impls, cc, config, onComplete) {
    buildCompileQueue(impls, config, function(compileQueue) {
        compileFiles(compileQueue, cc, config, onComplete);
    });
};

var archive = function(ar, arName, config, onComplete) {
    Fs.readdir(OBJ_DIR, function(err, files) {
        if (err) {
            throw err;
        }
        var args = [];
        // NOTE: if ar is provided with the 's' argument, it will provide the same roll as ar + ranlib.
        args.push('scr', BUILD_DIR + '/libnacl' + config.ext.lib);

        console.log('\033[1;31mLinking static C library ' + args[1] + '\033[0m');
        files.forEach(function(file) {
            args.push(OBJ_DIR + '/' + file);
        });
        //console.log('ar ' + args.join(' \\\n'));
        ar(args, function(retcode, out) {
            if (out !== '') {
                console.log(out);
            }
            if (retcode) {
                throw new Error('ar returned ' + retcode);
            }
            onComplete();
        });
    });
};

module.exports.run = function(plan, cc, config, ar, arName, onComplete) {
    var impls = plan.PLAN_IMPLEMENTATIONS;
    getPrototypes(function(protos) {
        //console.log(protos);
        genIncludes(protos, impls, function() {
            console.log("implementations generated");
            compile(impls, cc, config, function() {
                archive(ar, arName, config, onComplete);
            });
        });
    });
};
