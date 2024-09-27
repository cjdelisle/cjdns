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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "interface/ETHInterface.h"
#include "exception/Err.h"
#include "memory/Allocator.h"
#include "net/InterfaceController.h"
#include "util/platform/Sockaddr.h"
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
#include "util/CString.h"

#include <string.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include <ifaddrs.h>

#define MAX_PACKET_SIZE 1496
#define MIN_PACKET_SIZE 46

#define PADDING 512

// 2 last 0x00 of .sll_addr are removed from original size (20)
#define SOCKADDR_LL_LEN 18

struct ETHInterface_pvt
{
    struct ETHInterface pub;

    Iface_t iface;

    int socket;

    /** The unix interface index which is used to identify the eth device. */
    int ifindex;

    struct Log* logger;

    struct sockaddr_ll addrBase;

    String* ifName;

    Identity
};

static void sendMessageInternal(Message_t* message,
                                struct sockaddr_ll* addr,
                                struct ETHInterface_pvt* context)
{
    /* Cut down on the noise
    uint8_t buff[sizeof(*addr) * 2 + 1] = {0};
    Hex_encode(buff, sizeof(buff), (uint8_t*)addr, sizeof(*addr));
    Log_debug(context->logger, "Sending ethernet frame to [%s]", buff);
    */

