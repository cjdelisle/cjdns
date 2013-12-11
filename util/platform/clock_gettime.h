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
#ifndef clock_gettime_H
#define clock_gettime_H
<?js
    if (typeof(state.clock_gettime) !== 'undefined') { return; }
    console.log("Searching for clock_gettime()");
    var done = this.async();

    var nThen = require("nthen");
    var Fs = require("fs");

    var file = builder.tmpFile();

    nThen(function (waitFor) {

        Fs.writeFile(file, "int main() { clock_gettime(); }", waitFor(function (err, ret) {
            if (err) { throw err; }
        }));

    }).nThen(function (waitFor) {

        builder.compile(file);

    });
?>
#endif
