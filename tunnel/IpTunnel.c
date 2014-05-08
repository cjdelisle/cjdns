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
#include "admin/angel/Hermes.h"
#include "benc/String.h"
#include "benc/Dict.h"
#include "benc/List.h"
#include "benc/Int.h"
#include "benc/serialization/standard/StandardBencSerializer.h"
#include "benc/serialization/BencSerializer.h"
#include "crypto/random/Random.h"
#include "exception/Jmp.h"
#include "io/ArrayWriter.h"
#include "io/ArrayReader.h"
#include "interface/tuntap/TUNMessageType.h"
#include "memory/Allocator.h"
#include "tunnel/IpTunnel.h"
#include "crypto/AddressCalc.h"
#include "util/Checksum.h"
#include "util/AddrTools.h"
#include "util/events/EventBase.h"
#include "util/Identity.h"
#include "util/events/Timeout.h"
#include "wire/Error.h"
#include "wire/Headers.h"
#include "wire/Ethernet.h"

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

    /** The angel connector for setting IP addresses. */
    struct Hermes* hermes;

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
        if (!Bits_memcmp(pubKey, context->pub.connectionList.connections[i].header.nodeKey, 32)) {
            return &context->pub.connectionList.connections[i];
        }
    }
    return NULL;
}

/**
 * Allow another node to tunnel IPv4 and/or ICANN IPv6 through this node.
 *
 * @param publicKeyOfAuthorizedNode the key for the node which will be allowed to connect.
 * @param ip6Addr the IPv6 address which the node will be issued or NULL.
 * @param ip4Addr the IPv4 address which the node will be issued or NULL.
 * @param tunnel the IpTunnel.
 * @return an connection number which is usable with IpTunnel_remove().
 */
int IpTunnel_allowConnection(uint8_t publicKeyOfAuthorizedNode[32],
                             struct Sockaddr* ip6Addr,
                             struct Sockaddr* ip4Addr,
                             struct IpTunnel* tunnel)
{
    struct IpTunnel_pvt* context = Identity_check((struct IpTunnel_pvt*)tunnel);

    uint8_t* ip6Address = NULL;
    uint8_t* ip4Address = NULL;
    if (ip6Addr) {
        Sockaddr_getAddress(ip6Addr, &ip6Address);
    }
    if (ip4Addr) {
        Sockaddr_getAddress(ip4Addr, &ip4Address);
    }

    struct IpTunnel_Connection* conn = newConnection(false, context);
    Bits_memcpyConst(conn->header.nodeKey, publicKeyOfAuthorizedNode, 32);
    AddressCalc_addressForPublicKey(conn->header.nodeIp6Addr, publicKeyOfAuthorizedNode);
    if (ip4Address) {
        Bits_memcpyConst(conn->connectionIp4, ip4Address, 4);
    }
    if (ip6Address) {
        Bits_memcpyConst(conn->connectionIp6, ip6Address, 16);
    }
    return conn->number;
}

static uint8_t sendToNode(struct Message* message,
                          struct IpTunnel_Connection* connection,
                          struct IpTunnel_pvt* context)
{
    Message_push(message, &connection->header, IpTunnel_PacketInfoHeader_SIZE, NULL);
    if (context->pub.nodeInterface.receiveMessage) {
        return context->pub.nodeInterface.receiveMessage(message, &context->pub.nodeInterface);
    }
    Log_info(context->logger, "Message undeliverable because IpTunnel is not registered");
    return Error_UNDELIVERABLE;
}

static uint8_t sendControlMessage(Dict* dict,
                                  struct IpTunnel_Connection* connection,
                                  struct Allocator* requestAlloc,
                                  struct IpTunnel_pvt* context)
{
    struct Message* message = Message_new(512, 512, requestAlloc);

    struct Writer* w = ArrayWriter_new(message->bytes, message->length, requestAlloc);
    StandardBencSerializer_get()->serializeDictionary(w, dict);
    message->length = w->bytesWritten;

    #ifdef Log_DEBUG
        message->bytes[message->length] = '\0';
        uint8_t addr[40];
        AddrTools_printIp(addr, connection->header.nodeIp6Addr);
        Log_debug(context->logger, "Send message to [%s] with content [%s]", addr, message->bytes);
    #endif

    // do UDP header.
    Message_shift(message, Headers_UDPHeader_SIZE, NULL);
    struct Headers_UDPHeader* uh = (struct Headers_UDPHeader*) message->bytes;
    uh->srcPort_be = 0;
    uh->destPort_be = 0;
    uh->length_be = Endian_hostToBigEndian16(w->bytesWritten);
    uh->checksum_be = 0;

    uint16_t payloadLength = message->length;

    Message_shift(message, Headers_IP6Header_SIZE, NULL);
    struct Headers_IP6Header* header = (struct Headers_IP6Header*) message->bytes;
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
                                      message->length - Headers_IP6Header_SIZE);

    return sendToNode(message, connection, context);
}

