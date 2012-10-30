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
#include "interface/TUNInterface_pvt.h"
#include "benc/String.h"
#include "util/Endian.h"

#include <string.h>
#include <event2/event.h>
#include <net/if.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>

#ifdef Linux
    #include <linux/if_ether.h>
    #define INET6_ETHERTYPE ETH_P_IPV6
#else
    #ifdef FreeBSD
        #include <netinet/in.h>  // struct in_addr
    #endif
    #include <netinet/if_ether.h>
    #define INET6_ETHERTYPE PF_INET6
#endif

// Defined extra large so large MTU can be taken advantage of later.
#define MAX_PACKET_SIZE 8192
#define PADDING_SPACE (10240 - MAX_PACKET_SIZE)

// The number of bytes at the beginning of the message which is used
// to contain the type of packet.
#ifdef Illumos
    #define PACKET_INFO_SIZE 0
#else
    #define PACKET_INFO_SIZE 4

    static void setPacketInfo(uint8_t* toLocation)
    {
        ((uint16_t*) toLocation)[0] = 0;
        ((uint16_t*) toLocation)[1] = Endian_bigEndianToHost16(INET6_ETHERTYPE);
    }
#endif


struct Context
{
    struct TUNInterface pub;
    struct event* incomingEvent;
    int fileDescriptor;
};

static void closeInterface(void* vcontext)
{
    struct Context* tun = vcontext;
    close(tun->fileDescriptor);
    event_del(tun->incomingEvent);
    event_free(tun->incomingEvent);
}

static void handleEvent(evutil_socket_t socket, short eventType, void* vcontext)
{
    // 292 bytes of extra padding to build headers back from for better efficiency.
    uint8_t messageBuffer[MAX_PACKET_SIZE + PADDING_SPACE];

    struct Message message = {
        .bytes = messageBuffer + PADDING_SPACE,
        .padding = PADDING_SPACE,
        .length = MAX_PACKET_SIZE
    };

    ssize_t length =
        read(socket, messageBuffer + PADDING_SPACE - PACKET_INFO_SIZE, MAX_PACKET_SIZE);

    if (length < 0) {
        printf("Error reading from TUN device %d\n", (int) length);
        return;
    }
    message.length = length - PACKET_INFO_SIZE;

    struct Interface* iface = &((struct Context*) vcontext)->pub.iface;
    if (iface->receiveMessage) {
        iface->receiveMessage(&message, iface);
    }
}

static uint8_t sendMessage(struct Message* message, struct Interface* iface)
{
    #if PACKET_INFO_SIZE > 0
        // The type of packet we send,
        // older linux kernels need this hint otherwise they assume it's ipv4.
        Message_shift(message, PACKET_INFO_SIZE);
        setPacketInfo(message->bytes);
    #endif

    struct Context* tun = iface->senderContext;
    ssize_t ret = write(tun->fileDescriptor, message->bytes, message->length);
    if (ret < 0) {
        printf("Error writing to TUN device %d\n", errno);
    }
    // TODO: report errors
    return 0;
}

struct TUNInterface* TUNInterface_new(void* tunSocket,
                                      struct event_base* base,
                                      struct Allocator* allocator)
{
    int tunFileDesc = (int) ((intptr_t) tunSocket);

    evutil_make_socket_nonblocking(tunFileDesc);

    struct Context* tun = allocator->malloc(sizeof(struct Context), allocator);
    tun->incomingEvent = event_new(base, tunFileDesc, EV_READ | EV_PERSIST, handleEvent, tun);
    tun->fileDescriptor = tunFileDesc;

    if (tun->incomingEvent == NULL) {
        abort();
    }

    struct Interface iface = {
        .senderContext = tun,
        .sendMessage = sendMessage,
        .allocator = allocator,
        .requiredPadding = 0,
        .maxMessageLength = MAX_PACKET_SIZE
    };

    Bits_memcpyConst(&tun->pub.iface, &iface, sizeof(struct Interface));

    event_add(tun->incomingEvent, NULL);

    allocator->onFree(closeInterface, tun, allocator);

    return &tun->pub;
}
