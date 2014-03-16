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

#define Version_COMPAT(one, twoRange) <?js \
    file.Version_COMPAT = file.Version_COMPAT || []; \
    file.Version_COMPAT[one] = twoRange;             \
?>

#include "util/version/Version.h"

static const uint8_t VERSION_MATRIX[Version_CURRENT_PROTOCOL+1][Version_CURRENT_PROTOCOL+1] =
<?js
    var matrix = [];
    for (var i = 0; i <= Version_CURRENT_PROTOCOL; i++) {
        var row = matrix[matrix.length] = [];
        for (var j = 0; j <= Version_CURRENT_PROTOCOL; j++) {
            if (j == i) {
                row[j] = 1;
            } else {
                row[j] =
                    (file.Version_COMPAT[Math.max(i, j)].indexOf(Math.min(i,j)) > -1) ? 1 : 0;
            }
        }
    }
    return JSON.stringify(matrix).replace(/\[/g,'{').replace(/\]/g,'}');
?>;

int Version_isCompatible(uint32_t one, uint32_t two)
{
    if (one > Version_CURRENT_PROTOCOL) { one = Version_CURRENT_PROTOCOL; }
    if (two > Version_CURRENT_PROTOCOL) { two = Version_CURRENT_PROTOCOL; }

    return VERSION_MATRIX[one][two];
}