    if (sendto(context->socket,
               Message_bytes(message),
               Message_getLength(message),
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

static Iface_DEFUN sendMessage(Message_t* msg, struct Iface* iface)
{
    struct ETHInterface_pvt* ctx = Identity_containerOf(iface, struct ETHInterface_pvt, iface);

    struct Sockaddr_storage ss = { .addr.addrLen = 0 };
    Err(Sockaddr_read(&ss, msg));

    struct sockaddr_ll addr;
    Bits_memcpy(&addr, &ctx->addrBase, sizeof(struct sockaddr_ll));

    if (ss.addr.flags & Sockaddr_flags_BCAST) {
        Bits_memset(addr.sll_addr, 0xff, 6);
    } else {
        if (Sockaddr_getMac(addr.sll_addr, &ss.addr)) {
            Err_raise(Message_getAlloc(msg), "Sockaddr on message not ETH type");
        }
    }

    struct ETHInterface_Header hdr = {
        .version = ETHInterface_CURRENT_VERSION,
        .zero = 0,
        .length_be = Endian_hostToBigEndian16(Message_getLength(msg) + ETHInterface_Header_SIZE),
        .fc00_be = Endian_hostToBigEndian16(0xfc00)
    };
    Err(Message_epush(msg, &hdr, ETHInterface_Header_SIZE));
    sendMessageInternal(msg, &addr, ctx);
    return NULL;
}

static void handleEvent2(struct ETHInterface_pvt* context, struct Allocator* messageAlloc)
{
    Message_t* msg = Message_new(MAX_PACKET_SIZE, PADDING, messageAlloc);

    struct sockaddr_ll addr;
    uint32_t addrLen = sizeof(struct sockaddr_ll);

    // Knock it out of alignment by 2 bytes so that it will be
    // aligned when the idAndPadding is shifted off.
    Err_assert(Message_eshift(msg, 2));

    int rc = recvfrom(context->socket,
                      Message_bytes(msg),
                      Message_getLength(msg),
                      0,
                      (struct sockaddr*) &addr,
                      &addrLen);

    if (rc < ETHInterface_Header_SIZE) {
        Log_debug(context->logger, "Failed to receive eth frame");
        return;
    }

    Err_assert(Message_truncate(msg, rc));

    //Assert_true(addrLen == SOCKADDR_LL_LEN);

    struct ETHInterface_Header hdr;
    Err_assert(Message_epop(msg, &hdr, ETHInterface_Header_SIZE));

    // here we could put a switch statement to handle different versions differently.
    if (hdr.version != ETHInterface_CURRENT_VERSION) {
        Log_debug(context->logger, "DROP unknown version");
        return;
    }

    uint16_t reportedLength = Endian_bigEndianToHost16(hdr.length_be);
    reportedLength -= ETHInterface_Header_SIZE;
    if (Message_getLength(msg) != reportedLength) {
        if (Message_getLength(msg) < reportedLength) {
            Log_debug(context->logger, "DROP size field is larger than frame");
            return;
        }
        Err_assert(Message_truncate(msg, reportedLength));
    }
    if (hdr.fc00_be != Endian_hostToBigEndian16(0xfc00)) {
        Log_debug(context->logger, "DROP bad magic");
        return;
    }

    struct Sockaddr_storage ss;
    Sockaddr_initFromEth(&ss, addr.sll_addr);
    if (addr.sll_pkttype == PACKET_BROADCAST) {
        ss.addr.flags |= Sockaddr_flags_BCAST;
    }

    Err_assert(Sockaddr_write(&ss.addr, msg));

    Assert_true(!((uintptr_t)Message_bytes(msg) % 4) && "Alignment fault");

    Iface_send(context->pub.generic.iface, msg);
}

static void handleEvent(void* vcontext)
{
    struct ETHInterface_pvt* context = Identity_check((struct ETHInterface_pvt*) vcontext);
    struct Allocator* messageAlloc = Allocator_child(context->pub.generic.alloc);
    handleEvent2(context, messageAlloc);
    Allocator_free(messageAlloc);
}

Err_DEFUN ETHInterface_listDevices(List** outP, struct Allocator* alloc)
{
    List* out = List_new(alloc);
    struct ifaddrs* ifaddr = NULL;
    if (getifaddrs(&ifaddr) || ifaddr == NULL) {
        Err_raise(alloc, "getifaddrs() -> errno:%d [%s]", errno, strerror(errno));
    }
    for (struct ifaddrs* ifa = ifaddr; ifa; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_PACKET) {
            List_addString(out, String_new(ifa->ifa_name, alloc), alloc);
        }
    }
    freeifaddrs(ifaddr);
    *outP = out;
    return NULL;
}

static void closeSocket(struct Allocator_OnFreeJob* j)
{
    struct ETHInterface_pvt* ctx = Identity_check((struct ETHInterface_pvt*) j->userData);
    close(ctx->socket);
}

Err_DEFUN ETHInterface_new(
    struct ETHInterface** out,
    EventBase_t* eventBase,
    const char* bindDevice,
    struct Allocator* alloc,
    struct Log* logger)
{
    struct ETHInterface_pvt* ctx = Allocator_calloc(alloc, sizeof(struct ETHInterface_pvt), 1);
    Identity_set(ctx);
    ctx->iface.send = sendMessage;
    ctx->pub.generic.iface = &ctx->iface;
    ctx->pub.generic.alloc = alloc;
    ctx->logger = logger;

    struct ifreq ifr = { .ifr_ifindex = 0 };

    ctx->socket = socket(AF_PACKET, SOCK_DGRAM, Ethernet_TYPE_CJDNS);
    if (ctx->socket == -1) {
        Err_raise(alloc, "call to socket() failed. [%s]", strerror(errno));
    }
    Allocator_onFree(alloc, closeSocket, ctx);

    CString_safeStrncpy(ifr.ifr_name, bindDevice, IFNAMSIZ);
    ctx->ifName = String_new(bindDevice, alloc);

    if (ioctl(ctx->socket, SIOCGIFINDEX, &ifr) == -1) {
        Err_raise(alloc, "failed to find interface index [%s]", strerror(errno));
    }
    ctx->ifindex = ifr.ifr_ifindex;

    if (ioctl(ctx->socket, SIOCGIFFLAGS, &ifr) < 0) {
        Err_raise(alloc, "ioctl(SIOCGIFFLAGS) [%s]", strerror(errno));
    }
    if (!((ifr.ifr_flags & IFF_UP) && (ifr.ifr_flags & IFF_RUNNING))) {
        Log_info(logger, "Bringing up interface [%s]", ifr.ifr_name);
        ifr.ifr_flags |= IFF_UP | IFF_RUNNING;
        if (ioctl(ctx->socket, SIOCSIFFLAGS, &ifr) < 0) {
            Err_raise(alloc, "ioctl(SIOCSIFFLAGS) [%s]", strerror(errno));
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
        Err_raise(alloc, "call to bind() failed [%s]", strerror(errno));
    }

    Socket_makeNonBlocking(ctx->socket);

    Err(Event_socketRead(handleEvent, ctx, ctx->socket, alloc));

    *out = &ctx->pub;
    return NULL;
}
