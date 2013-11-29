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
#include "util/version/Version.h"

const uint8_t* Version_gitVersion()
{
    return (uint8_t*)
    #ifdef HAS_JS_PREPROCESSOR
        <?js
            var done = this.async();
            require('fs').readFile('.git/logs/HEAD', function (err, ret) {
                if (err) { throw err; }
                var lines = ret.toString('utf8').split('\n');
                var hashes = lines[lines.length-2].split(' ');
                var head = hashes[1];
                if (!(/^[a-f0-9]{40}$/.test(head))) {
                    throw new Error(head + ' does not look like a git hash');
                }
                done('"'+head+'"');
            });
        ?>
    #else
        GIT_VERSION
    #endif
    ;
}
