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

#define Map_NAME OfLongsByInteger
#define Map_KEY_TYPE uint32_t
#define Map_VALUE_TYPE uint64_t
#define Map_ENABLE_HANDLES
#include "util/Map.h"

#include <stdio.h>
#include <stdbool.h>

// Increase this number to make the fuzz test run longer.
#define QUICK_CYCLES 5
#define SLOW_CYCLES 1000

int main(int argc, char* argv[])
{
    int cycles = QUICK_CYCLES;
    for (int i = 0; i < argc; i++) {
        if (!CString_strcmp("--fuzz", argv[i])) {
            cycles = SLOW_CYCLES;
            break;
        }
    }

    // This test is too slow to run with the normal battery of tests
    if (cycles == QUICK_CYCLES) {
        return 0;
    }

    struct Allocator* mainAlloc = MallocAllocator_new(1<<18);
    struct Random* rand = Random_new(mainAlloc, NULL, NULL);
    struct Log* logger = FileWriterLog_new(stdout, mainAlloc);

    for (int cycle = 0; cycle < cycles; cycle++) {
        struct Allocator* alloc = MallocAllocator_new(1<<24);
        struct Map_OfLongsByInteger* map = Map_OfLongsByInteger_new(alloc);
        uint32_t size;
        Random_bytes(rand, (uint8_t*) &size, 4);
        size = size % 4096;
        uint32_t* keys = Allocator_malloc(alloc, sizeof(uint32_t) * size);
        uint64_t* vals = Allocator_malloc(alloc, sizeof(uint64_t) * size);

        uint32_t key = 3;
        uint64_t val = 4;
        int64_t begin = Time_hrtime();
        for (uint32_t i = 0; i < size; i++) {
            keys[i] = key;
            vals[i] = val;
            Map_OfLongsByInteger_put(&key, &val, map);
            key += val >> 13 ^ size << 19;
            val += key >> 19 ^ i << 13;

        }
        int64_t timeUsed = Time_hrtime() - begin;
        Log_debug(logger, "cycle %d Map put %u values used %lu ms.\n",
                cycle, size, (unsigned long)(timeUsed / 1000000));

        // check all keys there
        for (uint32_t i = 0; i < size; ++i) {
            int index = Map_OfLongsByInteger_indexForKey(&keys[i], map);
            Assert_true(map->values[index] == vals[i]);
        }
        Allocator_free(alloc);
    }
    Log_debug(logger, "===+++=== Completed Ok ===++++===");
    Allocator_free(mainAlloc);
    return 0;
}
