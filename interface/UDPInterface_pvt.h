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
#ifndef UDPInterface_pvt_H
#define UDPInterface_pvt_H

#include "util/events/EventBase.h"
#include "interface/UDPInterface.h"
#include "net/InterfaceController.h"
#include "util/log/Log.h"
#include "memory/Allocator.h"
#include "util/Identity.h"

#include <event2/event.h>

#define UDPInterface_MAX_PACKET_SIZE 8192

#define UDPInterface_PADDING 512


struct UDPInterface_pvt
{
    struct UDPInterface pub;

    evutil_socket_t socket;

    /**
     * The event registered with libevent.
     * Needed only so it can be freed.
     */
    struct event* incomingMessageEvent;

    /** Used for testing. */
    struct sockaddr_storage addr;

    /** Used to tell what address type is being used. */
    ev_socklen_t addrLen;

    uint8_t messageBuff[UDPInterface_PADDING + UDPInterface_MAX_PACKET_SIZE];

    struct Log* logger;

    struct InterfaceController* ic;

    Identity
};

#endif
