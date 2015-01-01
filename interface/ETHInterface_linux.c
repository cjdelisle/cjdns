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
#include "util/platform/Socket.h"
#include "util/events/Event.h"
#include "util/Identity.h"
#include "util/AddrTools.h"
#include "util/version/Version.h"
#include "util/events/Timeout.h"

#include <string.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>

#define MAX_PACKET_SIZE 1496
#define MIN_PACKET_SIZE 46

#define PADDING 512

// 2 last 0x00 of .sll_addr are removed from original size (20)
#define SOCKADDR_LL_LEN 18

/** Wait 32 seconds between sending beacon messages. */
#define BEACON_INTERVAL 32768

#ifdef Version_12_COMPAT
    struct EightBytes { uint8_t bytes[8]; };
    #define Map_NAME isV12ByMacAddr
    #define Map_KEY_TYPE struct EightBytes
    #define Map_VALUE_TYPE uint32_t
    #include "util/Map.h"
#endif

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

    String* ifName;

    #ifdef Version_12_COMPAT
        /**
         * A unique(ish) id which will be different every time the router starts.
         * This will prevent new eth frames from being confused with old frames from an expired
         * session.
         */
        uint16_t id;

        struct Map_isV12ByMacAddr v12Map;
    #endif

    Identity
};

#ifdef Version_12_COMPAT
static uint16_t getIdAndPadding(int msgLength, struct ETHInterface* context)
{
    int pad = 0;
    for (int length = msgLength; length+2 < MIN_PACKET_SIZE; length += 8) {
        pad++;
    }
    Assert_true(pad < 8);
    return (context->id << 3) | pad;
}
#endif

static uint8_t sendMessageInternal(struct Message* message, struct ETHInterface* context)
{
    struct sockaddr_ll addr;
    Bits_memcpyConst(&addr, &context->addrBase, sizeof(struct sockaddr_ll));
    Message_pop(message, addr.sll_addr, 6, NULL);

    /* Cut down on the noise
    uint8_t buff[sizeof(addr) * 2 + 1] = {0};
    Hex_encode(buff, sizeof(buff), (uint8_t*)&addr, sizeof(addr));
    Log_debug(context->logger, "Sending ethernet frame to [%s]", buff);
    */

    if (sendto(context->socket,
               message->bytes,
               message->length,
               0,
               (struct sockaddr*) &addr,
               sizeof(struct sockaddr_ll)) < 0)
    {
        switch (errno) {
            case EMSGSIZE:
                return Error_OVERSIZE_MESSAGE;

            case ENOBUFS:
            case EAGAIN:
                return Error_LINK_LIMIT_EXCEEDED;

            default:;
                Log_info(context->logger, "[%s] Got error sending to socket [%s]",
                         context->ifName->bytes, strerror(errno));
        }
    }
    return 0;
}

static uint8_t sendMessage(struct Message* msg, struct Interface* ethIf)
{
    struct ETHInterface* ctx = Identity_check((struct ETHInterface*) ethIf);
    struct EightBytes addr = { .bytes = { 0 } };
    Message_pop(msg, addr.bytes, 6, NULL);
    Message_shift(msg, -2, NULL);

    #ifdef Version_12_COMPAT
    int idx = Map_isV12ByMacAddr_indexForKey(&addr, &ctx->v12Map);
    if (idx > -1 && ctx->v12Map.values[idx]) {
        Message_push16(msg, getIdAndPadding(msg->length, ctx), NULL);
        Message_push(msg, &addr, 6, NULL);
        return sendMessageInternal(msg, ctx);
    }
    #endif

    struct ETHInterface_Header hdr = {
        .version = Version_CURRENT_PROTOCOL & 0xff,
        .zero = 0,
        .length_be = Endian_hostToBigEndian16(msg->length + ETHInterface_Header_SIZE),
        .fc00_be = Endian_hostToBigEndian16(0xfc00)
    };
    Message_push(msg, &hdr, ETHInterface_Header_SIZE, NULL);
    Message_push(msg, &addr, 6, NULL);
    return sendMessageInternal(msg, ctx);
}

