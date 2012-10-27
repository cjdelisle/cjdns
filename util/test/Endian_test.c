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
#include "util/Endian.h"

#include <stdio.h>
#include "util/Assert.h"

void printInfo()
{
    printf("According to testing this machine is %s\n",
           Endian_isBigEndian() ? "big endian" : "little endian");
}

int main()
{
    printInfo();

    volatile uint64_t b =  0x0123456789abcdef;
    volatile uint64_t sb = 0xefcdab8967452301;

    volatile uint32_t a =  0x01234567;
    volatile uint32_t sa = 0x67452301;

    volatile uint16_t c =  0xcabe;
    volatile uint16_t sc = 0xbeca;

    if (!Endian_isBigEndian()) {
        Assert_always(c == Endian_bigEndianToHost16(sc));
        Assert_always(a == Endian_bigEndianToHost32(sa));
        Assert_always(a == Endian_littleEndianToHost32(a));
        Assert_always(b == Endian_bigEndianToHost64(sb));
    } else {
        Assert_always(c == Endian_bigEndianToHost16(c));
        Assert_always(a == Endian_bigEndianToHost32(a));
        Assert_always(a == Endian_littleEndianToHost32(sa));
        Assert_always(b == Endian_bigEndianToHost64(b));
    }

    Assert_always(b == Endian_byteSwap64(sb));
    Assert_always(a == Endian_byteSwap32(sa));
}
