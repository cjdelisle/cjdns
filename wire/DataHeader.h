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
 *  0 |  ver  | unusd |     unused    |         Content Type          |
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * The DataHeader is protected from the switches by the l2 encryption layer.
 * It's primary use is to tell the endpoint the protocol of the content.
 */
struct DataHeader
{
    /** Version is set to DataHeader_CURRENT_VERSION version. */
    uint8_t versionAndFlags;

    uint8_t unused;

    uint16_t contentType_be;
};
#define DataHeader_SIZE 4
Assert_compileTime(sizeof(struct DataHeader) == DataHeader_SIZE);

#define DataHeader_CURRENT_VERSION 1


static inline enum ContentType DataHeader_getContentType(struct DataHeader* hdr)
{
    return Endian_bigEndianToHost16(hdr->contentType_be);
}

static inline void DataHeader_setContentType(struct DataHeader* hdr, enum ContentType type)
{
    Assert_true(type <= 0xffff);
    hdr->contentType_be = Endian_hostToBigEndian16(type);
}

static inline void DataHeader_setVersion(struct DataHeader* hdr, uint8_t version)
{
    hdr->versionAndFlags = (hdr->versionAndFlags & 0x0f) | (version << 4);
}

static inline uint8_t DataHeader_getVersion(struct DataHeader* hdr)
{
    return hdr->versionAndFlags >> 4;
}

#endif
