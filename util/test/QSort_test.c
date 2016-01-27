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

#include "util/Assert.h"
#include "util/QSort.h"

static int compare(const void* a, const void* b)
{
    return (*(int *) a - *(int *) b);
}

int main()
{
    int values[] = { 40, 10, 100, 90, 20, 25, -15, 30, -225};

    QSort(values, sizeof(values) / sizeof(int), sizeof(int), compare);

    int last = values[0];
    for (size_t i = 1; i < sizeof(values) / sizeof(int); i++) {
        Assert_true(last <= values[i]);
        last = values[i];
    }
    // Should not crash
    QSort(values, 0, sizeof(int), compare);

    return 0;
}
