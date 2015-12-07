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
#include "benc/String.h"
#include "util/Assert.h"
#include "util/events/Time.h"
#include "util/log/FileWriterLog.h"

/* for Crypto key size is 32 bytes, so use it for fuzz main test case.*/
struct Key {
    uint8_t key[32];
};

struct Value {
    uint8_t value[32];
};

#define Map_NAME OfKeyValue
#define Map_KEY_TYPE struct Key
#define Map_VALUE_TYPE struct Value
#define Map_ENABLE_HANDLES
#include "util/Map.h"

#include <stdbool.h>

// Increase this number to make the fuzz test run longer.
#define QUICK_CYCLES 5
#define SLOW_CYCLES 3000

int main(int argc, char* argv[])
{
    int cycles = QUICK_CYCLES;
    for (int i = 0; i < argc; i++) {
        if (!CString_strcmp("--fuzz", argv[i])) {
            cycles = SLOW_CYCLES;
            break;
        }
    }

    struct Allocator* mainAlloc = MallocAllocator_new(1<<18);
    struct Random* rand = Random_new(mainAlloc, NULL, NULL);
    struct Log* logger = FileWriterLog_new(stdout, mainAlloc);

    for (int cycle = 0; cycle < cycles; cycle++) {
        struct Allocator* alloc = MallocAllocator_new(1<<24);
        struct Map_OfKeyValue* map = Map_OfKeyValue_new(alloc);
        uint32_t size;
        Random_bytes(rand, (uint8_t*) &size, 4);
        size = size % 4096;
        struct Key* keys = Allocator_malloc(alloc, sizeof(struct Key) * size);
        struct Value* vals = Allocator_malloc(alloc, sizeof(struct Value) * size);

        struct Key key;
        struct Value val;
        Random_bytes(rand, (uint8_t*)&key, sizeof(key));
        Random_bytes(rand, (uint8_t*)&val, sizeof(val));
        int64_t begin = Time_hrtime();
        for (uint32_t i = 0; i < size; i++) {
            Bits_memcpy(&keys[i], &key, sizeof(key));
            Bits_memcpy(&vals[i], &val, sizeof(val));
            // replace new value with old key
            for (uint32_t j = 0; j < i; ++j) {
                if (Bits_memcmp(&keys[j], &key, sizeof(key)) == 0) {
                    Bits_memcpy(&vals[j], &val, sizeof(val));
                }
            }
            Map_OfKeyValue_put(&key, &val, map);
            Random_bytes(rand, (uint8_t*)&key, sizeof(key));
            Random_bytes(rand, (uint8_t*)&val, sizeof(val));
        }
        int64_t timeUsed = Time_hrtime() - begin;
        Log_debug(logger, "cycle %d Map put %u values used %lu ms.\n",
                cycle, size, timeUsed / 1000000);

        // check all keys there
        for (int32_t i = map->count - 1; i >= 0; --i) {
            int index = Map_OfKeyValue_indexForKey(&keys[i], map);
            Assert_true(index != -1 &&
                        (Bits_memcmp(&map->values[index], &vals[i],
                                    sizeof(struct Value)) == 0));
            Map_OfKeyValue_remove(index, map);
        }

        Assert_true(map->count == 0);
        Allocator_free(alloc);
    }
    Log_debug(logger, "===+++=== Completed Ok ===++++===");
    Allocator_free(mainAlloc);
    return 0;
}
