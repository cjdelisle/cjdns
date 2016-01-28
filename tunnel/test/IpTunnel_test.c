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
#include "benc/String.h"
#include "benc/Dict.h"
#include "benc/serialization/standard/BencMessageWriter.h"
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "interface/tuntap/TUNMessageType.h"
#include "util/log/Log.h"
#include "util/log/FileWriterLog.h"
#include "util/events/EventBase.h"
#include "crypto/random/Random.h"
#include "crypto/Key.h"
#include "tunnel/IpTunnel.h"
#include "util/Bits.h"
#include "util/Checksum.h"
#include "util/CString.h"
#include "util/Escape.h"
#include "wire/DataHeader.h"
#include "wire/Message.h"
#include "wire/Headers.h"
#include "wire/Ethernet.h"

#define PUBKEY "f3yqyp5qpmpfgvjyvtklff40510gxuuuh52vpyzvpbhh5glyfr60.k"
#define IPV6 "fca9:f505:c650:8723:72a8:a524:530a:25c3"

struct Context
{
    struct Allocator* alloc;
    struct Log* log;
    struct Random* rand;
    struct EventBase* base;
    uint8_t pubKey[32];
    uint8_t ipv6[16];

    // Per-request
    uint8_t sendingAddress[16];
    String* expectedResponse;
    int called;

    Identity
};

struct IfaceContext
{
    struct Iface iface;
    struct Context* ctx;
};

static Iface_DEFUN responseWithIpCallback(struct Message* message, struct Iface* iface)
{
    struct Context* ctx = Identity_check(((struct IfaceContext*)iface)->ctx);
    struct RouteHeader* rh = (struct RouteHeader*) message->bytes;
    Assert_true(!Bits_memcmp(ctx->ipv6, rh->ip6, 16));
    Assert_true(!Bits_memcmp(ctx->pubKey, rh->publicKey, 32));

    Message_shift(message, -(RouteHeader_SIZE + DataHeader_SIZE), NULL);
    struct Headers_IP6Header* ip = (struct Headers_IP6Header*) message->bytes;
    Assert_true(Headers_getIpVersion(ip) == 6);
    uint16_t length = Endian_bigEndianToHost16(ip->payloadLength_be);
    Assert_true(length + Headers_IP6Header_SIZE == message->length);
    Assert_true(ip->nextHeader == 17);
    Assert_true(Bits_isZero(ip->sourceAddr, 32));

    Message_shift(message, -Headers_IP6Header_SIZE, NULL);
    struct Headers_UDPHeader* uh = (struct Headers_UDPHeader*) message->bytes;
    Assert_true(!Checksum_udpIp6(ip->sourceAddr, message->bytes, length));

    Assert_true(uh->srcPort_be == 0);
    Assert_true(uh->destPort_be == 0);
    Assert_true(Endian_bigEndianToHost16(uh->length_be) + Headers_UDPHeader_SIZE == length);

    Message_shift(message, -Headers_UDPHeader_SIZE, NULL);

    struct Allocator* alloc = Allocator_child(ctx->alloc);
    char* messageContent = Escape_getEscaped(message->bytes, message->length, alloc);
    char* expectedContent =
        Escape_getEscaped(ctx->expectedResponse->bytes, ctx->expectedResponse->len, alloc);
    Log_debug(ctx->log, "Response: [%s]", messageContent);
    Log_debug(ctx->log, "Expected: [%s]", expectedContent);
    Allocator_free(alloc);

    // We can't check that the message is an exact match because the padding depends on the
    // alignment of the output but we can make sure the right content is there...
    // Message should start with "d0000" (with some number of zeros)
    Assert_true((int)ctx->expectedResponse->len == message->length);
    Assert_true(!Bits_memcmp(message->bytes, ctx->expectedResponse->bytes, message->length));
    ctx->called |= 2;

    return NULL;
}

static Iface_DEFUN messageToTun(struct Message* msg, struct Iface* iface)
{
    struct Context* ctx = Identity_check(((struct IfaceContext*)iface)->ctx);
    uint16_t type = TUNMessageType_pop(msg, NULL);
    if (type == Ethernet_TYPE_IP6) {
        struct Headers_IP6Header* ip = (struct Headers_IP6Header*) msg->bytes;
        Assert_true(Headers_getIpVersion(ip) == 6);
        Assert_true(!Bits_memcmp(ip->sourceAddr, ctx->sendingAddress, 16));
        Message_shift(msg, -Headers_IP6Header_SIZE, NULL);
        ctx->called |= 4;
    } else if (type == Ethernet_TYPE_IP4) {
        struct Headers_IP4Header* ip = (struct Headers_IP4Header*) msg->bytes;
        Assert_true(Headers_getIpVersion(ip) == 4);
        Assert_true(!Bits_memcmp(ip->sourceAddr, ctx->sendingAddress, 4));
        Message_shift(msg, -Headers_IP4Header_SIZE, NULL);
        ctx->called |= 1;
    } else {
        Assert_failure("unrecognized message type %u", (unsigned int)type);
    }
    Assert_true(msg->length == 12 && CString_strcmp(msg->bytes, "hello world") == 0);
    return 0;
}

