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
#include "benc/String.h"
#include "crypto/random/Random.h"
#include "switch/EncodingScheme.h"
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "util/Bits.h"

static void randomForm(struct EncodingScheme_Form* form, struct Random* rand)
{
    Random_bytes(rand, (uint8_t*)form, sizeof(struct EncodingScheme_Form));
    //Bits_memset(form, 0xff, sizeof(struct EncodingScheme_Form));
    form->bitCount &= ((1<<5)-1);
    if (!form->bitCount) {
        form->bitCount++;
    }
    form->prefixLen &= ((1<<5)-1);
    form->prefix &= ((((uint64_t)1)<<form->prefixLen)-1);
}

static struct EncodingScheme* randomList(struct Random* rand, struct Allocator* alloc)
{
    struct EncodingScheme* out =
        Allocator_malloc(alloc, sizeof(struct EncodingScheme));
    out->count = Random_uint32(rand) % 50;
    out->forms = Allocator_malloc(alloc, sizeof(struct EncodingScheme_Form) * out->count);
    for (int i = 0; i < (int)out->count; i++) {
        randomForm(&out->forms[i], rand);
    }
    return out;
}

static void assertEqual(struct EncodingScheme* a,
                        struct EncodingScheme* b)
{
    Assert_always(b);
    Assert_always(a->count == b->count);
    Assert_always(
        !Bits_memcmp(a->forms, b->forms, sizeof(struct EncodingScheme_Form) * a->count));
}

static void randomTest(struct Allocator* parent, struct Random* rand)
{
    struct Allocator* alloc = Allocator_child(parent);
    struct EncodingScheme* control = randomList(rand, alloc);
    String* data = EncodingScheme_serialize(control, alloc);
    struct EncodingScheme* test = EncodingScheme_deserialize(data, alloc);
    assertEqual(control, test);
    Allocator_free(alloc);
}

// Just make sure random crap doesn't crash it.
static void fuzzTest(struct Allocator* parent, struct Random* rand)
{
    struct Allocator* alloc = Allocator_child(parent);
    String* data = String_newBinary(NULL, Random_uint32(rand) % 1024, alloc);
    Random_bytes(rand, (uint8_t*)data->bytes, data->len);
    EncodingScheme_deserialize(data, alloc);
    Allocator_free(alloc);
}

static void encoding(struct Allocator* parent)
{
    struct EncodingScheme_Form forms[3] = {
        {
            .prefixLen = 15,
            .bitCount = 0,
            .prefix = (1<<15)-1,
        }, {
            .prefixLen = 20,
            .bitCount = 0,
            .prefix = (1<<20)-1,
        }, {
            .prefixLen = 18,
            .bitCount = 0,
            .prefix = (1<<18)-1,
        }
    };
    // should encode as prefixLen, bitCount, prefix
    // 111111111111111 00000 01111       15/0/~0 -> 00001111 00111111 11111111 remainder 1
    // 11111111111111111111 00000 10100  20/0/~0 -> 11111111 11111000 00101001 remainder 7f
    // 111111111111111111 00000 10010    18/0/~0 -> 01111111 00001001 11111110 11111111 remainder 7
    //
    // In bytes:
    // 00001111 11111100 11111111 00101001 11111000 11111111
    // 01111111 00001001 11111110 11111111 00000111
    //
    // or \x0f\xfc\xff\x29\xf8\xff\x7f\x09\xfe\xff\x07
    struct EncodingScheme list = {
        .count = 3,
        .forms = forms
    };
    struct Allocator* alloc = Allocator_child(parent);
    String* data = EncodingScheme_serialize(&list, alloc);
    Assert_always(data->len == 11);
    Assert_always(!Bits_memcmp(data->bytes, "\x0f\xfc\xff\x29\xf8\xff\x7f\x09\xfe\xff\x07", 11));
    Allocator_free(alloc);
}

int main()
{
    struct Allocator* alloc = MallocAllocator_new(20000000);
    struct Random* rand = Random_new(alloc, NULL, NULL);

    encoding(alloc);

    for (int i = 0; i < 10000; i++) {
        randomTest(alloc, rand);
    }

    for (int i = 0; i < 10000; i++) {
        fuzzTest(alloc, rand);
    }
}
