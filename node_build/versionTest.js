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
var currentVersion = process.version;
var verArray = currentVersion.substring(1).split(".");
var minVerArray = process.argv[process.argv.length-1].substring(1).split(".");
for (var i = 0; i < minVerArray.length; i++) {
    if (Number(verArray[i]) < Number(minVerArray[i])) {
        process.exit(1);
    } else if (Number(verArray[i]) > Number(minVerArray[i])) {
        process.exit(0);
    }
}