static uint8_t requestAddresses(struct IpTunnel_Connection* conn,
                                struct IpTunnel_pvt* context)
{
    #ifdef Log_DEBUG
        uint8_t addr[40];
        AddrTools_printIp(addr, conn->header.nodeIp6Addr);
        Log_debug(context->logger, "Requesting addresses from [%s] for connection [%d]",
                  addr, conn->number);
    #endif

    int number = conn->number;
    Dict d = Dict_CONST(
        String_CONST("q"), String_OBJ(String_CONST("IpTunnel_getAddresses")), Dict_CONST(
        String_CONST("txid"), String_OBJ((&(String){ .len = 4, .bytes = (char*)&number })),
        NULL
    ));
    struct Allocator* msgAlloc = Allocator_child(context->allocator);
    uint8_t ret = sendControlMessage(&d, conn, msgAlloc, context);
    Allocator_free(msgAlloc);
    return ret;
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
    Bits_memcpyConst(conn->header.nodeKey, publicKeyOfNodeToConnectTo, 32);
    AddressCalc_addressForPublicKey(conn->header.nodeIp6Addr, publicKeyOfNodeToConnectTo);

    #ifdef Log_DEBUG
        uint8_t addr[40];
        AddrTools_printIp(addr, conn->header.nodeIp6Addr);
        Log_debug(context->logger, "Trying to connect to [%s]", addr);
    #endif

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

static uint8_t isControlMessageInvalid(struct Message* message, struct IpTunnel_pvt* context)
{
    struct Headers_IP6Header* header = (struct Headers_IP6Header*) message->bytes;
    uint16_t length = Endian_bigEndianToHost16(header->payloadLength_be);
    if (header->nextHeader != 17 || message->length < length + Headers_IP6Header_SIZE) {
        Log_warn(context->logger, "Invalid IPv6 packet (not UDP or length field too big)");
        return Error_INVALID;
    }

    Message_shift(message, -Headers_IP6Header_SIZE, NULL);
    struct Headers_UDPHeader* udp = (struct Headers_UDPHeader*) message->bytes;

    if (Checksum_udpIp6(header->sourceAddr, message->bytes, length)) {
        Log_warn(context->logger, "Checksum mismatch");
        return Error_INVALID;
    }

    length -= Headers_UDPHeader_SIZE;
    if (Endian_bigEndianToHost16(udp->length_be) != length
        || udp->srcPort_be != 0
        || udp->destPort_be != 0)
    {
        Log_warn(context->logger, "Invalid UDP packet (length mismatch or wrong ports)");
        return Error_INVALID;
    }

    Message_shift(message, -Headers_UDPHeader_SIZE, NULL);

    message->length = length;
    return 0;
}

static uint8_t requestForAddresses(Dict* request,
                                   struct IpTunnel_Connection* conn,
                                   struct Allocator* requestAlloc,
                                   struct IpTunnel_pvt* context)
{
    #ifdef Log_DEBUG
        uint8_t addr[40];
        AddrTools_printIp(addr, conn->header.nodeIp6Addr);
        Log_debug(context->logger, "Got request for addresses from [%s]", addr);
    #endif

