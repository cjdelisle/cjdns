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
#ifndef BridgeHeader_H
#define BridgeHeader_H

#include "wire/ContentType.h"

/**
 *                     1               2               3
 *     0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  0 |    version    |    unused     |             length            |
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *    |                         ContentType                           |
 *    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * The BridgeHeader is a data header which is used internally for sending messages
 * around the inside of cjdns. Length and ContentType fields are both in host
 * endian encoding.
 */
struct BridgeHeader
{
    /** If this is a mismatch, fail. */
    uint8_t version;

    uint8_t unused;
    uint16_t length;
    enum ContentType type;
};
#define BridgeHeader_SIZE 8
Assert_compileTime(sizeof(struct BridgeHeader) == BridgeHeader_SIZE);

#define BridgeHeader_CURRENT_VERSION 0

#endif
