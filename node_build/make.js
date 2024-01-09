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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
/*@flow*/
'use strict';
var Fs = require('fs');
var nThen = require('nthen');
var Cp = require('./Cp');
var Builder = require('./builder');
const CjdnsTest = require('./CjdnsTest');
const GetVersion = require('./GetVersion');

var CFLAGS = process.env['CFLAGS'];
var LDFLAGS = process.env['LDFLAGS'];
// march=native really only makes a lot of sense on x86/amd64 where the available features
// are a hodgepodge per-CPU. On arm (32) you may or may not have NEON available but in any
// case clang doesn't reliably support march except on x86/amd64.
var NO_MARCH_FLAG = ['arm', 'ppc', 'ppc64', 'arm64'];

if (process.version.replace('v','').split('.').map(Number)[0] >= 19) {
    // OK
} else if ('OLD_NODE_VERSION_I_EXPECT_ERRORS' in process.env) {
    console.log('OLD_NODE_VERSION_I_EXPECT_ERRORS is set, ignoring old version');
} else {
    throw new Error("Your version of nodejs is old/untested. " +
        "Old enough versions (pre-es6) do not work correctly and give weird build failures. " +
        "If you want to force building anyway, try: " +
        "OLD_NODE_VERSION_I_EXPECT_ERRORS=1 ./do");
}

