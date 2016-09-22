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
#include "benc/List.h"
#include "benc/Int.h"
#include "benc/serialization/standard/BencMessageWriter.h"
#include "benc/serialization/standard/BencMessageReader.h"
#include "crypto/random/Random.h"
#include "exception/Jmp.h"
#include "interface/tuntap/TUNMessageType.h"
#include "memory/Allocator.h"
#include "tunnel/IpTunnel.h"
#include "tunnel/RouteGen.h"
#include "crypto/AddressCalc.h"
#include "util/platform/netdev/NetDev.h"
#include "util/Checksum.h"
#include "util/AddrTools.h"
#include "util/events/EventBase.h"
#include "util/Identity.h"
#include "util/events/Timeout.h"
#include "util/Defined.h"
#include "util/Escape.h"
#include "wire/Error.h"
#include "wire/Headers.h"
#include "wire/Ethernet.h"
#include "wire/DataHeader.h"

#include <stddef.h>

struct IpTunnel_pvt
{
    struct IpTunnel pub;

    struct Allocator* allocator;
    struct Log* logger;

    struct RouteGen* rg;

    uint32_t connectionCapacity;

    /** An always incrementing number which represents the connections. */
    uint32_t nextConnectionNumber;

    /** The name of the TUN interface so that ip addresses can be added. */
    String* ifName;

    /**
     * Every 10 seconds check for connections which the other end has
     * not provided ip addresses and send more requests.
     */
    struct Timeout* timeout;
    struct Random* rand;

    /** For verifying the integrity of the structure. */
    Identity
};

static struct IpTunnel_Connection* newConnection(bool isOutgoing, struct IpTunnel_pvt* context)
{
    if (context->pub.connectionList.count == context->connectionCapacity) {
        uint32_t newSize = (context->connectionCapacity + 4) * sizeof(struct IpTunnel_Connection);
        context->pub.connectionList.connections =
            Allocator_realloc(context->allocator, context->pub.connectionList.connections, newSize);
        context->connectionCapacity += 4;
    }
    struct IpTunnel_Connection* conn =
        &context->pub.connectionList.connections[context->pub.connectionList.count];

    // If it's an incoming connection, it must be lower on the list than any outgoing connections.
    if (!isOutgoing) {
        for (int i = (int)context->pub.connectionList.count - 1; i >= 0; i--) {
            if (!context->pub.connectionList.connections[i].isOutgoing
                && conn != &context->pub.connectionList.connections[i + 1])
            {
                Bits_memcpy(conn,
                                 &context->pub.connectionList.connections[i + 1],
                                 sizeof(struct IpTunnel_Connection));
                conn = &context->pub.connectionList.connections[i + 1];
            }
        }
    }

    context->pub.connectionList.count++;

    Bits_memset(conn, 0, sizeof(struct IpTunnel_Connection));
    conn->number = context->nextConnectionNumber++;
    conn->isOutgoing = isOutgoing;

    // if there are 2 billion calls, die.
    Assert_true(context->nextConnectionNumber < (UINT32_MAX >> 1));

    return conn;
}

static void deleteConnection(struct IpTunnel_Connection* conn, struct IpTunnel_pvt* context)
{
    // Delete connection and shift the list elements following the removed connection

    int i = (((char *)conn)-((char *)&context->pub.connectionList.connections[0]))
                                                  / sizeof(struct IpTunnel_Connection);

    // Sanity check
    Assert_true(i >= 0 && i < (signed int)context->pub.connectionList.count);

    for (; (unsigned int)i < context->pub.connectionList.count-1; ++i) {
        Bits_memcpy(&context->pub.connectionList.connections[i],
                    &context->pub.connectionList.connections[i + 1],
                    sizeof(struct IpTunnel_Connection));
    }

    int last = context->pub.connectionList.count-1;
    if (last > 0) {
        Bits_memset(&context->pub.connectionList.connections[last], 0,
                    sizeof(struct IpTunnel_Connection));
    }

    context->pub.connectionList.count--;
}

