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
#include "crypto/random/test/DeterminentRandomSeed.h"
#include "io/FileWriter.h"
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "util/Assert.h"
#include "util/Bits.h"
#include "util/log/Log.h"
#include "util/log/WriterLog.h"

static void checkBytes(struct Random* rand, int alignment, int length)
{
    Assert_true(length < 128 && alignment < 8);

    uint64_t buff64[20] = {0};
    uint8_t* buff = (uint8_t*) (&buff64[1]);
    buff += alignment;

    // Check for bytes which are always the same, a few \0s are ok
    // but if every cycle they are always zero then there's a bug.
    uint8_t oldBuff[128] = {0};
    // Preload into the output buff so alignment is same.
    Random_bytes(rand, buff, length);
    Bits_memcpy(oldBuff, buff, length);

    uint8_t sameAsOld[128];
    Bits_memset(sameAsOld, 0xff, 128);

    // Check for bytes which are the same as other bytes every time.
    // if buff[3] always equals buff[8] then there's a bug.
    uint8_t sameBytes[128+128];
    Bits_memset(sameBytes, 0xff, 128+128);

    for (int i = 0; i < 100; i++) {
        Random_bytes(rand, buff, length);

        for (int j = 0; j < length; j++) {
            for (int jj = j; jj < length; jj++) {
                sameBytes[j+jj] &= (jj != j && buff[j] == buff[jj]);
            }
        }

        for (int j = 0; j < length; j++) {
            sameAsOld[j] &= (oldBuff[i] == buff[i]);
        }

        // Check that the function did not write after or before the buffer.
        uint8_t* origBuff = (uint8_t*) (buff64);
        Assert_true(Bits_isZero(origBuff, 8+alignment));
        Assert_true(Bits_isZero(buff+length, 8));
    }

    for (int i = 0; i < length+length-1; i++) {
        Assert_true(!sameBytes[i]);
    }
    for (int i = 0; i < length; i++) {
        Assert_true(!sameAsOld[i]);
    }
}

/** https://github.com/cjdelisle/cjdns/issues/179 */
static void test179(struct Allocator* alloc, struct Log* logger)
{
    uint8_t buff[32] = {0};
    uint8_t buff2[32] = {0};

    struct Random* rand = Random_new(alloc, logger, NULL);
    struct Random* rand2 = Random_new(alloc, logger, NULL);

    Random_bytes(rand, buff, 32);
    Random_bytes(rand2, buff, 32);

    Assert_true(Bits_memcmp(buff, buff2, 32));
}

int main()
{
    struct Allocator* alloc = MallocAllocator_new(1<<20);
    struct RandomSeed* seed = DeterminentRandomSeed_new(alloc, NULL);
    struct Writer* w = FileWriter_new(stdout, alloc);
    struct Log* logger = WriterLog_new(w, alloc);
    struct Random* rand = Random_newWithSeed(alloc, logger, seed, NULL);

    // < 8 length
    checkBytes(rand, 0, 3);
    checkBytes(rand, 3, 6);

    // unaligned beginning (length and align add up to a multiple of 8)
    checkBytes(rand, 3, 29); // 1 byte aligned
    checkBytes(rand, 6, 10); // 2 byte aligned
    checkBytes(rand, 4, 20); // 4 byte aligned

    // unaligned end
    checkBytes(rand, 0, 9); // 1 byte
    checkBytes(rand, 0, 14); // 2 byte
    checkBytes(rand, 0, 20); // 4 byte

    // both unaligned
    checkBytes(rand, 1, 28); // 1,1 byte aligned
    checkBytes(rand, 3, 19); // 1,2 byte aligned
    checkBytes(rand, 5, 23); // 1,4 byte aligned
    checkBytes(rand, 2, 9);  // 2,1 byte aligned
    checkBytes(rand, 6, 12); // 2,2 byte aligned
    checkBytes(rand, 6, 14); // 2,4 byte aligned
    checkBytes(rand, 4, 19); // 4,1 byte aligned
    checkBytes(rand, 4, 10); // 4,2 byte aligned
    checkBytes(rand, 4, 32); // 4,4 byte aligned


    test179(alloc, logger);


    /* torture
    uint8_t selections[2];
    for (int i = 0; i < 100000; i++) {
        Random_bytes(rand, selections, 2);
        checkBytes(rand, selections[0] % 8, selections[1] % 128);
    }*/

    Allocator_free(alloc);
    return 0;
}
