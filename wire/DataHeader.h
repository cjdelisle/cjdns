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
#ifndef DataHeader_H
#define DataHeader_H

#include "util/Assert.h"
#include "util/Endian.h"
#include "wire/ContentType.h"

/**
 *                     1               2               3
 *     0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  0 | ver |R| unused|    unused     |         Content Type          |
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * The DataHeader is protected from the switches by the l2 encryption layer and from the
 * routers by the l3 layer of encryption. It's primary uses are to tell the endpoint enough
 * information to route the packet to the correct cjdns subsystem and (maybe) reconstruct
 * the IPv6 header.
 */
struct DataHeader
{
    /**
     * If set, the receiving node should respond within 10 milliseconds, if in the next 10
     * milliseconds it has something else to send back, this is acceptable, otherwise it should
     * synthisize a packet to respond with.
     */
    #define DataHeader_RESPOND_TO_ME (1<<7)

    /**
     * Version is set to DataHeader_CURRENT_VERSION version.
     * If the number is over 255, it wraps
     */
    uint8_t versionAndFlags;

    uint8_t unused;

    uint16_t contentType_be;
};
#define DataHeader_SIZE 4
Assert_compileTime(sizeof(struct DataHeader) == DataHeader_SIZE);

#define DataHeader_CURRENT_VERSION 0


static inline enum ContentType DataHeader_getContentType(struct DataHeader* hdr)
{
    return Endian_bigEndianToHost16(hdr->contentType_be);
}

static inline void DataHeader_setContentType(struct DataHeader* hdr, enum ContentType type)
{
    Assert_true(type <= DataHeader_ContentType_AVAILABLE_MAX);
    hdr->contentType_be = Endian_hostToBigEndian16(type);
}

static inline void DataHeader_setRespondToMe(struct DataHeader* hdr, bool rtm)
{
    type->versionAndFlags = (type->versionAndFlags & ~DataHeader_RESPOND_TO_ME) |
        (rtm) ? DataHeader_RESPOND_TO_ME : 0;
}

static inline bool DataHeader_getRespondToMe(struct DataHeader* hdr)
{
    return type->versionAndFlags & DataHeader_RESPOND_TO_ME;
}

static inline void DataHeader_setVersion(struct DataHeader* hdr, uint8_t version)
{
    type->versionAndFlags = (type->versionAndFlags & 0x1f) | ((version % 8) << 5);
}

static inline uint8_t DataHeader_getVersion(struct DataHeader* hdr)
{
    return type->versionAndFlags >> 5;
}

#endif
