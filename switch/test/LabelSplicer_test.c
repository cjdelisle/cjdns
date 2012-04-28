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
#include "switch/LabelSplicer.h"
#include "util/Endian.h"

#include <assert.h>
#include <stdio.h>
#include <inttypes.h>

static void splice()
{
    // 000000100
    uint64_t goHere = 1<<2;

    // 000000100
    uint64_t viaHere = 1<<2;

    // 000010000
    uint64_t expected = 1<<4;

    uint64_t out = LabelSplicer_splice(goHere, viaHere);

    printf("Splicing %" PRIu64 " with %" PRIu64 " yields %" PRIu64 ", expecting %" PRIu64 "\n",
           goHere, viaHere, out, expected);

    assert(expected == out);
}

static void isOneHop()
{
    assert(!LabelSplicer_isOneHop(4));
}

int main()
{
    splice();
    isOneHop();
    return 0;
}
