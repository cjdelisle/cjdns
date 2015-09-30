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
var Os = require('os');
var FindPython2 = require('./FindPython2');
var CanCompile = require('./CanCompile');
var Builder = require('./builder');
var TestRunner = require('./TestRunner');

// ['linux','darwin','sunos','win32','freebsd','openbsd']
var SYSTEM = process.env['SYSTEM'] || process.platform;
var TMP = process.env['TMP'] || process.env['TEMP'];
var GCC = process.env['CC'];
var CFLAGS = process.env['CFLAGS'];
var LDFLAGS = process.env['LDFLAGS'];

var NO_MARCH_FLAG = ['ppc', 'ppc64'];

if (!GCC) {
    if (SYSTEM === 'freebsd') {
        GCC = 'gcc47';
    } else if (SYSTEM === 'openbsd') {
        GCC = 'egcc';
    } else {
        GCC = 'gcc';
    }
}

if (!TMP) {
    TMP = '/tmp';
}

Builder.configure({
    systemName: SYSTEM,
    crossCompiling: process.env['CROSS'] !== undefined,
    gcc: GCC,
    tempDir: TMP,
    devNull: '/dev/null',
    logLevel: process.env['Log_LEVEL'] || 'DEBUG',
}, function(builder, waitFor) {

    builder.config.ext = {};
    builder.config.ext.exe = '';
    builder.config.ext.obj = '.o';
    builder.config.ext.lib = '.a';


    builder.config.flag = {};
    builder.config.flag.precompileOnly = '-E';
    builder.config.flag.showIncludes = '-MM';
    builder.config.flag.define = '-D';
    builder.config.flag.outputObj = '-o';
    builder.config.flag.outputExe = '-o';
    builder.config.flag.outputPreprocessed = '-o';
    builder.config.flag.compileOnly = '-c';
    builder.config.flag.include = '-I';
    builder.config.flag.optimizeLevel = '-O3';
    builder.config.flag.compileAsC = [];
    builder.config.flag.languageC = '-xc';
    builder.config.flag.languageCppOutput = '-xcpp-output';
    builder.config.flag.retainSymbolInfo = '-g';
    builder.config.flag.lto = '-flto';
    builder.config.flag.pic = '-fPIC';


    // c version
    builder.config.cflags.push(
        '-std=c99'
    );

    // warnings
    builder.config.cflags.push(
        '-pedantic',
        '-Wall',
        '-Wextra',
        '-Werror',
        '-Wno-pointer-sign',
        '-Wno-unused-parameter'
    );

    // defines
    builder.config.cflags.push(
        builder.config.flag.define + 'NODE_BUILD',
        builder.config.flag.define + builder.config.systemName + '=1',
        builder.config.flag.define + 'Log_' + builder.config.logLevel,

        // f4 = 16 peers max, fixed width 4 bit
        // f8 = 241 peers max, fixed width 8 bit
        // v3x5x8 = 256 peers max, variable width, 3, 5 or 8 bits plus 1 or 2 bits of prefix
        // v4x8 = 256 peers max, variable width, 4, or 8 bits plus 1 bit prefix
        builder.config.flag.define + 'NumberCompress_TYPE=v3x5x8',

        // enable for safety (don't worry about speed, profiling shows they add ~nothing)
        builder.config.flag.define + 'Identity_CHECK=1',
        builder.config.flag.define + 'Allocator_USE_CANARIES=1',
        builder.config.flag.define + 'PARANOIA=1'
    );

    var android = /android/i.test(builder.config.gcc);

    if (process.env['TESTING']) {
        builder.config.cflags.push(builder.config.flag.define + 'TESTING=1');
    }

    if (!builder.config.crossCompiling) {
        if (NO_MARCH_FLAG.indexOf(process.arch) < -1) {
            builder.config.cflags.push('-march=native');
        }
    }

    if (builder.config.systemName === 'win32') {
        builder.config.cflags.push('-Wno-format');
    } else if (builder.config.systemName === 'linux') {
        builder.config.ldflags.push('-Wl,-z,relro,-z,now,-z,noexecstack');
        builder.config.cflags.push('-DHAS_ETH_INTERFACE=1');
    } else if (builder.config.systemName === 'darwin') {
        builder.config.cflags.push('-DHAS_ETH_INTERFACE=1');
    }

    if (process.env['NO_PIE'] === undefined && builder.config.systemName !== 'freebsd' && builder.config.systemName !== 'win32') {
        builder.config.cflags.push('-fPIE');

        // just using `-pie` on OS X >= 10.10 results in this warning:
        // clang: warning: argument unused during compilation: '-pie'
        if (builder.config.systemName !== "darwin") {
            builder.config.ldflags.push('-pie');
        } else {
            builder.config.ldflags.push('-Wl,-pie');
        }
    }

    if (/clang/i.test(builder.config.gcc) || builder.config.systemName === 'darwin') {
        // blows up when preprocessing before js preprocessor
        builder.config.cflags.push(
            '-Wno-invalid-pp-token',
            '-Wno-dollar-in-identifier-extension',
            '-Wno-newline-eof',
            '-Wno-unused-value',

            // lots of places where depending on preprocessor conditions, a statement might be
            // a case of if (1 == 1)
            '-Wno-tautological-compare',

            '-Wno-error'
        );
        builder.config.cflags.slice(builder.config.cflags.indexOf('-Werror'), 1);
    }

    // Install any user-defined CFLAGS. Necessary if you are messing about with building cnacl
    // with NEON on the BBB, or want to set -Os (OpenWrt)
    // Allow -O0 so while debugging all variables are present.
    if (CFLAGS) {
        var cflags = CFLAGS.split(' ');
        cflags.forEach(function(flag) {
            if (/^\-O[^02s]$/.test(flag)) {
                console.log("Skipping " + flag + ", assuming " +
                    builder.config.flag.optimizeLevel + " instead.");
            } else if (/^\-O[02s]$/.test(flag)) {
                builder.config.flag.optimizeLevel = flag;
            } else {
                [].push.apply(builder.config.cflags, cflags);
            }
        });
    }

    // We also need to pass various architecture/floating point flags to GCC when invoked as
    // a linker.
    if (LDFLAGS) {
        [].push.apply(builder.config.ldflags, LDFLAGS.split(' '));
    }

    if (android) {
        builder.config.cflags.push('-Dandroid=1');
    }

    var checkCode = 'int main() { return 0; }';
    var checkFlags = [builder.config.cflags, builder.config.flag.lto, builder.config.flag.languageC];

    CanCompile.check(builder, checkCode, checkFlags, function(err, can) {
        if (can) {
            console.log("Compiler supports link time optimization");
            builder.config.ldflags.push(
                builder.config.flag.lto,
                builder.config.flag.optimizeLevel
            );
        } else {
            console.log("Link time optimization not supported [" + err + "]");
        }
        builder.config.cflags.push(builder.config.flag.optimizeLevel);
    });

    var uclibc = process.env['UCLIBC'] == '1';
    var libssp;
    switch (process.env['SSP_SUPPORT']) {
        case 'y':
        case '1':
            libssp = true;
            break;
        case 'n':
        case '':
        case '0':
            libssp = false;
            break;
        case undefined:
            break;
        default:
            throw new Error();
    }
    if (libssp === false) {
        console.log("Stack Smashing Protection (security feature) is disabled");
    } else if (builder.config.systemName == 'win32') {
        builder.config.libs.push('-lssp');
    } else if ((!uclibc && builder.config.systemName !== 'sunos') || libssp === true) {
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
        if (uclibc) {
            if (x86) {
                builder.config.libs.push('-Wl,-Bstatic', '-lssp', '-Wl,-Bdynamic');
            } else {
                builder.config.libs.push('-lssp');
            }
        }
    } else {
        console.log("Stack Smashing Protection (security feature) is disabled");
    }

    if (process.env['Pipe_PREFIX'] !== undefined) {
        builder.config.cflags.push(
            builder.config.flag.define + 'Pipe_PREFIX="' + process.env['Pipe_PREFIX'] + '"'
        );
    }

    var dependencyDir = builder.config.buildDir + '/dependencies';
    var libuvLib = dependencyDir + '/libuv/out/Release/libuv' + builder.config.ext.lib;
    if (builder.config.systemName === 'win32') {
        libuvLib = dependencyDir + '/libuv/out/Release/obj.target/libuv' + builder.config.ext.lib;
    }

    // Build dependencies
    nThen(function(waitFor) {

        Fs.exists(dependencyDir, waitFor(function(exists) {
            if (exists) {
                return;
            }

            console.log("Copy dependencies");
            Cp('./node_build/dependencies', dependencyDir, waitFor());
        }));

    }).nThen(function(waitFor) {

        var libnaclName = 'libnacl' + builder.config.ext.lib;

        builder.config.libs.push(dependencyDir + '/cnacl/jsbuild/' + libnaclName);
        builder.config.includeDirs.push(dependencyDir + '/cnacl/jsbuild/include/');

        Fs.exists(dependencyDir + '/cnacl/jsbuild/' + libnaclName, waitFor(function(exists) {
            if (exists) {
                return;
            }

            console.log("Build NaCl");
            var cwd = process.cwd();
            process.chdir(dependencyDir + '/cnacl/');

            var NaCl = require(process.cwd() + '/node_build/make.js');
            NaCl.build(function(args, callback) {
                    if (builder.config.systemName !== 'win32') {
                        args.unshift(builder.config.flag.pic);
                    }

                    args.unshift('-fomit-frame-pointer');

                    args.unshift(builder.config.flag.optimizeLevel);

                    if (CFLAGS) {
                        [].push.apply(args, CFLAGS.split(' '));
                    }

                    if (!builder.config.crossCompiling) {
                        if (NO_MARCH_FLAG.indexOf(process.arch) < -1) {
                            builder.config.cflags.push('-march=native');
                        }
                    }

                    builder.cc(args, callback);
                },
                builder.config,
                waitFor(function() {
                    process.chdir(cwd);
                }));
        }));

    }).nThen(function(waitFor) {

        builder.config.libs.push(libuvLib);
        if (!android) {
            builder.config.libs.push('-lpthread');
        }

        if (builder.config.systemName === 'win32') {
            builder.config.libs.push(
                '-lws2_32',
                '-lpsapi', // GetProcessMemoryInfo()
                '-liphlpapi' // GetAdapterAddresses()
            );
        } else if (builder.config.systemName === 'linux' && !android) {
            builder.config.libs.push('-lrt'); // clock_gettime()
        } else if (builder.config.systemName === 'darwin') {
            builder.config.libs.push('-framework', 'CoreServices');
        } else if (['freebsd', 'openbsd'].indexOf(builder.config.systemName) >= 0) {
            builder.config.cflags.push('-Wno-overlength-strings');
            builder.config.libs.push('-lkvm');
        } else if (builder.config.systemName === 'sunos') {
            builder.config.libs.push(
                '-lsocket',
                '-lsendfile',
                '-lkstat',
                '-lnsl'
            );
        }

        builder.config.includeDirs.push(dependencyDir + '/libuv/include/');

        var libuvBuilt;
        var python;
        nThen(function(waitFor) {

            Fs.exists(libuvLib, waitFor(function(exists) {
                if (exists) {
                    libuvBuilt = true;
                }
            }));

        }).nThen(function(waitFor) {

            if (libuvBuilt) {
                return;
            }
            FindPython2.find(builder.tmpFile(), waitFor(function(err, pythonExec) {
                if (err) {
                    throw err;
                }
                python = pythonExec;
            }));

        }).nThen(function(waitFor) {

            var cwd = process.cwd();

            var gyp = function(callback) {
                if (libuvBuilt) {
                    return;
                }
                console.log("Build Libuv");
                process.chdir(dependencyDir + '/libuv/');

                var args = ['./gyp_uv.py'];
                var env = process.env;

                env.CC = builder.config.gcc;

                if (env.TARGET_ARCH) {
                    args.push('-Dtarget_arch=' + env.TARGET_ARCH);
                }

                //args.push('--root-target=libuv');
                if (android) {
                    args.push('-DOS=android');
                }

                if (builder.config.systemName === 'win32') {
                    args.push('-DOS=win');
                }

                if (env.GYP_ADDITIONAL_ARGS) {
                    args.push.apply(args, env.GYP_ADDITIONAL_ARGS.split(' '));
                }


                var exe = Spawn(python, args, {
                    env: env,
                    stdio: 'inherit'
                });

                exe.on('error', function() {
                    console.error("couldn't launch gyp [" + python + "]");
                });
                exe.on('close', waitFor(callback));
            };

            var make = function() {
                var args = [
                    '-j', builder.processors,
                    '-C', 'out',
                    'BUILDTYPE=Release',
                    'CC=' + builder.config.gcc,
                    'CXX=' + builder.config.gcc,
                    'V=1'
                ];
                var cflags = [builder.config.flag.optimizeLevel, '-DNO_EMFILE_TRICK=1'];

                if (!(/darwin|win32/i.test(builder.config.systemName))) {
                    cflags.push(builder.config.flag.pic);
                }
                args.push('CFLAGS=' + cflags.join(' '));

                var makeCommand = ['freebsd', 'openbsd'].indexOf(builder.config.systemName) >= 0 ? 'gmake' : 'make';

                var exe = Spawn(makeCommand, args, {
                    stdio: 'inherit'
                });

                exe.on('error', function(err) {
                    if (err.code === 'ENOENT') {
                        console.error('\033[1;31mError: ' + makeCommand + ' is required!\033[0m');
                    } else {
                        console.error(
                            '\033[1;31mFail run ' + process.cwd() + ': ' + makeCommand + ' ' + args.join(' ') + '\033[0m'
                        );
                        console.error('Message:', err);
                    }
                    waitFor.abort();
                });

                exe.on('close', waitFor(function() {
                    process.chdir(cwd);
                }));
            };

            gyp(make);

        }).nThen(waitFor());

    }).nThen(waitFor());

}).build(function(builder, waitFor) {

    builder.buildExecutable('client/cjdroute2.c', 'cjdroute');

    builder.buildExecutable('contrib/c/publictoip6.c');
    builder.buildExecutable('contrib/c/privatetopublic.c');
    builder.buildExecutable('contrib/c/sybilsim.c');
    builder.buildExecutable('contrib/c/makekeys.c');

    builder.buildExecutable('crypto/random/randombytes.c');

    builder.lintFiles(function(fileName, file, callback) {
        if (/dependencies/.test(fileName)) {
            callback('', false);
            return;
        }

        Codestyle.lint(fileName, file, callback);
    });

    var testcjdroute = builder.buildTest('test/testcjdroute.c');
    if (builder.config.crossCompiling) {
        console.log("Cross compiling. Building, but not running tests.");
        return;
    }

    var testRunner = TestRunner.local(['all']);
    if (process.env['REMOTE_TEST']) {
        testRunner = TestRunner.remote(process.env['REMOTE_TEST'], ['all']);
    }
    builder.runTest(testcjdroute, testRunner);

}).success(function(builder, waitFor) {

    console.log('\033[1;32mBuild completed successfully, type ./cjdroute to begin setup.\033[0m');

}).failure(function(builder, waitFor) {

    console.log('\033[1;31mFailed to build cjdns.\033[0m');
    process.exit(1);

}).complete(function(builder, waitFor) {

    if (builder.failure) {
        process.exit(1);
    }

});
