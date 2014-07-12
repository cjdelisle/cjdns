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
var Builder = require('./builder');
var TestRunner = require('./TestRunner');

// ['linux','darwin','sunos','win32','freebsd']
var SYSTEM = process.env['SYSTEM'] || process.platform;
var CROSS = process.env['CROSS'] || '';
var LOG_LEVEL = process.env['Log_LEVEL'] || 'DEBUG';

var GCC = process.env['CC'];
if (!GCC) {
    if (SYSTEM === 'freebsd') {
        GCC = 'gcc47';
    } else {
        GCC = 'gcc';
    }
}

var BUILDDIR = process.env['BUILDDIR'];
if (BUILDDIR === undefined) {
    BUILDDIR = 'build_'+SYSTEM;
}

var OPTIMIZE = '-O2';


Builder.configure({
    system: SYSTEM
}, function(builder, waitFor) {

    builder.config.gcc = GCC;
    builder.config.tempDir = '/tmp';

    builder.config.cflags.push.apply(builder.config.cflags, [
        '-std=c99',
        '-Wall',
        '-Wextra',
        '-Werror',
        '-Wno-pointer-sign',
        '-pedantic',
        '-D',builder.config.system + '=1',
        '-Wno-unused-parameter',
        '-Wno-unused-result',

        '-D','HAS_BUILTIN_CONSTANT_P',

        '-D','Log_'+LOG_LEVEL,

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

    if (process.env['TESTING']) { builder.config.cflags.push('-D', 'TESTING=1'); }

    if (SYSTEM === 'win32') {
        builder.config.cflags.push('-Wno-format');
        builder.config.libs.push('-lssp');
    }

    if (SYSTEM === 'linux') {
        builder.config.ldflags.push(
            '-Wl,-z,relro,-z,now,-z,noexecstack'
        );
        builder.config.cflags.push(
            '-DHAS_ETH_INTERFACE=1'
        );
    }

    if (process.env['NO_PIE'] === undefined && SYSTEM !== 'freebsd' && SYSTEM !== 'win32') {
        builder.config.cflags.push('-fPIE');
        builder.config.ldflags.push('-pie');
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

    var uclibc = process.env['UCLIBC'] == '1';
    var libssp = process.env['SSP_SUPPORT'] == 'y';
    if ((!uclibc && SYSTEM !== 'win32' && SYSTEM !== 'sunos') || libssp) {
        builder.config.cflags.push(
            // Broken GCC patch makes -fstack-protector-all not work
            // workaround is to give -fno-stack-protector first.
            // see: https://bugs.launchpad.net/ubuntu/+source/gcc-4.5/+bug/691722
            '-fno-stack-protector',
            '-fstack-protector-all',
            '-Wstack-protector'
        );

        // Static libssp provides __stack_chk_fail_local, which x86 needs in
        // order to avoid expensively looking up the location of __stack_chk_fail.
        var x86 = process.env['TARGET_ARCH'] == 'i386';
        if (uclibc && !x86) { builder.config.libs.push('-lssp'); }
        if (uclibc && x86) { builder.config.libs.push('-Wl,-Bstatic', '-lssp', '-Wl,-Bdynamic'); }
    } else {
        console.log("Stack Smashing Protection (security feature) is disabled");
    }

    var dependencyDir = builder.config.buildDir+'/dependencies';
    var libuvLib = dependencyDir+'/libuv/out/Release/libuv.a';
    if (SYSTEM === 'win32') { libuvLib = dependencyDir+'/libuv/out/Release/obj.target/libuv.a'; }

    // Build dependencies
    nThen(function (waitFor) {
        Fs.exists(dependencyDir, waitFor(function (exists) {
            if (exists) { return; }
            console.log("Copy dependencies");
            Cp('./node_build/dependencies', dependencyDir, waitFor());
        }));
    }).nThen(function (waitFor) {
        builder.config.libs.push(
            dependencyDir+'/cnacl/jsbuild/libnacl.a'
        );
        builder.config.includeDirs.push(
            dependencyDir+'/cnacl/jsbuild/include/'
        );
        Fs.exists(dependencyDir+'/cnacl/jsbuild/libnacl.a', waitFor(function (exists) {
            if (exists) { return; }
            console.log("Build NaCl");
            var cwd = process.cwd();
            process.chdir(dependencyDir+'/cnacl/');
            var NaCl = require(process.cwd() + '/node_build/make.js');
            NaCl.build(function (args, callback) {
                if (builder.config.system !== 'win32') { args.unshift('-fPIC'); }
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
        builder.config.libs.push(libuvLib);
        if (!(/.*android.*/.test(GCC))) {
            builder.config.libs.push(
                '-lpthread'
            );
        }
        if (builder.config.system === 'win32') {
            builder.config.libs.push(
                '-lws2_32',
                '-lpsapi',   // GetProcessMemoryInfo()
                '-liphlpapi' // GetAdapterAddresses()
            );
        } else if (builder.config.system === 'linux' && !(/.*android.*/).test(GCC)) {
            builder.config.libs.push(
                '-lrt' // clock_gettime()
            );
        } else if (builder.config.system === 'darwin') {
            builder.config.libs.push(
                '-framework', 'CoreServices'
            );
        } else if (builder.config.systemName === 'freebsd') {
            builder.config.cflags.push(
                    '-Wno-overlength-strings'
                    );
            builder.config.libs.push(
                '-lkvm'
            );
        } else if (builder.config.systemName === 'sunos') {
            builder.config.libs.push(
                '-lsocket',
                '-lsendfile',
                '-lkstat',
                '-lnsl'
            );
        }
        builder.config.includeDirs.push(
            dependencyDir+'/libuv/include/'
        );
        var libuvBuilt;
        var python;
        nThen(function (waitFor) {
            Fs.exists(libuvLib, waitFor(function (exists) {
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
            process.chdir(dependencyDir+'/libuv/');

            var args = ['./gyp_uv.py'];
            var env = Extend({}, process.env);
            env.CC = builder.config.gcc;
            if (env.TARGET_ARCH) {
                args.push('-Dtarget_arch='+env.TARGET_ARCH);
            }
            //args.push('--root-target=libuv');
            if (/.*android.*/.test(GCC)) { args.push('-Dtarget_arch=arm', '-DOS=android'); }
            if (SYSTEM === 'win32') { args.push('-DOS=win'); }
            var gyp = Spawn(python, args, {env:env});
            gyp.stdout.on('data', function(dat) { process.stdout.write(dat.toString()); });
            gyp.stderr.on('data', function(dat) { process.stderr.write(dat.toString()); });
            gyp.on('close', waitFor(function () {
                var args = [
                    '-j', builder.processors,
                    '-C', 'out',
                    'BUILDTYPE=Release',
                    'CC='+builder.config.gcc,
                    'CXX='+builder.config.gcc,
                    'V=1'
                ];
                if (!(/darwin|win32/.test(SYSTEM))) { args.push('CFLAGS=-fPIC'); }
                var make;
                if (builder.config.system == 'freebsd') {
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

    builder.buildExecutable('admin/angel/cjdroute2.c', 'cjdroute');

    builder.buildExecutable('contrib/c/publictoip6.c');
    builder.buildExecutable('contrib/c/privatetopublic.c');
    builder.buildExecutable('contrib/c/sybilsim.c');
    builder.buildExecutable('contrib/c/makekeys.c');

    builder.buildExecutable('crypto/random/randombytes.c');

    builder.lintFiles(function (fileName, file, callback) {
        if (/dependencies/.test(fileName)) { callback('', false); return; }
        Codestyle.lint(fileName, file, callback);
    });

    if (CROSS) { console.log("Cross compiling.  Test disabled."); return; }
    var testRunner = TestRunner.local(['all']);
    if (process.env['REMOTE_TEST']) {
        testRunner = TestRunner.remote(process.env['REMOTE_TEST'], ['all']);
    }
    builder.buildTest('test/testcjdroute.c', testRunner);

}).success(function (builder, waitFor) {

    console.log('\033[1;32mBuild completed successfully, type ./cjdroute to begin setup.\033[0m');

}).failure(function (builder, waitFor) {

    console.log('\033[1;31mFailed to build cjdns.\033[0m');

});
