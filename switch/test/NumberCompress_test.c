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

static const uint64_t MAGIC = 0xa984f5b; // last bit should be set.

static void numberCompressions_4() {
    for (uint32_t i = 0; i < NumberCompress_4_INTERFACES; ++i) {
        uint32_t bits = NumberCompress_4_bitsUsedForNumber(i);
        uint64_t label = NumberCompress_4_getCompressed(i, bits);
        if (1 == i) {
            Assert_always(1 == label);
            Assert_always(4 == bits);
        }
        Assert_always(bits == NumberCompress_4_bitsUsedForLabel(label));
        Assert_always(i == NumberCompress_4_getDecompressed(label, bits));

        // put some other stuff before the discrimator
        label |= MAGIC << bits;
        Assert_always(bits == NumberCompress_4_bitsUsedForLabel(label));
        Assert_always(i == NumberCompress_4_getDecompressed(label, bits));
    }
}

static void numberCompressions_8() {
    for (uint32_t i = 0; i < NumberCompress_8_INTERFACES; ++i) {
        uint32_t bits = NumberCompress_8_bitsUsedForNumber(i);
        uint64_t label = NumberCompress_8_getCompressed(i, bits);
        if (1 == i) {
            Assert_always(1 == label);
            Assert_always(4 == bits);
        }
        Assert_always(bits == NumberCompress_8_bitsUsedForLabel(label));
        Assert_always(i == NumberCompress_8_getDecompressed(label, bits));

        // put some other stuff before the discrimator
        label |= MAGIC << bits;
        Assert_always(bits == NumberCompress_8_bitsUsedForLabel(label));
        Assert_always(i == NumberCompress_8_getDecompressed(label, bits));
    }
}

static void numberCompressions_dyn() {
    for (uint32_t i = 0; i < NumberCompress_dyn_INTERFACES; ++i) {
        uint32_t bits = NumberCompress_dyn_bitsUsedForNumber(i);
        uint64_t label = NumberCompress_dyn_getCompressed(i, bits);
        if (1 == i) {
            Assert_always(1 == label);
            Assert_always(4 == bits);
        }
        Assert_always(bits == NumberCompress_dyn_bitsUsedForLabel(label));
        Assert_always(i == NumberCompress_dyn_getDecompressed(label, bits));

        // put some other stuff before the discrimator
        label |= MAGIC << bits;
        Assert_always(bits == NumberCompress_dyn_bitsUsedForLabel(label));
        Assert_always(i == NumberCompress_dyn_getDecompressed(label, bits));
    }
}


int main()
{
    numberCompressions_4();
    numberCompressions_8();
    numberCompressions_dyn();
    return 0;
}
