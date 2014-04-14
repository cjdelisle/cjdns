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
#include "util/Assert.h"

#define Map_NAME OfLongsByInteger
#define Map_KEY_TYPE uint32_t
#define Map_VALUE_TYPE uint64_t
#define Map_ENABLE_HANDLES
#include "util/Map.h"

#include <stdio.h>
#include <stdbool.h>

#define CYCLES 1

int main()
{
    struct Allocator* mainAlloc = MallocAllocator_new(20000);
    struct Random* rand = Random_new(mainAlloc, NULL, NULL);

    for (int cycles = 0; cycles < CYCLES; cycles++) {
        struct Allocator* alloc = MallocAllocator_new(1<<18);
        struct Map_OfLongsByInteger* map = Map_OfLongsByInteger_new(alloc);
        uint32_t size;
        Random_bytes(rand, (uint8_t*) &size, 4);
        size = (size % 4096) + 101;

        uint32_t key = 3;
        uint64_t val = 4;
        for (uint32_t i = 0; i < size; i++) {
            Map_OfLongsByInteger_put(&key, &val, map);
            key += val >> 13 ^ size << 19;
            val += key >> 19 ^ i << 13;
        }

        // If a key is duplicated, the entry will br replaced.
        size = map->count;

        for (uint32_t i = size - 1; i > size - 100; i--) {
            int index = map->keys[i] % size;
            uint32_t handle = map->handles[index];
            if (index != Map_OfLongsByInteger_indexForHandle(handle, map)) {
                uint32_t num = 0;
                for (int i = 0; i < (int)map->count; i++) {
                    if (num > map->handles[i]) {
                        Assert_true(!"map out of order");
                    }
                    num = map->handles[i];
                }
                printf("failed to find the correct index for the handle "
                       "handle[%u], index[%u], indexForHandle[%u]\n",
                       handle, index, Map_OfLongsByInteger_indexForHandle(handle, map));
                Assert_true(false);
            }
        }
        Allocator_free(alloc);
    }
    Allocator_free(mainAlloc);
    return 0;
}
