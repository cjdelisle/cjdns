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
#include "crypto/Crypto.h"
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
}

int main()
{
    printInfo();

    volatile uint64_t b =    0x0123456789abcdef;
    volatile uint64_t b_be = 0xefcdab8967452301;

    if (!Endian_isBigEndian()) {
        volatile uint32_t a =    0x01234567;
        volatile uint32_t a_be = 0x67452301;

        volatile uint16_t c =    0xcabe;
        volatile uint16_t c_be = 0xbeca;

        assert(c == Endian_bigEndianToHost16(c_be));
        assert(a == Endian_bigEndianToHost32(a_be));
        assert(b == Endian_bigEndianToHost64(b_be));
    }

    assert(b == Endian_byteSwap64_manual(b_be));
    #ifdef Endian_byteSwap64_bswap_64
        assert(b == Endian_byteSwap64_bswap_64(b_be));
    #endif
}
