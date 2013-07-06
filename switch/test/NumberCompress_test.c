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
#include "switch/NumberCompress.h"

#include "util/Assert.h"
#include <stdio.h>
#include <inttypes.h>

static void numberCompressions_generic(
    uint32_t nInterfaces,
    uint32_t (*bitsUsedForLabel)(const uint64_t label),
    uint32_t (*bitsUsedForNumber)(const uint32_t number),
    uint64_t (*getCompressed)(const uint32_t number, const uint32_t bitsUsed),
    uint32_t (*getDecompressed)(const uint64_t label, const uint32_t bitsUsed))
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
                Assert_always(1 == label);
                continue;
            }

            Assert_always(bits == bitsUsedForLabel(label));
            Assert_always(i == getDecompressed(label, bits));
        }
    }

    for (uint64_t label = 0; label < 0x10000u; ++label) {
        uint32_t bits = bitsUsedForLabel(label);
        Assert_always(1 == bitWidths[bits]);
        if (1 == (label & 0xf)) {
            Assert_always(4 == bits);
            Assert_always(1 == getDecompressed(label, 4));
        } else {
            uint32_t i = getDecompressed(label, bits);
            Assert_always(i < nInterfaces);
        }
    }
}

int main()
{
    numberCompressions_generic(
        NumberCompress_4_INTERFACES,
        NumberCompress_4_bitsUsedForLabel,
        NumberCompress_4_bitsUsedForNumber,
        NumberCompress_4_getCompressed,
        NumberCompress_4_getDecompressed);

    numberCompressions_generic(
        NumberCompress_8_INTERFACES,
        NumberCompress_8_bitsUsedForLabel,
        NumberCompress_8_bitsUsedForNumber,
        NumberCompress_8_getCompressed,
        NumberCompress_8_getDecompressed);

    numberCompressions_generic(
        NumberCompress_dyn_INTERFACES,
        NumberCompress_dyn_bitsUsedForLabel,
        NumberCompress_dyn_bitsUsedForNumber,
        NumberCompress_dyn_getCompressed,
        NumberCompress_dyn_getDecompressed);
    return 0;
}
