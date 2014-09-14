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
#include "util/Constant.h"

#include "util/Assert.h"
#include <stdio.h>
#include <inttypes.h>

static void unsplice()
{
    Assert_true(0x13 == LabelSplicer_unsplice(0x13, 1));
}

static void splice()
{
    uint64_t goHere =   Constant_base2(000000100);
    uint64_t viaHere =  Constant_base2(000000100);
    uint64_t expected = Constant_base2(000010000);

    uint64_t out = LabelSplicer_splice(goHere, viaHere);

    printf("Splicing %" PRIu64 " with %" PRIu64 " yields %" PRIu64 ", expecting %" PRIu64 "\n",
           goHere, viaHere, out, expected);

    Assert_true(expected == out);
}

static uint64_t routeToInterface(uint32_t number)
{
    uint32_t bits = NumberCompress_bitsUsedForNumber(number);
    return (1 << bits) | NumberCompress_getCompressed(number, bits);
}

static void isOneHop()
{
    Assert_true(LabelSplicer_isOneHop(routeToInterface(0)));
}

static void routesThrough()
{
    uint64_t dst = Constant_base2(0000000000000000100100000000101011101010100101011100101001010101);
    uint64_t mid = Constant_base2(0000000000000000000000010110010100100110001110011100011001010101);
    Assert_true(!LabelSplicer_routesThrough(dst, mid));
    Assert_true(LabelSplicer_routesThrough(dst, 1));
}

static void findErrorSender()
{
    uint64_t fwd;
    uint64_t err;
    uint64_t exp;

    fwd = Constant_base2(0000000000000000100100000000101011101010100101011100101001010101);
    err = Constant_base2(0100011011010101010100000000000000001001000000001010111010101001);
    exp = Constant_base2(0000000000000000000000000000000000000000000101011100101001010101);
    Assert_true(exp == LabelSplicer_findErrorHop(fwd, err));

    fwd = Constant_base2(0000000000000000100100000000101011101010100101011100101001010101);
    err = Constant_base2(1101101110101010100010101101010100000000000000001001000000001010);
    exp = Constant_base2(0000000000000000000000000000000111101010100101011100101001010101);
    Assert_true(exp == LabelSplicer_findErrorHop(fwd, err));

    fwd = Constant_base2(0000000000000000100100000000101011101010100101011100101001010101);
    err = Constant_base2(1111101000000000000000010010000000010101110101010010101110010100);
    exp = Constant_base2(0000000000000000000000000000000000000000000000000000000011010101);
    Assert_true(exp == LabelSplicer_findErrorHop(fwd, err));
}

int main()
{
    splice();
    isOneHop();
    routesThrough();
    unsplice();
    findErrorSender();
    return 0;
}