static struct IpTunnel_Connection* connectionByPubKey(uint8_t pubKey[32],
                                                      struct IpTunnel_pvt* context)
{
    for (int i = 0; i < (int)context->pub.connectionList.count; i++) {
        struct IpTunnel_Connection* conn = &context->pub.connectionList.connections[i];
        if (!Bits_memcmp(pubKey, conn->routeHeader.publicKey, 32)) {
            return conn;
        }
    }
    return NULL;
}

/**
 * Allow another node to tunnel IPv4 and/or ICANN IPv6 through this node.
 *
 * @param publicKeyOfAuthorizedNode the key for the node which will be allowed to connect.
 * @param ip6Addr the IPv6 address which the node will be issued or NULL.
 * @param ip6Prefix the IPv6 netmask/prefix length.
 * @param ip4Addr the IPv4 address which the node will be issued or NULL.
 * @param ip4Prefix the IPv4 netmask/prefix length.
 * @param tunnel the IpTunnel.
 * @return an connection number which is usable with IpTunnel_remove().
 */
int IpTunnel_allowConnection(uint8_t publicKeyOfAuthorizedNode[32],
                             struct Sockaddr* ip6Addr,
                             uint8_t ip6Prefix,
                             uint8_t ip6Alloc,
                             struct Sockaddr* ip4Addr,
                             uint8_t ip4Prefix,
                             uint8_t ip4Alloc,
                             struct IpTunnel* tunnel)
{
    struct IpTunnel_pvt* context = Identity_check((struct IpTunnel_pvt*)tunnel);

    Log_debug(context->logger, "IPv4 Prefix to allow: %d", ip4Prefix);

    uint8_t* ip6Address = NULL;
    uint8_t* ip4Address = NULL;
    if (ip6Addr) {
        Sockaddr_getAddress(ip6Addr, &ip6Address);
    }
    if (ip4Addr) {
        Sockaddr_getAddress(ip4Addr, &ip4Address);
    }

    struct IpTunnel_Connection* conn = newConnection(false, context);
    Bits_memcpy(conn->routeHeader.publicKey, publicKeyOfAuthorizedNode, 32);
    AddressCalc_addressForPublicKey(conn->routeHeader.ip6, publicKeyOfAuthorizedNode);
    if (ip4Address) {
        Bits_memcpy(conn->connectionIp4, ip4Address, 4);
        conn->connectionIp4Prefix = ip4Prefix;
        conn->connectionIp4Alloc = ip4Alloc;
        Assert_true(ip4Alloc);
    }

    if (ip6Address) {
        Bits_memcpy(conn->connectionIp6, ip6Address, 16);
        conn->connectionIp6Prefix = ip6Prefix;
        conn->connectionIp6Alloc = ip6Alloc;
        Assert_true(ip6Alloc);
    }

    return conn->number;
}

static Iface_DEFUN sendToNode(struct Message* message,
                              struct IpTunnel_Connection* connection,
                              struct IpTunnel_pvt* context)
{
    Message_push(message, NULL, DataHeader_SIZE, NULL);
    struct DataHeader* dh = (struct DataHeader*) message->bytes;
    DataHeader_setContentType(dh, ContentType_IPTUN);
    DataHeader_setVersion(dh, DataHeader_CURRENT_VERSION);
    Message_push(message, &connection->routeHeader, RouteHeader_SIZE, NULL);
    return Iface_next(&context->pub.nodeInterface, message);
}

static void sendControlMessage(Dict* dict,
                               struct IpTunnel_Connection* connection,
                               struct Allocator* requestAlloc,
                               struct IpTunnel_pvt* context)
{
    struct Message* msg = Message_new(0, 1024, requestAlloc);
    BencMessageWriter_write(dict, msg, NULL);

    int length = msg->length;

    // do UDP header.
    Message_shift(msg, Headers_UDPHeader_SIZE, NULL);
    struct Headers_UDPHeader* uh = (struct Headers_UDPHeader*) msg->bytes;
    uh->srcPort_be = 0;
    uh->destPort_be = 0;
    uh->length_be = Endian_hostToBigEndian16(length);
    uh->checksum_be = 0;

    uint16_t payloadLength = msg->length;

