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
#include "memory/MallocAllocator.h"
#include "crypto/random/Random.h"
#include "dht/dhtcore/VersionList.h"
#include "util/Assert.h"
#include "util/Hex.h"

#include <stddef.h>
#include <stdio.h>

int main()
{
    struct Allocator* alloc = MallocAllocator_new(1<<20);
    struct Random* rand = Random_new(alloc, NULL, NULL);

    for (int cycles = 0; cycles < 1; cycles++) {
        for (int bytes = 1; bytes < 5; bytes++) {
            uint32_t count = Random_uint32(rand) % 30;
            uint32_t max = Random_uint32(rand) % UINT32_MAX >> ((4-bytes) * 8);
            if (max < 2) {
                max += 2;
            }
            struct VersionList* vl = VersionList_new(count, alloc);
            for (uint32_t i = 0; i < count; i++) {
                vl->versions[i] = Random_uint32(rand) % max;
            }

            String* str = VersionList_stringify(vl, alloc);

            uint8_t* buff = Allocator_malloc(alloc, (str->len+1) * 2 + 1);
            Hex_encode(buff, (str->len+1) * 2 + 1, (uint8_t*)str->bytes, str->len+1);
            printf("Got versions [%s]\n", buff);

            struct VersionList* vl2 = VersionList_parse(str, alloc);

            Assert_true(vl->length == vl2->length && vl->length == count);
            for (uint32_t i = 0; i < count; i++) {
                printf("[%d] [%d]\n", vl2->versions[i], vl->versions[i]);
                Assert_true(vl2->versions[i] == vl->versions[i]);
            }
        }
    }

    Allocator_free(alloc);
    return 0;
}
