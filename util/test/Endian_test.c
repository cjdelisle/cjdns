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
#include <assert.h>

void printInfo()
{
    #if defined(__BYTE_ORDER) && defined(__BIG_ENDIAN)
        printf("This machine is defined as %s\n",
               Endian_isBigEndian() ? "big endian" : "little endian");
    #endif

    printf("According to testing this machine is %s\n",
           Endian_checkIsBigEndian() ? "big endian" : "little endian");

    printf("Endian_byteSwap64() uses " Endian_byteSwap64_uses "\n");

    printf("Endian_hostToBigEndian64() uses " Endian_hostToBigEndian64_uses "\n");

    printf("Endian_byteSwap32() uses " Endian_byteSwap32_uses "\n");

    printf("Endian_hostToLittleEndian32() uses " Endian_hostToLittleEndian32_uses "\n");
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
        assert(c == Endian_bigEndianToHost16(sc));
        assert(a == Endian_bigEndianToHost32(sa));
        assert(a == Endian_littleEndianToHost32(a));
        assert(b == Endian_bigEndianToHost64(sb));
    } else {
        assert(c == Endian_bigEndianToHost16(c));
        assert(a == Endian_bigEndianToHost32(a));
        assert(a == Endian_littleEndianToHost32(sa));
        assert(b == Endian_bigEndianToHost64(b));
    }

    assert(b == Endian_byteSwap64(sb));
    assert(a == Endian_byteSwap32(sa));
}
