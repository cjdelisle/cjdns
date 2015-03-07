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
#include "crypto/AddressCalc.h"
#include "util/platform/netdev/NetDev.h"
#include "util/Checksum.h"
#include "util/AddrTools.h"
#include "util/events/EventBase.h"
#include "util/Identity.h"
#include "util/events/Timeout.h"
#include "util/Defined.h"
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
                Bits_memcpyConst(conn,
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
                             struct Sockaddr* ip6Addr, uint8_t ip6Prefix,
                             struct Sockaddr* ip4Addr, uint8_t ip4Prefix,
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
    Bits_memcpyConst(conn->routeHeader.publicKey, publicKeyOfAuthorizedNode, 32);
    AddressCalc_addressForPublicKey(conn->routeHeader.ip6, publicKeyOfAuthorizedNode);
    if (ip4Address) {
        Bits_memcpyConst(conn->connectionIp4, ip4Address, 4);
        conn->connectionIp4Prefix = ip4Prefix;
    }
    if (ip6Address) {
        Bits_memcpyConst(conn->connectionIp6, ip6Address, 16);
        conn->connectionIp6Prefix = ip6Prefix;
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
    Bits_memcpyConst(conn->routeHeader.publicKey, publicKeyOfNodeToConnectTo, 32);
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
    //struct IpTunnel_pvt* context = Identity_check((struct IpTunnel_pvt*)tunnel);

    return 0;
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

static void addAddress(char* printedAddr, uint8_t prefixLen, struct IpTunnel_pvt* ctx)
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
    struct Jmp j;
    Jmp_try(j) {
        NetDev_addAddress(ctx->ifName->bytes, &ss.addr, prefixLen, NULL, &j.handler);
    } Jmp_catch {
        Log_error(ctx->logger, "Error setting ip address on TUN [%s]", j.message);
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
    Bits_memcpyConst(&number, txid->bytes, 4);
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
    if (ip4 && ip4->len == 4) {
        Bits_memcpyConst(conn->connectionIp4, ip4->bytes, 4);

        if (ip4Prefix && *ip4Prefix >= 0 && *ip4Prefix <= 32) {
            conn->connectionIp4Prefix = (uint8_t) *ip4Prefix;
        } else {
            conn->connectionIp4Prefix = 0;
        }

        struct Sockaddr* sa = Sockaddr_clone(Sockaddr_LOOPBACK, alloc);
        uint8_t* addrBytes = NULL;
        Sockaddr_getAddress(sa, &addrBytes);
        Bits_memcpy(addrBytes, ip4->bytes, 4);
        char* printedAddr = Sockaddr_print(sa, alloc);

        Log_info(context->logger, "Got issued address [%s/%d] for connection [%d]",
                 printedAddr, conn->connectionIp4Prefix, conn->number);

        addAddress(printedAddr, conn->connectionIp4Prefix, context);
    }

    String* ip6 = Dict_getString(addresses, String_CONST("ip6"));
    int64_t* ip6Prefix = Dict_getInt(addresses, String_CONST("ip6Prefix"));
    if (ip6 && ip6->len == 16) {
        Bits_memcpyConst(conn->connectionIp6, ip6->bytes, 16);

        if (ip6Prefix && *ip6Prefix >= 0 && *ip6Prefix <= 128) {
            conn->connectionIp6Prefix = (uint8_t) *ip6Prefix;
        } else {
            conn->connectionIp6Prefix = 0;
        }

        if (Defined(Darwin) && conn->connectionIp6Prefix < 3) {
            // Apple doesn't handle prefix length of 0 properly. 3 covers
            // all IPv6 unicast space.
            conn->connectionIp6Prefix = 3;
        }

        struct Sockaddr* sa = Sockaddr_clone(Sockaddr_LOOPBACK6, alloc);
        uint8_t* addrBytes = NULL;
        Sockaddr_getAddress(sa, &addrBytes);
        Bits_memcpy(addrBytes, ip6->bytes, 16);
        char* printedAddr = Sockaddr_print(sa, alloc);

        Log_info(context->logger, "Got issued address [%s/%d] for connection [%d]",
                 printedAddr, conn->connectionIp6Prefix, conn->number);

        addAddress(printedAddr, conn->connectionIp6Prefix, context);
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

    if (Defined(Log_DEBUG)) {
        uint8_t lastChar = message->bytes[message->length - 1];
        message->bytes[message->length - 1] = '\0';
        Log_debug(context->logger, "Message content [%s%c]", message->bytes, lastChar);
        message->bytes[message->length - 1] = lastChar;
    }

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

/**
 * If there are multiple connections to the same server,
 * the ip address on the packet might belong to the wrong one.
 * In that case we get the right connection.
 * If the other party has sent a packet from an address which is not
 * valid, this will return NULL and their packet can be dropped.
 *
 * @param conn the connection which matches the other node's key.
 * @param sourceAndDestIp6 the source and destination IPv6 addresses,
 *                         must be NULL if sourceAndDestIp4 is specified.
 * @param sourceAndDestIp4 the source and destination IPv4 addresses.
 *                         must be NULL if sourceAndDestIp6 is specified.
 * @param context
 * @return the real connection or null if the packet is invalid.
 */
static struct IpTunnel_Connection* getConnection(struct IpTunnel_Connection* conn,
                                                 uint8_t sourceAndDestIp6[32],
                                                 uint8_t sourceAndDestIp4[8],
                                                 bool isFromTun,
                                                 struct IpTunnel_pvt* context)
{
    uint8_t* source = (sourceAndDestIp6) ? sourceAndDestIp6 : sourceAndDestIp4;
    uint32_t length = (sourceAndDestIp6) ? 16 : 4;
    uint8_t* destination = source + length;

    if (sourceAndDestIp6) {
        // never allowed
        if (source[0] == 0xfc || destination[0] == 0xfc) {
            return NULL;
        }
    }

    struct IpTunnel_Connection* lastConnection =
        &context->pub.connectionList.connections[context->pub.connectionList.count];

    do {
        // If this is an incoming message from the w0rld, and we're the client, we want
        // to make sure it's addressed to us (destination), if we're the server we want to make
        // sure our clients are using the addresses we gave them (source).
        //
        // If this is an outgoing message from the TUN, we just want to find a sutable server to
        // handle it. The behavior of this function relies on the fact that all incoming
        // connections are first on the list.
        //
        uint8_t* compareAddr = (isFromTun)
            ? ((conn->isOutgoing) ? source : destination)
            : ((conn->isOutgoing) ? destination : source);

        uint8_t* connectionAddr = (sourceAndDestIp6) ? conn->connectionIp6 : conn->connectionIp4;
        if (!Bits_memcmp(compareAddr, connectionAddr, length)) {
            return conn;
        }
        conn++;
    } while (conn <= lastConnection);

    return NULL;
}

static Iface_DEFUN incomingFromTun(struct Message* message, struct Iface* tunIf)
{
    struct IpTunnel_pvt* context = Identity_check((struct IpTunnel_pvt*)tunIf);

    if (message->length < 20) {
        Log_debug(context->logger, "Dropping runt.");
    }

    struct IpTunnel_Connection* conn = NULL;
    if (!context->pub.connectionList.connections) {
        // No connections authorized, fall through to "unrecognized address"
    } else if (message->length > 40 && Headers_getIpVersion(message->bytes) == 6) {
        struct Headers_IP6Header* header = (struct Headers_IP6Header*) message->bytes;
        conn = getConnection(context->pub.connectionList.connections,
                             header->sourceAddr,
                             NULL,
                             true,
                             context);
    } else if (message->length > 20 && Headers_getIpVersion(message->bytes) == 4) {
        struct Headers_IP4Header* header = (struct Headers_IP4Header*) message->bytes;
        conn = getConnection(context->pub.connectionList.connections,
                             NULL,
                             header->sourceAddr,
                             true,
                             context);
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
    if (!getConnection(conn, header->sourceAddr, NULL, false, context)) {
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
    } else if (!getConnection(conn, NULL, header->sourceAddr, false, context)) {
        Log_debug(context->logger, "Got message with wrong address for connection");
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
                              struct Random* rand)
{
    struct IpTunnel_pvt* context = Allocator_clone(alloc, (&(struct IpTunnel_pvt) {
        .pub = {
            .tunInterface = { .send = incomingFromTun },
            .nodeInterface = { .send = incomingFromNode }
        },
        .allocator = alloc,
        .logger = logger,
        .rand = rand
    }));
    context->timeout = Timeout_setInterval(timeout, context, 10000, eventBase, alloc);
    Identity_set(context);

    return &context->pub;
}
