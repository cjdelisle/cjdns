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
#include "io/Reader.h"
#include "io/FileReader.h"
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "util/Assert.h"
#include "util/Bits.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

int main()
{
    struct Allocator* alloc = MallocAllocator_new(1024);
    struct Random* rand = Random_new(alloc, NULL, NULL);

    FILE* tmp = tmpfile();
    uint8_t buffer1[2048];
    size_t checkSize;
    Random_bytes(rand, buffer1, 2048);
    checkSize = fwrite(buffer1, 1, 2048, tmp);
    if (checkSize != 2048)
    {
        return 1;
    }

    uint8_t buffer2[1024];
    rewind(tmp);
    struct Reader* r = FileReader_new(tmp, alloc);

    Reader_read(r, buffer2, 128);
    Reader_skip(r, 128);
    Reader_read(r, buffer2+128, 128);
    Reader_skip(r, 512);
    Reader_read(r, buffer2+128+128, 256);
    Reader_skip(r, 300);
    Reader_read(r, buffer2+128+128+256, 128);

    Assert_true(r->bytesRead == 128+128+128+512+256+300+128);

    uint8_t* ptr1 = buffer1;
    uint8_t* ptr2 = buffer2;

    #define SKIP(x) ptr1 += x
    #define CMP(x) Assert_true(!Bits_memcmp(ptr1, ptr2, x)); ptr1 += x; ptr2 += x

    CMP(128);
    SKIP(128);
    CMP(128);
    SKIP(512);
    CMP(256);
    SKIP(300);
    CMP(128);

    Allocator_free(alloc);
    return 0;
}