    Message_shift(msg, Headers_IP6Header_SIZE, NULL);
    struct Headers_IP6Header* header = (struct Headers_IP6Header*) msg->bytes;
    header->versionClassAndFlowLabel = 0;
    header->flowLabelLow_be = 0;
    header->nextHeader = 17;
    header->hopLimit = 0;
    header->payloadLength_be = Endian_hostToBigEndian16(payloadLength);
    Headers_setIpVersion(header);

    // zero the source and dest addresses.
    Bits_memset(header->sourceAddr, 0, 32);

    uh->checksum_be = Checksum_udpIp6(header->sourceAddr,
                                      (uint8_t*) uh,
                                      msg->length - Headers_IP6Header_SIZE);

    Iface_CALL(sendToNode, msg, connection, context);
}

static void requestAddresses(struct IpTunnel_Connection* conn, struct IpTunnel_pvt* context)
{
    if (Defined(Log_DEBUG)) {
        uint8_t addr[40];
        AddrTools_printIp(addr, conn->routeHeader.ip6);
        Log_debug(context->logger, "Requesting addresses from [%s] for connection [%d]",
                  addr, conn->number);
    }

    int number = conn->number;
    Dict d = Dict_CONST(
        String_CONST("q"), String_OBJ(String_CONST("IpTunnel_getAddresses")), Dict_CONST(
        String_CONST("txid"), String_OBJ((&(String){ .len = 4, .bytes = (char*)&number })),
        NULL
    ));
    struct Allocator* msgAlloc = Allocator_child(context->allocator);
    sendControlMessage(&d, conn, msgAlloc, context);
    Allocator_free(msgAlloc);
}

/**
 * Connect to another node and get IPv4 and/or IPv6 addresses from it.
 *
 * @param publicKeyOfNodeToConnectTo the key for the node to connect to.
 * @param tunnel the IpTunnel.
 * @return an connection number which is usable with IpTunnel_remove().
 */
int IpTunnel_connectTo(uint8_t publicKeyOfNodeToConnectTo[32], struct IpTunnel* tunnel)
{
    struct IpTunnel_pvt* context = Identity_check((struct IpTunnel_pvt*)tunnel);

    struct IpTunnel_Connection* conn = newConnection(true, context);
    Bits_memcpy(conn->routeHeader.publicKey, publicKeyOfNodeToConnectTo, 32);
    AddressCalc_addressForPublicKey(conn->routeHeader.ip6, publicKeyOfNodeToConnectTo);

    if (Defined(Log_DEBUG)) {
        uint8_t addr[40];
        AddrTools_printIp(addr, conn->routeHeader.ip6);
        Log_debug(context->logger, "Trying to connect to [%s]", addr);
    }

    requestAddresses(conn, context);

    return conn->number;
}

/**
 * Disconnect from a node or remove authorization to connect.
 *
 * @param connection the connection to remove.
 * @param tunnel the IpTunnel.
 */
int IpTunnel_removeConnection(int connectionNumber, struct IpTunnel* tunnel)
{
    struct IpTunnel_pvt* context = Identity_check((struct IpTunnel_pvt*)tunnel);

    for (int i = 0; i < (int)tunnel->connectionList.count; ++i)
    {
        if (tunnel->connectionList.connections[i].number==connectionNumber)
        {
            deleteConnection(&tunnel->connectionList.connections[i], context);
            return 0;
        }
    }

    return IpTunnel_removeConnection_NOT_FOUND;
}

static bool isControlMessageInvalid(struct Message* message, struct IpTunnel_pvt* context)
{
    struct Headers_IP6Header* header = (struct Headers_IP6Header*) message->bytes;
    uint16_t length = Endian_bigEndianToHost16(header->payloadLength_be);
    if (header->nextHeader != 17 || message->length < length + Headers_IP6Header_SIZE) {
        Log_warn(context->logger, "Invalid IPv6 packet (not UDP or length field too big)");
        return true;
    }

    Message_shift(message, -Headers_IP6Header_SIZE, NULL);
    struct Headers_UDPHeader* udp = (struct Headers_UDPHeader*) message->bytes;

    if (Checksum_udpIp6(header->sourceAddr, message->bytes, length)) {
        Log_warn(context->logger, "Checksum mismatch");
        return true;
    }

    length -= Headers_UDPHeader_SIZE;
    if (Endian_bigEndianToHost16(udp->length_be) != length
        || udp->srcPort_be != 0
        || udp->destPort_be != 0)
    {
        Log_warn(context->logger, "Invalid UDP packet (length mismatch or wrong ports)");
        return true;
    }

    Message_shift(message, -Headers_UDPHeader_SIZE, NULL);

    message->length = length;
    return false;
}

