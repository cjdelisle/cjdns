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
var Codestyle = require('./Codestyle');

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
    builder.config.libs.push(
        './build/libuv/libuv.a',
        '-lpthread',
        './build/nacl_build/libnacl.a'
    );
    builder.config.includeDirs.push(
        'build/nacl_build/include/',
        'build/libuv/include/'
    );
/*
    var GitVersion = require('./GitVersion');
    GitVersion.get(waitFor(function (version) {
        builder.config.cflags.push('-D','GIT_VERSION="'+version+'"');
    }));
*/
}).build(function (builder, waitFor) {

    builder.compile('admin/angel/cjdroute2.c', 'cjdroutejs');
    builder.compile('publictoip6.c', 'publictoip6');
    builder.compile('privatetopublic.c', 'privatetopublic');

    Codestyle.checkDir('.', true, waitFor(function (err) {
        if (err) { console.log("Codestyle error"); throw err; }
    }));

});
