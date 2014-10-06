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
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "switch/PenaltyFloat.h"
#include "util/Assert.h"

static void packUnpack(struct Random* rand)
{
    for (int i = 0; i < 1000; i++) {
        uint64_t p = Random_uint64(rand);
        p >>= (i % 64);
        uint32_t c1 = PenaltyFloat_pack(p);
        uint64_t p1 = PenaltyFloat_unpack(c1);
        uint32_t c2 = PenaltyFloat_pack(p1);
        uint64_t p2 = PenaltyFloat_unpack(c2);
        Assert_true(!(c1 >> 22));
        Assert_true(c1 == c2);
        Assert_true(p1 == p2);
    }
    Assert_true(PenaltyFloat_MAX == PenaltyFloat_pack(UINT64_MAX));
}

int main()
{
    struct Allocator* alloc = MallocAllocator_new(20000000);
    struct Random* rand = Random_new(alloc, NULL, NULL);

    packUnpack(rand);

    return 0;
}
