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
#include "benc/String.h"
#include "benc/Dict.h"
#include "benc/List.h"
#include "benc/Int.h"
#include "benc/serialization/standard/BencMessageWriter.h"
#include "benc/serialization/standard/BencMessageReader.h"
#include "crypto/AddressCalc.h"
#include "crypto/random/Random.h"
#include "interface/tuntap/TUNMessageType.h"
#include "memory/Allocator.h"
#include "tunnel/IpTunnel.h"
#include "rust/cjdns_sys/RTypes.h"
#include "rust/cjdns_sys/Rffi.h"
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

    /** To get the name of the TUN interface so that ip addresses can be added. */
    struct GlobalConfig* globalConf;

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

static Iface_DEFUN sendToNode(Message_t* message,
                              struct IpTunnel_Connection* connection,
                              struct IpTunnel_pvt* context)
{
    Err(Message_epush(message, NULL, DataHeader_SIZE));
    struct DataHeader* dh = (struct DataHeader*) Message_bytes(message);
    DataHeader_setContentType(dh, ContentType_IPTUN);
    DataHeader_setVersion(dh, DataHeader_CURRENT_VERSION);
    Err(Message_epush(message, &connection->routeHeader, RouteHeader_SIZE));
    return Iface_next(&context->pub.nodeInterface, message);
}

static void sendControlMessage(Dict* dict,
                               struct IpTunnel_Connection* connection,
                               struct Allocator* requestAlloc,
                               struct IpTunnel_pvt* context)
{
    Message_t* msg = Message_new(0, 1024, requestAlloc);
    Err_assert(BencMessageWriter_write(dict, msg));

    int length = Message_getLength(msg);

    // do UDP header.
    Err_assert(Message_eshift(msg, Headers_UDPHeader_SIZE));
    struct Headers_UDPHeader* uh = (struct Headers_UDPHeader*) Message_bytes(msg);
    uh->srcPort_be = 0;
    uh->destPort_be = 0;
    uh->length_be = Endian_hostToBigEndian16(length);
    uh->checksum_be = 0;

    uint16_t payloadLength = Message_getLength(msg);

    Err_assert(Message_eshift(msg, Headers_IP6Header_SIZE));
    struct Headers_IP6Header* header = (struct Headers_IP6Header*) Message_bytes(msg);
    header->versionClassAndFlowLabel = 0;
    header->flowLabelLow_be = 0;
    header->nextHeader = 17;
    header->hopLimit = 0;
    header->payloadLength_be = Endian_hostToBigEndian16(payloadLength);
    Headers_setIpVersion(header);

    // zero the source and dest addresses.
    Bits_memset(header->sourceAddr, 0, 32);

    uh->checksum_be = Checksum_udpIp6_be(header->sourceAddr,
                                      (uint8_t*) uh,
                                      Message_getLength(msg) - Headers_IP6Header_SIZE);

    Iface_CALL(sendToNode, msg, connection, context);
}

