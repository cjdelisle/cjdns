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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "switch/LabelSplicer.h"
#include "util/Endian.h"

#include "util/Assert.h"
#include <stdio.h>
#include <inttypes.h>

static void unsplice()
{
    Assert_true(0x13 == LabelSplicer_unsplice(0x13, 1));
}

static void splice()
{
    uint64_t goHere =   0x04; // 000000100
    uint64_t viaHere =  0x04; // 000000100
    uint64_t expected = 0x10; // 000010000

    uint64_t out = LabelSplicer_splice(goHere, viaHere);

    printf("Splicing %" PRIu64 " with %" PRIu64 " yields %" PRIu64 ", expecting %" PRIu64 "\n",
           goHere, viaHere, out, expected);

    Assert_true(expected == out);
}

static void routesThrough()
{
    // 0000000000000000100100000000101011101010100101011100101001010101
    uint64_t dst = 0x0000900aea95ca55;
    // 0000000000000000000000010110010100100110001110011100011001010101
    uint64_t mid = 0x000001652639c655;
    Assert_true(!LabelSplicer_routesThrough(dst, mid));
    Assert_true(LabelSplicer_routesThrough(dst, 1));
}

int main()
{
    splice();
    routesThrough();
    unsplice();
    return 0;
}
