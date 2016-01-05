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
#include "interface/ETHInterface.h"
#include "exception/Except.h"
#include "memory/Allocator.h"
#include "net/InterfaceController.h"
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

#ifndef android
    #include <ifaddrs.h>
#endif

#define MAX_PACKET_SIZE 1496
#define MIN_PACKET_SIZE 46

#define PADDING 512

// 2 last 0x00 of .sll_addr are removed from original size (20)
#define SOCKADDR_LL_LEN 18

struct ETHInterface_pvt
{
    struct ETHInterface pub;

    Socket socket;

    /** The unix interface index which is used to identify the eth device. */
    int ifindex;

    struct Log* logger;

    struct sockaddr_ll addrBase;

    String* ifName;

    Identity
};

static void sendMessageInternal(struct Message* message,
                                struct sockaddr_ll* addr,
                                struct ETHInterface_pvt* context)
{
    /* Cut down on the noise
    uint8_t buff[sizeof(*addr) * 2 + 1] = {0};
    Hex_encode(buff, sizeof(buff), (uint8_t*)addr, sizeof(*addr));
    Log_debug(context->logger, "Sending ethernet frame to [%s]", buff);
    */

    if (sendto(context->socket,
               message->bytes,
               message->length,
               0,
               (struct sockaddr*) addr,
               sizeof(struct sockaddr_ll)) < 0)
    {
        switch (errno) {
            default:;
                Log_info(context->logger, "[%s] Got error sending to socket [%s]",
                         context->ifName->bytes, strerror(errno));

            case EMSGSIZE:
            case ENOBUFS:
            case EAGAIN:;
                // todo: care
        }
    }
    return;
}

static Iface_DEFUN sendMessage(struct Message* msg, struct Iface* iface)
{
    struct ETHInterface_pvt* ctx =
        Identity_containerOf(iface, struct ETHInterface_pvt, pub.generic.iface);

    struct Sockaddr* sa = (struct Sockaddr*) msg->bytes;
    Assert_true(msg->length >= Sockaddr_OVERHEAD);
    Assert_true(sa->addrLen <= ETHInterface_Sockaddr_SIZE);

    struct ETHInterface_Sockaddr sockaddr = { .generic = { .addrLen = 0 } };
    Message_pop(msg, &sockaddr, sa->addrLen, NULL);

    struct sockaddr_ll addr;
    Bits_memcpy(&addr, &ctx->addrBase, sizeof(struct sockaddr_ll));

    if (sockaddr.generic.flags & Sockaddr_flags_BCAST) {
        Bits_memset(addr.sll_addr, 0xff, 6);
    } else {
        Bits_memcpy(addr.sll_addr, sockaddr.mac, 6);
    }

    struct ETHInterface_Header hdr = {
        .version = ETHInterface_CURRENT_VERSION,
        .zero = 0,
        .length_be = Endian_hostToBigEndian16(msg->length + ETHInterface_Header_SIZE),
        .fc00_be = Endian_hostToBigEndian16(0xfc00)
    };
    Message_push(msg, &hdr, ETHInterface_Header_SIZE, NULL);
    sendMessageInternal(msg, &addr, ctx);
    return NULL;
}

static void handleEvent2(struct ETHInterface_pvt* context, struct Allocator* messageAlloc)
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

    if (rc < ETHInterface_Header_SIZE) {
        Log_debug(context->logger, "Failed to receive eth frame");
        return;
    }

    Assert_true(msg->length >= rc);
    msg->length = rc;

    //Assert_true(addrLen == SOCKADDR_LL_LEN);

    struct ETHInterface_Header hdr;
    Message_pop(msg, &hdr, ETHInterface_Header_SIZE, NULL);

    // here we could put a switch statement to handle different versions differently.
    if (hdr.version != ETHInterface_CURRENT_VERSION) {
        Log_debug(context->logger, "DROP unknown version");
        return;
    }

    uint16_t reportedLength = Endian_bigEndianToHost16(hdr.length_be);
    reportedLength -= ETHInterface_Header_SIZE;
    if (msg->length != reportedLength) {
        if (msg->length < reportedLength) {
            Log_debug(context->logger, "DROP size field is larger than frame");
            return;
        }
        msg->length = reportedLength;
    }
    if (hdr.fc00_be != Endian_hostToBigEndian16(0xfc00)) {
        Log_debug(context->logger, "DROP bad magic");
        return;
    }

    struct ETHInterface_Sockaddr  sockaddr = { .zero = 0 };
    Bits_memcpy(sockaddr.mac, addr.sll_addr, 6);
    sockaddr.generic.addrLen = ETHInterface_Sockaddr_SIZE;
    if (addr.sll_pkttype == PACKET_BROADCAST) {
        sockaddr.generic.flags |= Sockaddr_flags_BCAST;
    }

    Message_push(msg, &sockaddr, ETHInterface_Sockaddr_SIZE, NULL);

    Assert_true(!((uintptr_t)msg->bytes % 4) && "Alignment fault");

    Iface_send(&context->pub.generic.iface, msg);
}