static void handleBeacon(struct Message* msg, struct ETHInterface* context)
{
    if (!context->beaconState) {
        // accepting beacons disabled.
        Log_debug(context->logger, "[%s] Dropping beacon because beaconing is disabled",
                  context->ifName->bytes);
        return;
    }

    struct sockaddr_ll addr;
    Bits_memcpyConst(&addr, &context->addrBase, sizeof(struct sockaddr_ll));
    Message_pop(msg, addr.sll_addr, 8, NULL);
    addr.sll_addr[6] = 0;
    addr.sll_addr[7] = 0;
    if (msg->length < Headers_Beacon_SIZE) {
        // Oversize messages are ok because beacons may contain more information in the future.
        Log_debug(context->logger, "[%s] Dropping wrong size beacon, expected [%d] got [%d]",
                  context->ifName->bytes, Headers_Beacon_SIZE, msg->length);
        return;
    }
    struct Headers_Beacon* beacon = (struct Headers_Beacon*) msg->bytes;

    uint32_t theirVersion = Endian_bigEndianToHost32(beacon->version_be);
    if (!Version_isCompatible(theirVersion, Version_CURRENT_PROTOCOL)) {
        #ifdef Log_DEBUG
            uint8_t mac[18];
            AddrTools_printMac(mac, addr.sll_addr);
            Log_debug(context->logger, "[%s] Dropped beacon from [%s] which was version [%d] "
                      "our version is [%d] making them incompatable",
                      context->ifName->bytes, mac, theirVersion, Version_CURRENT_PROTOCOL);
        #endif
        return;
    }

    #ifdef Log_DEBUG
        uint8_t mac[18];
        AddrTools_printMac(mac, addr.sll_addr);
        Log_debug(context->logger, "[%s] Got beacon from [%s]", context->ifName->bytes, mac);
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
        Log_info(context->logger, "[%s] Got beacon from [%s] and registerPeer returned [%d]",
                 context->ifName->bytes, mac, ret);
        Allocator_free(iface->allocator);
    }
}

static void sendBeacon(void* vcontext)
{
    struct ETHInterface* context = Identity_check((struct ETHInterface*) vcontext);

    if (context->beaconState != ETHInterface_beacon_ACCEPTING_AND_SENDING) {
        Log_debug(context->logger, "sendBeacon(%s) -> beaconing disabled", context->ifName->bytes);
        // beaconing disabled
        return;
    }

    Log_debug(context->logger, "sendBeacon(%s)", context->ifName->bytes);

    struct {
        uint32_t ffffffff;
        uint16_t ffff;
        uint16_t firstTwo;
        struct Headers_Beacon beacon;
    } content = {
        .ffffffff = 0xffffffff,
        .ffff = 0xffff
    };

    #ifdef Version_12_COMPAT
        content.firstTwo = Endian_hostToBigEndian16(getIdAndPadding(Headers_Beacon_SIZE, context));
    #endif

    InterfaceController_populateBeacon(context->ic, &content.beacon);

    struct Message m = {
        .bytes = (uint8_t*) &content,
        .padding = 0,
        .length= sizeof(content)
    };

    int ret;
    if ((ret = sendMessageInternal(&m, context)) != 0) {
        Log_info(context->logger, "Got error [%d] sending beacon [%s]", ret, strerror(errno));
    }
}

#ifdef Version_12_COMPAT
static int isVersion12(struct ETHInterface* ctx,
                       struct sockaddr_ll* srcAddr,
                       struct Message* msg)
{
    struct EightBytes addr = { .bytes = { 0 } };
    Bits_memcpyConst(addr.bytes, srcAddr->sll_addr, 6);
    int idx = Map_isV12ByMacAddr_indexForKey(&addr, &ctx->v12Map);
    if (idx < 0) {
        uint32_t zero = 0;
        idx = Map_isV12ByMacAddr_put(&addr, &zero, &ctx->v12Map);
    }
    if (srcAddr->sll_pkttype == PACKET_BROADCAST) {
        // unused 2 bytes for alignment
        uint16_t verAndId = Message_pop16(msg, NULL);
        uint32_t version = Message_pop32(msg, NULL);
        ctx->v12Map.values[idx] = (version < 13);
        Message_push32(msg, version, NULL);
        Message_push16(msg, verAndId, NULL);
    }
    return ctx->v12Map.values[idx];
}
static void handleEventV12(struct ETHInterface* ctx, struct Message* msg, struct sockaddr_ll* addr)
{
    Message_pop16(msg, NULL);
    Message_push16(msg, 0, NULL);
    Message_push(msg, addr->sll_addr, 6, NULL);

    if (addr->sll_pkttype == PACKET_BROADCAST) {
        handleBeacon(msg, ctx);
        return;
    }

    Interface_receiveMessage(&ctx->generic, msg);
}
#endif