static Iface_DEFUN requestForAddresses(Dict* request,
                                       struct IpTunnel_Connection* conn,
                                       struct Allocator* requestAlloc,
                                       struct IpTunnel_pvt* context)
{
    if (Defined(Log_DEBUG)) {
        uint8_t addr[40];
        AddrTools_printIp(addr, conn->routeHeader.ip6);
        Log_debug(context->logger, "Got request for addresses from [%s]", addr);
    }

    if (conn->isOutgoing) {
        Log_warn(context->logger, "got request for addresses from outgoing connection");
        return 0;
    }
    Dict* addresses = Dict_new(requestAlloc);
    bool noAddresses = true;
    if (!Bits_isZero(conn->connectionIp6, 16)) {
        Dict_putString(addresses,
                       String_CONST("ip6"),
                       String_newBinary((char*)conn->connectionIp6, 16, requestAlloc),
                       requestAlloc);
        Dict_putInt(addresses,
                    String_CONST("ip6Prefix"), (int64_t)conn->connectionIp6Prefix,
                    requestAlloc);
        Dict_putInt(addresses,
                    String_CONST("ip6Alloc"), (int64_t)conn->connectionIp6Alloc,
                    requestAlloc);

        noAddresses = false;
    }
    if (!Bits_isZero(conn->connectionIp4, 4)) {
        Dict_putString(addresses,
                       String_CONST("ip4"),
                       String_newBinary((char*)conn->connectionIp4, 4, requestAlloc),
                       requestAlloc);
        Dict_putInt(addresses,
                    String_CONST("ip4Prefix"), (int64_t)conn->connectionIp4Prefix,
                    requestAlloc);
        Dict_putInt(addresses,
                    String_CONST("ip4Alloc"), (int64_t)conn->connectionIp4Alloc,
                    requestAlloc);

        noAddresses = false;
    }
    if (noAddresses) {
        Log_warn(context->logger, "no addresses to provide");
        return 0;
    }

    Dict* msg = Dict_new(requestAlloc);
    Dict_putDict(msg, String_CONST("addresses"), addresses, requestAlloc);

    String* txid = Dict_getString(request, String_CONST("txid"));
    if (txid) {
        Dict_putString(msg, String_CONST("txid"), txid, requestAlloc);
    }

    sendControlMessage(msg, conn, requestAlloc, context);
    return 0;
}

static void addAddress(char* printedAddr, uint8_t prefixLen,
                       uint8_t allocSize, struct IpTunnel_pvt* ctx)
{
    if (!ctx->ifName) {
        Log_error(ctx->logger, "Failed to set IP address because TUN interface is not setup");
        return;
    }
    struct Sockaddr_storage ss;
    if (Sockaddr_parse(printedAddr, &ss)) {
        Log_error(ctx->logger, "Invalid ip, setting ip address on TUN");
        return;
    }
    ss.addr.flags |= Sockaddr_flags_PREFIX;

    ss.addr.prefix = allocSize;
    struct Jmp j;
    Jmp_try(j) {
        NetDev_addAddress(ctx->ifName->bytes, &ss.addr, ctx->logger, &j.handler);
    } Jmp_catch {
        Log_error(ctx->logger, "Error setting ip address on TUN [%s]", j.message);
        return;
    }

    ss.addr.prefix = prefixLen;
    bool installRoute = false;
    if (Sockaddr_getFamily(&ss.addr) == Sockaddr_AF_INET) {
        installRoute = (prefixLen < 32);
    } else if (Sockaddr_getFamily(&ss.addr) == Sockaddr_AF_INET6) {
        installRoute = (prefixLen < 128);
    } else {
        Assert_failure("bad address family");
    }
    if (installRoute) {
        RouteGen_addPrefix(ctx->rg, &ss.addr);
    }
}

