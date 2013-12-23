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

    var HasFunction = require("./HasFunction");

    HasFunction.check(builder, "clock_gettime", function (err, has) {
        if (err) { throw err; }
        if (has) {
            console.log("Successfully found clock_gettime()");
        } else {
            console.log("Could not find clock_gettime()");
        }
    });
?>
#endif
