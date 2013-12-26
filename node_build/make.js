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

// ['linux','darwin','sunos','win32','freebsd']
var SYSTEM = process.platform;
var GCC = process.env['CC'] || 'gcc';

var BUILDDIR = process.env['BUILDDIR'];
if (BUILDDIR == undefined) 
    BUILDDIR = 'build_'+SYSTEM;
var WORKERS = Math.floor(Os.cpus().length * 1.25);

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
    builder.config.cflags.push(
        '-std=c99',
        '-Wall',
        '-Wextra',
        '-Werror',
        '-Wno-pointer-sign',
        '-pedantic',
        '-D',builder.config.systemName + '=1',
        '-Wno-unused-parameter',
        '-Wno-unused-result',

        // Broken GCC patch makes -fstack-protector-all not work
        // workaround is to give -fno-stack-protector first.
        // see: https://bugs.launchpad.net/ubuntu/+source/gcc-4.5/+bug/691722
        '-fno-stack-protector',
        '-fstack-protector-all',
        '-Wstack-protector',

        '-D','HAS_BUILTIN_CONSTANT_P',

        '-g',

//        '-flto', not available on some  machines

        // f4 = 16 peers max, fixed width 4 bit
        // f8 = 241 peers max, fixed width 8 bit
        // v3x5x8 = 256 peers max, variable width, 3, 5 or 8 bits plus 1 or 2 bits of prefix
        // v4x8 = 256 peers max, variable width, 4, or 8 bits plus 1 bit prefix
        '-D',' NumberCompress_TYPE=v4x8',

        // disable for speed, enable for safety
        '-D','Log_DEBUG',
        '-D','Identity_CHECK=1',
        '-D','Allocator_USE_CANARIES=1',
        '-D','PARANOIA=1'
    );
    if (process.env['NO_PIE'] == undefined) {
        builder.config.cflags.push('-fPIE')
    }
    if (process.env['EXPERIMENTAL_PATHFINDER']) {
        console.log("Building with experimental pathfinder");
        builder.config.cflags.push(
            '-D','EXPERIMENTAL_PATHFINDER=1'
        );
    }
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

    if (process.env['NO_PIE'] == undefined) {
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

            // lots of places where depending on preprocessor conditions, a statement might be
            // a case of if (1 == 1)
            '-Wno-tautological-compare'
        );
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
                builder.cc(args, callback);
            }, waitFor(function () {
                process.chdir(cwd);
            }));
        }));

    }).nThen(function (waitFor) {
        builder.config.libs.push(
            BUILDDIR+'/dependencies/libuv/libuv.a',
            '-lpthread'
        );
        if (builder.config.systemName === 'win32') {
            builder.config.libs.push(
                '-lws2_32',
                '-lpsapi',   // GetProcessMemoryInfo()
                '-liphlpapi' // GetAdapterAddresses()
            );
        } else if (builder.config.systemName === 'linux') {
            builder.config.libs.push(
                '-lrt' // clock_gettime()
            );
        } else if (builder.config.systemName === 'darwin') {
            builder.config.libs.push(
                '-framework', 'CoreServices'
            );
        }
        builder.config.includeDirs.push(
            BUILDDIR+'/dependencies/libuv/include/'
        );
        Fs.exists(BUILDDIR+'/dependencies/libuv/libuv.a', waitFor(function (exists) {
            if (exists) { return; }
            console.log("Build Libuv");
            var cwd = process.cwd();
            process.chdir(BUILDDIR+'/dependencies/libuv/');
            var args = ['-j', WORKERS, 'CC='+builder.config.gcc]
            if (builder.config.systemName === 'win32') { args.push('PLATFORM=mingw32'); }
            if (builder.config.systemName !== 'darwin') { args.push('CFLAGS=-fPIC'); }
            var make = Spawn('make', args);
            make.stdout.on('data', function(dat) { process.stdout.write(dat.toString()); });
            make.stderr.on('data', function(dat) { process.stderr.write(dat.toString()); });
            make.on('close', waitFor(function () {
                process.chdir(cwd);
            }));
        }));

    }).nThen(waitFor());

}).build(function (builder, waitFor) {

    builder.buildExecutable('admin/angel/cjdroute2.c', BUILDDIR+'/cjdroute', waitFor());
    builder.buildExecutable('test/testcjdroute.c',     BUILDDIR+'/testcjdroute', waitFor());

    builder.buildExecutable('contrib/c/publictoip6.c',     './publictoip6', waitFor());
    builder.buildExecutable('contrib/c/privatetopublic.c', './privatetopublic', waitFor());
    builder.buildExecutable('contrib/c/sybilsim.c',        './sybilsim', waitFor());
    builder.buildExecutable('contrib/c/benc2json.c',       './benc2json', waitFor());
    builder.buildExecutable('contrib/c/cleanconfig.c',     './cleanconfig', waitFor());
    builder.buildExecutable('contrib/c/dnsserv.c',         './dnsserv', waitFor());
    builder.buildExecutable('contrib/c/makekeys.c',        './makekeys', waitFor());

}).test(function (builder, waitFor) {

    nThen(function (waitFor) {

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
