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
#include "net/InterfaceController.h"
#include "wire/Headers.h"
#include "wire/Message.h"
#include "wire/Error.h"
#include "util/Assert.h"
#include "util/Errno.h"
#include "util/platform/libc/string.h"

#ifdef WIN32
    #include <winsock.h>
    #undef interface
#else
    #include <sys/socket.h>
    #include <linux/if_packet.h>
    #include <linux/if_ether.h>
    #include <linux/if_arp.h>
#endif

#include <event2/event.h>
#include <sys/types.h>
#include <sys/ioctl.h>

#define MAX_PACKET_SIZE 1496
#define MIN_PACKET_SIZE 46

#define PADDING 512

#define MAX_INTERFACES 256

#define ETH_P_CJDNS 0xFC00

// 2 last 0x00 of .sll_addr are removed from original size (20)
#define SOCKADDR_LL_LEN 18

struct ETHInterface
{
    struct Interface interface;

    evutil_socket_t socket;

    /**
     * The event registered with libevent.
     * Needed only so it can be freed.
     */
    struct event* incomingMessageEvent;

    uint8_t messageBuff[PADDING + MAX_PACKET_SIZE];

    /** The unix interface index which is used to identify the eth device. */
    int ifindex;

    struct Log* logger;

    struct InterfaceController* ic;

    /** A base sockaddr_ll which is used to build the address to send packets to. */
    struct sockaddr_ll addrBase;
};

static inline void sockaddrForKey(struct sockaddr_ll* sockaddr,
                                  uint8_t key[InterfaceController_KEY_SIZE],
                                  struct ETHInterface* ethIf)
{
    Assert_true(key[6] == 'e' && key[7] == 'n');
    Bits_memcpyConst(sockaddr, &ethIf->addrBase, sizeof(struct sockaddr_ll));
    Bits_memcpyConst(sockaddr->sll_addr, key, 6);
}

static inline void keyForSockaddr(uint8_t key[InterfaceController_KEY_SIZE],
                                  struct sockaddr_ll* sockaddr,
                                  struct ETHInterface* ethIf)
{
    Bits_memcpyConst(key, sockaddr->sll_addr, 6);
    key[6] = 'e';
    key[7] = 'n';
}

static uint8_t sendMessage(struct Message* message, struct Interface* ethIf)
{
    struct ETHInterface* context = ethIf->senderContext;
    Assert_true(&context->interface == ethIf);

    struct sockaddr_ll addr;

    sockaddrForKey(&addr, message->bytes, context);
    Bits_memcpyConst(&addr.sll_addr, message->bytes, 6);
    Message_shift(message, -InterfaceController_KEY_SIZE);

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

/**
 * Release the event used by this module.
 *
 * @param vevent a void pointer cast of the event structure.
 */
static void freeEvent(void* vevent)
{
    event_del((struct event*) vevent);
    event_free((struct event*) vevent);
}

static void handleEvent(evutil_socket_t socket, short eventType, void* vcontext)
{
    struct ETHInterface* context = (struct ETHInterface*) vcontext;

    struct Message message =
        { .bytes = context->messageBuff + PADDING, .padding = PADDING, .length = MAX_PACKET_SIZE };

    struct sockaddr_ll addr;
    Bits_memset(&addr, 0, sizeof(struct sockaddr_ll));
    ev_socklen_t addrLen = sizeof(struct sockaddr_ll);

    // Start writing InterfaceController_KEY_SIZE after the beginning,
    // keyForSockaddr() will write the key there.
    int rc = recvfrom(socket,
                      message.bytes + InterfaceController_KEY_SIZE,
                      message.length - InterfaceController_KEY_SIZE,
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

    message.length = rc + InterfaceController_KEY_SIZE;

    keyForSockaddr(message.bytes, &addr, context);

    context->interface.receiveMessage(&message, &context->interface);
}

int ETHInterface_beginConnection(const char* macAddress,
                                 uint8_t cryptoKey[32],
                                 String* password,
                                 struct ETHInterface* ethIf)
{
    struct sockaddr_ll addr;
    if (AddrTools_parseMac(addr.sll_addr, (const uint8_t*)macAddress)) {
        return ETHInterface_beginConnection_BAD_MAC;
    }

    uint8_t key[InterfaceController_KEY_SIZE];
    keyForSockaddr(key, &addr, ethIf);
    int ret = ethIf->ic->insertEndpoint(key, cryptoKey, password, &ethIf->interface, ethIf->ic);
    switch(ret) {
        case 0:
            return 0;

        case InterfaceController_registerInterface_BAD_KEY:
            return ETHInterface_beginConnection_BAD_KEY;

        case InterfaceController_registerInterface_OUT_OF_SPACE:
            return ETHInterface_beginConnection_OUT_OF_SPACE;

        default:
            return ETHInterface_beginConnection_UNKNOWN_ERROR;
    }
}

struct ETHInterface* ETHInterface_new(struct event_base* base,
                                      const char* bindDevice,
                                      struct Allocator* allocator,
                                      struct Except* exHandler,
                                      struct Log* logger,
                                      struct InterfaceController* ic)
{
    struct ETHInterface* context = allocator->malloc(sizeof(struct ETHInterface), allocator);
    Bits_memcpyConst(context, (&(struct ETHInterface) {
        .interface = {
            .sendMessage = sendMessage,
            .senderContext = context,
            .allocator = allocator
        },
        .logger = logger,
        .ic = ic
    }), sizeof(struct ETHInterface));

    struct ifreq ifr;

    context->socket = socket(AF_PACKET, SOCK_DGRAM, Endian_hostToBigEndian16(ETH_P_CJDNS));
    if (context->socket == -1) {
        Except_raise(exHandler, ETHInterface_new_SOCKET_FAILED,
                     "call to socket() failed. [%s]", Errno_getString());
    }
    strncpy(ifr.ifr_name, bindDevice, IFNAMSIZ);
    ifr.ifr_name[sizeof(ifr.ifr_name)-1] = '\0';

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
        .sll_protocol = Endian_hostToBigEndian16(ETH_P_CJDNS),
        .sll_ifindex = context->ifindex,
        .sll_hatype = ARPHRD_ETHER,
        .sll_pkttype = PACKET_OTHERHOST,
        .sll_halen = ETH_ALEN,
        .sll_addr[6] = 0x00,
        .sll_addr[7] = 0x00,
    };


    if (bind(context->socket, (struct sockaddr*) &context->addrBase, sizeof(struct sockaddr_ll))) {
        Except_raise(exHandler, ETHInterface_new_BIND_FAILED,
                     "call to bind() failed [%s]", Errno_getString());
    }

    evutil_make_socket_nonblocking(context->socket);

    context->incomingMessageEvent =
        event_new(base, context->socket, EV_READ | EV_PERSIST, handleEvent, context);

    if (!context->incomingMessageEvent || event_add(context->incomingMessageEvent, NULL)) {
        Except_raise(exHandler, ETHInterface_new_FAILED_CREATING_EVENT,
                     "failed to create ETHInterface event [%s]", Errno_getString());
    }

    allocator->onFree(freeEvent, context->incomingMessageEvent, allocator);

    ic->registerInterface(&context->interface, ic);

    return context;
}
