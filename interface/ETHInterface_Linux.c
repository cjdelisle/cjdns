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
#define string_strncpy
#include "exception/Except.h"
#include "interface/Interface.h"
#include "interface/ETHInterface.h"
#include "memory/Allocator.h"
#include "interface/InterfaceController.h"
#include "interface/MultiInterface.h"
#include "wire/Headers.h"
#include "wire/Message.h"
#include "wire/Error.h"
#include "wire/Ethernet.h"
#include "util/Assert.h"
#include "util/Errno.h"
#include "util/platform/libc/string.h"
#include "util/platform/Socket.h"
#include "util/events/Event.h"
#include "util/Identity.h"
#include "util/AddrTools.h"

#include <sys/socket.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>
#include <event2/event.h>
#include <sys/types.h>
#include <sys/ioctl.h>

#define MAX_PACKET_SIZE 1496
#define MIN_PACKET_SIZE 46

#define PADDING 512

// 2 last 0x00 of .sll_addr are removed from original size (20)
#define SOCKADDR_LL_LEN 18

struct ETHInterface
{
    struct Interface interface;

    Socket socket;

    uint8_t messageBuff[PADDING + MAX_PACKET_SIZE];

    /** The unix interface index which is used to identify the eth device. */
    int ifindex;

    struct Log* logger;

    struct InterfaceController* ic;

    struct MultiInterface* multiIface;

    struct sockaddr_ll addrBase;

    Identity
};

static uint8_t sendMessage(struct Message* message, struct Interface* ethIf)
{
    struct ETHInterface* context = Identity_cast((struct ETHInterface*) ethIf);

    struct sockaddr_ll addr;
    Message_pop(message, &addr, sizeof(struct sockaddr_ll));

    if (sendto(context->socket,
               message->bytes,
               message->length,
               0,
               (struct sockaddr*) &addr,
               sizeof(struct sockaddr_ll)) < 0)
    {
        enum Errno err = Errno_get();
        switch (err) {
            case Errno_EMSGSIZE:
                return Error_OVERSIZE_MESSAGE;

            case Errno_ENOBUFS:
            case Errno_EAGAIN:
                return Error_LINK_LIMIT_EXCEEDED;

            default:;
                Log_info(context->logger, "Got error sending to socket [%s]", Errno_strerror(err));
        }
    }
    return 0;
}

static void handleEvent(void* vcontext)
{
    struct ETHInterface* context = Identity_cast((struct ETHInterface*) vcontext);

    struct Message message =
        { .bytes = context->messageBuff + PADDING, .padding = PADDING, .length = MAX_PACKET_SIZE };

    struct sockaddr_ll addr = { .sll_family = 0 };
    uint32_t addrLen = sizeof(struct sockaddr_ll);

    // Start writing InterfaceController_KEY_SIZE after the beginning,
    // keyForSockaddr() will write the key there.
    int rc = recvfrom(context->socket,
                      message.bytes,
                      message.length,
                      0,
                      (struct sockaddr*) &addr,
                      &addrLen);

    Assert_true(addrLen == SOCKADDR_LL_LEN);

    /** "Magic" Warning!
     *
     * - smallest payload for AF_PACKET: 46 bytes
     * - smallest packet (empyrical): 40 bytes (aka "ctrl packet")
     */

    /* Begin of "magic" */
    if (rc == MIN_PACKET_SIZE) {
        // remove extra 0x00 from the end of the paylod
        rc = 40;
    }
    /* End of "magic" */

    if (rc < 0) {
        return;
    }

    message.length = rc;
    Message_push(&message, &addr, sizeof(struct sockaddr_ll));

    context->interface.receiveMessage(&message, &context->interface);
}