static Iface_DEFUN incomingAddresses(Dict* d,
                                     struct IpTunnel_Connection* conn,
                                     struct Allocator* alloc,
                                     struct IpTunnel_pvt* context)
{
    if (!conn->isOutgoing) {
        Log_warn(context->logger, "got offer of addresses from incoming connection");
        return 0;
    }

    String* txid = Dict_getString(d, String_CONST("txid"));
    if (!txid || txid->len != 4) {
        Log_info(context->logger, "missing or wrong length txid");
        return 0;
    }

    int number;
    Bits_memcpy(&number, txid->bytes, 4);
    if (number < 0 || number >= (int)context->nextConnectionNumber) {
        Log_info(context->logger, "txid out of range");
        return 0;
    }

    if (number != conn->number) {
        for (int i = 0; i < (int)context->pub.connectionList.count; i++) {
            if (context->pub.connectionList.connections[i].number == number) {
                if (Bits_memcmp(conn->routeHeader.publicKey,
                                context->pub.connectionList.connections[i].routeHeader.publicKey,
                                32))
                {
                    Log_info(context->logger, "txid doesn't match origin");
                    return 0;
                } else {
                    conn = &context->pub.connectionList.connections[i];
                }
            }
        }
    }

    Dict* addresses = Dict_getDict(d, String_CONST("addresses"));

    String* ip4 = Dict_getString(addresses, String_CONST("ip4"));
    int64_t* ip4Prefix = Dict_getInt(addresses, String_CONST("ip4Prefix"));
    int64_t* ip4Alloc = Dict_getInt(addresses, String_CONST("ip4Alloc"));

    if (ip4 && ip4->len == 4) {
        Bits_memcpy(conn->connectionIp4, ip4->bytes, 4);

        if (ip4Prefix && *ip4Prefix >= 0 && *ip4Prefix <= 32) {
            conn->connectionIp4Prefix = (uint8_t) *ip4Prefix;
        } else {
            conn->connectionIp4Prefix = 32;
        }

        if (ip4Alloc && *ip4Alloc >= 0 && *ip4Alloc <= 32) {
            conn->connectionIp4Alloc = (uint8_t) *ip4Alloc;
        } else {
            conn->connectionIp4Alloc = 32;
        }

        struct Sockaddr* sa = Sockaddr_clone(Sockaddr_LOOPBACK, alloc);
        uint8_t* addrBytes = NULL;
        Sockaddr_getAddress(sa, &addrBytes);
        Bits_memcpy(addrBytes, ip4->bytes, 4);
        char* printedAddr = Sockaddr_print(sa, alloc);

        Log_info(context->logger, "Got issued address [%s/%d:%d] for connection [%d]",
                 printedAddr, conn->connectionIp4Alloc, conn->connectionIp4Prefix, conn->number);

        addAddress(printedAddr, conn->connectionIp4Prefix, conn->connectionIp4Alloc, context);
    }

    String* ip6 = Dict_getString(addresses, String_CONST("ip6"));
    int64_t* ip6Prefix = Dict_getInt(addresses, String_CONST("ip6Prefix"));
    int64_t* ip6Alloc = Dict_getInt(addresses, String_CONST("ip6Alloc"));

    if (ip6 && ip6->len == 16) {
        Bits_memcpy(conn->connectionIp6, ip6->bytes, 16);

        if (ip6Prefix && *ip6Prefix >= 0 && *ip6Prefix <= 128) {
            conn->connectionIp6Prefix = (uint8_t) *ip6Prefix;
        } else {
            conn->connectionIp6Prefix = 128;
        }

        if (ip6Alloc && *ip6Alloc >= 0 && *ip6Alloc <= 128) {
            conn->connectionIp6Alloc = (uint8_t) *ip6Alloc;
        } else {
            conn->connectionIp6Alloc = 128;
        }

        struct Sockaddr* sa = Sockaddr_clone(Sockaddr_LOOPBACK6, alloc);
        uint8_t* addrBytes = NULL;
        Sockaddr_getAddress(sa, &addrBytes);
        Bits_memcpy(addrBytes, ip6->bytes, 16);
        char* printedAddr = Sockaddr_print(sa, alloc);

        Log_info(context->logger, "Got issued address block [%s/%d:%d] for connection [%d]",
                 printedAddr, conn->connectionIp6Alloc, conn->connectionIp6Prefix, conn->number);

        addAddress(printedAddr, conn->connectionIp6Prefix, conn->connectionIp6Alloc, context);
    }
    if (context->rg->hasUncommittedChanges) {
        if (!context->ifName) {
            Log_error(context->logger, "Failed to set routes because TUN interface is not setup");
            return 0;
        }
        struct Jmp j;
        Jmp_try(j) {
            RouteGen_commit(context->rg, context->ifName->bytes, alloc,  &j.handler);
        } Jmp_catch {
            Log_error(context->logger, "Error setting routes for TUN [%s]", j.message);
            return 0;
        }
    }
    return 0;
}

