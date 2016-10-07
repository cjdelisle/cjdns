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
#define NumberCompress_OLD_CODE
#include "switch/NumberCompress.h"
#include "switch/EncodingScheme.h"
#include "memory/MallocAllocator.h"
#include "util/Assert.h"

#include <stdio.h>
#include <inttypes.h>

static void numberCompressions_generic(
    uint32_t nInterfaces,
    uint32_t (*bitsUsedForLabel)(const uint64_t label),
    uint32_t (*bitsUsedForNumber)(const uint32_t number),
    uint64_t (*getCompressed)(const uint32_t number, const uint32_t bitsUsed),
    uint32_t (*getDecompressed)(const uint64_t label, const uint32_t bitsUsed),
    struct EncodingScheme* (* defineScheme)(struct Allocator* alloc) )
{

    uint8_t bitWidths[64] = { 0 };

    for (uint32_t i = 0; i < nInterfaces; ++i) {
        bitWidths[bitsUsedForNumber(i)] = 1;
    }

    for (uint32_t bits = 0; bits < 64; ++bits) {
        if (!bitWidths[bits]) {
            continue;
        }

        for (uint32_t i = 0; i < nInterfaces; ++i) {
            /* only check for greater-or-equal bit widths */
            if (bits < bitsUsedForNumber(i)) {
                continue;
            }

            uint64_t label = getCompressed(i, bits);

            if (1 == i) {
                Assert_true(1 == label);
                continue;
            }

            Assert_true(bits == bitsUsedForLabel(label));
            Assert_true(i == getDecompressed(label, bits));
        }
    }

    for (uint64_t label = 0; label < 0x10000u; ++label) {
        uint32_t bits = bitsUsedForLabel(label);
        Assert_true(1 == bitWidths[bits]);
        if (1 == (label & Bits_maxBits64(bits))) {
            //Assert_true(4 == bits);
            Assert_true(1 == getDecompressed(label, bits));
        } else {
            uint32_t i = getDecompressed(label, bits);
            Assert_true(i < nInterfaces);
        }
    }

    struct Allocator* alloc = MallocAllocator_new(20000);
    struct EncodingScheme* scheme = defineScheme(alloc);
    for (uint32_t i = 0; i < nInterfaces; i++) {
        for (int j = 0; j < scheme->count; j++) {
            int bits = EncodingScheme_formSize(&scheme->forms[j]);
            if ((int)bitsUsedForNumber(i) > bits) { continue; }
            uint64_t label = getCompressed(i, bits);
            for (int k = j; k < scheme->count; k++) {
                uint64_t labelB = EncodingScheme_convertLabel(scheme, label, k);
                if (1 == i && k != 0) {
                    Assert_true(1 == label);
                    Assert_true(EncodingScheme_convertLabel_INVALID == labelB);
                    continue;
                }
                int bitsB = bitsUsedForLabel(labelB);
                Assert_true(bitsB == scheme->forms[k].prefixLen + scheme->forms[k].bitCount
                    || (i == 1 && bitsB == 4));
                Assert_true(i == getDecompressed(labelB, bitsB));

                uint64_t labelC = EncodingScheme_convertLabel(scheme, labelB, j);
                Assert_true(labelC == label);
            }
        }
    }
    Allocator_free(alloc);
}

#define TEST(impl) \
    numberCompressions_generic(            \
        GLUE(impl, INTERFACES),            \
        GLUE(impl, bitsUsedForLabel),      \
        GLUE(impl, bitsUsedForNumber),     \
        GLUE(impl, getCompressed),         \
        GLUE(impl, getDecompressed),       \
        GLUE(impl, defineScheme)           \
    )

#define GLUE(a,b) GLUE2(a,b)
#define GLUE2(a,b) NumberCompress_ ## a ## _ ## b

int main()
{
    TEST(f4);
    TEST(f8);
    TEST(v3x5x8);
    TEST(v4x8);
    return 0;
}