static void pushRouteDataHeaders(struct Context* ctx, struct Message* message)
{
    Message_shift(message, RouteHeader_SIZE + DataHeader_SIZE, NULL);
    struct RouteHeader* rh = (struct RouteHeader*) message->bytes;
    struct DataHeader* dh = (struct DataHeader*) &rh[1];
    Bits_memset(rh, 0, RouteHeader_SIZE + DataHeader_SIZE);
    Bits_memcpy(rh->ip6, ctx->ipv6, 16);
    Bits_memcpy(rh->publicKey, ctx->pubKey, 32);
    DataHeader_setContentType(dh, ContentType_IPTUN);
}

static bool trySend4(struct Allocator* alloc,
                     uint32_t addr,
                     struct Iface* sendTo,
                     struct Context* ctx)
{
    struct Message* msg4 = Message_new(0, 512, alloc);
    Message_push(msg4, "hello world", 12, NULL);
    Message_push(msg4, NULL, Headers_IP4Header_SIZE, NULL);
    struct Headers_IP4Header* iph = (struct Headers_IP4Header*) msg4->bytes;
    Headers_setIpVersion(iph);
    uint32_t addr_be = Endian_hostToBigEndian32(addr);
    Bits_memcpy(iph->sourceAddr, &addr_be, 4);
    Bits_memcpy(ctx->sendingAddress, &addr_be, 4);
    Bits_memcpy(iph->destAddr, ((uint8_t[]){ 11, 0, 0, 1 }), 4);
    pushRouteDataHeaders(ctx, msg4);
    Iface_send(sendTo, msg4);
    if (ctx->called == 1) {
        ctx->called = 0;
        return true;
    }
    Assert_true(ctx->called == 0);
    return false;
}

static bool trySend6(struct Allocator* alloc,
                     uint64_t addrHigh,
                     uint64_t addrLow,
                     struct Iface* sendTo,
                     struct Context* ctx)
{
    struct Message* msg6 = Message_new(0, 512, alloc);
    Message_push(msg6, "hello world", 12, NULL);
    Message_push(msg6, NULL, Headers_IP6Header_SIZE, NULL);
    struct Headers_IP6Header* iph = (struct Headers_IP6Header*) msg6->bytes;
    Headers_setIpVersion(iph);
    uint64_t addrHigh_be = Endian_hostToBigEndian64(addrHigh);
    uint64_t addrLow_be = Endian_hostToBigEndian64(addrLow);
    Bits_memcpy(iph->sourceAddr, &addrHigh_be, 8);
    Bits_memcpy(&iph->sourceAddr[8], &addrLow_be, 8);
    Bits_memcpy(ctx->sendingAddress, iph->sourceAddr, 16);
    uint8_t destAddr[16] = { 20, 01 };
    destAddr[15] = 1;
    Bits_memcpy(iph->destinationAddr, destAddr, 16);
    pushRouteDataHeaders(ctx, msg6);
    Iface_send(sendTo, msg6);
    if (ctx->called == 4) {
        ctx->called = 0;
        return true;
    }
    Assert_true(ctx->called == 0);
    return false;
}