static void handleEvent2(struct ETHInterface* context, struct Allocator* messageAlloc)
{
    struct Message* msg = Message_new(MAX_PACKET_SIZE, PADDING, messageAlloc);

    struct sockaddr_ll addr;
    uint32_t addrLen = sizeof(struct sockaddr_ll);

    // Knock it out of alignment by 2 bytes so that it will be
    // aligned when the idAndPadding is shifted off.
    Message_shift(msg, 2, NULL);

    int rc = recvfrom(context->socket,
                      msg->bytes,
                      msg->length,
                      0,
                      (struct sockaddr*) &addr,
                      &addrLen);

    if (rc < 0) {
        Log_debug(context->logger, "Failed to receive eth frame");
        return;
    }

    Assert_true(msg->length >= rc);
    msg->length = rc;

    //Assert_true(addrLen == SOCKADDR_LL_LEN);

    #ifdef Version_12_COMPAT
    if (isVersion12(context, &addr, msg)) {
        handleEventV12(context, msg, &addr);
        return;
    }
    #endif

    if (addr.sll_pkttype == PACKET_BROADCAST) {
        Message_pop16(msg, NULL);
        Message_push16(msg, 0, NULL);
        Message_push(msg, addr.sll_addr, 6, NULL);
        handleBeacon(msg, context);
        return;
    }

    struct ETHInterface_Header header;
    Message_pop(msg, &header, ETHInterface_Header_SIZE, NULL);
    uint16_t reportedLength = Endian_bigEndianToHost16(header.length_be);
    reportedLength -= ETHInterface_Header_SIZE;
    if (msg->length != reportedLength) {
        if (msg->length < reportedLength) {
            Log_debug(context->logger, "DROP size field is larger than frame");
            return;
        }
        msg->length = reportedLength;
    }
    if (header.fc00_be != Endian_hostToBigEndian16(0xfc00)) {
        Log_debug(context->logger, "DROP bad magic");
        return;
    }
    Message_push16(msg, 0, NULL);
    Message_push(msg, addr.sll_addr, 6, NULL);

    Assert_true(!((uintptr_t)msg->bytes % 4) && "Alignment fault");

    Interface_receiveMessage(&context->generic, msg);
}

static void handleEvent(void* vcontext)
{
    struct ETHInterface* context = Identity_check((struct ETHInterface*) vcontext);
    struct Allocator* messageAlloc = Allocator_child(context->generic.allocator);
    handleEvent2(context, messageAlloc);
    Allocator_free(messageAlloc);
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
        .ic = ic
    }));

    #ifdef Version_12_COMPAT
        context->id = getpid();
        context->v12Map.allocator = allocator;
    #endif

    Identity_set(context);

    struct ifreq ifr = { .ifr_ifindex = 0 };

    context->socket = socket(AF_PACKET, SOCK_DGRAM, Ethernet_TYPE_CJDNS);
    if (context->socket == -1) {
        Except_throw(exHandler, "call to socket() failed. [%s]", strerror(errno));
    }
    CString_strncpy(ifr.ifr_name, bindDevice, IFNAMSIZ - 1);
    context->ifName = String_new(bindDevice, allocator);

    if (ioctl(context->socket, SIOCGIFINDEX, &ifr) == -1) {
        Except_throw(exHandler, "failed to find interface index [%s]", strerror(errno));
    }
    context->ifindex = ifr.ifr_ifindex;

    if (ioctl(context->socket, SIOCGIFHWADDR, &ifr) == -1) {
       Except_throw(exHandler, "failed to find mac address of interface [%s]",
                    strerror(errno));
    }

    uint8_t srcMac[6];
    Bits_memcpyConst(srcMac, ifr.ifr_hwaddr.sa_data, 6);

    // TODO(cjd): is the node's mac addr private information?
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
        Except_throw(exHandler, "call to bind() failed [%s]", strerror(errno));
    }

    Socket_makeNonBlocking(context->socket);

    Event_socketRead(handleEvent, context, context->socket, base, allocator, exHandler);

    // size of key is 8, 6 for mac + 2 for id.
    context->multiIface = MultiInterface_new(8, &context->generic, ic, logger);

    Timeout_setInterval(sendBeacon, context, BEACON_INTERVAL, base, allocator);

    return context;
}