static void requestAddresses(struct IpTunnel_Connection* conn, struct IpTunnel_pvt* context)
{
    if (Defined(Log_DEBUG)) {
        uint8_t addr[60];
        AddrTools_printPath(&addr[0], Endian_bigEndianToHost64(conn->routeHeader.sh.label_be));
        addr[19] = '@';
        AddrTools_printIp(&addr[20], conn->routeHeader.ip6);
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

static bool isControlMessageInvalid(Message_t* message, struct IpTunnel_pvt* context)
{
    struct Headers_IP6Header* header = (struct Headers_IP6Header*) Message_bytes(message);
    uint16_t length = Endian_bigEndianToHost16(header->payloadLength_be);
    if (header->nextHeader != 17 || Message_getLength(message) < length + Headers_IP6Header_SIZE) {
        Log_warn(context->logger, "Invalid IPv6 packet (not UDP or length field too big)");
        return true;
    }

    Err_assert(Message_eshift(message, -Headers_IP6Header_SIZE));
    struct Headers_UDPHeader* udp = (struct Headers_UDPHeader*) Message_bytes(message);

    if (Checksum_udpIp6_be(header->sourceAddr, Message_bytes(message), length)) {
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

    Err_assert(Message_eshift(message, -Headers_UDPHeader_SIZE));

    Err_assert(Message_truncate(message, length));
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
        return Rffi_error("INVALID", requestAlloc);
    }
    Dict* addresses = Dict_new(requestAlloc);
    bool noAddresses = true;
    if (!Bits_isZero(conn->connectionIp6, 16)) {
        Dict_putStringC(addresses,
                       "ip6",
                       String_newBinary((char*)conn->connectionIp6, 16, requestAlloc),
                       requestAlloc);
        Dict_putIntC(addresses,
                    "ip6Prefix", (int64_t)conn->connectionIp6Prefix,
                    requestAlloc);
        Dict_putIntC(addresses,
                    "ip6Alloc", (int64_t)conn->connectionIp6Alloc,
                    requestAlloc);

        noAddresses = false;
    }
    if (!Bits_isZero(conn->connectionIp4, 4)) {
        Dict_putStringC(addresses,
                       "ip4",
                       String_newBinary((char*)conn->connectionIp4, 4, requestAlloc),
                       requestAlloc);
        Dict_putIntC(addresses,
                    "ip4Prefix", (int64_t)conn->connectionIp4Prefix,
                    requestAlloc);
        Dict_putIntC(addresses,
                    "ip4Alloc", (int64_t)conn->connectionIp4Alloc,
                    requestAlloc);

        noAddresses = false;
    }
    if (noAddresses) {
        Log_warn(context->logger, "no addresses to provide");
        // The message is ok, this one is our fault
        return NULL;
    }

    Dict* msg = Dict_new(requestAlloc);
    Dict_putDictC(msg, "addresses", addresses, requestAlloc);

    String* txid = Dict_getStringC(request, "txid");
    if (txid) {
        Dict_putStringC(msg, "txid", txid, requestAlloc);
    }

    sendControlMessage(msg, conn, requestAlloc, context);
    return NULL;
}

static void addAddress(char* printedAddr, uint8_t prefixLen,
                       uint8_t allocSize, struct IpTunnel_pvt* ctx,
                       struct Allocator* tempAlloc)
{
    String* tunName = GlobalConfig_getTunName(ctx->globalConf);
    if (!tunName) {
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
    RTypes_Error_t* er = NetDev_addAddress(tunName->bytes, &ss.addr, ctx->logger, tempAlloc);
    if (er) {
        Log_error(ctx->logger, "Error setting ip address on TUN [%s]",
            Rffi_printError(er, tempAlloc));
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
        return Rffi_error("INVALID", alloc);
    }

    String* txid = Dict_getStringC(d, "txid");
    if (!txid || txid->len != 4) {
        Log_info(context->logger, "missing or wrong length txid");
        return Rffi_error("INVALID", alloc);
    }

    int number;
    Bits_memcpy(&number, txid->bytes, 4);
    if (number < 0 || number >= (int)context->nextConnectionNumber) {
        Log_info(context->logger, "txid out of range");
        return Rffi_error("INVALID", alloc);
    }

    if (number != conn->number) {
        for (int i = 0; i < (int)context->pub.connectionList.count; i++) {
            if (context->pub.connectionList.connections[i].number == number) {
                if (Bits_memcmp(conn->routeHeader.publicKey,
                                context->pub.connectionList.connections[i].routeHeader.publicKey,
                                32))
                {
                    Log_info(context->logger, "txid doesn't match origin");
                    return Rffi_error("INVALID", alloc);
                } else {
                    conn = &context->pub.connectionList.connections[i];
                }
            }
        }
    }

    Dict* addresses = Dict_getDictC(d, "addresses");

    String* ip4 = Dict_getStringC(addresses, "ip4");
    int64_t* ip4Prefix = Dict_getIntC(addresses, "ip4Prefix");
    int64_t* ip4Alloc = Dict_getIntC(addresses, "ip4Alloc");

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

        addAddress(printedAddr,
            conn->connectionIp4Prefix, conn->connectionIp4Alloc, context, alloc);
    }

    String* ip6 = Dict_getStringC(addresses, "ip6");
    int64_t* ip6Prefix = Dict_getIntC(addresses, "ip6Prefix");
    int64_t* ip6Alloc = Dict_getIntC(addresses, "ip6Alloc");

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

        addAddress(printedAddr,
            conn->connectionIp6Prefix, conn->connectionIp6Alloc, context, alloc);
    }
    if (context->rg->hasUncommittedChanges) {
        String* tunName = GlobalConfig_getTunName(context->globalConf);
        if (!tunName) {
            Log_error(context->logger, "Failed to set routes because TUN interface is not setup");
            return Rffi_error("INVALID", alloc);
        }
        struct Er_Ret* er = NULL;
        Er_check(&er, RouteGen_commit(context->rg, tunName->bytes, alloc));
        if (er) {
            Log_error(context->logger, "Error setting routes for TUN [%s]", er->message);
            return Rffi_error("INVALID", alloc);
        }
    }
    return NULL;
}

static Iface_DEFUN incomingControlMessage(Message_t* message,
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
        return Error(message, "INVALID");
    }

    Log_debug(context->logger, "Message content [%s]",
        Escape_getEscaped(Message_bytes(message), Message_getLength(message), Message_getAlloc(message)));

    struct Allocator* alloc = Allocator_child(Message_getAlloc(message));

    Dict* d = NULL;
    const char* err = BencMessageReader_readNoExcept(message, alloc, &d);
    if (err) {
        Log_info(context->logger, "Failed to parse message [%s]", err);
        return Error(message, "INVALID");
    }

    if (Dict_getDictC(d, "addresses")) {
        return incomingAddresses(d, conn, alloc, context);
    }
    if (String_equals(String_CONST("IpTunnel_getAddresses"),
                      Dict_getStringC(d, "q")))
    {
        return requestForAddresses(d, conn, alloc, context);
    }
    Log_warn(context->logger, "Message which is unhandled");
    return Error(message, "INVALID");
}

