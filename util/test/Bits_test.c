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
#include "crypto/random/Random.h"
#include "memory/MallocAllocator.h"
#include "util/Bits.h"
#include "util/Endian.h"

#include <inttypes.h>
#include <stdio.h>
#include "util/Assert.h"

int main()
{
    struct Allocator* alloc = MallocAllocator_new(20000);
    struct Random* rand = Random_new(alloc, NULL, NULL);

    Assert_true(Bits_log2x64(1)==0);

    uint64_t x;
    Random_bytes(rand, (uint8_t*) &x, 8);
    printf("x = 0x%016" PRIx64 "\n", x);

    Assert_true(Bits_bitReverse64(Bits_bitReverse64(x)) == x);
    Assert_true(
        Bits_bitReverse64(Endian_byteSwap64(Bits_bitReverse64(x))) == Endian_byteSwap64(x));
    Assert_true(Bits_bitReverse64(1) == ((uint64_t)1)<<63);
    Assert_true(Bits_bitReverse64(0) == 0);

    Allocator_free(alloc);
    return 0;
}
