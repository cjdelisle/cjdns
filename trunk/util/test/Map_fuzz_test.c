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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "crypto/random/Random.h"
#include "util/Assert.h"
#include "wire/Message.h"
#include "test/FuzzTest.h"
#include "util/events/EventBase.h"

#define Map_NAME OfLongsByInteger
#define Map_KEY_TYPE uint32_t
#define Map_VALUE_TYPE uint64_t
#define Map_ENABLE_HANDLES
#include "util/Map.h"

void* CJDNS_FUZZ_INIT(struct Allocator* alloc, struct Random* rand)
{
    return alloc;
}

void CJDNS_FUZZ_MAIN(void* vctx, struct Message* fuzz)
{
    struct Allocator* alloc = (struct Allocator*) vctx;
    if (fuzz->length < 4) { return; }
    uint32_t size = Er_assert(Message_epop16be(fuzz)) % 4096;

    struct Map_OfLongsByInteger* map = Map_OfLongsByInteger_new(alloc);
    size = size % 4096;
    uint32_t* keys = Allocator_malloc(alloc, sizeof(uint32_t) * size);
    uint64_t* vals = Allocator_malloc(alloc, sizeof(uint64_t) * size);

    uint32_t key = 3;
    uint64_t val = 4;
    for (uint32_t i = 0; i < size; i++) {
        keys[i] = key;
        vals[i] = val;
        Map_OfLongsByInteger_put(&key, &val, map);
        key += ((val >> 13 ^ size << 19) & 0x000fffff) + 1;
        //Log_debug(logger, "%u", (val >> 13 ^ size << 19) & 0x000fffff);
        Assert_true(key > keys[i]);
        val += key >> 19 ^ i << 13;
    }

    // check all keys there
    for (uint32_t i = 0; i < size; ++i) {
        int index = Map_OfLongsByInteger_indexForKey(&keys[i], map);
        Assert_true(map->values[index] == vals[i]);
    }
}