static void handleEvent(void* vcontext)
{
    struct ETHInterface_pvt* context = Identity_check((struct ETHInterface_pvt*) vcontext);
    struct Allocator* messageAlloc = Allocator_child(context->pub.generic.alloc);
    handleEvent2(context, messageAlloc);
    Allocator_free(messageAlloc);
}

List* ETHInterface_listDevices(struct Allocator* alloc, struct Except* eh)
{
    List* out = List_new(alloc);
#ifndef android
    struct ifaddrs* ifaddr = NULL;
    if (getifaddrs(&ifaddr) || ifaddr == NULL) {
        Except_throw(eh, "getifaddrs() -> errno:%d [%s]", errno, strerror(errno));
    }
    for (struct ifaddrs* ifa = ifaddr; ifa; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_PACKET) {
            List_addString(out, String_new(ifa->ifa_name, alloc), alloc);
        }
    }
    freeifaddrs(ifaddr);
#endif
    return out;
}

static int closeSocket(struct Allocator_OnFreeJob* j)
{
    struct ETHInterface_pvt* ctx = Identity_check((struct ETHInterface_pvt*) j->userData);
    close(ctx->socket);
    return 0;
}

struct ETHInterface* ETHInterface_new(struct EventBase* eventBase,
                                      const char* bindDevice,
                                      struct Allocator* alloc,
                                      struct Except* exHandler,
                                      struct Log* logger)
{
    struct ETHInterface_pvt* ctx = Allocator_calloc(alloc, sizeof(struct ETHInterface_pvt), 1);
    Identity_set(ctx);
    ctx->pub.generic.iface.send = sendMessage;
    ctx->pub.generic.alloc = alloc;
    ctx->logger = logger;

    struct ifreq ifr = { .ifr_ifindex = 0 };

    ctx->socket = socket(AF_PACKET, SOCK_DGRAM, Ethernet_TYPE_CJDNS);
    if (ctx->socket == -1) {
        Except_throw(exHandler, "call to socket() failed. [%s]", strerror(errno));
    }
    Allocator_onFree(alloc, closeSocket, ctx);

    CString_strncpy(ifr.ifr_name, bindDevice, IFNAMSIZ - 1);
    ctx->ifName = String_new(bindDevice, alloc);

    if (ioctl(ctx->socket, SIOCGIFINDEX, &ifr) == -1) {
        Except_throw(exHandler, "failed to find interface index [%s]", strerror(errno));
    }
    ctx->ifindex = ifr.ifr_ifindex;

    if (ioctl(ctx->socket, SIOCGIFFLAGS, &ifr) < 0) {
        Except_throw(exHandler, "ioctl(SIOCGIFFLAGS) [%s]", strerror(errno));
    }
    if (!((ifr.ifr_flags & IFF_UP) && (ifr.ifr_flags & IFF_RUNNING))) {
        Log_info(logger, "Bringing up interface [%s]", ifr.ifr_name);
        ifr.ifr_flags |= IFF_UP | IFF_RUNNING;
        if (ioctl(ctx->socket, SIOCSIFFLAGS, &ifr) < 0) {
            Except_throw(exHandler, "ioctl(SIOCSIFFLAGS) [%s]", strerror(errno));
        }
    }

    ctx->addrBase = (struct sockaddr_ll) {
        .sll_family = AF_PACKET,
        .sll_protocol = Ethernet_TYPE_CJDNS,
        .sll_ifindex = ctx->ifindex,
        .sll_hatype = ARPHRD_ETHER,
        .sll_pkttype = PACKET_OTHERHOST,
        .sll_halen = ETH_ALEN
    };

    if (bind(ctx->socket, (struct sockaddr*) &ctx->addrBase, sizeof(struct sockaddr_ll))) {
        Except_throw(exHandler, "call to bind() failed [%s]", strerror(errno));
    }

    Socket_makeNonBlocking(ctx->socket);

    Event_socketRead(handleEvent, ctx, ctx->socket, eventBase, alloc, exHandler);

    return &ctx->pub;
}
