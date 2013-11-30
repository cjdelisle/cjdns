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
var Semaphore = require('./Semaphore');
var Spawn = require('child_process').spawn;
var Os = require('os');

var WORKERS = Math.floor(Os.cpus().length * 1.25);

var sema = Semaphore.create(512);
var cp = function (src, dest, callback) {
    Fs.stat(src, function (err, stat) {
        if (err) { throw err; }
        if (stat.isDirectory()) {
            var subFiles;
            nThen(function (waitFor) {
                Fs.mkdir(dest, waitFor(function (err) {
                    if (err) { throw err; }
                }));
                Fs.readdir(src, waitFor(function (err, list) {
                    if (err) { throw err; }
                    subFiles = list;
                }));
            }).nThen(function (waitFor) {
                subFiles.forEach(function (file) {
                    cp(src + '/' + file, dest + '/' + file, waitFor());
                });
            }).nThen(function (waitFor) {
                callback();
            });
        } else {
            sema.take(function (returnAfter) {
                Fs.readFile(src, function (err, content) {
                    if (err) { throw err; }
                    Fs.writeFile(dest, content, returnAfter(function (err) {
                        if (err) { throw err; }
                        callback();
                    }));
                });
            });
        }
    });
};

process.on('exit', function () {
    console.log("Total build time: " + Math.floor(process.uptime() * 1000) + "ms.");
});

require('./builder').configure({
    rebuildIfChanges: Fs.readFileSync(__filename).toString('utf8'),
    buildDir: 'buildjs'
}, function(builder, waitFor) {

    builder.config.systemName = 'Linux';
    builder.config.tempDir = '/tmp';
    builder.config.useTempFiles = true;
    builder.config.cflags.push(
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

        // Broken GCC patch makes -fstack-protector-all not work
        // workaround is to give -fno-stack-protector first.
        // see: https://bugs.launchpad.net/ubuntu/+source/gcc-4.5/+bug/691722
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
    );
    builder.config.ldflags.push(
        '-pie',
        '-Wl,-z,relro,-z,now,-z,noexecstack'
    );

    // Build dependencies
    nThen(function (waitFor) {
        Fs.exists('buildjs/dependencies', waitFor(function (exists) {
            if (exists) { return; }
            console.log("Copy dependencies");
            cp('./node_build/dependencies', './buildjs/dependencies', waitFor());
        }));
    }).nThen(function (waitFor) {
        builder.config.libs.push(
            'buildjs/dependencies/cnacl/jsbuild/libnacl.a'
        );
        builder.config.includeDirs.push(
            'buildjs/dependencies/cnacl/jsbuild/include/'
        );
        Fs.exists('buildjs/dependencies/cnacl/jsbuild/libnacl.a', waitFor(function (exists) {
            if (exists) { return; }
            console.log("Build NaCl");
            var cwd = process.cwd();
            process.chdir('./buildjs/dependencies/cnacl/');
            var NaCl = require(process.cwd() + '/node_build/make.js');
            NaCl.build(function (args, callback) {
                args.unshift('-fPIC', '-O2', '-fomit-frame-pointer');
                builder.compiler(args, callback);
            }, waitFor(function () {
                process.chdir(cwd);
            }));
        }));

    }).nThen(function (waitFor) {
        builder.config.libs.push(
            'buildjs/dependencies/libuv/libuv.a',
            '-lpthread'
        );
        builder.config.includeDirs.push(
            'buildjs/dependencies/libuv/include/'
        );
        Fs.exists('buildjs/dependencies/libuv/libuv.a', waitFor(function (exists) {
            if (exists) { return; }
            console.log("Build Libuv");
            var cwd = process.cwd();
            process.chdir('./buildjs/dependencies/libuv/');
            var make = Spawn('make', ['-j', WORKERS, 'CFLAGS=-fPIC']);
            make.stdout.on('data', function(dat) { process.stdout.write(dat.toString()); });
            make.stderr.on('data', function(dat) { process.stderr.write(dat.toString()); });
            make.on('close', waitFor(function () {
                process.chdir(cwd);
            }));
        }));

    }).nThen(waitFor());

}).build(function (builder, waitFor) {

    builder.compile('admin/angel/cjdroute2.c', 'cjdroute');
    builder.compile('publictoip6.c', 'publictoip6');
    builder.compile('privatetopublic.c', 'privatetopublic');
    builder.compile('test/cjdroutetest.c', 'testcjdroute');

    Codestyle.checkDir('.', true, waitFor(function (err) {
        if (err) { console.log("Codestyle error"); throw err; }
    }));

});
