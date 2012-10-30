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
#include "crypto/Crypto.h"
#include "memory/MallocAllocator.h"
#include "util/Assert.h"

#define Map_NAME OfLongsByInteger
#define Map_KEY_TYPE uint32_t
#define Map_VALUE_TYPE uint64_t
#define Map_ENABLE_HANDLES
#include "util/Map.h"

int main()
{
    uint32_t size = 8192;

    struct Allocator* alloc = MallocAllocator_new(1<<18);
    struct Map_OfLongsByInteger* map = Map_OfLongsByInteger_new(alloc);

    uint32_t key = 3;
    uint64_t val = 4;
    for (uint32_t i = 0; i < size; i++) {
        Map_OfLongsByInteger_put(&key, &val, map);
        key += val >> 13 ^ size << 19;
        val += key >> 19 ^ i << 13;
    }

    for (uint32_t i = size - 1; i > size - 100; i--) {
        int index = map->keys[i] % size;
        uint32_t handle = map->handles[index];
        Assert_always(index == Map_OfLongsByInteger_indexForHandle(handle, map));
    }
}
