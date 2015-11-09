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
#include "wire/Message.h"
#include "wire/Ethernet.h"
#include "util/Assert.h"
#include "util/platform/Socket.h"
#include "util/events/Event.h"
#include "util/Identity.h"
#include "util/version/Version.h"

#include <ifaddrs.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <net/bpf.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <net/if.h>
#include <net/if_dl.h>

#define MAX_PACKET_SIZE 1496
#define MIN_PACKET_SIZE 46

#define PADDING 512

// single ethernet_frame
struct ethernet_frame
{
    uint8_t dest[6];
    uint8_t src[6];
    uint16_t type;
} Gcc_PACKED;
#define ethernet_frame_SIZE 14
Assert_compileTime(ethernet_frame_SIZE == sizeof(struct ethernet_frame));


struct ETHInterface_pvt
{
    struct ETHInterface pub;

    Socket socket;

    struct Log* logger;

    uint8_t myMac[6];

    String* ifName;

    uint8_t* buffer;
    int bufLen;

    Identity
};

static Iface_DEFUN sendMessage(struct Message* msg, struct Iface* iface)
{
    struct ETHInterface_pvt* ctx =
        Identity_containerOf(iface, struct ETHInterface_pvt, pub.generic.iface);

    struct Sockaddr* sa = (struct Sockaddr*) msg->bytes;
    Assert_true(msg->length >= Sockaddr_OVERHEAD);
    Assert_true(sa->addrLen <= ETHInterface_Sockaddr_SIZE);

    struct ETHInterface_Sockaddr sockaddr = { .generic = { .addrLen = 0 } };
    Message_pop(msg, &sockaddr, sa->addrLen, NULL);

    struct ETHInterface_Header hdr = {
        .version = ETHInterface_CURRENT_VERSION,
        .zero = 0,
        .length_be = Endian_hostToBigEndian16(msg->length + ETHInterface_Header_SIZE),
        .fc00_be = Endian_hostToBigEndian16(0xfc00)
    };
    Message_push(msg, &hdr, ETHInterface_Header_SIZE, NULL);

    struct ethernet_frame ethFr = {
        .type = Ethernet_TYPE_CJDNS
    };
    if (sockaddr.generic.flags & Sockaddr_flags_BCAST) {
        Bits_memset(ethFr.dest, 0xff, 6);
    } else {
        Bits_memcpy(ethFr.dest, sockaddr.mac, 6);
    }
    Bits_memcpy(ethFr.src, ctx->myMac, 6);
    Message_push(msg, &ethFr, ethernet_frame_SIZE, NULL);
  /*
    struct bpf_hdr bpfPkt = {
        .bh_caplen = msg->length,
        .bh_datalen = msg->length,
        .bh_hdrlen = BPF_WORDALIGN(sizeof(struct bpf_hdr))
    };
    Message_push(msg, &bpfPkt, bpfPkt.bh_hdrlen, NULL);
*/
    if (msg->length != write(ctx->socket, msg->bytes, msg->length)) {
        Log_debug(ctx->logger, "Error writing to eth device [%s]", strerror(errno));
    }
Log_debug(ctx->logger, "message sent");
    return NULL;
}

static void handleEvent2(struct ETHInterface_pvt* context,
                         uint8_t src[6],
                         uint8_t dst[6],
                         int length,
                         uint8_t* data,
                         struct Allocator* alloc)
{
    if (length < ETHInterface_Header_SIZE) {
        Log_debug(context->logger, "runt");
        return;
    }

    uint32_t contentLength = BPF_WORDALIGN(length - ETHInterface_Header_SIZE);
    struct Message* msg = Message_new(contentLength, PADDING, alloc);

    struct ETHInterface_Header hdr;
    Bits_memcpy(&hdr, data, ETHInterface_Header_SIZE);

    Bits_memcpy(msg->bytes, &data[ETHInterface_Header_SIZE], contentLength);

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

    struct ETHInterface_Sockaddr sockaddr = { .zero = 0 };
    Bits_memcpy(sockaddr.mac, src, 6);
    sockaddr.generic.addrLen = ETHInterface_Sockaddr_SIZE;
    if (dst[0] == 0xff) {
        sockaddr.generic.flags |= Sockaddr_flags_BCAST;
    }

    Message_push(msg, &sockaddr, ETHInterface_Sockaddr_SIZE, NULL);

    Assert_true(!((uintptr_t)msg->bytes % 4) && "Alignment fault");

    Iface_send(&context->pub.generic.iface, msg);
}

static void handleEvent(void* vcontext)
{
    struct ETHInterface_pvt* context = Identity_check((struct ETHInterface_pvt*) vcontext);
    ssize_t bytes = read(context->socket, context->buffer, context->bufLen);
    if (bytes < 0) {
        Log_debug(context->logger, "read(bpf, bpf_buf, buf_len) -> [%s]", strerror(errno));
    }
    if (bytes < 1) { return; }
    if (bytes < (ssize_t)sizeof(struct bpf_hdr)) {
        Log_debug(context->logger, "runt [%lld]", (long long) bytes);
        return;
    }
    int offset = 0;
    while (offset < bytes) {
        struct bpf_hdr* bpfPkt = (struct bpf_hdr*) &context->buffer[offset];
        struct ethernet_frame* ethFr =
            (struct ethernet_frame*) &context->buffer[offset + bpfPkt->bh_hdrlen];
        int frameLength = bpfPkt->bh_datalen;
        uint8_t* frameContent =
            (uint8_t*) &context->buffer[offset + bpfPkt->bh_hdrlen + ethernet_frame_SIZE];
        int contentLength = frameLength - ethernet_frame_SIZE;

        Assert_true(offset + bpfPkt->bh_hdrlen + frameLength <= bytes);
        Assert_true(Ethernet_TYPE_CJDNS == ethFr->type);

        struct Allocator* messageAlloc = Allocator_child(context->pub.generic.alloc);
        handleEvent2(context, ethFr->src, ethFr->dest, contentLength, frameContent, messageAlloc);
        Allocator_free(messageAlloc);

        offset += BPF_WORDALIGN(bpfPkt->bh_hdrlen + bpfPkt->bh_caplen);
    }
}