static String* getExpectedResponse(struct Sockaddr* sa4, int prefix4, int alloc4,
                                   struct Sockaddr* sa6, int prefix6, int alloc6,
                                   struct Allocator* allocator)
{
    Assert_true(alloc6 >= prefix6);
    Assert_true(alloc4 >= prefix4);
    struct Allocator* alloc = Allocator_child(allocator);
    Dict* addresses = Dict_new(alloc);
    if (sa4) {
        uint8_t* addr = NULL;
        Assert_true(Sockaddr_getAddress(sa4, &addr) == 4);
        String* addrStr = String_newBinary(addr, 4, alloc);
        Dict_putString(addresses, String_new("ip4", alloc), addrStr, alloc);
        Dict_putInt(addresses, String_new("ip4Prefix", alloc), prefix4, alloc);
        Dict_putInt(addresses, String_new("ip4Alloc", alloc), alloc4, alloc);
    }
    if (sa6) {
        uint8_t* addr = NULL;
        Assert_true(Sockaddr_getAddress(sa6, &addr) == 16);
        String* addrStr = String_newBinary(addr, 16, alloc);
        Dict_putString(addresses, String_new("ip6", alloc), addrStr, alloc);
        Dict_putInt(addresses, String_new("ip6Prefix", alloc), prefix6, alloc);
        Dict_putInt(addresses, String_new("ip6Alloc", alloc), alloc6, alloc);
    }
    Dict* output = Dict_new(alloc);
    Dict_putDict(output, String_new("addresses", alloc), addresses, alloc);
    Dict_putString(output, String_new("txid", alloc), String_new("abcd", alloc), alloc);
    struct Message* msg = Message_new(0, 512, alloc);
    BencMessageWriter_write(output, msg, NULL);

    String* outStr = String_newBinary(msg->bytes, msg->length, allocator);
    Allocator_free(alloc);
    return outStr;
}

static void testAddr(struct Context* ctx,
                     char* addr4, int prefix4, int alloc4,
                     char* addr6, int prefix6, int alloc6)
{
    struct Allocator* alloc = Allocator_child(ctx->alloc);
    struct IpTunnel* ipTun = IpTunnel_new(ctx->log, ctx->base, alloc, ctx->rand, NULL);

    struct Sockaddr* sa4 = NULL;
    struct Sockaddr_storage ip6ToGive;
    struct Sockaddr_storage ip4ToGive;
    if (addr4) {
        Assert_true(!Sockaddr_parse(addr4, &ip4ToGive));
        sa4 = &ip4ToGive.addr;
        Assert_true(Sockaddr_getFamily(sa4) == Sockaddr_AF_INET);
    }
    struct Sockaddr* sa6 = NULL;
    if (addr6) {
        Assert_true(!Sockaddr_parse(addr6, &ip6ToGive));
        sa6 = &ip6ToGive.addr;
        Assert_true(Sockaddr_getFamily(sa6) == Sockaddr_AF_INET6);
    }

    IpTunnel_allowConnection(ctx->pubKey,
                             sa6, prefix6, alloc6,
                             sa4, prefix4, alloc4,
                             ipTun);

    struct Message* msg = Message_new(64, 512, alloc);
    const char* requestForAddresses =
        "d"
          "1:q" "21:IpTunnel_getAddresses"
          "4:txid" "4:abcd"
        "e";
    CString_strcpy(msg->bytes, requestForAddresses);
    msg->length = CString_strlen(requestForAddresses);

    Message_push(msg, NULL, Headers_UDPHeader_SIZE, NULL);
    struct Headers_UDPHeader* uh = (struct Headers_UDPHeader*) msg->bytes;
    uh->length_be = Endian_hostToBigEndian16(msg->length - Headers_UDPHeader_SIZE);

    uint16_t* checksum = &((struct Headers_UDPHeader*) msg->bytes)->checksum_be;
    *checksum = 0;
    uint32_t length = msg->length;

    // Because of old reasons, we need to have at least an empty IPv6 header
    Message_push(msg, NULL, Headers_IP6Header_SIZE, NULL);
    struct Headers_IP6Header* ip = (struct Headers_IP6Header*) msg->bytes;
    Headers_setIpVersion(ip);
    ip->payloadLength_be = Endian_hostToBigEndian16(msg->length - Headers_IP6Header_SIZE);
    ip->nextHeader = 17;

    *checksum = Checksum_udpIp6(ip->sourceAddr, (uint8_t*) uh, length);

    pushRouteDataHeaders(ctx, msg);

    struct IfaceContext* nodeIf = Allocator_calloc(alloc, sizeof(struct IfaceContext), 1);
    nodeIf->ctx = ctx;
    nodeIf->iface.send = responseWithIpCallback;
    struct IfaceContext* tunIf = Allocator_calloc(alloc, sizeof(struct IfaceContext), 1);
    tunIf->ctx = ctx;
    tunIf->iface.send = messageToTun;
    Iface_plumb(&nodeIf->iface, &ipTun->nodeInterface);
    Iface_plumb(&tunIf->iface, &ipTun->tunInterface);
    ctx->expectedResponse =
        getExpectedResponse(sa4, prefix4, alloc4, sa6, prefix6, alloc6, alloc);
    Iface_send(&nodeIf->iface, msg);
    Assert_true(ctx->called == 2);
    ctx->called = 0;

    if (sa4) {
        uint8_t* addrBytes = NULL;
        Assert_true(Sockaddr_getAddress(sa4, &addrBytes) == 4);
        uint32_t addr;
        Bits_memcpy(&addr, addrBytes, 4);
        addr = Endian_bigEndianToHost32(addr);
        // Send from the address specified
        Assert_true(trySend4(alloc, addr, &nodeIf->iface, ctx));

        if (alloc4 < 32) {
            // Send from another (random) address in the prefix
            uint32_t flip = Random_uint32(ctx->rand) >> alloc4;
            if (prefix4 != 32) {
                Assert_true(trySend4(alloc, addr ^ flip, &nodeIf->iface, ctx));
            } else {
                // If netSize is not specified, we do not allow multi-address
                Assert_true(!trySend4(alloc, addr ^ flip, &nodeIf->iface, ctx));
            }
        } else {
            Assert_true(!trySend4(alloc, addr ^ 1, &nodeIf->iface, ctx));
        }
    } else {
        uint32_t addr = Random_uint32(ctx->rand);
        Assert_true(!trySend4(alloc, addr, &nodeIf->iface, ctx));
    }

    if (sa6) {
        uint8_t* addrBytes = NULL;
        Assert_true(Sockaddr_getAddress(sa6, &addrBytes) == 16);
        uint64_t addrHigh;
        uint64_t addrLow;
        Bits_memcpy(&addrHigh, addrBytes, 8);
        Bits_memcpy(&addrLow, &addrBytes[8], 8);
        addrHigh = Endian_bigEndianToHost64(addrHigh);
        addrLow = Endian_bigEndianToHost64(addrLow);

        Assert_true(trySend6(alloc, addrHigh, addrLow, &nodeIf->iface, ctx));
        if (alloc6 < 128) {
            // Send from another (random) address in the prefix
            uint64_t flipHigh = Random_uint64(ctx->rand);
            uint64_t flipLow = Random_uint64(ctx->rand);
            if (alloc6 > 64) {
                flipHigh = flipHigh >> (alloc6 - 64);
            } else {
                flipHigh = 0;
                flipLow = flipLow >> alloc6;
            }

            if (prefix6 != 128) {
                Assert_true(trySend6(alloc,
                                     addrHigh ^ flipHigh,
                                     addrLow ^ flipLow,
                                     &nodeIf->iface,
                                     ctx) == true);
            } else {
                // If netSize is not specified, we do not allow multi-address
                Assert_true(trySend6(alloc,
                                     addrHigh ^ flipHigh,
                                     addrLow ^ flipLow,
                                     &nodeIf->iface,
                                     ctx) == false);
            }
        } else {
            Assert_true(!trySend6(alloc, addrHigh, addrLow ^ 1, &nodeIf->iface, ctx));
        }
    } else {
        uint64_t addr = Random_uint64(ctx->rand);
        Assert_true(!trySend6(alloc, 0, addr, &nodeIf->iface, ctx));
    }

    Allocator_free(alloc);
}