int ETHInterface_beginConnection(const char* macAddress,
                                 uint8_t cryptoKey[32],
                                 String* password,
                                 struct ETHInterface* ethIf)
{
    Identity_check(ethIf);
    struct sockaddr_ll addr;
    Bits_memcpyConst(&addr, &ethIf->addrBase, sizeof(struct sockaddr_ll));
    if (AddrTools_parseMac(addr.sll_addr, (const uint8_t*)macAddress)) {
        return ETHInterface_beginConnection_BAD_MAC;
    }

    struct Interface* iface = MultiInterface_ifaceForKey(ethIf->multiIface, &addr);
    int ret = InterfaceController_registerPeer(ethIf->ic, cryptoKey, password, false, iface);
    if (ret) {
        Allocator_free(iface->allocator);
        switch(ret) {
            case InterfaceController_registerPeer_BAD_KEY:
                return ETHInterface_beginConnection_BAD_KEY;

            case InterfaceController_registerPeer_OUT_OF_SPACE:
                return ETHInterface_beginConnection_OUT_OF_SPACE;

            default:
                return ETHInterface_beginConnection_UNKNOWN_ERROR;
        }
    }
    return 0;
}

struct ETHInterface* ETHInterface_new(struct EventBase* base,
                                      const char* bindDevice,
                                      struct Allocator* allocator,
                                      struct Except* exHandler,
                                      struct Log* logger,
                                      struct InterfaceController* ic)
{
    struct ETHInterface* context = Allocator_clone(allocator, (&(struct ETHInterface) {
        .interface = {
            .sendMessage = sendMessage,
            .allocator = allocator
        },
        .logger = logger,
        .ic = ic
    }));

    struct ifreq ifr = { .ifr_ifindex = 0 };

    context->socket = socket(AF_PACKET, SOCK_DGRAM, Ethernet_TYPE_CJDNS);
    if (context->socket == -1) {
        Except_raise(exHandler, ETHInterface_new_SOCKET_FAILED,
                     "call to socket() failed. [%s]", Errno_getString());
    }
    strncpy(ifr.ifr_name, bindDevice, IFNAMSIZ - 1);

    if (ioctl(context->socket, SIOCGIFINDEX, &ifr) == -1) {
        Except_raise(exHandler, ETHInterface_new_FAILED_FIND_IFACE,
                     "failed to find interface index [%s]", Errno_getString());
    }
    context->ifindex = ifr.ifr_ifindex;

    if (ioctl(context->socket, SIOCGIFHWADDR, &ifr) == -1) {
       Except_raise(exHandler, ETHInterface_new_FAILED_FIND_MACADDR,
                    "failed to find mac address of interface [%s]", Errno_getString());
    }

    uint8_t srcMac[6];
    Bits_memcpyConst(srcMac, ifr.ifr_hwaddr.sa_data, 6);

    // TODO: is the node's mac addr private information?
    Log_info(context->logger, "found MAC for device %s [%i]: %02x:%02x:%02x:%02x:%02x:%02x\n",
            bindDevice, context->ifindex,
            srcMac[0], srcMac[1], srcMac[2], srcMac[3], srcMac[4], srcMac[5]);

    context->addrBase = (struct sockaddr_ll) {
        .sll_family = AF_PACKET,
        .sll_protocol = Ethernet_TYPE_CJDNS,
        .sll_ifindex = context->ifindex,
        .sll_hatype = ARPHRD_ETHER,
        .sll_pkttype = PACKET_OTHERHOST,
        .sll_halen = ETH_ALEN
    };

    if (bind(context->socket, (struct sockaddr*) &context->addrBase, sizeof(struct sockaddr_ll))) {
        Except_raise(exHandler, ETHInterface_new_BIND_FAILED,
                     "call to bind() failed [%s]", Errno_getString());
    }

    evutil_make_socket_nonblocking(context->socket);

    Event_socketRead(handleEvent, context, context->socket, base, allocator, exHandler);

    context->multiIface = MultiInterface_new(sizeof(struct sockaddr_ll), &context->interface, ic);

    return context;
}
