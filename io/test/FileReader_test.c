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
#include "io/Reader.h"
#include "io/FileReader.h"
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

int main()
{
    FILE* tmp = tmpfile();
    uint8_t buffer1[2048];
    randombytes(buffer1, 2048);
    size_t iDoNotCare = fwrite(buffer1, 1, 2048, tmp);
    iDoNotCare = iDoNotCare;

    struct Allocator* alloc = MallocAllocator_new(256);

    uint8_t buffer2[1024];
    rewind(tmp);
    struct Reader* r = FileReader_new(tmp, alloc);

    r->read(buffer2, 128, r);
    r->skip(128, r);
    r->read(buffer2+128, 128, r);
    r->skip(512, r);
    r->read(buffer2+128+128, 256, r);
    r->skip(300, r);
    r->read(buffer2+128+128+256, 128, r);

    assert(r->bytesRead(r) == 128+128+128+512+256+300+128);

    uint8_t* ptr1 = buffer1;
    uint8_t* ptr2 = buffer2;

    #define SKIP(x) ptr1 += x
    #define CMP(x) assert(!memcmp(ptr1, ptr2, x)); ptr1 += x; ptr2 += x

    CMP(128);
    SKIP(128);
    CMP(128);
    SKIP(512);
    CMP(256);
    SKIP(300);
    CMP(128);
}