int main()
{
    struct Allocator* alloc = MallocAllocator_new(1<<20);
    struct EventBase* eb = EventBase_new(alloc);
    struct Log* logger = FileWriterLog_new(stdout, alloc);
    struct Random* rand = Random_new(alloc, logger, NULL);
    struct Context* ctx = Allocator_calloc(alloc, sizeof(struct Context), 1);
    Identity_set(ctx);
    ctx->alloc = alloc;
    ctx->log = logger;
    ctx->rand = rand;
    ctx->base = eb;
    Assert_true(!Key_parse(String_CONST(PUBKEY), ctx->pubKey, ctx->ipv6));

    testAddr(ctx, "192.168.1.1", 0, 32, NULL, 0, 0);
    testAddr(ctx, "192.168.1.1", 16, 24, NULL, 0, 0);
    testAddr(ctx, "192.168.1.1", 24, 32, NULL, 0, 0);

    testAddr(ctx, "192.168.1.1", 16, 24, "fd00::1", 0, 64);
    testAddr(ctx, "192.168.1.1", 16, 24, "fd00::1", 8, 64);
    testAddr(ctx, "192.168.1.1", 16, 24, "fd00::1", 64, 128);

    //Allocator_free(alloc); //TODO(cjd): This is caused by an allocator bug.
    /* To repeat the bug, create a test like this:
    struct Allocator* allocx = Allocator_child(alloc);
    Timeout_setInterval(NULL, NULL, 10000, eb, allocx);
    Allocator_snapshot(alloc, true);
    Allocator_free(allocx);
    Allocator_snapshot(alloc, true);
    return 0;
    */
    return 0;
}