Builder.configure({
    buildDir: process.env['OUT_DIR'], // set by cargo
    systemName: process.env['SYSTEM'] || process.platform,
    gcc: process.env['CC'],
}, function (builder, waitFor) {

    builder.config.crossCompiling = process.env['CROSS'] !== undefined;
    let optimizeLevel = '-O2';

    builder.config.cflags.push(
        '-std=c99',
        '-Wall',
        '-Wextra',
        '-Werror',
        '-Wno-pointer-sign',
        '-Wno-strict-prototypes',
        '-Wmissing-prototypes',
        '-pedantic',
        '-D', builder.config.systemName + '=1',
        '-Wno-unused-parameter',
        '-fomit-frame-pointer',
        '-ffunction-sections',
        '-fdata-sections',

        '-D', 'Log_' + (process.env['Log_LEVEL'] || 'DEBUG'),

        '-g',

        // f4 = 16 peers max, fixed width 4 bit
        // f8 = 241 peers max, fixed width 8 bit
        // v3x5x8 = 256 peers max, variable width, 3, 5 or 8 bits plus 1 or 2 bits of prefix
        // v4x8 = 256 peers max, variable width, 4, or 8 bits plus 1 bit prefix
        '-D', 'NumberCompress_TYPE=v3x5x8',

        // enable for safety (don't worry about speed, profiling shows they add ~nothing)
        '-D', 'Identity_CHECK=1',
        '-D', 'Allocator_USE_CANARIES=1',
        '-D', 'PARANOIA=1'
    );

    if (process.env["CJDNS_RELEASE_VERSION"]) {
        builder.config.version = '' + process.env["CJDNS_RELEASE_VERSION"];
    }

    if (process.env['SUBNODE']) { builder.config.cflags.push('-DSUBNODE=1'); }

    if (process.env['GCOV']) {
        builder.config.cflags.push('-fprofile-arcs', '-ftest-coverage');
        builder.config.ldflags.push('-fprofile-arcs', '-ftest-coverage');
    }

    var android = /android/i.test(builder.config.gcc);

    if (process.env['TESTING']) {
        builder.config.cflags.push('-D', 'TESTING=1');
    }

    if (process.env['ADDRESS_PREFIX']) {
        builder.config.cflags.push('-D', 'ADDRESS_PREFIX=' + process.env['ADDRESS_PREFIX']);
    }
    if (process.env['ADDRESS_PREFIX_BITS']) {
        builder.config.cflags.push('-D', 'ADDRESS_PREFIX_BITS=' + process.env['ADDRESS_PREFIX_BITS']);
    }

    if (!builder.config.crossCompiling) {
        if (NO_MARCH_FLAG.indexOf(process.arch) == -1) {
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

    if (process.env['NO_PIE'] === undefined && builder.config.systemName !== 'freebsd'
        && builder.config.systemName !== 'win32') {
        builder.config.cflags.push('-fPIE');

        // just using `-pie` on OS X >= 10.10 results in this warning:
        // clang: warning: argument unused during compilation: '-pie'
        if (builder.config.systemName !== "darwin") {
            builder.config.ldflags.push('-pie');
        } else {
            builder.config.ldflags.push('-Wl,-pie');
        }
    }

    if (builder.compilerType().isClang) {
        // blows up when preprocessing before js preprocessor
        builder.config.cflags.push(
            '-Wno-invalid-pp-token',
            '-Wno-dollar-in-identifier-extension',
            '-Wno-newline-eof',
            '-Wno-unused-value',

            // lots of places where depending on preprocessor conditions, a statement might be
            // a case of if (1 == 1)
            '-Wno-tautological-compare',

            //'-Wno-error'
            '-Wno-gnu-line-marker'
        );
    } else {
        builder.config.cflags.push(
            '-fdiagnostics-color=always'
        );
    }

    // Install any user-defined CFLAGS. Necessary if you are messing about with building cnacl
    // with NEON on the BBB, or want to set -Os (OpenWrt)
    // Allow -O0 so while debugging all variables are present.
    if (CFLAGS) {
        var cflags = CFLAGS.split(' ');
        cflags.forEach(function (flag) {
            if (/^\-O[^02s]$/.test(flag)) {
                console.log("Skipping " + flag + ", assuming " + optimizeLevel + " instead.");
            } else if (/^\-O[02s]$/.test(flag)) {
                optimizeLevel = flag;
            } else {
                builder.config.cflags.push(flag);
            }
        });
    }

    builder.config.cflags.push(optimizeLevel);
    if (!/^\-O0$/.test(optimizeLevel)) {
        builder.config.cflags.push('-D_FORTIFY_SOURCE=2');
    }

    // We also need to pass various architecture/floating point flags to GCC when invoked as
    // a linker.
    if (LDFLAGS) {
        [].push.apply(builder.config.ldflags, LDFLAGS.split(' '));
    }

    if (android) {
        // NDK uses the word `android` in places
        builder.config.cflags.push('-DCjdns_android=1');
    }

    var uclibc = process.env['UCLIBC'] == '1';
    var libssp;
    switch (process.env['SSP_SUPPORT']) {
        case 'y':
        case '1': libssp = true; break;
        case 'n':
        case '':
        case '0': libssp = false; break;
        case undefined: break;
        default: throw new Error();
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

    if (process.env['Pipe_PREFIX']) {
        builder.config.cflags.push(
            '-D', 'Pipe_PREFIX="' + process.env['Pipe_PREFIX'] + '"'
        );
    }

    if (typeof (builder.config.cjdnsTest_files) === 'undefined') {
        CjdnsTest.generate(builder, process.env['SUBNODE'] !== '', waitFor());
    }

    nThen((w) => {
        if (builder.config.version) { return; }
        GetVersion(w(function (err, data) {
            if (!err) {
                builder.config.version = ('' + data).replace(/(\r\n|\n|\r)/gm, "");
            } else {
                builder.config.version = 'unknown';
            }
        }));
    }).nThen((w) => {
        builder.config.cflags.push('-D', 'CJD_PACKAGE_VERSION="' + builder.config.version + '"');
    }).nThen(waitFor());

    // Build dependencies
    let foundSodium = false;
    nThen(function (waitFor) {

        const dir = `${builder.config.buildDir}/../..`;
        Fs.readdir(dir, waitFor((err, ret) => {
            if (err) { throw err; }
            ret.forEach((f) => {
                if (!/^libsodium-sys-/.test(f)) { return; }
                const inclPath = `${dir}/${f}/out/source/libsodium/src/libsodium/include`;
                Fs.readdir(inclPath, waitFor((err, ret) => {
                    if (foundSodium) { return; }
                    if (err && err.code === 'ENOENT') { return; }
                    if (err) { throw err; }
                    builder.config.includeDirs.push(inclPath);
                    foundSodium = true;
                }));
            });
        }));

    }).nThen(function (waitFor) {

        if (!foundSodium) {
            throw new Error("Unable to find a path to libsodium headers");
        }

        if (!android) {
            builder.config.libs.push('-lpthread');
        }

        if (builder.config.systemName === 'win32') {
            builder.config.libs.push(
                '-lws2_32',
                '-lpsapi',   // GetProcessMemoryInfo()
                '-liphlpapi' // GetAdapterAddresses()
            );
        } else if (builder.config.systemName === 'linux' && !android) {
            builder.config.libs.push('-lrt'); // clock_gettime()
        } else if (builder.config.systemName === 'darwin') {
            builder.config.libs.push('-framework', 'CoreServices');
        } else if (['freebsd', 'openbsd', 'netbsd'].indexOf(builder.config.systemName) >= 0) {
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

        builder.config.includeDirs.push('node_build/dependencies/libuv/include/');
        builder.config.includeDirs.push('node_build/dependencies/libuv/src/');

    }).nThen(waitFor());

}).build(function (builder, waitFor) {

    builder.buildLibrary('client/cjdroute2.c');

    builder.buildLibrary('contrib/c/publictoip6.c');
    builder.buildLibrary('contrib/c/privatetopublic.c');
    builder.buildLibrary('contrib/c/makekeys.c');
    builder.buildLibrary('contrib/c/mkpasswd.c');
    builder.buildLibrary('crypto/random/randombytes.c');
    builder.buildLibrary('rust/cjdns_sys/cffi.h');
    builder.buildLibrary('test/testcjdroute.c');

}).failure(function (builder, waitFor) {

    console.log('\x1b[1;31mFailed to build cjdns.\x1b[0m');
    process.exit(1);

});