    if (conn->isOutgoing) {
        Log_warn(context->logger, "got request for addresses from outgoing connection");
        return Error_INVALID;
    }
    Dict* addresses = Dict_new(requestAlloc);
    bool noAddresses = true;
    if (!Bits_isZero(conn->connectionIp6, 16)) {
        Dict_putString(addresses,
                       String_CONST("ip6"),
                       String_newBinary((char*)conn->connectionIp6, 16, requestAlloc),
                       requestAlloc);
        noAddresses = false;
    }
    if (!Bits_isZero(conn->connectionIp4, 4)) {
        Dict_putString(addresses,
                       String_CONST("ip4"),
                       String_newBinary((char*)conn->connectionIp4, 4, requestAlloc),
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

    return sendControlMessage(msg, conn, requestAlloc, context);
}

static void addAddressCallback(Dict* responseMessage, void* vcontext)
{
    struct IpTunnel_pvt* ctx = Identity_check((struct IpTunnel_pvt*) vcontext);
    char* err = "invalid response";
    String* error = Dict_getString(responseMessage, String_CONST("error"));
    if (error) {
        err = error->bytes;
    }
    if (!error || !String_equals(error, String_CONST("none"))) {
        Log_error(ctx->logger, "Error setting ip address on TUN [%s]", err);

        #ifndef Log_ERROR
            // this whole function does essentially nothing but log an error so unused vars.
            err = err;
            ctx = ctx;
        #endif
    }
}

static void addAddress(char* printedAddr, struct IpTunnel_pvt* ctx)
{
#ifdef Darwin
    int prefixLen = 3;
#else
    int prefixLen = 0;
#endif
// Apple doesn't handle prefix length of 0 properly. 3 covers all IPv6 unicast space.
    if (!ctx->ifName) {
        Log_error(ctx->logger, "Failed to set IP address because TUN interface is not setup");
        return;
    }
    struct Jmp j;
    Jmp_try(j) {
        Dict args = Dict_CONST(
            String_CONST("address"), String_OBJ(String_CONST(printedAddr)), Dict_CONST(
            String_CONST("interfaceName"), String_OBJ(ctx->ifName), Dict_CONST(
            String_CONST("prefixLen"), Int_OBJ(prefixLen), NULL
        )));
        Dict msg = Dict_CONST(
            String_CONST("args"), Dict_OBJ(&args), Dict_CONST(
            String_CONST("q"), String_OBJ(String_CONST("Angel_addIp")), NULL
        ));
        Hermes_callAngel(&msg, addAddressCallback, ctx, ctx->allocator, &j.handler, ctx->hermes);
    } Jmp_catch {
        Log_error(ctx->logger, "Error setting ip address on TUN [%s]", j.message);
    }
}

static int incomingAddresses(Dict* d,
                             struct IpTunnel_Connection* conn,
                             struct Allocator* alloc,
                             struct IpTunnel_pvt* context)
{
    if (!conn->isOutgoing) {
        Log_warn(context->logger, "got offer of addresses from incoming connection");
        return Error_INVALID;
    }

    String* txid = Dict_getString(d, String_CONST("txid"));
    if (!txid || txid->len != 4) {
        Log_info(context->logger, "missing or wrong length txid");
        return Error_INVALID;
    }

    int number;
    Bits_memcpyConst(&number, txid->bytes, 4);
    if (number < 0 || number >= (int)context->nextConnectionNumber) {
        Log_info(context->logger, "txid out of range");
        return Error_INVALID;
    }

    if (number != conn->number) {
        for (int i = 0; i < (int)context->pub.connectionList.count; i++) {
            if (context->pub.connectionList.connections[i].number == number) {
                if (Bits_memcmp(conn->header.nodeKey,
                                context->pub.connectionList.connections[i].header.nodeKey,
                                32))
                {
                    Log_info(context->logger, "txid doesn't match origin");
                    return Error_INVALID;
                } else {
                    conn = &context->pub.connectionList.connections[i];
                }
            }
        }
    }

    Dict* addresses = Dict_getDict(d, String_CONST("addresses"));

    String* ip4 = Dict_getString(addresses, String_CONST("ip4"));
    if (ip4 && ip4->len == 4) {
        Bits_memcpyConst(conn->connectionIp4, ip4->bytes, 4);

        struct Sockaddr* sa = Sockaddr_clone(Sockaddr_LOOPBACK, alloc);
        uint8_t* addrBytes = NULL;
        Sockaddr_getAddress(sa, &addrBytes);
        Bits_memcpy(addrBytes, ip4->bytes, 4);
        char* printedAddr = Sockaddr_print(sa, alloc);

        Log_info(context->logger, "Got issued address [%s] for connection [%d]",
                 printedAddr, conn->number);

        addAddress(printedAddr, context);
    }

    String* ip6 = Dict_getString(addresses, String_CONST("ip6"));
    if (ip6 && ip6->len == 16) {
        Bits_memcpyConst(conn->connectionIp6, ip6->bytes, 16);

        struct Sockaddr* sa = Sockaddr_clone(Sockaddr_LOOPBACK6, alloc);
        uint8_t* addrBytes = NULL;
        Sockaddr_getAddress(sa, &addrBytes);
        Bits_memcpy(addrBytes, ip6->bytes, 16);
        char* printedAddr = Sockaddr_print(sa, alloc);

        Log_info(context->logger, "Got issued address [%s] for connection [%d]",
                 printedAddr, conn->number);

        addAddress(printedAddr, context);
    }
    return 0;
}

static uint8_t incomingControlMessage(struct Message* message,
                                      struct IpTunnel_Connection* conn,
                                      struct IpTunnel_pvt* context)
{
    #ifdef Log_DEBUG
        uint8_t addr[40];
        AddrTools_printIp(addr, conn->header.nodeIp6Addr);
        Log_debug(context->logger, "Got incoming message from [%s]", addr);
    #endif

    // This aligns the message on the content.
    if (isControlMessageInvalid(message, context)) {
        return Error_INVALID;
    }

    #ifdef Log_DEBUG
        uint8_t lastChar = message->bytes[message->length - 1];
        message->bytes[message->length - 1] = '\0';
        Log_debug(context->logger, "Message content [%s%c]", message->bytes, lastChar);
        message->bytes[message->length - 1] = lastChar;
    #endif

    struct Allocator* alloc = Allocator_child(message->alloc);

    struct Reader* r = ArrayReader_new(message->bytes, message->length, alloc);
    Dict dStore;
    Dict* d = &dStore;
    if (StandardBencSerializer_get()->parseDictionary(r, alloc, d)) {
        Log_info(context->logger, "Failed to parse message");
        return Error_INVALID;
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
    return Error_INVALID;
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

static uint8_t incomingFromTun(struct Message* message, struct Interface* tunIf)
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
        return Error_INVALID;
    }

    if (!conn) {
        Log_info(context->logger, "Message with unrecognized address from TUN");
        return Error_INVALID;
    }

    return sendToNode(message, conn, context);
}

static uint8_t ip6FromNode(struct Message* message,
                           struct IpTunnel_Connection* conn,
                           struct IpTunnel_pvt* context)
{
    struct Headers_IP6Header* header = (struct Headers_IP6Header*) message->bytes;
    if (Bits_isZero(header->sourceAddr, 16) || Bits_isZero(header->destinationAddr, 16)) {
        if (Bits_isZero(header->sourceAddr, 32)) {
            return incomingControlMessage(message, conn, context);
        }
        Log_debug(context->logger, "Got message with zero address");
        return Error_INVALID;
    }
    if (!getConnection(conn, header->sourceAddr, NULL, false, context)) {
        Log_debug(context->logger, "Got message with wrong address for connection");
        return Error_INVALID;
    }

    TUNMessageType_push(message, Ethernet_TYPE_IP6, NULL);

    struct Interface* tunIf = &context->pub.tunInterface;
    if (tunIf->receiveMessage) {
        tunIf->receiveMessage(message, tunIf);
    }
    return 0;
}

static uint8_t ip4FromNode(struct Message* message,
                           struct IpTunnel_Connection* conn,
                           struct IpTunnel_pvt* context)
{
    struct Headers_IP4Header* header = (struct Headers_IP4Header*) message->bytes;
    if (Bits_isZero(header->sourceAddr, 4) || Bits_isZero(header->destAddr, 4)) {
        Log_debug(context->logger, "Got message with zero address");
        return Error_INVALID;
    }
    if (!getConnection(conn, NULL, header->sourceAddr, false, context)) {
        Log_debug(context->logger, "Got message with wrong address for connection");
        return Error_INVALID;
    }

    TUNMessageType_push(message, Ethernet_TYPE_IP4, NULL);

    struct Interface* tunIf = &context->pub.tunInterface;
    if (tunIf->receiveMessage) {
        return tunIf->receiveMessage(message, tunIf);
    }
    return 0;
}

static uint8_t incomingFromNode(struct Message* message, struct Interface* nodeIf)
{
    struct IpTunnel_pvt* context =
        (struct IpTunnel_pvt*)(((char*)nodeIf) - offsetof(struct IpTunnel, nodeInterface));
    Identity_check(context);

    //Log_debug(context->logger, "Got incoming message");

    Assert_true(message->length >= IpTunnel_PacketInfoHeader_SIZE);
    struct IpTunnel_PacketInfoHeader* header = (struct IpTunnel_PacketInfoHeader*) message->bytes;
    struct IpTunnel_Connection* conn = connectionByPubKey(header->nodeKey, context);
    if (!conn) {
        #ifdef Log_DEBUG
            uint8_t addr[40];
            AddrTools_printIp(addr, header->nodeIp6Addr);
            Log_debug(context->logger, "Got message from unrecognized node [%s]", addr);
        #endif
        return 0;
    }

    Message_shift(message, -IpTunnel_PacketInfoHeader_SIZE, NULL);

    if (message->length > 40 && Headers_getIpVersion(message->bytes) == 6) {
        return ip6FromNode(message, conn, context);
    }
    if (message->length > 20 && Headers_getIpVersion(message->bytes) == 4) {
        return ip4FromNode(message, conn, context);
    }

    #ifdef Log_DEBUG
        uint8_t addr[40];
        AddrTools_printIp(addr, header->nodeIp6Addr);
        Log_debug(context->logger,
                  "Got message of unknown type, length: [%d], IP version [%d] from [%s]",
                  message->length,
                  (message->length > 1) ? Headers_getIpVersion(message->bytes) : 0,
                  addr);
    #endif
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
                              struct Hermes* hermes)
{
    struct IpTunnel_pvt* context = Allocator_clone(alloc, (&(struct IpTunnel_pvt) {
        .pub = {
            .tunInterface = { .sendMessage = incomingFromTun },
            .nodeInterface = { .sendMessage = incomingFromNode }
        },
        .allocator = alloc,
        .logger = logger,
        .rand = rand,
        .hermes = hermes
    }));
    context->timeout = Timeout_setInterval(timeout, context, 10000, eventBase, alloc);
    Identity_set(context);

    return &context->pub;
}
