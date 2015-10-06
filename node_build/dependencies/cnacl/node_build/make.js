var Os = require('os');
var Fs = require('fs');
var Spawn = require('child_process').spawn;
var AbiName = require('./AbiName');
var PlanRunner = require('./PlanRunner');
var TestRunner = require('./TestRunner');
var RandomBytes = require('./RandomBytes');
var Common = require('./Common');

// on BSD and iphone systems, os.cpus() is not reliable so if it
// returns undefined let's just assume 1
var WORKERS = Math.floor((typeof Os.cpus() == 'undefined' ? 1 : Os.cpus().length));

var GCC = process.env['CC'] || 'gcc';
var AR = process.env['AR'] || 'ar';

var cc = function(args, onComplete, noArg) {
    if (noArg) {
        throw new Error();
    }
    cflags = process.env['CFLAGS'];
    if (cflags) {
        flags = cflags.split(' ');
        flags.forEach(function(flag) {
            args.push(flag);
        });
    }
    var exe = Spawn(GCC, args);
    var err = '';
    exe.stderr.on('data', function(dat) {
        err += dat.toString();
    });
    exe.on('error', function(err) {
        // handle the error safely
        console.log(args);
        console.log(err);
    });
    exe.on('close', function(ret) {
        onComplete(ret, err);
    });
};

var ar = function(args, onComplete) {
    var exe = Spawn(AR, args);
    var out = '';
    exe.stderr.on('data', function(dat) {
        out += dat.toString();
    });
    exe.stdout.on('data', function(dat) {
        out += dat.toString();
    });
    exe.on('error', function(err) {
        // handle the error safely
        console.log(args);
        console.log(err);
    });
    exe.on('close', function(ret) {
        onComplete(ret, out);
    });
};

var getPlan = function(abiName, config) {
    if (config.systemName === 'darwin') {
        abiName = 'apple_' + abiName;
    }
    var planPath = 'node_build/plans/' + abiName + '_plan.json';
    if (!Fs.existsSync(planPath)) {
        // TODO
        throw new Error("build with no premade plan, TODO: generate one");
    }
    console.log('Using premade plan at [' + planPath + ']');
    var text = Fs.readFileSync(planPath);
    return JSON.parse(text);
};

var writeTypesHeaders = function(plan, onComplete) {
    console.log('Creating system specific integer type headers');
    var workers = 0;
    var done = function() {
        if (!workers) {
            throw new Error();
        }
        if (!--workers) {
            onComplete();
        }
    };
    out = [
        "#ifndef crypto_types_h",
        "#define crypto_types_h"
    ];
    plan.PLAN_TYPES.forEach(function(type) {
        out.push(type);
    });
    out.push("#endif");
    workers++;
    Fs.writeFile(Common.INCLUDE_INTERNAL + '/crypto_types.h', out.join('\n'), done);

    [
        "crypto_uint8.h",
        "crypto_int8.h",
        "crypto_uint16.h",
        "crypto_int26.h",
        "crypto_uint32.h",
        "crypto_int32.h",
        "crypto_uint64.h",
        "crypto_int64.h"
    ].forEach(function(header) {
        workers++;
        Fs.writeFile(Common.INCLUDE_INTERNAL + '/' + header,
            '#include <crypto_types.h>',
            done);
    });
};

var runTests = function(cc, plan, onComplete) {
    TestRunner.run(cc, plan, onComplete);
};

var beginBuild = function(compiler, plan, callback) {
    console.log('beginning build');
    PlanRunner.run(plan, compiler, ar, AR, function() {
        runTests(compiler, plan, function() {
            console.log('done');
            callback();
        });
    });
};

var main = module.exports.build = function(compiler, config, callback) {
    console.log("Creating directories");
    Common.init();

    RandomBytes.run(compiler, function() {
        console.log("Getting system type");
        AbiName.get(compiler, function(abiName) {
            console.log('System is [' + abiName + ']');
            var plan = getPlan(abiName, config);
            writeTypesHeaders(plan, function() {
                beginBuild(compiler, plan, callback);
            });
        });
    });
};

if (!module.parent) {
    var conf = {
        systemName: process.platform
    };
    main(function(cc, conf) {});
}
