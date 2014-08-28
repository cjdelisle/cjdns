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
 *  8 |   Congest   |S|                  Priority                     |
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
 */
#ifdef Version_7_COMPAT
    #define SwitchHeader_TYPE_DATA 0
    #define SwitchHeader_TYPE_CONTROL 1
#endif

#pragma pack(push)
#pragma pack(4)
struct SwitchHeader
{
    /** The label, this is how the switch decides where to send the packet. Big Endian. */
    uint64_t label_be;

    /**
     * Top 7 bits: congest
     * Next bit: suppressErrors
     *
     * Bottom 24 bits: priority
     * Anti-flooding, this is a big endian uint32_t with the high 8 bits cut off.
     *
     * This entire number is in big endian encoding.
     */
    uint32_t lowBits_be;
};
#define SwitchHeader_SIZE 12
Assert_compileTime(sizeof(struct SwitchHeader) == SwitchHeader_SIZE);
#pragma pack(pop)

static inline uint32_t SwitchHeader_getCongestion(const struct SwitchHeader* header)
{
    return Endian_bigEndianToHost32(header->lowBits_be) >> 25;
}

static inline void SwitchHeader_setCongestion(struct SwitchHeader* header, uint32_t cong)
{
    Assert_true(cong <= 127);
    if (!cong) { cong++; }
    header->lowBits_be &= Endian_hostToBigEndian32( ~0xfe000000 );
    header->lowBits_be |= Endian_hostToBigEndian32( cong << 25 );
}

static inline uint32_t SwitchHeader_getPriority(const struct SwitchHeader* header)
{
    return Endian_bigEndianToHost32(header->lowBits_be) & ((1 << 24) - 1);
}

static inline void SwitchHeader_setPriority(struct SwitchHeader* header, uint32_t priority)
{
    Assert_true(priority <= ((1 << 24) - 1) );
    header->lowBits_be &= Endian_hostToBigEndian32( ~((1 << 24) - 1) );
    header->lowBits_be |= Endian_hostToBigEndian32( priority & ((1 << 24) - 1) );
}

static inline bool SwitchHeader_getSuppressErrors(const struct SwitchHeader* header)
{
    return !!( Endian_hostToBigEndian32(1<<24) & header->lowBits_be );
}

static inline void SwitchHeader_setSuppressErrors(struct SwitchHeader* header, bool suppress)
{
    if (suppress) {
        header->lowBits_be |= Endian_hostToBigEndian32(1<<24);
    } else {
        header->lowBits_be &= Endian_hostToBigEndian32(~(1<<24));
    }
}

#ifdef Version_7_COMPAT
static inline void SwitchHeader_setPriorityAndMessageType(struct SwitchHeader* header,
                                                          const uint32_t priority,
                                                          const uint32_t messageType)
{
    Assert_true(messageType < 2);
    header->lowBits_be =
        Endian_hostToBigEndian32( (priority & ((1 << 24) - 1)) | messageType << 24 );
}
static inline uint32_t SwitchHeader_getMessageType(struct SwitchHeader* header)
{
    return Endian_bigEndianToHost32(header->lowBits_be) >> 24;
}
#endif

#endif
