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
#include "switch/LabelSplicer.h"
#include "util/Bits.h"
#include "util/Endian.h"

#include <stdint.h>

/**
 * Splice a label and a label fragment together.
 *
 */
uint64_t LabelSplicer_splice(uint64_t goHere_be, uint64_t viaHere_be)
{
    uint64_t goHere = Endian_bigEndianToHost64(goHere_be);
    uint64_t viaHere = Endian_bigEndianToHost64(viaHere_be);
    uint64_t log2ViaHere = Bits_log2x64(viaHere);

    if (Bits_log2x64(goHere) + log2ViaHere > 60) {
        // Too big, can't splice.
        return UINT64_MAX;
    }

    return Endian_hostToBigEndian64(((goHere ^ 1) << log2ViaHere) ^ viaHere);
}

/**
 * Get the label for a particular destination from a given source.
 * This needs to be called before handing out a label because if a source interface is
 * represented using more bits than the destination interface, the destination interface
 * must be padded out so that the switch will find the source and destination labels compatable.
 *
 * @param target_be the label for the location to send to in big endian.
 * @param whoIsAsking_be the label for the node which we are sending the target to in big endian.
 * @return the modified target for that node in big endian.
 */
uint64_t LabelSplicer_getLabelFor(uint64_t target_be, uint64_t whoIsAsking_be)
{
    uint64_t target = Endian_bigEndianToHost64(target_be);
    uint64_t whoIsAsking = Endian_bigEndianToHost64(whoIsAsking_be);

    uint32_t targetBits = NumberCompress_bitsUsedForLabel(target);
    uint32_t whoIsAskingBits = NumberCompress_bitsUsedForLabel(whoIsAsking);

    if (targetBits >= whoIsAskingBits) {
        return target_be;
    }

    uint32_t targetIfaceNum = NumberCompress_getDecompressed(target, targetBits);

    uint64_t out = ((target & (UINT64_MAX << targetBits)) << (whoIsAskingBits - targetBits))
        | NumberCompress_getCompressed(targetIfaceNum, whoIsAskingBits);

    return Endian_hostToBigEndian64(out);
}

/**
 * Determine if the node at the end of the given label is one hop away.
 *
 * @param label_be the label to test.
 * @return true if the node is 1 hop away, false otherwise.
 */
bool LabelSplicer_isOneHop(uint64_t label_be)
{
    uint64_t label = Endian_bigEndianToHost64(label_be);
    return (int)NumberCompress_bitsUsedForLabel(label) == Bits_log2x64(label);
}
