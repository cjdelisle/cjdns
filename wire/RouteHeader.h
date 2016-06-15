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
#ifndef RouteHeader_H
#define RouteHeader_H

#include "util/Assert.h"
#include "wire/SwitchHeader.h"

/**
 * This header is never sent on the wire, it is only used for communicating to the SessionManager
 * which node you wish to communicate with and what path the packet should take.
 * Everything "below" the SessionManager is encrypted end-to-end, everything "above" has this
 * header. SessionManager will place this header on top of packets in order to tell the upper
 * layers the packet's origin. Upper layers must use this header in order to tell the SessionManager
 * to whom the packet should be sent.
 * Some of the fields in this header may be left zero when sending to the SessionManager,
 * for example if the publicKey of the node to communicate with is not known, you may send 32
 * bytes of zeros and the SessionManager will attempt to find the key in the SessionTable or
 * it will buffer the packet and trigger a search.
 * When the SessionManager emits a packet, all fields except possibly the version field will be
 * properly filled. If the version field is zero, the SessionManager has not determined the version
 * of the peer.
 */
struct RouteHeader
{
    /** public key of peer node, 0 if unknown, always sent from SessionManager. */
    uint8_t publicKey[32];

    /**
     * The switch header to use.
     * label_be may be zero if unknown.
     * version will be automatically set to the node's current version.
     */
    struct SwitchHeader sh;

    /** Protocol version of peer node, 0 if unknown, sometimes 0 from SessionManager. */
    uint32_t version_be;

    /**
     * If set, this header is incoming from another node, this means the ip6, publicKey and
     * switch label are of the *source* of the packet, not the destination.
     */
    #define RouteHeader_flags_INCOMING 1
    uint8_t flags;

    uint8_t unused;
    uint16_t alsoUnused;

    /** IPv6 of peer node REQUIRED */
    uint8_t ip6[16];
};
#define RouteHeader_SIZE (56 + SwitchHeader_SIZE)
Assert_compileTime(RouteHeader_SIZE == sizeof(struct RouteHeader));


#endif
