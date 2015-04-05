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
#ifndef PeerHeader_H
#define PeerHeader_H

#include "util/Assert.h"
#include "util/Endian.h"

/**
 *                     1               2               3
 *     0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  0 | ver | sessNum |    congest    |           Timestamp           |
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * The PeerHeader is sent between direct peers in order to detect congestion.
 * It also provides for peers to wish to intentionally maintain multiple simultanious sessions,
 * for example a wifi connection and an eth connection.
 *
 * @ver the version number of the PeerHeader, set to PeerHeader_CURRENT_VERSION
 * @sessNum if one node connects to another one multiple times using different interfaces, this
 *          number should be incremented for each connection between the two nodes.
 * @congest an explicit value indicating congestion in opposite direction of the link.
 * @param timestamp a (big endian) number containing which is set using the millisecond clock on
 *                  the sending node. The receiving node can use this to detect probable skew
 *                  and then notify via the congest field if packets begin arriving late.
 */
struct PeerHeader
{
    /** Version is set to PeerHeader_CURRENT_VERSION */
    uint8_t versionAndSessNum;

    /**
     * A number used to indicate that the link has become congested and traffic flow should be
     * decreased. In practice this may be the number of milliseconds of latency (clock - timestamp)
     * minus the smallest latency ever detected on the link. It should go without saying that if
     * the value is negative, it should be set to zero and if it is above 255, it should be 255.
     */
    uint8_t congest;

    /** The time when the packet left the last internal buffer in the sending node. */
    uint16_t timestamp_be;
};
#define PeerHeader_SIZE 4
Assert_compileTime(sizeof(struct PeerHeader) == PeerHeader_SIZE);

#define PeerHeader_CURRENT_VERSION 0


static inline uint16_t PeerHeader_getTimestamp(struct PeerHeader* hdr)
{
    return Endian_bigEndianToHost16(hdr->timestamp_be);
}

static inline void PeerHeader_setTimestamp(struct PeerHeader* hdr, uint16_t timestamp)
{
    hdr->timestamp_be = Endian_hostToBigEndian16(timestamp);
}

static inline void PeerHeader_setVersion(struct PeerHeader* hdr, uint8_t version)
{
    Assert_true(version < (1<<5));
    hdr->versionAndSessNum = (hdr->versionAndSessNum & ((1<<5)-1)) | (version << 5);
}

static inline uint8_t PeerHeader_getVersion(struct PeerHeader* hdr)
{
    return hdr->versionAndSessNum >> 5;
}

static inline uint8_t PeerHeader_getSessionNum(struct PeerHeader* hdr)
{
    return hdr->versionAndSessNum & ((1<<5)-1);
}

static inline uint8_t PeerHeader_setSessionNum(struct PeerHeader* hdr, uint8_t sessNum)
{
    Assert_true(sessNum <= 1<<5);
    hdr->versionAndSessNum = ( hdr->versionAndSessNum & ((1<<5)-1) ) | sessNum;
}

#endif
