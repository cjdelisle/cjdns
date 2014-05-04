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
var Fs = require('fs');
var nThen = require('nthen');
var Codestyle = require('./Codestyle');
var Cp = require('./Cp');
var Spawn = require('child_process').spawn;
var Extend = require('node.extend');
var Os = require('os');
var FindPython2 = require('./FindPython2');
var CanCompile = require('./CanCompile');

// ['linux','darwin','sunos','win32','freebsd']
var SYSTEM = process.platform;
var CROSS = process.env['CROSS'] || '';
var GCC = process.env['CC'] || 'gcc';

var BUILDDIR = process.env['BUILDDIR'];
if (BUILDDIR === undefined) {
    BUILDDIR = 'build_'+SYSTEM;
}

var OPTIMIZE = '-O2';

// on BSD and iphone systems, os.cpus() is not reliable so if it
// returns undefined, let's just assume 1
var WORKERS = Math.floor((typeof Os.cpus() == 'undefined' ? 1 : Os.cpus().length) * 1.25);

process.on('exit', function () {
    console.log("Total build time: " + Math.floor(process.uptime() * 1000) + "ms.");
});

var Builder = require('./builder');

Builder.configure({
    rebuildIfChanges: Fs.readFileSync(__filename).toString('utf8') + JSON.stringify(process.env),
    buildDir: BUILDDIR
}, function(builder, waitFor) {

    builder.config.systemName = SYSTEM;
    builder.config.gcc = GCC;

    builder.config.tempDir = '/tmp';
    builder.config.useTempFiles = true;
    builder.config.cflags.push.apply(builder.config.cflags, [
        '-std=c99',
        '-Wall',
        '-Wextra',
        '-Werror',
        '-Wno-pointer-sign',
        '-pedantic',
        '-D',builder.config.systemName + '=1',
        '-Wno-unused-parameter',
        '-Wno-unused-result',

        '-D','HAS_BUILTIN_CONSTANT_P',

        '-g',

        // f4 = 16 peers max, fixed width 4 bit
        // f8 = 241 peers max, fixed width 8 bit
        // v3x5x8 = 256 peers max, variable width, 3, 5 or 8 bits plus 1 or 2 bits of prefix
        // v4x8 = 256 peers max, variable width, 4, or 8 bits plus 1 bit prefix
        '-D','NumberCompress_TYPE=v3x5x8',

        // disable for speed, enable for safety
        '-D','Identity_CHECK=1',
        '-D','Allocator_USE_CANARIES=1',
        '-D','PARANOIA=1'
    ]);

    var logLevel = process.env['Log_LEVEL'] || 'DEBUG';
    builder.config.cflags.push('-D','Log_'+logLevel);
    if (process.env['NO_PIE'] === undefined) {
        builder.config.cflags.push('-fPIE');
    }
    if (process.env['TESTING']) { builder.config.cflags.push('-D', 'TESTING=1'); }
    if (SYSTEM === 'win32') {
        builder.config.cflags.push(
            '!-fPIE',
            '!-pie',
            '-Wno-format'
        );
        builder.config.libs.push(
            '-lssp'
        );
    } else if (SYSTEM === 'linux') {
        builder.config.ldflags.push(
            '-Wl,-z,relro,-z,now,-z,noexecstack'
        );
        builder.config.cflags.push(
            '-DHAS_ETH_INTERFACE=1'
        );
    }

    if (process.env['NO_PIE'] === undefined) {
        builder.config.ldflags.push(
            '-pie'
        );
    }

    if (/.*clang.*/.test(GCC) || SYSTEM === 'darwin') {
        // blows up when preprocessing before js preprocessor
        builder.config.cflags.push(
            '-Wno-invalid-pp-token',
            '-Wno-dollar-in-identifier-extension',
            '-Wno-newline-eof',
            '-Wno-unused-value',

            // lots of places where depending on preprocessor conditions, a statement might be
            // a case of if (1 == 1)
            '-Wno-tautological-compare'
        );
    }

    // Install any user-defined CFLAGS. Necessary if you are messing about with building cnacl
    // with NEON on the BBB
    cflags = process.env['CFLAGS'];
    if (cflags) {
        flags = cflags.split(' ');
        flags.forEach(function(flag) {
            builder.config.cflags.push(flag);
        });
    }

    // We also need to pass various architecture/floating point flags to GCC when invoked as
    // a linker.
    ldflags = process.env['LDFLAGS'];
    if (ldflags) {
        flags = ldflags.split(' ');
        flags.forEach(function(flag) {
            builder.config.ldflags.push(flag);
        });
    }

    if (/.*android.*/.test(GCC)) {
        builder.config.cflags.push(
            '-Dandroid=1'
        );
    }

    CanCompile.check(builder,
                     'int main() { return 0; }',
                     [ builder.config.cflags, '-flto', '-x', 'c' ],
                     function (err, can) {
        if (can) {
            console.log("Compiler supports link time optimization");
            builder.config.ldflags.push(
                '-flto',
                OPTIMIZE
            );
            // No optimization while building since actual compile happens during linking.
            builder.config.cflags.push('-O0');
        } else {
            console.log("Link time optimization not supported [" + err + "]");
            builder.config.cflags.push(OPTIMIZE);
        }
    });

    var uclibc = /uclibc/i.test(GCC);
    var libssp = process.env['SSP_SUPPORT'] == 'y';
    if (!uclibc || libssp) {
        builder.config.cflags.push(
            // Broken GCC patch makes -fstack-protector-all not work
            // workaround is to give -fno-stack-protector first.
            // see: https://bugs.launchpad.net/ubuntu/+source/gcc-4.5/+bug/691722
            '-fno-stack-protector',
            '-fstack-protector-all',
            '-Wstack-protector'
        );
        if (uclibc) { builder.config.libs.push('-lssp'); }
    } else {
        console.log("Stack Smashing Protection (security feature) is disabled");
    }

    // Build dependencies
    nThen(function (waitFor) {
        Fs.exists(BUILDDIR+'/dependencies', waitFor(function (exists) {
            if (exists) { return; }
            console.log("Copy dependencies");
            Cp('./node_build/dependencies', BUILDDIR+'/dependencies', waitFor());
        }));
    }).nThen(function (waitFor) {
        builder.config.libs.push(
            BUILDDIR+'/dependencies/cnacl/jsbuild/libnacl.a'
        );
        builder.config.includeDirs.push(
            BUILDDIR+'/dependencies/cnacl/jsbuild/include/'
        );
        Fs.exists(BUILDDIR+'/dependencies/cnacl/jsbuild/libnacl.a', waitFor(function (exists) {
            if (exists) { return; }
            console.log("Build NaCl");
            var cwd = process.cwd();
            process.chdir(BUILDDIR+'/dependencies/cnacl/');
            var NaCl = require(process.cwd() + '/node_build/make.js');
            NaCl.build(function (args, callback) {
                if (builder.config.systemName !== 'win32') { args.unshift('-fPIC'); }
                args.unshift('-O2', '-fomit-frame-pointer');
                cflags = process.env['CFLAGS'];
                if (cflags) {
                    flags = cflags.split(' ');
                    flags.forEach(function(flag) {
                        args.push(flag);
                    });
                }
                builder.cc(args, callback);
            }, waitFor(function () {
                process.chdir(cwd);
            }));
        }));

    }).nThen(function (waitFor) {
        builder.config.libs.push(
            BUILDDIR+'/dependencies/libuv/out/Release/libuv.a'
        );
        if (!/.*android.*/.test(GCC)) {
            builder.config.libs.push(
                '-lpthread'
            );
        }
        if (builder.config.systemName === 'win32') {
            builder.config.libs.push(
                '-lws2_32',
                '-lpsapi',   // GetProcessMemoryInfo()
                '-liphlpapi' // GetAdapterAddresses()
            );
        } else if (builder.config.systemName === 'linux' && !/.*android.*/.test(GCC)) {
            builder.config.libs.push(
                '-lrt' // clock_gettime()
            );
        } else if (builder.config.systemName === 'darwin') {
            builder.config.libs.push(
                '-framework', 'CoreServices'
            );
        } else if (builder.config.systemName === 'freebsd') {
            builder.config.libs.push(
                '-lkvm'
            );
        }
        builder.config.includeDirs.push(
            BUILDDIR+'/dependencies/libuv/include/'
        );
        var libuvBuilt;
        var python;
        nThen(function (waitFor) {
            Fs.exists(BUILDDIR+'/dependencies/libuv/out/Release/libuv.a', waitFor(function (exists) {
                if (exists) { libuvBuilt = true; }
            }));
        }).nThen(function (waitFor) {
            if (libuvBuilt) { return; }
            FindPython2.find(builder.tmpFile(), waitFor(function (err, pythonExec) {
                if (err) { throw err; }
                python = pythonExec;
            }));
        }).nThen(function (waitFor) {
            if (libuvBuilt) { return; }
            console.log("Build Libuv");
            var cwd = process.cwd();
            process.chdir(BUILDDIR+'/dependencies/libuv/');

            var args = ['./gyp_uv.py'];
            var env = Extend({}, process.env);
            env.CC = builder.config.gcc;
            if (env.TARGET_ARCH) {
                args.push('-Dtarget_arch='+env.TARGET_ARCH);
            }
            if (/.*android.*/.test(GCC)) { args.push('-Dtarget_arch=arm', '-DOS=android'); }
            var gyp = Spawn(python, args, {env:env});
            gyp.stdout.on('data', function(dat) { process.stdout.write(dat.toString()); });
            gyp.stderr.on('data', function(dat) { process.stderr.write(dat.toString()); });
            gyp.on('close', waitFor(function () {
                var args = ['-j', WORKERS, '-C', 'out', 'BUILDTYPE=Release', 'CC='+builder.config.gcc];
                if (builder.config.systemName === 'win32') { args.push('PLATFORM=mingw32'); }
                if (builder.config.systemName !== 'darwin') { args.push('CFLAGS=-fPIC'); }
                var make;
                if (builder.config.systemName == 'freebsd') {
                    make = Spawn('gmake', args);
                } else {
                    make = Spawn('make', args);
                }
                make.stdout.on('data', function(dat) { process.stdout.write(dat.toString()); });
                make.stderr.on('data', function(dat) { process.stderr.write(dat.toString()); });
                make.on('close', waitFor(function () {
                    process.chdir(cwd);
                }));
            }));
        }).nThen(waitFor());

    }).nThen(waitFor());

}).build(function (builder, waitFor) {

    builder.buildExecutable('admin/angel/cjdroute2.c', BUILDDIR+'/cjdroute', waitFor());
    builder.buildExecutable('test/testcjdroute.c',     BUILDDIR+'/testcjdroute', waitFor());

    builder.buildExecutable('contrib/c/publictoip6.c',     './publictoip6', waitFor());
    builder.buildExecutable('contrib/c/privatetopublic.c', './privatetopublic', waitFor());
    builder.buildExecutable('contrib/c/sybilsim.c',        './sybilsim', waitFor());
    builder.buildExecutable('contrib/c/makekeys.c',        './makekeys', waitFor());

    builder.buildExecutable('crypto/random/randombytes.c',        './randombytes', waitFor());

}).test(function (builder, waitFor) {

    nThen(function (waitFor) {

        if (CROSS) { console.log("Cross compiling.  Test disabled."); return; }
        var out = '';
        var err = '';
        var test = Spawn(BUILDDIR+'/testcjdroute', ['all']);
        test.stdout.on('data', function(dat) { out += dat.toString(); });
        test.stderr.on('data', function(dat) { err += dat.toString(); });
        test.on('close', waitFor(function (ret) {
            if (ret !== 0) {
                console.log(out);
                console.log(err);
                console.log('\033[1;31mFailed to build cjdns.\033[0m');
                waitFor.abort();
            } else {
                console.log(err);
            }
        }));

    }).nThen(function (waitFor) {

        console.log("Checking codestyle");
        var files = [];
        builder.rebuiltFiles.forEach(function (fileName) {
            if (fileName.indexOf('/dependencies/') !== -1) { return; }
            console.log("Checking " + fileName);
            files.push(fileName);
        });
        Codestyle.checkFiles(files, waitFor(function (output) {
            if (output !== '') {
                throw new Error("Codestyle failure\n" + output);
            }
        }));

    }).nThen(waitFor());

}).pack(function (builder, waitFor) {

    Fs.exists(BUILDDIR+'/cjdroute', waitFor(function (exists) {
        if (!exists) { return; }
        Fs.rename(BUILDDIR+'/cjdroute', './cjdroute', waitFor(function (err) {
            if (err) { throw err; }
        }));
    }));

    console.log('\033[1;32mBuild completed successfully, type ./cjdroute to begin setup.\033[0m');

});
