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
#include "benc/String.h"
#include "crypto/random/Random.h"
#include "switch/EncodingScheme.h"
#define NumberCompress_OLD_CODE
#include "switch/NumberCompress.h"
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "util/Bits.h"

#define Order_TYPE struct EncodingScheme_Form
#define Order_NAME OfEncodingForms
#define Order_COMPARE compareEncodingForms
#include "util/Order.h"
static inline int compareEncodingForms(const struct EncodingScheme_Form* a,
                                       const struct EncodingScheme_Form* b)
{
    int diff = a->bitCount;
    diff -= b->bitCount;
    return diff == 0 ? 0 : diff > 0 ? 1 : -1;
}

static void randomForm(struct EncodingScheme_Form* form, struct Random* rand)
{
    for (;;) {
        Random_bytes(rand, (uint8_t*)form, sizeof(struct EncodingScheme_Form));
        //Bits_memset(form, 0xff, sizeof(struct EncodingScheme_Form));
        form->bitCount &= ((1<<5)-1);
        if (!form->bitCount) {
            form->bitCount++;
        }
        form->prefixLen &= ((1<<5)-1);
        if (!form->prefixLen) {
            form->prefixLen++;
        }
        if (EncodingScheme_formSize(form) > 59) { continue; }
        if (form->prefixLen > 3 && (form->prefix & 0xf) == 1) { continue; }
        break;
    }
    form->prefix &= ((((uint64_t)1)<<form->prefixLen)-1);
}

static struct EncodingScheme* randomScheme(struct Random* rand, struct Allocator* alloc)
{
    struct EncodingScheme* out =
        Allocator_malloc(alloc, sizeof(struct EncodingScheme));
    do {
        out->count = Random_uint32(rand) % 32;
    } while (out->count < 2);
    out->forms = Allocator_malloc(alloc, sizeof(struct EncodingScheme_Form) * out->count);
    for (int i = 0; i < (int)out->count; i++) {
        randomForm(&out->forms[i], rand);
        for (int j = 0; j < i; j++) {
            if (out->forms[i].bitCount == out->forms[j].bitCount) {
                i--;
                break;
            }
            int minPfx = (out->forms[i].prefixLen < out->forms[j].prefixLen)
                ? out->forms[i].prefixLen : out->forms[j].prefixLen;
            if (((out->forms[j].prefix ^ out->forms[i].prefix) & ((1<<minPfx)-1)) == 0) {
                // collision, destroy both entries and try again.
                if (j != i-1) {
                    Bits_memcpy(&out->forms[j],
                                     &out->forms[i-1],
                                     sizeof(struct EncodingScheme_Form));
                }
                i -= 2;
                break;
            }
        }
    }
    Order_OfEncodingForms_qsort(out->forms, out->count);
    return out;
}

static void assertEqual(struct EncodingScheme* a,
                        struct EncodingScheme* b)
{
    Assert_true(b);
    Assert_true(a->count == b->count);
    Assert_true(
        !Bits_memcmp(a->forms, b->forms, sizeof(struct EncodingScheme_Form) * a->count));
}

