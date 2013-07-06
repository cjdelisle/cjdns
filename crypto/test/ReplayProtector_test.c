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
#include "crypto/ReplayProtector.h"
#include "crypto/random/Random.h"
#include "memory/BufferAllocator.h"

#include "util/Assert.h"
#include <stdint.h>

int main()
{
    uint16_t randomShorts[8192];
    uint16_t out[8192];
    struct ReplayProtector rp = {0,0};

    struct Allocator* alloc;
    BufferAllocator_STACK(alloc, 1024);

    struct Random* rand = Random_new(alloc, NULL, NULL);

    Random_bytes(rand, (uint8_t*)randomShorts, sizeof(randomShorts));

    uint32_t outIdx = 0;
    for (uint32_t i = 0; i < 1024; i++) {
        if (ReplayProtector_checkNonce((randomShorts[i] % (i + 20)), &rp)) {
            out[outIdx] = (randomShorts[i] % (i + 20));
            outIdx++;
        }
    }

    for (uint32_t i = 0; i < outIdx; i++) {
        for (uint32_t j = i + 1; j < outIdx; j++) {
            Assert_always(out[i] != out[j]);
        }
    }

    return 0;
}
