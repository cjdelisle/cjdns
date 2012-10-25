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
#include "util/Time.h"

#include <stdio.h>
#include "util/Assert.h"

int main()
{
    uint64_t time1;
    uint64_t time2;
    uint64_t bestTime = ~0;
    char* best = NULL;

    volatile uint64_t b =    0x0123456789abcdef;
    volatile uint64_t b_be = 0xefcdab8967452301;
    volatile uint64_t test = b;

    #define TRY(operation) \
        if (b == operation(b_be)) {                                                         \
            time1 = Time_currentTimeMilliseconds(NULL);                                     \
            for (uint64_t i = 0; i < 10000000llu; i++) {                                    \
                test = operation(test);                                                     \
            }                                                                               \
            time2 = Time_currentTimeMilliseconds(NULL);                                     \
            if (time2 - time1 < bestTime) {                                                 \
                best = "" #operation "";                                                    \
                bestTime = time2 - time1;                                                   \
            }                                                                               \
            printf("// " #operation " performed in %u ms.\n", (uint32_t)(time2 - time1));   \
        }

    TRY(Endian_byteSwap64_manual)

    #ifdef Endian_byteSwap64_bswap_64
        TRY(Endian_byteSwap64_bswap_64)
    #endif
    #ifdef Endian_byteSwap64_OSSwapInt64
        TRY(Endian_byteSwap64_OSSwapInt64)
    #endif

    printf("// Best time: %u\n", (uint32_t) bestTime);
    printf("#define Endian_byteSwap64 %s\n", best);
}
