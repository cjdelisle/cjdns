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
var Builder = require('./builder');

var CONFIG = {
    systemName: 'Linux',
    buildDir: 'buildjs',
    cflags: [
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
        '-D','HAS_JS_PREPROCESSOR',
        '-D','GIT_VERSION="0000000000000000000000000000000000000000"'
    ],
    ldflags: [
        '-pie',
        '-Wl,-z,relro,-z,now,-z,noexecstack'
    ],
    libs: [
        './build/libuv/libuv.a',
        '-lpthread',
        './build/nacl_build/libnacl.a'
    ],
    includeDirs: [
        'build/nacl_build/include/',
        'build/libuv/include/'
    ]
};

Builder.setUp(CONFIG, function (builder) {

   builder.makeExecutable('admin/angel/cjdroute2.c', 'cjdroutejs');
   builder.makeExecutable('publictoip6.c', 'publictoip6');
   builder.makeExecutable('privatetopublic.c', 'privatetopublic');

});

