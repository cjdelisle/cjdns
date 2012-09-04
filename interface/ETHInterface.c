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
#include "crypto/Crypto.h"
#include "exception/ExceptionHandler.h"
#include "interface/Interface.h"
#include "interface/ETHInterface.h"
#include "memory/Allocator.h"
#include "net/InterfaceController.h"
#include "wire/Message.h"
#include "wire/Error.h"
#include "util/Assert.h"

#include <string.h>
#include <stdio.h>

#ifdef WIN32
    #include <winsock.h>
    #undef interface
    #define EMSGSIZE WSAEMSGSIZE
    #define ENOBUFS WSAENOBUFS
    #define EWOULDBLOCK WSAEWOULDBLOCK
#else
    #include <sys/socket.h>
    #include <linux/if_packet.h>
    #include <linux/if_ether.h>
    #include <linux/if_arp.h>
#endif

#include <event2/event.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <errno.h>

#define MAX_PACKET_SIZE 1496

#define PADDING 0

#define MAX_INTERFACES 256

#define ETH_P_CJDNS ETH_P_ALL

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

    struct sockaddr_ll sll;

    sockaddrForKey(&sll, message->bytes, context);
    Bits_memcpyConst(&sll.sll_addr, message->bytes, InterfaceController_KEY_SIZE);
    Message_shift(message, -InterfaceController_KEY_SIZE);

printf("sin->ll_addr: %02X:%02X:%02X:%02X:%02X:%02X\n",
        sll.sll_addr[0],
        sll.sll_addr[1],
        sll.sll_addr[2],
        sll.sll_addr[3],
        sll.sll_addr[4],
        sll.sll_addr[5]);

printf("sendto(%d, %02X%02X%02X%02X%02X%02X%02X%02X, %d, 0, sin)\n",
        context->socket,
        message->bytes[0],
        message->bytes[1],
        message->bytes[2],
        message->bytes[3],
        message->bytes[4],
        message->bytes[5],
        message->bytes[6],
        message->bytes[7],
        message->length);

    if (sendto(context->socket,
               message->bytes,
               message->length + ETH_ALEN,
               0,
               (struct sockaddr*) &sll,
               sizeof(struct sockaddr)) < 0)
    {
        switch (EVUTIL_SOCKET_ERROR()) {
            case EMSGSIZE:
                return Error_OVERSIZE_MESSAGE;

            case ENOBUFS:
            case EAGAIN:
            #if EWOULDBLOCK != EAGAIN
                case EWOULDBLOCK:
            #endif
                return Error_LINK_LIMIT_EXCEEDED;

            default:;
                perror("sendto");
                Log_info(context->logger, "Got error sending to socket errno=%d",
                          EVUTIL_SOCKET_ERROR());
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
    memset(&addr, 0, sizeof(struct sockaddr_ll));
    ev_socklen_t addrLen = sizeof(struct sockaddr_ll);

printf("Got Event!\n");

    // Start writing InterfaceController_KEY_SIZE after the beginning,
    // keyForSockaddr() will write the key there.
    int rc = recvfrom(socket,
                      message.bytes + InterfaceController_KEY_SIZE,
                      message.length - InterfaceController_KEY_SIZE,
                      0,
                      (struct sockaddr*) &addr,
                      &addrLen);

printf("rc = %i\n", rc);

    Assert_true(addrLen == sizeof(struct sockaddr_ll));
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

printf("---> MAC ADDRESS: %s\n", macAddress);
uint8_t printedMac[18];
AddrTools_printMac(printedMac, addr.sll_addr);
printf("connectTo:        %s\n", printedMac);

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
                                      struct ExceptionHandler* exHandler,
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
                     "call to socket() failed. [%s]", strerror(errno));
    }
    strncpy(ifr.ifr_name, bindDevice, IFNAMSIZ);
    ifr.ifr_name[sizeof(ifr.ifr_name)-1] = '\0';

    if (ioctl(context->socket, SIOCGIFINDEX, &ifr) == -1) {
        Except_raise(exHandler, ETHInterface_new_FAILED_FIND_IFACE,
                     "failed to find interface index [%s]", strerror(errno));
    }
    context->ifindex = ifr.ifr_ifindex;

    if (ioctl(context->socket, SIOCGIFHWADDR, &ifr) == -1) {
       Except_raise(exHandler, ETHInterface_new_FAILED_FIND_MACADDR,
                    "failed to find mac address of interface [%s]", strerror(errno));
    }

    uint8_t srcMac[6];
    Bits_memcpyConst(srcMac, ifr.ifr_hwaddr.sa_data, 6);

    // TODO: is the node's mac addr private information?
    Log_info(context->logger, "found MAC for device %s [%i]: %02x:%02x:%02x:%02x:%02x:%02x\n",
            bindDevice, context->ifindex,
            srcMac[0], srcMac[1], srcMac[2], srcMac[3], srcMac[4], srcMac[5]);


    context->addrBase.sll_family = AF_PACKET;
    context->addrBase.sll_protocol = Endian_hostToBigEndian16(ETH_P_CJDNS);    // used in bind()
    context->addrBase.sll_ifindex = context->ifindex;     // used in bind()
    context->addrBase.sll_hatype = ARPHRD_ETHER;
    context->addrBase.sll_pkttype = PACKET_OTHERHOST;
    context->addrBase.sll_halen = ETH_ALEN;
    context->addrBase.sll_addr[6] = 0x00;
    context->addrBase.sll_addr[7] = 0x00;


    if (bind(context->socket, (struct sockaddr*) &context->addrBase, sizeof(struct sockaddr_ll))) {
        Except_raise(exHandler, ETHInterface_new_BIND_FAILED,
                     "call to bind() failed [%s]", strerror(errno));
    }

    evutil_make_socket_nonblocking(context->socket);

    context->incomingMessageEvent =
        event_new(base, context->socket, EV_READ | EV_PERSIST, handleEvent, context);

    if (!context->incomingMessageEvent || event_add(context->incomingMessageEvent, NULL)) {
        Except_raise(exHandler, ETHInterface_new_FAILED_CREATING_EVENT,
                     "failed to create ETHInterface event [%s]", strerror(errno));
    }

    allocator->onFree(freeEvent, context->incomingMessageEvent, allocator);

    ic->registerInterface(&context->interface, ic);

    return context;
}