List* ETHInterface_listDevices(struct Allocator* alloc, struct Except* eh)
{
    List* out = List_new(alloc);
    struct ifaddrs* ifaddr = NULL;
    if (getifaddrs(&ifaddr) || ifaddr == NULL) {
        Except_throw(eh, "getifaddrs() -> errno:%d [%s]", errno, strerror(errno));
    }
    for (struct ifaddrs* ifa = ifaddr; ifa; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr) {
        } else if (ifa->ifa_addr->sa_family != AF_LINK) {
        } else if (!(ifa->ifa_flags & IFF_UP)) {
        } else if (ifa->ifa_flags & IFF_LOOPBACK) {
        } else {
            List_addString(out, String_new(ifa->ifa_name, alloc), alloc);
        }
    }
    freeifaddrs(ifaddr);
    return out;
}

static int closeSocket(struct Allocator_OnFreeJob* j)
{
    struct ETHInterface_pvt* ctx = Identity_check((struct ETHInterface_pvt*) j->userData);
    close(ctx->socket);
    return 0;
}

static int openBPF(struct Except* eh)
{
    for (int retry = 0; retry < 100; retry++) {
        for (int i = 0; i < 256; i++) {
            char buf[11] = { 0 };
            snprintf(buf, 10, "/dev/bpf%i", i);
            int bpf = open(buf, O_RDWR);
            if (bpf != -1) { return bpf; }
        }
        // sleep for 0.1 seconds
        usleep(1000 * 100);
    }
    Except_throw(eh, "Could not find available /dev/bpf device");
}

static void macaddr(const char* ifname, uint8_t addrOut[6], struct Except* eh)
{
    struct ifaddrs* ifa;
    if (getifaddrs(&ifa)) {
        Except_throw(eh, "getifaddrs() -> [%s]", strerror(errno));
    } else {
        for (struct ifaddrs* ifap = ifa; ifap; ifap = ifap->ifa_next) {
            if (!strcmp(ifap->ifa_name, ifname) && ifap->ifa_addr->sa_family == AF_LINK) {
                Bits_memcpy(addrOut, LLADDR((struct sockaddr_dl*) ifap->ifa_addr), 6);
                freeifaddrs(ifa);
                return;
            }
        }
    }
    freeifaddrs(ifa);
    Except_throw(eh, "Could not find mac address for [%s]", ifname);
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

    ctx->socket = openBPF(exHandler);

    macaddr(bindDevice, ctx->myMac, exHandler);

    struct ifreq ifr = { .ifr_name = { 0 } };
    CString_strcpy(ifr.ifr_name, bindDevice);
    if (ioctl(ctx->socket, BIOCSETIF, &ifr) > 0) {
        Except_throw(exHandler, "ioctl(BIOCSETIF, [%s]) [%s]", bindDevice, strerror(errno));
    }

    // activate immediate mode (therefore, bufLen is initially set to "1")
    int bufLen = 1;
    if (ioctl(ctx->socket, BIOCIMMEDIATE, &bufLen) == -1) {
        Except_throw(exHandler, "ioctl(BIOCIMMEDIATE) [%s]", strerror(errno));
    }

    // request buffer length
    if (ioctl(ctx->socket, BIOCGBLEN, &bufLen) == -1) {
        Except_throw(exHandler, "ioctl(BIOCGBLEN) [%s]", strerror(errno));
    }
    Log_debug(logger, "ioctl(BIOCGBLEN) -> bufLen=%i", bufLen);
    ctx->buffer = Allocator_malloc(alloc, bufLen);
    ctx->bufLen = bufLen;

    // filter for cjdns ethertype (0xfc00)
    static struct bpf_insn cjdnsFilter[] = {
        BPF_STMT(BPF_LD+BPF_H+BPF_ABS, 12),
        BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, /* Ethernet_TYPE_CJDNS */ 0xfc00, 1, 0),
        // drop
        BPF_STMT(BPF_RET+BPF_K, 0),
        // How much of the packet to ask for...
        BPF_STMT(BPF_RET+BPF_K, ~0u)
    };
    struct bpf_program cjdnsFilterProgram = {
        .bf_len = (sizeof(cjdnsFilter) / sizeof(struct bpf_insn)),
        .bf_insns = cjdnsFilter,
    };
    if (ioctl(ctx->socket, BIOCSETF, &cjdnsFilterProgram) == -1) {
        Except_throw(exHandler, "ioctl(BIOCSETF) [%s]", strerror(errno));
    }

    Socket_makeNonBlocking(ctx->socket);

    Event_socketRead(handleEvent, ctx, ctx->socket, eventBase, alloc, exHandler);

    Allocator_onFree(alloc, closeSocket, ctx);

    return &ctx->pub;
}