#define GET64(buffer) \
    (__extension__ ({                                               \
        Assert_true(!((uintptr_t)(buffer) % 4));                    \
        uint64_t x = (uint64_t) (((uint32_t*)(buffer))[0]);         \
        x |= (( (uint64_t) ((uint32_t*)(buffer))[1]) << 32);        \
        Endian_bigEndianToHost64(x);                                \
    }))

#define GET32(buffer) \
    (__extension__ ({                                               \
        Assert_true(!((uintptr_t)(buffer) % 4));                    \
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

static bool isValidAddress4(uint8_t sourceIp4[4],
                            uint8_t destIp4[4],
                            bool isFromTun,
                            struct IpTunnel_Connection* conn)
{
    uint8_t* compareAddr = (isFromTun)
        ? ((conn->isOutgoing) ? sourceIp4 : destIp4)
        : ((conn->isOutgoing) ? destIp4 : sourceIp4);
    return prefixMatches4(compareAddr, conn->connectionIp4, conn->connectionIp4Alloc);
}

static bool isValidAddress6(uint8_t sourceIp6[16],
                            uint8_t destIp6[16],
                            bool isFromTun,
                            struct IpTunnel_Connection* conn)
{
    if (AddressCalc_validAddress(sourceIp6) || AddressCalc_validAddress(destIp6)) {
        return false;
    }
    uint8_t* compareAddr = (isFromTun)
        ? ((conn->isOutgoing) ? sourceIp6 : destIp6)
        : ((conn->isOutgoing) ? destIp6 : sourceIp6);
    return prefixMatches6(compareAddr, conn->connectionIp6, conn->connectionIp6Alloc);
}

static struct IpTunnel_Connection* findConnection(uint8_t sourceIp6[16],
                                                  uint8_t destIp6[16],
                                                  uint8_t sourceIp4[4],
                                                  uint8_t destIp4[4],
                                                  bool isFromTun,
                                                  struct IpTunnel_pvt* context)
{
    for (int i = 0; i < (int)context->pub.connectionList.count; i++) {
        struct IpTunnel_Connection* conn = &context->pub.connectionList.connections[i];
        if (sourceIp6 && destIp6 && isValidAddress6(sourceIp6, destIp6, isFromTun, conn)) {
            return conn;
        }
        if (sourceIp4 && destIp4 && isValidAddress4(sourceIp4, destIp4, isFromTun, conn)) {
            return conn;
        }
    }
    return NULL;
}

static Iface_DEFUN incomingFromTun(Message_t* message, struct Iface* tunIf)
{
    struct IpTunnel_pvt* context = Identity_check((struct IpTunnel_pvt*)tunIf);

    if (Message_getLength(message) < 20) {
        Log_debug(context->logger, "DROP runt");
        return Error(message, "RUNT");
    }

    struct IpTunnel_Connection* conn = NULL;
    if (!context->pub.connectionList.connections) {
        // No connections authorized, fall through to "unrecognized address"
    } else if (Message_getLength(message) > 40 && Headers_getIpVersion(Message_bytes(message)) == 6) {
        struct Headers_IP6Header* header = (struct Headers_IP6Header*) Message_bytes(message);
        conn = findConnection(
            header->sourceAddr, header->destinationAddr, NULL, NULL, true, context);
    } else if (Message_getLength(message) > 20 && Headers_getIpVersion(Message_bytes(message)) == 4) {
        struct Headers_IP4Header* header = (struct Headers_IP4Header*) Message_bytes(message);
        conn = findConnection(NULL, NULL, header->sourceAddr, header->destAddr, true, context);
    } else {
        Log_debug(context->logger, "Message of unknown type from TUN");
        return Error(message, "INVALID");
    }

    if (!conn) {
        Log_debug(context->logger, "Message with unrecognized address from TUN");
        return Error(message, "INVALID");
    }

    return sendToNode(message, conn, context);
}

static Iface_DEFUN ip6FromNode(Message_t* message,
                               struct IpTunnel_Connection* conn,
                               struct IpTunnel_pvt* context)
{
    struct Headers_IP6Header* header = (struct Headers_IP6Header*) Message_bytes(message);
    if (Bits_isZero(header->sourceAddr, 16) || Bits_isZero(header->destinationAddr, 16)) {
        if (Bits_isZero(header->sourceAddr, 32)) {
            return incomingControlMessage(message, conn, context);
        }
        Log_debug(context->logger, "Got message with zero address");
        return Error(message, "INVALID");
    }
    if (!isValidAddress6(header->sourceAddr, header->destinationAddr, false, conn)) {
        uint8_t addr[40];
        AddrTools_printIp(addr, header->sourceAddr);
        Log_debug(context->logger, "Got message with wrong address for connection [%s]", addr);
        return Error(message, "INVALID");
    }

    Er_assert(TUNMessageType_push(message, Ethernet_TYPE_IP6));
    return Iface_next(&context->pub.tunInterface, message);
}

static Iface_DEFUN ip4FromNode(Message_t* message,
                               struct IpTunnel_Connection* conn,
                               struct IpTunnel_pvt* context)
{
    struct Headers_IP4Header* header = (struct Headers_IP4Header*) Message_bytes(message);
    if (Bits_isZero(header->sourceAddr, 4) || Bits_isZero(header->destAddr, 4)) {
        Log_debug(context->logger, "Got message with zero address");
        return Error(message, "INVALID");
    } else if (!isValidAddress4(header->sourceAddr, header->destAddr, false, conn)) {
        Log_debug(context->logger, "Got message with wrong address [%d.%d.%d.%d] for connection "
                                   "[%d.%d.%d.%d/%d:%d]",
                  header->sourceAddr[0], header->sourceAddr[1],
                  header->sourceAddr[2], header->sourceAddr[3],
                  conn->connectionIp4[0], conn->connectionIp4[1],
                  conn->connectionIp4[2], conn->connectionIp4[3],
                  conn->connectionIp4Alloc, conn->connectionIp4Prefix);
        return Error(message, "INVALID");
    }

    Er_assert(TUNMessageType_push(message, Ethernet_TYPE_IP4));
    return Iface_next(&context->pub.tunInterface, message);
}

static Iface_DEFUN incomingFromNode(Message_t* message, struct Iface* nodeIf)
{
    struct IpTunnel_pvt* context =
        Identity_containerOf(nodeIf, struct IpTunnel_pvt, pub.nodeInterface);

    //Log_debug(context->logger, "Got incoming message");

    Assert_true(Message_getLength(message) >= RouteHeader_SIZE + DataHeader_SIZE);
    struct RouteHeader* rh = (struct RouteHeader*) Message_bytes(message);
    struct DataHeader* dh = (struct DataHeader*) &rh[1];
    Assert_true(DataHeader_getContentType(dh) == ContentType_IPTUN);
    struct IpTunnel_Connection* conn = connectionByPubKey(rh->publicKey, context);
    if (!conn) {
        if (Defined(Log_DEBUG)) {
            uint8_t addr[40];
            AddrTools_printIp(addr, rh->ip6);
            Log_debug(context->logger, "Got message from unrecognized node [%s]", addr);
        }
        return NULL;
    }

    Err(Message_eshift(message, -(RouteHeader_SIZE + DataHeader_SIZE)));

    if (Message_getLength(message) > 40 && Headers_getIpVersion(Message_bytes(message)) == 6) {
        return ip6FromNode(message, conn, context);
    }
    if (Message_getLength(message) > 20 && Headers_getIpVersion(Message_bytes(message)) == 4) {
        return ip4FromNode(message, conn, context);
    }

    if (Defined(Log_DEBUG)) {
        uint8_t addr[40];
        AddrTools_printIp(addr, rh->ip6);
        Log_debug(context->logger,
                  "Got message of unknown type, length: [%d], IP version [%d] from [%s]",
                  Message_getLength(message),
                  (Message_getLength(message) > 1) ? Headers_getIpVersion(Message_bytes(message)) : 0,
                  addr);
    }
    return Error(message, "INVALID");
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

struct IpTunnel* IpTunnel_new(struct Log* logger,
                              EventBase_t* eventBase,
                              struct Allocator* alloc,
                              struct Random* rand,
                              struct RouteGen* rg,
                              struct GlobalConfig* globalConf)
{
    struct IpTunnel_pvt* context = Allocator_clone(alloc, (&(struct IpTunnel_pvt) {
        .pub = {
            .tunInterface = { .send = incomingFromTun },
            .nodeInterface = { .send = incomingFromNode }
        },
        .allocator = alloc,
        .logger = logger,
        .rand = rand,
        .rg = rg,
        .globalConf = globalConf
    }));
    context->timeout = Timeout_setInterval(timeout, context, 10000, eventBase, alloc);
    Identity_set(context);

    return &context->pub;
}