static Iface_DEFUN incomingControlMessage(struct Message* message,
                                          struct IpTunnel_Connection* conn,
                                          struct IpTunnel_pvt* context)
{
    if (Defined(Log_DEBUG)) {
        uint8_t addr[40];
        AddrTools_printIp(addr, conn->routeHeader.ip6);
        Log_debug(context->logger, "Got incoming message from [%s]", addr);
    }

    // This aligns the message on the content.
    if (isControlMessageInvalid(message, context)) {
        return 0;
    }

    Log_debug(context->logger, "Message content [%s]",
        Escape_getEscaped(message->bytes, message->length, message->alloc));

    struct Allocator* alloc = Allocator_child(message->alloc);

    Dict* d = NULL;
    char* err = BencMessageReader_readNoExcept(message, alloc, &d);
    if (err) {
        Log_info(context->logger, "Failed to parse message [%s]", err);
        return 0;
    }

    if (Dict_getDict(d, String_CONST("addresses"))) {
        return incomingAddresses(d, conn, alloc, context);
    }
    if (String_equals(String_CONST("IpTunnel_getAddresses"),
                      Dict_getString(d, String_CONST("q"))))
    {
        return requestForAddresses(d, conn, alloc, context);
    }
    Log_warn(context->logger, "Message which is unhandled");
    return 0;
}

#define GET64(buffer) \
    (__extension__ ({                                               \
        Assert_true(!((long)(buffer) % 4));                         \
        uint64_t x = (uint64_t) (((uint32_t*)(buffer))[0]);         \
        x |= (( (uint64_t) ((uint32_t*)(buffer))[1]) << 32);        \
        Endian_bigEndianToHost64(x);                                \
    }))

#define GET32(buffer) \
    (__extension__ ({                                               \
        Assert_true(!((long)(buffer) % 4));                         \
        uint32_t x = (((uint32_t*)(buffer))[0]);                    \
        Endian_bigEndianToHost32(x);                                \
    }))

static bool prefixMatches6(uint8_t* addressA, uint8_t* refAddr, uint8_t prefixLen)
{
    if (!prefixLen) {
        Assert_true(Bits_isZero(refAddr, 16));
        return false;
    }
    Assert_true(prefixLen && prefixLen <= 128);
    uint64_t a0 = GET64(addressA);
    uint64_t b0 = GET64(refAddr);
    if (prefixLen <= 64) {
        return !( (a0 ^ b0) >> (64 - prefixLen) );
    }
    uint64_t a1 = GET64(addressA + 8);
    uint64_t b1 = GET64(refAddr + 8);
    return !( (a0 ^ b0) | ((a1 ^ b1) >> (128 - prefixLen)) );
}

static bool prefixMatches4(uint8_t* addressA, uint8_t* refAddr, uint32_t prefixLen)
{
    if (!prefixLen) {
        Assert_true(Bits_isZero(refAddr, 4));
        return false;
    }
    Assert_true(prefixLen && prefixLen <= 32);
    uint32_t a = GET32(addressA);
    uint32_t b = GET32(refAddr);
    return !((a ^ b) >> (32 - prefixLen));
}

static bool isValidAddress4(uint8_t sourceAndDestIp4[8],
                            bool isFromTun,
                            struct IpTunnel_Connection* conn)
{
    uint8_t* compareAddr = (isFromTun)
        ? ((conn->isOutgoing) ? sourceAndDestIp4 : &sourceAndDestIp4[4])
        : ((conn->isOutgoing) ? &sourceAndDestIp4[4] : sourceAndDestIp4);
    return prefixMatches4(compareAddr, conn->connectionIp4, conn->connectionIp4Alloc);
}

