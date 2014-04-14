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
#include "util/Base32.h"
#include "util/Assert.h"

#include <stdio.h>

int main()
{
    struct Allocator* alloc = MallocAllocator_new(20000);
    struct Random* rand = Random_new(alloc, NULL, NULL);

    uint8_t bytes[32];
    Random_bytes(rand, bytes, 32);

    uint8_t base32[64];
    Bits_memset(base32, 0, 64);

    Assert_true(Base32_encode(base32, 64, bytes, 32) == 52);

    //printf("base32 encoded: %s\n", base32);

    uint8_t bytes2[32];
    Assert_true(Base32_decode(bytes2, 32, base32, 52) == 32);

    Assert_true(Bits_memcmp(bytes, bytes2, 32) == 0);

    Allocator_free(alloc);
    return 0;
}
