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
#ifndef SwitchHeader_H
#define SwitchHeader_H

#include "util/Assert.h"
#include "util/Endian.h"
#include "util/version/Version.h"

#include <stdint.h>
#include <stdbool.h>

/**
 * The header which switches use to decide where to route traffic.
 *
 *                     1               2               3
 *     0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  0 |                                                               |
 *    +                         Switch Label                          +
 *  4 |                                                               |
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  8 |   Congest   |S| V |labelShift |            Penalty            |
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * Versions <= 7 byte number 8 is message type but the only 2 defined types were 0 (data)
 * and 1 (control).
 * Versions >= 8, byte number 8 is a congestion indicator but the lowest bit is a flag (S)
 * indicating that errors caused by this packet should be suppressed. Obviously an error message
 * should not trigger another error message so error messages will have the flag set.
 * Conveinently, all pre v8 versions will set the flag on every ctrl packet (including all errors).
 * Versions >= 8 will set congest to non-zero as a way to tell themselves apart from versions < 8
 * Versions < 8 are treating the byte as a number so all bits of congest will be zero.
 * Version >= 11 labelShift is the number of bits which the label has been shifted to the right
 *               in the course of switching. Switches older than 11 will not update this value!
 *               Penalty is used for QoS, see Penalty.h
 */

#pragma pack(push)
#pragma pack(4)
struct SwitchHeader
{
    /** The label, this is how the switch decides where to send the packet. Big Endian. */
    uint64_t label_be;

    /**
     * Top 7 bits: congest
     * Next bit: suppressErrors
     * In versions <= 7, this byte was set to 0 for data and 1 for ctrl packets.
     */
    uint8_t congestAndSuppressErrors;

    /**
     * High 2 bits: the version of the switch sending the packet.
     * Low 6 bits: the number of bits which the label has been shifted in the process of switching
     *             the packet.
     */
    uint8_t versionAndLabelShift;

    /** QoS Penalty, see Penalty.h */
    uint16_t penalty_be;
};
#define SwitchHeader_SIZE 12
Assert_compileTime(sizeof(struct SwitchHeader) == SwitchHeader_SIZE);
#pragma pack(pop)

#define SwitchHeader_MASK(x) ( (1 << (x)) - 1 )

#define SwitchHeader_CURRENT_VERSION 1

static inline uint32_t SwitchHeader_getVersion(const struct SwitchHeader* header)
{
    return header->versionAndLabelShift >> 6;
}

static inline void SwitchHeader_setVersion(struct SwitchHeader* header, uint8_t version)
{
    Assert_true(version < 4);
    header->versionAndLabelShift =
        (version << 6) | (header->versionAndLabelShift & SwitchHeader_MASK(6));
}

static inline uint32_t SwitchHeader_getLabelShift(const struct SwitchHeader* header)
{
    return header->versionAndLabelShift & SwitchHeader_MASK(6);
}

static inline void SwitchHeader_setLabelShift(struct SwitchHeader* header, uint32_t shift)
{
    Assert_true(shift < 64);
    header->versionAndLabelShift = header->versionAndLabelShift >> 6 << 6;
    header->versionAndLabelShift |= shift;
}

static inline uint32_t SwitchHeader_getCongestion(const struct SwitchHeader* header)
{
    return header->congestAndSuppressErrors >> 1;
}

static inline void SwitchHeader_setCongestion(struct SwitchHeader* header, uint32_t cong)
{
    Assert_true(cong <= 127);
    if (!cong) { cong++; }
    header->congestAndSuppressErrors = (header->congestAndSuppressErrors & 1) | (cong << 1);
}

static inline uint16_t SwitchHeader_getPenalty(const struct SwitchHeader* header)
{
    return Endian_bigEndianToHost16(header->penalty_be);
}

static inline void SwitchHeader_setPenalty(struct SwitchHeader* header, uint16_t penalty)
{
    header->penalty_be = Endian_hostToBigEndian16(penalty);
}

static inline bool SwitchHeader_getSuppressErrors(const struct SwitchHeader* header)
{
    return header->congestAndSuppressErrors & 1;
}

static inline void SwitchHeader_setSuppressErrors(struct SwitchHeader* header, bool suppress)
{
    header->congestAndSuppressErrors = header->congestAndSuppressErrors >> 1 << 1;
    header->congestAndSuppressErrors |= suppress;
}

#endif
