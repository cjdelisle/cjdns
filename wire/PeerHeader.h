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
 * The PeerHeader is sent between direct peers in order to detect congestion so packets can be
 * dropped when necessary. It also provides for peers to intentionally maintain multiple
 * simultanious sessions, for example a wifi connection and an eth connection.
 */
struct PeerHeader
{
    /** Version is set to PeerHeader_CURRENT_VERSION */
    uint8_t versionAndSessNum;

    /**
     * A number used to indicate that the link has become congested and traffic should be
     * decreased. In practice this may be the number of milliseconds of latency (clock - timestamp)
     *  
    uint8_t congest;

    uint16_t contentType_be;
};
#define PeerHeader_SIZE 4
Assert_compileTime(sizeof(struct PeerHeader) == PeerHeader_SIZE);

#define PeerHeader_CURRENT_VERSION 0


static inline enum ContentType PeerHeader_getContentType(struct PeerHeader* hdr)
{
    return Endian_bigEndianToHost16(hdr->contentType_be);
}

static inline void PeerHeader_setContentType(struct PeerHeader* hdr, enum ContentType type)
{
    Assert_true(type <= 0xffff);
    hdr->contentType_be = Endian_hostToBigEndian16(type);
}

static inline void PeerHeader_setVersion(struct PeerHeader* hdr, uint8_t version)
{
    hdr->versionAndFlags = (hdr->versionAndFlags & 0x0f) | (version << 4);
}

static inline uint8_t PeerHeader_getVersion(struct PeerHeader* hdr)
{
    return hdr->versionAndFlags >> 4;
}

#endif
