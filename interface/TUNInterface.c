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
#include "interface/Interface.h"
#include "interface/TUNInterface.h"
#include "util/Errno.h"
#include "util/events/Event.h"
#include "util/Identity.h"
#include "util/platform/Socket.h"
#include "wire/Ethernet.h"

#include <unistd.h>

// Defined extra large so large MTU can be taken advantage of later.
#define MAX_PACKET_SIZE 8192
#define PADDING_SPACE (10240 - MAX_PACKET_SIZE)

struct TUNInterface_pvt
{
    struct TUNInterface pub;
    Socket tunSocket;
    Identity
};

static void handleEvent(void* vcontext)
{
    struct TUNInterface_pvt* tun = Identity_cast((struct TUNInterface_pvt*) vcontext);

    struct Message* msg;
    Message_STACK(msg, MAX_PACKET_SIZE, PADDING_SPACE);

    ssize_t length = read(tun->tunSocket, msg->bytes, msg->length);

    if (length < 4) {
        printf("Error reading from TUN device %d\n", (int) length);
        return;
    }
    msg->length = length;

    struct Interface* iface = &tun->pub.iface;
    if (iface->receiveMessage) {
        iface->receiveMessage(msg, iface);
    }
}

static uint8_t sendMessage(struct Message* message, struct Interface* iface)
{
    struct TUNInterface_pvt* tun = Identity_cast((struct TUNInterface_pvt*) iface);

    ssize_t ret = write(tun->tunSocket, message->bytes, message->length);
    if (ret < 0) {
        printf("Error writing to TUN device %d\n", Errno_get());
    }
    // TODO: report errors
    return 0;
}

struct TUNInterface* TUNInterface_new(void* tunSocket,
                                      struct EventBase* base,
                                      struct Allocator* allocator)
{
    Socket tunSock = (Socket) ((intptr_t) tunSocket);

    Socket_makeNonBlocking(tunSock);

    struct TUNInterface_pvt* tun = Allocator_clone(allocator, (&(struct TUNInterface_pvt) {
        .pub = {
            .iface = {
                .sendMessage = sendMessage,
                .allocator = allocator,
                .requiredPadding = 0,
                .maxMessageLength = MAX_PACKET_SIZE
            }
        },
        .tunSocket = tunSock
    }));
    Identity_set(tun);

    Event_socketRead(handleEvent, tun, tunSock, base, allocator, NULL);

    return &tun->pub;
}
