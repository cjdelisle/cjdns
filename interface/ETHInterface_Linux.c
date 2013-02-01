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
#include "util/version/Version.h"
#include "util/events/Timeout.h"

#include <sys/socket.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define MAX_PACKET_SIZE 1496
#define MIN_PACKET_SIZE 46

#define PADDING 512

// 2 last 0x00 of .sll_addr are removed from original size (20)
#define SOCKADDR_LL_LEN 18

/** Wait 16 seconds between sending beacon messages. */
#define BEACON_INTERVAL 32768

struct ETHInterface
{
    struct Interface generic;

    Socket socket;

    uint8_t messageBuff[PADDING + MAX_PACKET_SIZE];

    /** The unix interface index which is used to identify the eth device. */
    int ifindex;

    struct Log* logger;

    struct InterfaceController* ic;

    struct MultiInterface* multiIface;

    struct sockaddr_ll addrBase;

    int beaconState;

    /**
     * A unique(ish) id which will be different every time the router starts.
     * This will prevent new eth frames from being confused with old frames from an expired session.
     */
    uint16_t id;

    Identity
};

static uint8_t sendMessage(struct Message* message, struct Interface* ethIf)
{
    struct ETHInterface* context = Identity_cast((struct ETHInterface*) ethIf);

    struct sockaddr_ll addr;
    Bits_memcpyConst(&addr, &context->addrBase, sizeof(struct sockaddr_ll));
    Message_pop(message, addr.sll_addr, 8);

    /* Cut down on the noise
    uint8_t buff[sizeof(addr) * 2 + 1] = {0};
    Hex_encode(buff, sizeof(buff), (uint8_t*)&addr, sizeof(addr));
    Log_debug(context->logger, "Sending ethernet frame to [%s]", buff);
    */

    // Check if we will have to pad the message and pad if necessary.
    int pad = 0;
    for (int length = message->length; length+2 < MIN_PACKET_SIZE; length += 8) {
        pad++;
    }
    if (pad > 0) {
        int length = message->length;
        Message_shift(message, pad*8);
        Bits_memset(message->bytes, 0, pad*8);
        Bits_memmove(message->bytes, &message->bytes[pad*8], length);
    }
    Assert_true(pad < 8);
    uint16_t padAndId_be = Endian_hostToBigEndian16((context->id << 3) | pad);
    Message_push(message, &padAndId_be, 2);

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

static void handleBeacon(struct Message* msg, struct ETHInterface* context)
{
    if (!context->beaconState) {
        // accepting beacons disabled.
        Log_debug(context->logger, "Dropping beacon because beaconing is disabled");
        return;
    }

    struct sockaddr_ll addr;
    Bits_memcpyConst(&addr, &context->addrBase, sizeof(struct sockaddr_ll));
    Message_pop(msg, addr.sll_addr, 8);
    if (msg->length < Headers_Beacon_SIZE) {
        // Oversize messages are ok because beacons may contain more information in the future.
        Log_debug(context->logger, "Dropping wrong size beacon, expected [%d] got [%d]",
                  Headers_Beacon_SIZE, msg->length);
        return;
    }
    struct Headers_Beacon* beacon = (struct Headers_Beacon*) msg->bytes;

    uint32_t theirVersion = Endian_bigEndianToHost32(beacon->version_be);
    if (!Version_isCompatible(theirVersion, Version_CURRENT_PROTOCOL)) {
        #ifdef Log_DEBUG
            uint8_t mac[18];
            AddrTools_printMac(mac, addr.sll_addr);
            Log_debug(context->logger, "Dropped beacon from [%s] which was version [%d] "
                      "our version is [%d] making them incompatable",
                      mac, theirVersion, Version_CURRENT_PROTOCOL);
        #endif
        return;
    }

    #ifdef Log_DEBUG
        uint8_t mac[18];
        AddrTools_printMac(mac, addr.sll_addr);
        Log_debug(context->logger, "Got beacon from [%s]", mac);
    #endif

    String passStr = { .bytes = (char*) beacon->password, .len = Headers_Beacon_PASSWORD_LEN };
    struct Interface* iface = MultiInterface_ifaceForKey(context->multiIface, addr.sll_addr);
    int ret = InterfaceController_registerPeer(context->ic,
                                               beacon->publicKey,
                                               &passStr,
                                               false,
                                               true,
                                               iface);
    if (ret != 0) {
        uint8_t mac[18];
        AddrTools_printMac(mac, addr.sll_addr);
        Log_info(context->logger, "Got beacon from [%s] and registerPeer returned [%d]", mac, ret);
    }
}

static void sendBeacon(void* vcontext)
{
    struct ETHInterface* context = Identity_cast((struct ETHInterface*) vcontext);
    if (context->beaconState != ETHInterface_beacon_ACCEPTING_AND_SENDING) {
        // beaconing disabled
        return;
    }

    struct {
        struct sockaddr_ll addr;
        struct Headers_Beacon beacon;
    } content;

    Bits_memcpyConst(&content.addr, &context->addrBase, sizeof(struct sockaddr_ll));
    Bits_memset(content.addr.sll_addr, 0xff, 6);
    InterfaceController_populateBeacon(context->ic, &content.beacon);

    struct Message m = {
        .bytes=(uint8_t*)content.addr.sll_addr,
        .padding=0,
        .length=sizeof(struct Headers_Beacon) + 8
    };

    int ret;
    if ((ret = sendMessage(&m, &context->generic)) != 0) {
        Log_info(context->logger, "Got error [%d] sending beacon [%s]", ret, Errno_getString());
    }
}

static void handleEvent(void* vcontext)
{
    struct ETHInterface* context = Identity_cast((struct ETHInterface*) vcontext);

    struct Message message =
        { .bytes = context->messageBuff + PADDING, .padding = PADDING, .length = MAX_PACKET_SIZE };

    struct sockaddr_ll addr;
    uint32_t addrLen = sizeof(struct sockaddr_ll);

    // Knock it out of alignment by 2 bytes so that it will be
    // aligned when the idAndPadding is shifted off.
    Message_shift(&message, 2);

    int rc = recvfrom(context->socket,
                      message.bytes,
                      message.length,
                      0,
                      (struct sockaddr*) &addr,
                      &addrLen);

    if (rc < 0) {
        Log_debug(context->logger, "Failed to receive eth frame");
        return;
    }

    //Assert_true(addrLen == SOCKADDR_LL_LEN);

    // Pop the first 2 bytes of the message containing the node id and amount of padding.
    uint16_t idAndPadding_be;
    Message_pop(&message, &idAndPadding_be, 2);

    const uint16_t idAndPadding = Endian_bigEndianToHost16(idAndPadding_be);
    message.length = rc - 2 - ((idAndPadding & 7) * 8);
    const uint16_t id = idAndPadding >> 3;
    Message_push(&message, &id, 2);
    Message_push(&message, addr.sll_addr, 6);

    if (addr.sll_pkttype == PACKET_BROADCAST) {
        handleBeacon(&message, context);
        return;
    }

    /* Cut down on the noise
    uint8_t buff[sizeof(addr) * 2 + 1] = {0};
    Hex_encode(buff, sizeof(buff), (uint8_t*)&addr, sizeof(addr));
    Log_debug(context->logger, "Got ethernet frame from [%s]", buff);
    */

    context->generic.receiveMessage(&message, &context->generic);
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
    int ret = InterfaceController_registerPeer(ethIf->ic, cryptoKey, password, false, false, iface);
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

int ETHInterface_beacon(struct ETHInterface* ethIf, int* state)
{
    Identity_check(ethIf);
    if (state) {
        ethIf->beaconState = *state;
        // Send out a beacon right away so we don't have to wait.
        if (ethIf->beaconState == ETHInterface_beacon_ACCEPTING_AND_SENDING) {
            sendBeacon(ethIf);
        }
    }
    return ethIf->beaconState;
}

struct ETHInterface* ETHInterface_new(struct EventBase* base,
                                      const char* bindDevice,
                                      struct Allocator* allocator,
                                      struct Except* exHandler,
                                      struct Log* logger,
                                      struct InterfaceController* ic)
{
    struct ETHInterface* context = Allocator_clone(allocator, (&(struct ETHInterface) {
        .generic = {
            .sendMessage = sendMessage,
            .allocator = allocator
        },
        .logger = logger,
        .ic = ic,
        .id = getpid()
    }));

    Identity_set(context);

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

    Socket_makeNonBlocking(context->socket);

    Event_socketRead(handleEvent, context, context->socket, base, allocator, exHandler);

    context->multiIface = MultiInterface_new(sizeof(struct sockaddr_ll), &context->generic, ic);

    Timeout_setInterval(sendBeacon, context, BEACON_INTERVAL, base, allocator);

    return context;
}
