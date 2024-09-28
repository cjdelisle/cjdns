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
#include "memory/Allocator.h"
#include "crypto/random/Random.h"
#include "util/VarInt.h"
#include "util/Assert.h"

#include <stdio.h>

#define BUF_SZ 1024

static void fidelityTest()
{
    struct Allocator* alloc = Allocator_new(1<<20);
    struct Random* rand = NULL;
    Err_assert(Random_new(&rand, alloc, NULL));
    uint64_t* buf = Allocator_malloc(alloc, BUF_SZ*8);
    Random_bytes(rand, (uint8_t*) buf, BUF_SZ*8);

    // make a mix of different size numbers
    for (int i = 0; i < BUF_SZ; i++) {
        uint64_t x = buf[i];
        switch (x & 3) {
            case 3: break;
            case 2: buf[i] = x >> 32; break;
            case 1: buf[i] = x >> 48; break;
            case 0: buf[i] = x >> 56; break;
        }
    }

    uint8_t* buf2 = Allocator_malloc(alloc, BUF_SZ*8);
    struct VarInt_Iter iter;
    VarInt_mk(&iter, buf2, BUF_SZ*8);
    VarInt_toEnd(&iter);

    for (int i = 0; i < BUF_SZ; i++) { Assert_true(!VarInt_push(&iter, buf[i])); }
    Assert_true(iter.ptr > buf2);

    for (int i = BUF_SZ - 1; i >= 0; i--) {
        uint64_t x = ~0;
        Assert_true(!VarInt_pop(&iter, &x));
        Assert_true(buf[i] == x);
    }

    Assert_true(iter.ptr == iter.end);

    Allocator_free(alloc);
}

static inline void test(uint8_t* bytes, int len, uint64_t num, uint8_t* buf)
{
    struct VarInt_Iter iter = { .ptr = bytes, .end = &bytes[len], .start = bytes };
    uint64_t out = ~0;
    Assert_true(!VarInt_pop(&iter, &out));
    Assert_true(out == num);
    Assert_true(iter.ptr == iter.end);

    buf[0] = buf[len+1] = 0xee;
    struct VarInt_Iter iter2 = { .ptr = &buf[len+1], .end = &buf[len+1], .start = &buf[1] };
    if (len < 9) { Assert_true(VarInt_push(&iter2, 0xf000000000000000ull)); }
    if (len < 5) { Assert_true(VarInt_push(&iter2, 0xf0000000)); }
    if (len < 3) { Assert_true(VarInt_push(&iter2, 0xf000)); }
    Assert_true(!VarInt_push(&iter2, num));
    Assert_true(iter2.ptr == iter2.start);
    Assert_true(buf[0] == 0xee);
    Assert_true(buf[len+1] == 0xee);
}
#define TEST(bytes, num) do { \
    uint8_t data[sizeof(bytes)+1] = {0}; \
    test(bytes, sizeof(bytes)-1, num, data); \
} while (0)
// CHECKFILES_IGNORE expecting a ;

static void simpleTest()
{
    TEST("\x00", 0x00);
    TEST("\xAC", 0xAC);
    TEST("\xFD\xAB\xCD", 0xABCD);
    TEST("\xFE\xAB\xCD\xEF\x01", 0xABCDEF01);
    TEST("\xFF\xAB\xCD\xEF\x01\x23\x45\x67\x89", 0xABCDEF0123456789ull);
}

int main()
{
    simpleTest();
    fidelityTest();
    return 0;
}