static bool isValidAddress6(uint8_t sourceAndDestIp6[32],
                            bool isFromTun,
                            struct IpTunnel_Connection* conn)
{
    if (sourceAndDestIp6[0] == 0xfc || sourceAndDestIp6[16] == 0xfc) { return false; }
    uint8_t* compareAddr = (isFromTun)
        ? ((conn->isOutgoing) ? sourceAndDestIp6 : &sourceAndDestIp6[16])
        : ((conn->isOutgoing) ? &sourceAndDestIp6[16] : sourceAndDestIp6);
    return prefixMatches6(compareAddr, conn->connectionIp6, conn->connectionIp6Alloc);
}

static struct IpTunnel_Connection* findConnection(uint8_t sourceAndDestIp6[32],
                                                  uint8_t sourceAndDestIp4[8],
                                                  bool isFromTun,
                                                  struct IpTunnel_pvt* context)
{
    for (int i = 0; i < (int)context->pub.connectionList.count; i++) {
        struct IpTunnel_Connection* conn = &context->pub.connectionList.connections[i];
        if (sourceAndDestIp6 && isValidAddress6(sourceAndDestIp6, isFromTun, conn)) {
            return conn;
        }
        if (sourceAndDestIp4 && isValidAddress4(sourceAndDestIp4, isFromTun, conn)) {
            return conn;
        }
    }
    return NULL;
}

static Iface_DEFUN incomingFromTun(struct Message* message, struct Iface* tunIf)
{
    struct IpTunnel_pvt* context = Identity_check((struct IpTunnel_pvt*)tunIf);

    if (message->length < 20) {
        Log_debug(context->logger, "DROP runt");
    }

    struct IpTunnel_Connection* conn = NULL;
    if (!context->pub.connectionList.connections) {
        // No connections authorized, fall through to "unrecognized address"
    } else if (message->length > 40 && Headers_getIpVersion(message->bytes) == 6) {
        struct Headers_IP6Header* header = (struct Headers_IP6Header*) message->bytes;
        conn = findConnection(header->sourceAddr, NULL, true, context);
    } else if (message->length > 20 && Headers_getIpVersion(message->bytes) == 4) {
        struct Headers_IP4Header* header = (struct Headers_IP4Header*) message->bytes;
        conn = findConnection(NULL, header->sourceAddr, true, context);
    } else {
        Log_info(context->logger, "Message of unknown type from TUN");
        return 0;
    }

    if (!conn) {
        Log_info(context->logger, "Message with unrecognized address from TUN");
        return 0;
    }

    return sendToNode(message, conn, context);
}

static Iface_DEFUN ip6FromNode(struct Message* message,
                               struct IpTunnel_Connection* conn,
                               struct IpTunnel_pvt* context)
{
    struct Headers_IP6Header* header = (struct Headers_IP6Header*) message->bytes;
    if (Bits_isZero(header->sourceAddr, 16) || Bits_isZero(header->destinationAddr, 16)) {
        if (Bits_isZero(header->sourceAddr, 32)) {
            return incomingControlMessage(message, conn, context);
        }
        Log_debug(context->logger, "Got message with zero address");
        return 0;
    }
    if (!isValidAddress6(header->sourceAddr, false, conn)) {
        Log_debug(context->logger, "Got message with wrong address for connection");
        return 0;
    }

    TUNMessageType_push(message, Ethernet_TYPE_IP6, NULL);
    return Iface_next(&context->pub.tunInterface, message);
}

static Iface_DEFUN ip4FromNode(struct Message* message,
                               struct IpTunnel_Connection* conn,
                               struct IpTunnel_pvt* context)
{
    struct Headers_IP4Header* header = (struct Headers_IP4Header*) message->bytes;
    if (Bits_isZero(header->sourceAddr, 4) || Bits_isZero(header->destAddr, 4)) {
        Log_debug(context->logger, "Got message with zero address");
        return 0;
    } else if (!isValidAddress4(header->sourceAddr, false, conn)) {
        Log_debug(context->logger, "Got message with wrong address [%d.%d.%d.%d] for connection "
                                   "[%d.%d.%d.%d/%d:%d]",
                  header->sourceAddr[0], header->sourceAddr[1],
                  header->sourceAddr[2], header->sourceAddr[3],
                  conn->connectionIp4[0], conn->connectionIp4[1],
                  conn->connectionIp4[2], conn->connectionIp4[3],
                  conn->connectionIp4Alloc, conn->connectionIp4Prefix);
        return 0;
    }