static void randomTest(struct Allocator* parent, struct Random* rand)
{
    struct Allocator* alloc = Allocator_child(parent);
    struct EncodingScheme* control = randomScheme(rand, alloc);
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

/** Greatest possible number using x bits, all are set. */
#define Bits_maxBits64(x) ((((uint64_t)1)<<(x))-1)

static void encoding(struct Allocator* parent)
{
    struct EncodingScheme_Form forms[3] = {
        {
            .prefixLen = 15,
            .bitCount = 2,
            .prefix = ((1<<15)-1) ^ (1<<1),
        }, {
            .prefixLen = 20,
            .bitCount = 4,
            .prefix = ((1<<20)-1) ^ (1<<2),
        }, {
            .prefixLen = 18,
            .bitCount = 8,
            .prefix = ((1<<18)-1) ^ (1<<3),
        }
    };
    // should encode as prefixLen, bitCount, prefix
    // 111111111111101 00010 01111       15/2/~0 -> 01001111 11110100 11111111 remainder 1
    // 11111111111111111011 00100 10100  20/4/~0 -> 00101001 11011001 11111111 remainder 7f
    // 111111111111110111 01000 10010    18/8/~0 -> 01111111 10001001 11101110 11111111 remainder 7
    //                                              00000111 <-- appliation of remainder
    //
    // In bytes:
    // 01001111 11110100 11111111 00101001 11011001 11111111
    // 01111111 10001001 11101110 11111111 00000111
    //
    // or \x4f\xf4\xff\x29\xd9\xff\x7f\x89\xee\xff\x07
    struct EncodingScheme list = {
        .count = 3,
        .forms = forms
    };
    struct Allocator* alloc = Allocator_child(parent);
    String* data = EncodingScheme_serialize(&list, alloc);
    Assert_true(data->len == 11);
    Assert_true(!Bits_memcmp(data->bytes, "\x4f\xf4\xff\x29\xd9\xff\x7f\x89\xee\xff\x07", 11));
    Allocator_free(alloc);
}

#define convertLabel_SELF_ROUTE 1
#define convertLabel_TOO_BIG 2
static int convertLabel(struct EncodingScheme_Form* iform,
                        struct EncodingScheme_Form* oform,
                        uint64_t label)
{
    struct {
        struct EncodingScheme scheme;
        struct EncodingScheme_Form forms[2];
    } s;
    s.scheme.count = 2;
    s.scheme.forms = s.forms;
    Bits_memcpy(&s.forms[0], iform, sizeof(struct EncodingScheme_Form));
    Bits_memcpy(&s.forms[1], oform, sizeof(struct EncodingScheme_Form));

    int iformNum = 0;
    int oformNum = 1;
    struct EncodingScheme* scheme = &s.scheme;

    Assert_true(EncodingScheme_getFormNum(scheme, label) == iformNum);

    uint64_t label2 = EncodingScheme_convertLabel(scheme, label, oformNum);

    if ((label & Bits_maxBits64(s.forms[0].prefixLen + s.forms[0].bitCount)) == 1) {
        Assert_true(label2 == EncodingScheme_convertLabel_INVALID);
        return convertLabel_SELF_ROUTE;
    }

    if (Bits_log2x64(label) + EncodingScheme_formSize(oform) -
        EncodingScheme_formSize(iform) > 59)
    {
        Assert_true(label2 == EncodingScheme_convertLabel_INVALID);
        return convertLabel_TOO_BIG;
    }

    uint64_t additional = label >> s.forms[0].prefixLen;
    uint64_t director = additional & Bits_maxBits64(s.forms[0].bitCount);

    if (!EncodingScheme_is358(&s.scheme)) {
    } else if (1 == (s.forms[0].prefix & Bits_maxBits64(s.forms[0].prefixLen))) {
        director = director - (director == 1) + (director == 0);
    } else {
        director += (director > 0);
    }

    // here is where the director is actually correct

    if (!EncodingScheme_is358(&s.scheme)) {
    } else if (1 == (s.forms[1].prefix & Bits_maxBits64(s.forms[1].prefixLen))) {
        director = director - (director == 1) + (director == 0);
    } else {
        director -= (director > 0);
    }

    additional = additional >> s.forms[0].bitCount;

    uint64_t converted = (additional << s.forms[1].bitCount) | director;
    converted = (converted << s.forms[1].prefixLen) | s.forms[1].prefix;

    if ((converted & Bits_maxBits64(s.forms[1].prefixLen + s.forms[1].bitCount)) == 1) {
        // looks like a self-route
        Assert_true(label2 == EncodingScheme_convertLabel_INVALID);
        return convertLabel_SELF_ROUTE;
    }

    //printf("%lx == %lx", label2, converted);
    Assert_true(label2 == converted);

    uint64_t label3 = EncodingScheme_convertLabel(scheme, label2, iformNum);
    Assert_true(label3 == label);
    return 0;
}

static void convertLabelRand(struct Random* rand, struct EncodingScheme* scheme)
{
    for (int i = 0; i < 100; i++) {
        int oformNum;
        do {
            oformNum = Random_uint8(rand) % scheme->count;
        } while (!oformNum);
        int iformNum = Random_uint8(rand) % oformNum;
        uint64_t label = Random_uint64(rand);
        label &= (UINT64_MAX >> (Random_uint8(rand) % 63));

        for (;;) {
            // make sure the label has the right prefix
            label <<= scheme->forms[iformNum].prefixLen;
            label |= scheme->forms[iformNum].prefix;
            Assert_true(EncodingScheme_getFormNum(scheme, label) == iformNum);

            if (Bits_log2x64(label) > 59) {
                // fall through
            } else {
                int ret = convertLabel(&scheme->forms[iformNum], &scheme->forms[oformNum], label);
                if (ret == convertLabel_SELF_ROUTE) {
                    i--;
                    break;
                } else if (ret == convertLabel_TOO_BIG) {
                    // fall through
                } else if (!ret) {
                    // success
                    break;
                } else {
                    Assert_true(0);
                }
            }
            label >>= scheme->forms[iformNum].prefixLen + 1;
        }
    }
}

static void isOneHopScheme(struct Allocator* allocator)
{
    struct Allocator* alloc = Allocator_child(allocator);
    struct EncodingScheme* s4x8 = NumberCompress_v4x8_defineScheme(alloc);
    Assert_true(!EncodingScheme_isOneHop(s4x8, 1));
    Assert_true(EncodingScheme_isOneHop(s4x8, 0x21));
    Assert_true(EncodingScheme_isOneHop(s4x8, 0x23));
    Assert_true(!EncodingScheme_isOneHop(s4x8, 0x12));
    Assert_true(EncodingScheme_isOneHop(s4x8, 0x220));
    Assert_true(EncodingScheme_isOneHop(s4x8, 0x210));
    Assert_true(!EncodingScheme_isOneHop(s4x8, 0x110));

    struct EncodingScheme* s3x5x8 = NumberCompress_v3x5x8_defineScheme(alloc);
    Assert_true(!EncodingScheme_isOneHop(s3x5x8, 1));
    Assert_true(EncodingScheme_isOneHop(s3x5x8, 0x13));
    Assert_true(EncodingScheme_isOneHop(s3x5x8, 0x15));
    Assert_true(EncodingScheme_isOneHop(s3x5x8, 0x96));
    Assert_true(EncodingScheme_isOneHop(s3x5x8, 0x400));
    Assert_true(!EncodingScheme_isOneHop(s3x5x8, 0x115));
    Assert_true(!EncodingScheme_isOneHop(s3x5x8, 0x166));
    Assert_true(!EncodingScheme_isOneHop(s3x5x8, 0x1400));
    Allocator_free(alloc);
}

typedef uint64_t (* EncodeNum)(uint32_t num);
typedef uint32_t (* DecodeNum)(uint64_t label);
static uint64_t encode358(uint32_t num)
{
    uint32_t bits = NumberCompress_v3x5x8_bitsUsedForNumber(num);
    return NumberCompress_v3x5x8_getCompressed(num, bits);
}
static uint32_t decode358(uint64_t label)
{
    uint32_t bits = NumberCompress_v3x5x8_bitsUsedForLabel(label);
    return NumberCompress_v3x5x8_getDecompressed(label, bits);
}
static uint64_t encode48(uint32_t num)
{
    uint32_t bits = NumberCompress_v4x8_bitsUsedForNumber(num);
    return NumberCompress_v4x8_getCompressed(num, bits);
}
static uint32_t decode48(uint64_t label)
{
    uint32_t bits = NumberCompress_v4x8_bitsUsedForLabel(label);
    return NumberCompress_v4x8_getDecompressed(label, bits);
}

static uint64_t encodef4(uint32_t num)
{
    uint32_t bits = NumberCompress_f4_bitsUsedForNumber(num);
    return NumberCompress_f4_getCompressed(num, bits);
}
static uint32_t decodef4(uint64_t label)
{
    uint32_t bits = NumberCompress_f4_bitsUsedForLabel(label);
    return NumberCompress_f4_getDecompressed(label, bits);
}

static void parseSerializeDir(struct EncodingScheme* scheme, EncodeNum en, DecodeNum dn)
{
    int max = Bits_maxBits64(scheme->forms[scheme->count - 1].bitCount);
    for (int i = 0; i <= max; i++) {
        //printf("\nTest [%d]\n", i);
        uint64_t dir = EncodingScheme_serializeDirector(scheme, i, -1);
        //printf("[%lu] == [%lu] (%d)\n", dir, en(i), i);
        Assert_true(dir == en(i));
        if (dir < ~0ull) {
            int out = EncodingScheme_parseDirector(scheme, dir);
            //printf("Test [%d] == [%u] decode(%lu)\n", i, (uint32_t)dn(dir), dir);
            Assert_true((uint32_t)i == dn(dir));
            //printf("[%d] == [%d]\n", i, out);
            Assert_true(i == out);
        }
        for (int j = 0; j < scheme->count; j++) {
            if (i >> scheme->forms[j].bitCount) { continue; }
            //printf(" With form [%d]\n", j);
            dir = scheme->forms[j].prefix | ( i << scheme->forms[j].prefixLen );
            int out = EncodingScheme_parseDirector(scheme, dir);
            Assert_true(out >= 0);
            uint64_t dir2 = EncodingScheme_serializeDirector(scheme, out, j);
            //printf(" [%lu] == [%lu] encode(%d)\n", dir2, dir, out);
            Assert_true(dir2 == dir);
        }
    }
}

static void bitsUsed(struct EncodingScheme* es, DecodeNum getBits, struct Random* rand)
{
    uint64_t label = Random_uint64(rand);
    int fn = EncodingScheme_getFormNum(es, label);
    Assert_true(fn > -1);
    uint32_t bits = getBits(label);
    uint32_t esbits = es->forms[fn].bitCount + es->forms[fn].prefixLen;
    //printf("%lx %u %u\n", label, bits, esbits);
    Assert_true(bits == esbits);
}

int main()
{
    struct Allocator* alloc = MallocAllocator_new(20000000);
    struct Random* rand = Random_new(alloc, NULL, NULL);

    encoding(alloc);
    isOneHopScheme(alloc);

    for (int i = 0; i < 1000; i++) {
        randomTest(alloc, rand);
    }

    for (int i = 0; i < 1000; i++) {
        fuzzTest(alloc, rand);
    }

    struct EncodingScheme* es48 = NumberCompress_v4x8_defineScheme(alloc);
    struct EncodingScheme* es358 = NumberCompress_v3x5x8_defineScheme(alloc);
    struct EncodingScheme* esf4 = NumberCompress_f4_defineScheme(alloc);

    for (int i = 0; i < 1000; i++) {
        bitsUsed(es48, NumberCompress_v4x8_bitsUsedForLabel, rand);
    }
    for (int i = 0; i < 1000; i++) {
        bitsUsed(es358, NumberCompress_v3x5x8_bitsUsedForLabel, rand);
    }

    parseSerializeDir(es358, encode358, decode358);
    parseSerializeDir(es48, encode48, decode48);
    parseSerializeDir(esf4, encodef4, decodef4);

    // for testing individual conversions in isolation.
    /*convertLabel(
        &(struct EncodingScheme_Form){.bitCount = 15, .prefixLen = 20, .prefix = 792003},
        &(struct EncodingScheme_Form){.bitCount = 31, .prefixLen = 30, .prefix = 385462496},
        11331704259
    );*/

    for (int i = 0; i < 10; i++) {
        struct Allocator* tempAlloc = Allocator_child(alloc);
        struct EncodingScheme* scheme = randomScheme(rand, tempAlloc);
        convertLabelRand(rand, scheme);
        convertLabelRand(rand, es48);
        convertLabelRand(rand, es358);
        Allocator_free(tempAlloc);
    }

    struct Allocator* tempAlloc = Allocator_child(alloc);
    struct EncodingScheme* scheme = NumberCompress_v3x5x8_defineScheme(alloc);
    for (int i = 0; i < NumberCompress_v3x5x8_INTERFACES; i++) {
        int bits = NumberCompress_bitsUsedForNumber(i);
        uint64_t expected = NumberCompress_getCompressed(i, bits);
        Assert_true(expected == EncodingScheme_convertLabel(scheme, expected,
                    EncodingScheme_convertLabel_convertTo_CANNONICAL));
    }
    Allocator_free(tempAlloc);

    return 0;
}
