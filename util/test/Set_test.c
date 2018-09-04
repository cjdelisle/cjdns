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
#include "memory/MallocAllocator.h"
#include "util/Assert.h"

#define Set_NAME OfInts
#define Set_TYPE uint32_t
#include "util/Set.h"

static inline int compareHighOnly(const uint32_t* a, const uint32_t* b)
{
    return (*a >> 16) - (*b >> 16);
}
static inline uint32_t hashCodeHighOnly(const uint32_t* a)
{
    return *a >> 16;
}
#define Set_COMPARE compareHighOnly
#define Set_HASHCODE hashCodeHighOnly
#define Set_NAME OfIntsHighOnly
#define Set_TYPE uint32_t
#include "util/Set.h"


#include <stdio.h>
#include <stdbool.h>

#define CYCLES 1

#define COUNT 100

static void simpleTest(struct Allocator* alloc, struct Random* rand)
{
    struct Set_OfInts* set = Set_OfInts_new(alloc);
    uint32_t size;
    Random_bytes(rand, (uint8_t*) &size, 4);
    size = (size % 4096) + 101;
    uint32_t* buff = Allocator_malloc(alloc, size * sizeof(uint32_t));
    Random_bytes(rand, (uint8_t*)buff, size * sizeof(uint32_t));

    for (uint32_t i = 0; i < size; i++) {
        int size = set->size;
        if (Set_OfInts_add(set, &buff[i]) != size + 1) {
            int ok = 0;
            for (uint32_t j = 0; j < i; j++) {
                if (buff[j] == buff[i]) { ok = 1; break; }
            }
            Assert_true(ok);
        } else {
            for (uint32_t j = 0; j < i; j++) { Assert_true(buff[j] != buff[i]); }
        }
    }

    uint32_t* val;
    Set_FOREACH(OfInts, set, val) {
        int size = set->size;
        Assert_true(val == Set_OfInts_remove(set, val));
        Assert_true(set->size == size - 1);
    }
    Assert_true(set->size == 0);

    for (uint32_t i = 0; i < COUNT; i++) {
        buff[i] = i;
        Assert_true(set->size == (int)i);
        Assert_true(Set_OfInts_add(set, &buff[i]) == (int) i+1);
    }

    for (uint32_t i = 0; i < COUNT; i++) {
        uint32_t num = i;
        Assert_true(Set_OfInts_get(set, &num) == &buff[i]);
    }
}

static void comparitorTest(struct Allocator* alloc, struct Random* rand)
{
    struct Set_OfIntsHighOnly* set = Set_OfIntsHighOnly_new(alloc);
    uint32_t buff[5] = { 0, 1, 100000, 100001, 200000 };
    uint32_t res[3] = { 0, 100000, 200000 };
    for (uint32_t i = 0; i < 5; i++) { Set_OfIntsHighOnly_add(set, &buff[i]); }
    Assert_true(set->size == 3);

    int i = 0;
    struct Set_OfIntsHighOnly_Iter iter;
    for (Set_OfIntsHighOnly_iter(set, &iter); iter.val; Set_OfIntsHighOnly_iterNext(&iter)) {
        Assert_true(i < 3);
        Assert_true(*iter.val == res[i++]);
    }
}

int main()
{
    struct Allocator* mainAlloc = MallocAllocator_new(20000);
    struct Random* rand = Random_new(mainAlloc, NULL, NULL);

    for (int cycles = 0; cycles < CYCLES; cycles++) {
        struct Allocator* alloc = MallocAllocator_new(1<<25);
        simpleTest(alloc, rand);
        Allocator_free(alloc);
        alloc = MallocAllocator_new(1<<18);
        comparitorTest(alloc, rand);
        Allocator_free(alloc);
    }
    Allocator_free(mainAlloc);
    return 0;
}