    TUNMessageType_push(message, Ethernet_TYPE_IP4, NULL);
    return Iface_next(&context->pub.tunInterface, message);
}

static Iface_DEFUN incomingFromNode(struct Message* message, struct Iface* nodeIf)
{
    struct IpTunnel_pvt* context =
        Identity_containerOf(nodeIf, struct IpTunnel_pvt, pub.nodeInterface);

    //Log_debug(context->logger, "Got incoming message");

    Assert_true(message->length >= RouteHeader_SIZE + DataHeader_SIZE);
    struct RouteHeader* rh = (struct RouteHeader*) message->bytes;
    struct DataHeader* dh = (struct DataHeader*) &rh[1];
    Assert_true(DataHeader_getContentType(dh) == ContentType_IPTUN);
    struct IpTunnel_Connection* conn = connectionByPubKey(rh->publicKey, context);
    if (!conn) {
        if (Defined(Log_DEBUG)) {
            uint8_t addr[40];
            AddrTools_printIp(addr, rh->ip6);
            Log_debug(context->logger, "Got message from unrecognized node [%s]", addr);
        }
        return 0;
    }

    Message_shift(message, -(RouteHeader_SIZE + DataHeader_SIZE), NULL);

    if (message->length > 40 && Headers_getIpVersion(message->bytes) == 6) {
        return ip6FromNode(message, conn, context);
    }
    if (message->length > 20 && Headers_getIpVersion(message->bytes) == 4) {
        return ip4FromNode(message, conn, context);
    }

    if (Defined(Log_DEBUG)) {
        uint8_t addr[40];
        AddrTools_printIp(addr, rh->ip6);
        Log_debug(context->logger,
                  "Got message of unknown type, length: [%d], IP version [%d] from [%s]",
                  message->length,
                  (message->length > 1) ? Headers_getIpVersion(message->bytes) : 0,
                  addr);
    }
    return 0;
}

static void timeout(void* vcontext)
{
    struct IpTunnel_pvt* context = vcontext;
    if (!context->pub.connectionList.count) {
        return;
    }
    Log_debug(context->logger, "Checking for connections to poll. Total connections [%u]",
                                context->pub.connectionList.count);
    uint32_t beginning = Random_uint32(context->rand) % context->pub.connectionList.count;
    uint32_t i = beginning;
    do {
        Assert_true(i < context->pub.connectionList.count);
        struct IpTunnel_Connection* conn = &context->pub.connectionList.connections[i];
        if (conn->isOutgoing
            && Bits_isZero(conn->connectionIp6, 16)
            && Bits_isZero(conn->connectionIp4, 4))
        {
            requestAddresses(conn, context);
            break;
        }
        i = (i + 1) % context->pub.connectionList.count;
    } while (i != beginning);
}

void IpTunnel_setTunName(char* interfaceName, struct IpTunnel* ipTun)
{
    struct IpTunnel_pvt* ctx = Identity_check((struct IpTunnel_pvt*) ipTun);
    ctx->ifName = String_new(interfaceName, ctx->allocator);
}

struct IpTunnel* IpTunnel_new(struct Log* logger,
                              struct EventBase* eventBase,
                              struct Allocator* alloc,
                              struct Random* rand,
                              struct RouteGen* rg)
{
    struct IpTunnel_pvt* context = Allocator_clone(alloc, (&(struct IpTunnel_pvt) {
        .pub = {
            .tunInterface = { .send = incomingFromTun },
            .nodeInterface = { .send = incomingFromNode }
        },
        .allocator = alloc,
        .logger = logger,
        .rand = rand,
        .rg = rg
    }));
    context->timeout = Timeout_setInterval(timeout, context, 10000, eventBase, alloc);
    Identity_set(context);

    return &context->pub;
}
