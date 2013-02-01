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
#include "util/Assert.h"
#include "util/Security.h"

void overflow(struct Allocator* alloc, struct Random* rand)
{
    uint32_t x = Random_uint32(rand);
    for (int i = 0; i < 32; i++) {
        struct Allocator* a = Allocator_child(alloc);
        uint32_t count = ((x>>i) & 31);
        while (count--) {
            a = Allocator_child(alloc);
            if (count == 27) {
                overflow(a, rand);
            }
        }
    }
}

int main()
{
    Security_maxMemory(1<<20, NULL);
    struct Allocator* alloc = MallocAllocator_new(1<<19);
    struct Random* rand = Random_new(alloc, NULL, NULL);
    overflow(alloc, rand);
    // sometimes it doesn't use up all of it's space.
    //Assert_always(0);
}
