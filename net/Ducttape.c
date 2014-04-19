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
#include "crypto/AddressCalc.h"
#include "crypto/CryptoAuth.h"
#include "util/log/Log.h"
#include "dht/Address.h"
#include "dht/DHTMessage.h"
#include "dht/DHTModule.h"
#include "dht/DHTModuleRegistry.h"
#include "dht/dhtcore/Node.h"
#include "dht/dhtcore/RouterModule.h"
#include "dht/dhtcore/SearchRunner.h"
#include "dht/dhtcore/RumorMill.h"
#include "interface/tuntap/TUNMessageType.h"
#include "interface/Interface.h"
#include "interface/SessionManager.h"
#include "util/log/Log.h"
#include "memory/Allocator.h"
#include "net/Ducttape_pvt.h"
#include "switch/SwitchCore.h"
#include "switch/LabelSplicer.h"
#include "util/AddrTools.h"
#include "util/Bits.h"
#include "util/Checksum.h"
#include "util/version/Version.h"
#include "util/Assert.h"
#include "tunnel/IpTunnel.h"
#include "util/events/Time.h"
#include "wire/Control.h"
#include "wire/Error.h"
#include "wire/Headers.h"
#include "wire/Ethernet.h"

#include <stdint.h>

/** Size of the per-message workspace. */
#define PER_MESSAGE_BUF_SZ 8192

#define FC_ONE "\xfc\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1"

/**
 * In order to easily tell the incoming connection requests from messages which
 * are addressed to a specific interface by its handle, the most significant bit
 * in the big endian representation of the handle shall be cleared to indicate
 * that a session is new and set otherwise.
 */
#define HANDLE_FLAG_BIT (0x80000000)
#define HANDLE_FLAG_BIT_be Endian_hostToBigEndian32(HANDLE_FLAG_BIT)

/*--------------------Prototypes--------------------*/
static int handleOutgoing(struct DHTMessage* message,
                          void* vcontext);

static inline uint8_t incomingDHT(struct Message* message,
                                  struct Address* addr,
                                  struct Ducttape_pvt* context)
{
    struct DHTMessage dht = {
        .address = addr,
        .binMessage = message,
        .allocator = message->alloc
    };

    DHTModuleRegistry_handleIncoming(&dht, context->registry);

    // TODO(cjd): return something meaningful.
    return Error_NONE;
}

/** Header must not be encrypted and must be aligned on the beginning of the ipv6 header. */
static inline uint8_t sendToRouter(struct Message* message,
                                   struct Ducttape_MessageHeader* dtHeader,
                                   struct SessionManager_Session* session,
                                   struct Ducttape_pvt* context)
{
    int safeDistance = Headers_SwitchHeader_SIZE;

    CryptoAuth_resetIfTimeout(session->internal);
    if (CryptoAuth_getState(session->internal) < CryptoAuth_HANDSHAKE3) {
        // Bug 104, see Version.h
        #ifdef Version_2_COMPAT
        if (session->version >= 3) {
        #endif
            // Put the handle into the message so that it's authenticated.
            // see: sendToSwitch()
            //Log_debug(context->logger, "Sending receive handle under CryptoAuth");
            Message_push(message, &session->receiveHandle_be, 4, NULL);
        #ifdef Version_2_COMPAT
        } else {
            // Otherwise it will be added on the other side.
            safeDistance += 4;
        }
        #endif

        safeDistance += Headers_CryptoAuth_SIZE;
    } else {
        // 16 for the authenticator, 4 for the nonce and 4 for the handle
        safeDistance += 24;
    }

    Message_shift(message, safeDistance, NULL);
    if (dtHeader->switchHeader) {
        if (message->bytes != (uint8_t*)dtHeader->switchHeader) {
            Bits_memmoveConst(message->bytes, dtHeader->switchHeader, Headers_SwitchHeader_SIZE);
            dtHeader->switchHeader = (struct Headers_SwitchHeader*) message->bytes;
        }
    } else {
        dtHeader->switchHeader = (struct Headers_SwitchHeader*) message->bytes;
        Bits_memset(dtHeader->switchHeader, 0, Headers_SwitchHeader_SIZE);
    }
    Message_shift(message, -safeDistance, NULL);

    dtHeader->switchHeader->label_be = Endian_hostToBigEndian64(dtHeader->switchLabel);


    // This comes out in outgoingFromCryptoAuth() then sendToSwitch()
    dtHeader->receiveHandle = Endian_bigEndianToHost32(session->receiveHandle_be);
    dtHeader->layer = Ducttape_SessionLayer_OUTER;
    return Interface_sendMessage(session->internal, message);
}

static struct Ducttape_MessageHeader* getDtHeader(struct Message* message, bool init)
{
    int padding = message->padding;
    Assert_true(padding > Ducttape_MessageHeader_SIZE);
    Message_shift(message, padding, NULL);
    struct Ducttape_MessageHeader* dtHeader = (struct Ducttape_MessageHeader*) message->bytes;
    Message_shift(message, -padding, NULL);
    if (init) {
        Bits_memset(dtHeader, 0, Ducttape_MessageHeader_SIZE);
        Identity_set(dtHeader);
    } else {
        Identity_check(dtHeader);
    }
    return dtHeader;
}

static int handleOutgoing(struct DHTMessage* dmessage,
                          void* vcontext)
{
    struct Ducttape_pvt* context = Identity_check((struct Ducttape_pvt*) vcontext);

    // Stub out all of the crypto code because setting up a CA session
    // with yourself causes problems.
    if (dmessage->address->path == 1) {
        struct Allocator* alloc = Allocator_child(context->alloc);
        Allocator_adopt(alloc, dmessage->binMessage->alloc);
        incomingDHT(dmessage->binMessage, dmessage->address, context);
        Allocator_free(alloc);
        return 0;
    }

    struct Message* msg = dmessage->binMessage;

    {
        Message_push(msg, (&(struct Headers_UDPHeader) {
            .srcPort_be = 0,
            .destPort_be = 0,
            .length_be = Endian_hostToBigEndian16(msg->length),
            .checksum_be = 0,
        }), Headers_UDPHeader_SIZE, NULL);
    }

    struct Headers_UDPHeader* uh = (struct Headers_UDPHeader*) msg->bytes;

    {
        struct Headers_IP6Header ip = {
            .versionClassAndFlowLabel = 0,
            .flowLabelLow_be = 0,
            .nextHeader = 17,
            .hopLimit = 0,
            .payloadLength_be = Endian_hostToBigEndian16(msg->length),
            .sourceAddr = {0}
        };
        Bits_memcpyConst(ip.sourceAddr,
                         context->myAddr.ip6.bytes,
                         Address_SEARCH_TARGET_SIZE);
        Bits_memcpyConst(ip.destinationAddr,
                         dmessage->address->ip6.bytes,
                         Address_SEARCH_TARGET_SIZE);
        Message_push(msg, &ip, Headers_IP6Header_SIZE, NULL);
    }

    struct Headers_IP6Header* ip = (struct Headers_IP6Header*) msg->bytes;

    Assert_true(!((uintptr_t)msg->bytes % 4) || !"alignment fault");

    uh->checksum_be = Checksum_udpIp6(ip->sourceAddr,
                                      (uint8_t*) uh,
                                      msg->length - Headers_IP6Header_SIZE);

    struct Ducttape_MessageHeader* dtHeader = getDtHeader(msg, true);
    dtHeader->ip6Header = ip;
    dtHeader->switchLabel = dmessage->address->path;

    struct SessionManager_Session* session =
        SessionManager_getSession(dmessage->address->ip6.bytes,
                                  dmessage->address->key,
                                  context->sm);

    session->version = dmessage->address->protocolVersion;
    Assert_true(session->version);

    sendToRouter(msg, dtHeader, session, context);

    return 0;
}

// Aligned on the beginning of the content.
static inline bool isRouterTraffic(struct Message* message, struct Headers_IP6Header* ip6)
{
    if (ip6->nextHeader != 17 || ip6->hopLimit != 0) {
        return false;
    }

    struct Headers_UDPHeader* uh = (struct Headers_UDPHeader*) message->bytes;
    return message->length >= Headers_UDPHeader_SIZE
        && uh->srcPort_be == 0
        && uh->destPort_be == 0
        && (int) Endian_bigEndianToHost16(uh->length_be) ==
            (message->length - Headers_UDPHeader_SIZE);
}

#define debugHandles(logger, session, message, ...) \
    do {                                                                               \
        uint8_t ip[40];                                                                \
        AddrTools_printIp(ip, session->ip6);                                           \
        Log_debug(logger, "ver[%u] send[%d] recv[%u] ip[%s] " message,                 \
                  session->version,                                                    \
                  Endian_hostToBigEndian32(session->sendHandle_be),                    \
                  Endian_hostToBigEndian32(session->receiveHandle_be),                 \
                  ip,                                                                  \
                  __VA_ARGS__);                                                        \
    } while (0)
//CHECKFILES_IGNORE expecting a ;

#define debugHandles0(logger, session, message) \
    debugHandles(logger, session, message "%s", "")

#define debugHandlesAndLabel(logger, session, label, message, ...) \
    do {                                                                               \
        uint8_t path[20];                                                              \
        AddrTools_printPath(path, label);                                              \
        debugHandles(logger, session, "path[%s] " message, path, __VA_ARGS__);         \
    } while (0)
//CHECKFILES_IGNORE expecting a ;

#define debugHandlesAndLabel0(logger, session, label, message) \
    debugHandlesAndLabel(logger, session, label, "%s", message)


/**
 * Message which is for us, message is aligned on the beginning of the content.
 * this is called from core() which calls through an interfaceMap.
 */
static inline uint8_t incomingForMe(struct Message* message,
                                    struct Ducttape_MessageHeader* dtHeader,
                                    struct SessionManager_Session* session,
                                    struct Ducttape_pvt* context,
                                    uint8_t herPublicKey[32])
{
    struct Address addr = { .protocolVersion = session->version };
    //Bits_memcpyConst(addr.ip6.bytes, session->ip6, 16);
    Bits_memcpyConst(addr.key, herPublicKey, 32);
    AddressCalc_addressForPublicKey(addr.ip6.bytes, herPublicKey);
    Assert_true(!Bits_memcmp(session->ip6, addr.ip6.bytes, 16));

    if (Bits_memcmp(addr.ip6.bytes, dtHeader->ip6Header->sourceAddr, 16)) {
        #ifdef Log_DEBUG
            uint8_t keyAddr[40];
            Address_printIp(keyAddr, &addr);
            Bits_memcpyConst(addr.ip6.bytes, dtHeader->ip6Header->sourceAddr, 16);
            uint8_t srcAddr[40];
            Address_printIp(srcAddr, &addr);
            Log_debug(context->logger,
                       "DROP packet because source address is not same as key.\n"
                       "    %s source addr\n"
                       "    %s hash of key\n",
                       srcAddr,
                       keyAddr);
        #endif
        return Error_INVALID;
    }

    if (isRouterTraffic(message, dtHeader->ip6Header)) {
        // Check the checksum.
        struct Headers_UDPHeader* uh = (struct Headers_UDPHeader*) message->bytes;

        if (Checksum_udpIp6(dtHeader->ip6Header->sourceAddr, (uint8_t*)uh, message->length)) {
            #ifdef Log_DEBUG
                uint8_t keyAddr[40];
                Address_printIp(keyAddr, &addr);
                Log_debug(context->logger,
                          "DROP Router packet with incorrect checksum, from [%s]", keyAddr);
            #endif
            return Error_INVALID;
        }

        // Shift off the UDP header.
        Message_shift(message, -Headers_UDPHeader_SIZE, NULL);
        addr.path = Endian_bigEndianToHost64(dtHeader->switchHeader->label_be);
        Bits_memcpyConst(addr.key, herPublicKey, 32);
        return incomingDHT(message, &addr, context);
    }

    if (!context->userIf) {
        Log_warn(context->logger, "DROP packet because there is no router interface configured");
        return Error_UNDELIVERABLE;
    }

    // prevent router advertizement schenanigans
    if (dtHeader->ip6Header->hopLimit == 255) {
        dtHeader->ip6Header->hopLimit--;
    }

    // Now write a message to the TUN device.
    // Need to move the ipv6 header forward up to the content because there's a crypto header
    // between the ipv6 header and the content which just got eaten.
    Message_shift(message, Headers_IP6Header_SIZE, NULL);
    uint16_t sizeDiff = message->bytes - (uint8_t*)dtHeader->ip6Header;
    if (sizeDiff) {
        dtHeader->ip6Header->payloadLength_be =
            Endian_hostToBigEndian16(
                Endian_bigEndianToHost16(dtHeader->ip6Header->payloadLength_be) - sizeDiff);
        Bits_memmoveConst(message->bytes, dtHeader->ip6Header, Headers_IP6Header_SIZE);
    }

    TUNMessageType_push(message, Ethernet_TYPE_IP6, NULL);

    context->userIf->sendMessage(message, context->userIf);
    return Error_NONE;
}

uint8_t Ducttape_injectIncomingForMe(struct Message* message,
                                     struct Ducttape* dt,
                                     uint8_t herPublicKey[32])
{
    struct Ducttape_pvt* context = Identity_check((struct Ducttape_pvt*)dt);
    struct Ducttape_MessageHeader* dtHeader = getDtHeader(message, true);
    struct Headers_SwitchHeader sh;
    Bits_memcpyConst(&sh, message->bytes, Headers_SwitchHeader_SIZE);
    dtHeader->switchHeader = &sh;
    Message_shift(message, -Headers_SwitchHeader_SIZE, NULL);

    struct Headers_IP6Header ip6;
    Bits_memcpyConst(&ip6, message->bytes, Headers_IP6Header_SIZE);
    dtHeader->ip6Header = &ip6;
    Message_shift(message, -Headers_IP6Header_SIZE, NULL);

    struct SessionManager_Session s;
    AddressCalc_addressForPublicKey(s.ip6, herPublicKey);
    s.version = Version_CURRENT_PROTOCOL;

    return incomingForMe(message, dtHeader, &s, context, herPublicKey);
}

/**
 * Send a message to another switch.
 * Switchheader will precede the message.
 */
static inline uint8_t sendToSwitch(struct Message* message,
                                   struct Ducttape_MessageHeader* dtHeader,
                                   struct SessionManager_Session* session,
                                   struct Ducttape_pvt* context)
{
    uint64_t label = dtHeader->switchLabel;

    CryptoAuth_resetIfTimeout(session->internal);
    if (CryptoAuth_getState(session->internal) >= CryptoAuth_HANDSHAKE3) {
        //debugHandlesAndLabel0(context->logger, session, label, "layer2 sending run message");
        uint32_t sendHandle_be = session->sendHandle_be;
        #ifdef Version_2_COMPAT
        if (session->version < 3) {
            sendHandle_be |= HANDLE_FLAG_BIT_be;
        }
        #endif
        Message_push(message, &sendHandle_be, 4, NULL);
    } else {
        debugHandlesAndLabel0(context->logger, session, label, "layer2 sending start message");
        #ifdef Version_2_COMPAT
        if (session->version < 3) {
            Message_push(message, &session->receiveHandle_be, 4, NULL);
        }
        #endif
    }

    Message_shift(message, Headers_SwitchHeader_SIZE, NULL);

    Assert_true(message->bytes == (uint8_t*)dtHeader->switchHeader);

    return context->switchInterface.receiveMessage(message, &context->switchInterface);
}

static inline bool validEncryptedIP6(struct Message* message)
{
    struct Headers_IP6Header* header = (struct Headers_IP6Header*) message->bytes;
    // Empty ipv6 headers are tolerated at this stage but dropped later.
    return message->length >= Headers_IP6Header_SIZE
        && AddressCalc_validAddress(header->sourceAddr)
        && AddressCalc_validAddress(header->destinationAddr);
}

static inline bool isForMe(struct Message* message, struct Ducttape_pvt* context)
{
    struct Headers_IP6Header* header = (struct Headers_IP6Header*) message->bytes;
    return (Bits_memcmp(header->destinationAddr, context->myAddr.ip6.bytes, 16) == 0);
}

static uint8_t magicInterfaceSendMessage(struct Message* msg, struct Interface* iface)
{
    struct Ducttape_pvt* ctx =
        Identity_check((struct Ducttape_pvt*)
            &((uint8_t*)iface)[-offsetof(struct Ducttape, magicInterface)]);

    Assert_ifParanoid(msg->length >= Headers_IP6Header_SIZE);
    #ifdef PARANOIA
        struct Headers_IP6Header* header = (struct Headers_IP6Header*) msg->bytes;
        Assert_ifParanoid(!Bits_memcmp(header->destinationAddr, ctx->myAddr.ip6.bytes, 16));
        Assert_ifParanoid(!Bits_memcmp(header->sourceAddr, FC_ONE, 16));
    #endif

    TUNMessageType_push(msg, Ethernet_TYPE_IP6, NULL);

    if (ctx->userIf) {
        return Interface_sendMessage(ctx->userIf, msg);
    }
    return Error_NONE;
}

// Called by the TUN device.
static inline uint8_t incomingFromTun(struct Message* message,
                                      struct Interface* iface)
{
    struct Ducttape_pvt* context = Identity_check((struct Ducttape_pvt*) iface->receiverContext);

    uint16_t ethertype = TUNMessageType_pop(message, NULL);

    struct Headers_IP6Header* header = (struct Headers_IP6Header*) message->bytes;

    int version = Headers_getIpVersion(message->bytes);
    if ((ethertype == Ethernet_TYPE_IP4 && version != 4)
        || (ethertype == Ethernet_TYPE_IP6 && version != 6))
    {
        Log_warn(context->logger, "DROP packet because ip version [%d] "
                 "doesn't match ethertype [%u].", version, Endian_bigEndianToHost16(ethertype));
        return Error_INVALID;
    }

    if (ethertype != Ethernet_TYPE_IP6 || !AddressCalc_validAddress(header->sourceAddr)) {
        return context->ipTunnel->tunInterface.sendMessage(message,
                                                           &context->ipTunnel->tunInterface);
    } else if (!AddressCalc_validAddress(header->destinationAddr)) {
        #ifdef Log_INFO
            uint8_t dst[40];
            AddrTools_printIp(dst, header->destinationAddr);
            Log_warn(context->logger, "DROP packet to [%s] because it must begin with fc", dst);
        #endif
        return Error_INVALID;
    }

    if (Bits_memcmp(header->sourceAddr, context->myAddr.ip6.bytes, 16)) {
        uint8_t expectedSource[40];
        AddrTools_printIp(expectedSource, context->myAddr.ip6.bytes);
        uint8_t packetSource[40];
        AddrTools_printIp(packetSource, header->sourceAddr);
        Log_warn(context->logger,
                 "DROP packet from [%s] because all messages must have source address [%s]",
                 (char*) packetSource, (char*) expectedSource);
        return Error_INVALID;
    }
    if (!Bits_memcmp(header->destinationAddr, context->myAddr.ip6.bytes, 16)) {
        // I'm Gonna Sit Right Down and Write Myself a Letter
        TUNMessageType_push(message, ethertype, NULL);
        iface->sendMessage(message, iface);
        return Error_NONE;
    }

    if (!Bits_memcmp(header->destinationAddr, FC_ONE, 16)) {
        return Interface_receiveMessage(&context->pub.magicInterface, message);
    }

    struct Ducttape_MessageHeader* dtHeader = getDtHeader(message, true);
    struct Node_Two* bestNext = RouterModule_lookup(header->destinationAddr, context->routerModule);
    struct SessionManager_Session* nextHopSession;
    if (bestNext) {
        nextHopSession = SessionManager_getSession(bestNext->address.ip6.bytes,
                                                   bestNext->address.key,
                                                   context->sm);

        bestNext->address.protocolVersion = nextHopSession->version =
            (bestNext->address.protocolVersion > nextHopSession->version)
                ? bestNext->address.protocolVersion : nextHopSession->version;

        dtHeader->switchLabel = bestNext->address.path;
        dtHeader->nextHopReceiveHandle = Endian_bigEndianToHost32(nextHopSession->receiveHandle_be);

        if (!Bits_memcmp(header->destinationAddr, bestNext->address.ip6.bytes, 16)) {
            // Direct send, skip the innermost layer of encryption.
            /*#ifdef Log_DEBUG
                uint8_t nhAddr[60];
                Address_print(nhAddr, &bestNext->address);
                Log_debug(context->logger, "Forwarding data to %s (last hop)\n", nhAddr);
            #endif*/
            return sendToRouter(message, dtHeader, nextHopSession, context);
        }
        // else { the message will need to be 3 layer encrypted but since we already did a lookup
        // of the best node to forward to, we can skip doing another lookup by storing a pointer
        // to that node in the context (bestNext).
    } else {
        #ifdef Log_WARN
            uint8_t thisAddr[40];
            uint8_t destAddr[40];
            AddrTools_printIp(thisAddr, context->myAddr.ip6.bytes);
            AddrTools_printIp(destAddr, header->destinationAddr);
            Log_warn(context->logger,
                     "DROP message from TUN because this node [%s] is closest to dest [%s]",
                     thisAddr, destAddr);
        #endif
        return Error_UNDELIVERABLE;
    }
/*
    #ifdef Log_DEBUG
        uint8_t destAddr[40];
        AddrTools_printIp(destAddr, header->destinationAddr);
        uint8_t nhAddr[60];
        Address_print(nhAddr, &bestNext->address);
        Log_debug(context->logger, "Sending to [%s] via [%s]", destAddr, nhAddr);
    #endif
*/
    struct SessionManager_Session* session =
        SessionManager_getSession(header->destinationAddr, NULL, context->sm);

    // Copy the IP6 header back from where the CA header will be placed.
    // this is a mess.
    // We can't just copy the header to a safe place because the CryptoAuth
    // might buffer the message and send a connect-to-me packet and when the
    // hello packet comes in return, the CA will send the message and the header
    // needs to be in the message buffer.
    //
    // The CryptoAuth may send a 120 byte CA header and it might only send a 4 byte
    // nonce and 16 byte authenticator depending on its state.

    CryptoAuth_resetIfTimeout(session->internal);
    if (CryptoAuth_getState(session->internal) < CryptoAuth_HANDSHAKE3) {
        // shift, copy, shift because shifting asserts that there is enough buffer space.
        Message_shift(message, Headers_CryptoAuth_SIZE + 4, NULL);
        Bits_memcpyConst(message->bytes, header, Headers_IP6Header_SIZE);
        Message_shift(message, -(Headers_IP6Header_SIZE + Headers_CryptoAuth_SIZE + 4), NULL);
        // now push the receive handle *under* the CA header.
        Message_push(message, &session->receiveHandle_be, 4, NULL);
        debugHandles0(context->logger, session, "layer3 sending start message");
    } else {
        // shift, copy, shift because shifting asserts that there is enough buffer space.
        Message_shift(message, 20, NULL);
        Bits_memmoveConst(message->bytes, header, Headers_IP6Header_SIZE);
        Message_shift(message, -(20 + Headers_IP6Header_SIZE), NULL);
        debugHandles0(context->logger, session, "layer3 sending run message");
    }

    // This comes out at outgoingFromCryptoAuth() then outgoingFromMe()
    dtHeader->receiveHandle = Endian_bigEndianToHost32(session->receiveHandle_be);
    dtHeader->layer = Ducttape_SessionLayer_INNER;
    return Interface_sendMessage(session->internal, message);
}

/**
 * Send an arbitrary message to a node.
 *
 * @param message to be sent, must be prefixed with IpTunnel_PacketInfoHeader.
 * @param iface an interface for which receiverContext is the ducttape.
 */
static uint8_t sendToNode(struct Message* message, struct Interface* iface)
{
    struct Ducttape_pvt* context = Identity_check((struct Ducttape_pvt*)iface->receiverContext);
    struct Ducttape_MessageHeader* dtHeader = getDtHeader(message, true);
    struct IpTunnel_PacketInfoHeader* header = (struct IpTunnel_PacketInfoHeader*) message->bytes;
    Message_shift(message, -IpTunnel_PacketInfoHeader_SIZE, NULL);
    struct Node_Two* n = RouterModule_lookup(header->nodeIp6Addr, context->routerModule);
    if (n) {
        if (!Bits_memcmp(header->nodeKey, n->address.key, 32)) {
            // Found the node.
            /* noisy
            #ifdef Log_DEBUG
                uint8_t nhAddr[60];
                Address_print(nhAddr, &n->address);
                Log_debug(context->logger, "Sending arbitrary data to [%s]", nhAddr);
            #endif*/

            struct SessionManager_Session* session =
                SessionManager_getSession(n->address.ip6.bytes, n->address.key, context->sm);

            n->address.protocolVersion = session->version =
                (n->address.protocolVersion > session->version)
                    ? n->address.protocolVersion : session->version;

            dtHeader->switchLabel = n->address.path;
            return sendToRouter(message, dtHeader, session, context);
        }
    }

    #ifdef Log_DEBUG
        uint8_t printedIp6[40];
        AddrTools_printIp(printedIp6, header->nodeIp6Addr);
        Log_debug(context->logger, "DROP Couldn't find node [%s] for sending to.", printedIp6);
    #endif

    // Now lets trigger a search for this node.
    uint64_t now = Time_currentTimeMilliseconds(context->eventBase);
    if (context->timeOfLastSearch + context->timeBetweenSearches < now) {
        context->timeOfLastSearch = now;
        SearchRunner_search(header->nodeIp6Addr, context->searchRunner, context->alloc);
    }
    return 0;
}

/**
 * Send an arbitrary message to the tun device.
 *
 * @param message to be sent.
 * @param iface an interface for which receiverContext is the ducttape.
 */
static uint8_t sendToTun(struct Message* message, struct Interface* iface)
{
    struct Ducttape_pvt* context = Identity_check((struct Ducttape_pvt*)iface->receiverContext);
    uint16_t msgType = TUNMessageType_pop(message, NULL);
    if (msgType == Ethernet_TYPE_IP6) {
        Assert_true(message->length >= Headers_IP6Header_SIZE);
        struct Headers_IP6Header* header = (struct Headers_IP6Header*) message->bytes;
        if (header->sourceAddr[0] == 0xfc || header->destinationAddr[0] == 0xfc) {
            Assert_failure("you can't do that");
        }
    }
    TUNMessageType_push(message, msgType, NULL);
    if (context->userIf) {
        return context->userIf->sendMessage(message, context->userIf);
    }
    return 0;
}

/**
 * Messages with content encrypted and header decrypted are sent here to be forwarded.
 * they may come from us, or from another node and may be to us or to any other node.
 * Message is aligned on the beginning of the ipv6 header.
 */
static inline int core(struct Message* message,
                       struct Ducttape_MessageHeader* dtHeader,
                       struct SessionManager_Session* session,
                       struct Ducttape_pvt* context)
{
    struct Headers_IP6Header* ip6Header = (struct Headers_IP6Header*) message->bytes;
    dtHeader->ip6Header = ip6Header;

    if (isForMe(message, context)) {
        Message_shift(message, -Headers_IP6Header_SIZE, NULL);

        if (Bits_memcmp(session->ip6, ip6Header->sourceAddr, 16)) {
            // triple encrypted
            // This call goes to incomingForMe()
            struct SessionManager_Session* session =
                SessionManager_getSession(ip6Header->sourceAddr, NULL, context->sm);

            #ifdef Log_DEBUG
                uint8_t addr[40];
                AddrTools_printIp(addr, ip6Header->sourceAddr);
                Log_debug(context->logger, "Incoming layer3 message, ostensibly from [%s]", addr);
            #endif

            dtHeader->receiveHandle = Endian_bigEndianToHost32(session->receiveHandle_be);
            dtHeader->layer = Ducttape_SessionLayer_INNER;
            return Interface_receiveMessage(&session->external, message);
        } else {
            // double encrypted, inner layer plaintext.
            // The session is still set from the router-to-router traffic and that is the one we use
            // to determine the node's id.
            return incomingForMe(message, dtHeader, session, context,
                                 CryptoAuth_getHerPublicKey(session->internal));
        }
    }

    if (ip6Header->hopLimit == 0) {
        Log_debug(context->logger, "DROP message because hop limit has been exceeded.\n");
        // TODO(cjd): send back an error message in response.
        return Error_UNDELIVERABLE;
    }
    ip6Header->hopLimit--;

    struct SessionManager_Session* nextHopSession = NULL;
    if (!dtHeader->nextHopReceiveHandle || !dtHeader->switchLabel) {
        struct Node_Two* n = RouterModule_lookup(ip6Header->destinationAddr, context->routerModule);
        if (n) {
            nextHopSession =
                SessionManager_getSession(n->address.ip6.bytes, n->address.key, context->sm);
            dtHeader->switchLabel = n->address.path;
        }
    } else {
        nextHopSession =
            SessionManager_sessionForHandle(dtHeader->nextHopReceiveHandle, context->sm);
    }

    if (nextHopSession) {
        #ifdef Log_DEBUG
            struct Address addr;
            Bits_memcpyConst(addr.ip6.bytes, nextHopSession->ip6, 16);
            addr.path = dtHeader->switchLabel;
            uint8_t nhAddr[60];
            Address_print(nhAddr, &addr);
            if (Bits_memcmp(ip6Header->destinationAddr, addr.ip6.bytes, 16)) {
                // Potentially forwarding for ourselves.
                struct Address destination;
                Bits_memcpyConst(destination.ip6.bytes, ip6Header->destinationAddr, 16);
                uint8_t ipAddr[40];
                Address_printIp(ipAddr, &destination);
                Log_debug(context->logger, "Forwarding data to %s via %s\n", ipAddr, nhAddr);
            } else {
                // Definitely forwarding on behalf of someone else.
                //Log_debug(context->logger, "Forwarding data to %s (last hop)\n", nhAddr);
            }
        #endif
        return sendToRouter(message, dtHeader, nextHopSession, context);
    }

    #ifdef Log_INFO
        struct Address destination;
        Bits_memcpyConst(destination.ip6.bytes, ip6Header->destinationAddr, 16);
        uint8_t ipAddr[40];
        Address_printIp(ipAddr, &destination);
        Log_info(context->logger, "DROP message because this node is the closest known "
                                   "node to the destination %s.", ipAddr);
    #endif
    return Error_UNDELIVERABLE;
}

/**
 * When we send a message it goes into the CryptoAuth.
 * for the content level crypto then it goes to outgoingFromCryptoAuth then comes here.
 * Message is aligned on the beginning of the CryptoAuth header.
 */
static inline uint8_t outgoingFromMe(struct Message* message,
                                     struct Ducttape_MessageHeader* dtHeader,
                                     struct SessionManager_Session* session,
                                     struct Ducttape_pvt* context)
{
    // Move back to the beginning of the ip6Header behind the crypto.
    Message_shift(message, Headers_IP6Header_SIZE, NULL);
    struct Headers_IP6Header* header = (struct Headers_IP6Header*) message->bytes;

    if (!Bits_memcmp(header->destinationAddr, context->myAddr.ip6.bytes, 16)) {
        // This happens when an empty connect-to-me packet is sent to us,
        // CryptoAuth is called with a message and instead of returning a decrypted message
        // to send to the TUN, it outputs a message to send back down the wire but the
        // header is still the same.
        // these messages are always empty so we just flip the source and destination around
        // and send it back.
        Bits_memcpyConst(header->destinationAddr, header->sourceAddr, 16);
        Bits_memcpyConst(header->sourceAddr, context->myAddr.ip6.bytes, 16);

    } else {
        // sanity check.
        Assert_ifParanoid(!Bits_memcmp(header->sourceAddr, context->myAddr.ip6.bytes, 16));
    }

    // Need to set the length field to take into account
    // the crypto headers which are hidden under the ipv6 packet.
    header->payloadLength_be =
        Endian_hostToBigEndian16(message->length - Headers_IP6Header_SIZE);


    // Forward this call to core() which will check its validity
    // and since it's not to us, forward it to the correct node.
    return core(message, dtHeader, session, context);
}

static inline int incomingFromRouter(struct Message* message,
                                     struct Ducttape_MessageHeader* dtHeader,
                                     struct SessionManager_Session* session,
                                     struct Ducttape_pvt* context)
{
    uint8_t* pubKey = CryptoAuth_getHerPublicKey(session->internal);
    if (!validEncryptedIP6(message)) {
        // Not valid cjdns IPv6, we'll try it as an IPv4 or ICANN-IPv6 packet
        // and check if we have an agreement with the node who sent it.
        Message_shift(message, IpTunnel_PacketInfoHeader_SIZE, NULL);
        struct IpTunnel_PacketInfoHeader* header =
            (struct IpTunnel_PacketInfoHeader*) message->bytes;

        uint8_t* addr = session->ip6;
        Bits_memcpyConst(header->nodeIp6Addr, addr, 16);
        Bits_memcpyConst(header->nodeKey, pubKey, 32);

        struct Interface* ipTun = &context->ipTunnel->nodeInterface;
        return ipTun->sendMessage(message, ipTun);
    }

    struct Address srcAddr = {
        .path = Endian_bigEndianToHost64(dtHeader->switchHeader->label_be)
    };
    Bits_memcpyConst(srcAddr.key, pubKey, 32);

    //Log_debug(context->logger, "Got message from router.\n");
    return core(message, dtHeader, session, context);
}


static uint8_t incomingFromCryptoAuth(struct Message* message, struct Interface* iface)
{
    struct Ducttape_pvt* context = Identity_check((struct Ducttape_pvt*) iface->receiverContext);
    struct Ducttape_MessageHeader* dtHeader = getDtHeader(message, false);
    enum Ducttape_SessionLayer layer = dtHeader->layer;
    dtHeader->layer = Ducttape_SessionLayer_INVALID;
    struct SessionManager_Session* session =
        SessionManager_sessionForHandle(dtHeader->receiveHandle, context->sm);

    if (!session) {
        // This should never happen but there's no strong preventitive.
        Log_info(context->logger, "SESSION DISAPPEARED!");
        return 0;
    }

    // If the packet came from a new session, put the send handle in the session.
    if (CryptoAuth_getState(session->internal) < CryptoAuth_ESTABLISHED) {
        // If this is true then the incoming message is definitely a handshake.
        if (message->length < 4) {
            debugHandles0(context->logger, session, "runt");
            return Error_INVALID;
        }
        if (layer == Ducttape_SessionLayer_OUTER) {
            #ifdef Version_2_COMPAT
            if (dtHeader->currentSessionVersion >= 3) {
                session->version = dtHeader->currentSessionVersion;
            #endif
                Message_pop(message, &session->sendHandle_be, 4, NULL);
            #ifdef Version_2_COMPAT
            } else {
                session->sendHandle_be = dtHeader->currentSessionSendHandle_be;
            }
            #endif
        } else {
            // inner layer, always grab the handle
            Message_pop(message, &session->sendHandle_be, 4, NULL);
            debugHandles0(context->logger, session, "New session, incoming layer3");
        }
    }

    switch (layer) {
        case Ducttape_SessionLayer_OUTER:
            return incomingFromRouter(message, dtHeader, session, context);
        case Ducttape_SessionLayer_INNER:
            return incomingForMe(message, dtHeader, session, context,
                                 CryptoAuth_getHerPublicKey(session->internal));
        default:
            Assert_true(false);
    }
    // never reached.
    return 0;
}

static uint8_t outgoingFromCryptoAuth(struct Message* message, struct Interface* iface)
{
    struct Ducttape_pvt* context = Identity_check((struct Ducttape_pvt*) iface->senderContext);
    struct Ducttape_MessageHeader* dtHeader = getDtHeader(message, false);
    struct SessionManager_Session* session =
        SessionManager_sessionForHandle(dtHeader->receiveHandle, context->sm);

    enum Ducttape_SessionLayer layer = dtHeader->layer;
    dtHeader->layer = Ducttape_SessionLayer_INVALID;

    if (!session) {
        // This should never happen but there's no strong preventitive.
        Log_info(context->logger, "SESSION DISAPPEARED!");
        return 0;
    }

    if (layer == Ducttape_SessionLayer_OUTER) {
        return sendToSwitch(message, dtHeader, session, context);
    } else if (layer == Ducttape_SessionLayer_INNER) {
        Log_debug(context->logger, "Sending layer3 message");
        return outgoingFromMe(message, dtHeader, session, context);
    } else {
        Assert_true(0);
    }
}

/**
 * Handle an incoming control message from a switch.
 *
 * @param context the ducttape context.
 * @param message the control message, this should be alligned on the beginning of the content,
 *                that is to say, after the end of the switch header.
 * @param switchHeader the header.
 * @param switchIf the interface which leads to the switch.
 */
static uint8_t handleControlMessage(struct Ducttape_pvt* context,
                                    struct Message* message,
                                    struct Headers_SwitchHeader* switchHeader,
                                    struct Interface* switchIf)
{
    uint8_t labelStr[20];
    uint64_t label = Endian_bigEndianToHost64(switchHeader->label_be);
    AddrTools_printPath(labelStr, label);
    if (message->length < Control_HEADER_SIZE) {
        Log_info(context->logger, "DROP runt ctrl packet from [%s]", labelStr);
        return Error_NONE;
    }
    struct Control* ctrl = (struct Control*) message->bytes;

    if (Checksum_engine(message->bytes, message->length)) {
        Log_info(context->logger, "DROP ctrl packet from [%s] with invalid checksum.", labelStr);
        return Error_NONE;
    }

    bool pong = false;
    if (ctrl->type_be == Control_ERROR_be) {
        if (message->length < Control_Error_MIN_SIZE) {
            Log_info(context->logger, "DROP runt error packet from [%s]", labelStr);
            return Error_NONE;
        }

        uint64_t path = Endian_bigEndianToHost64(switchHeader->label_be);
        RouterModule_brokenPath(path, context->routerModule);

        uint8_t causeType = Headers_getMessageType(&ctrl->content.error.cause);
        if (causeType == Headers_SwitchHeader_TYPE_CONTROL) {
            if (message->length < Control_Error_MIN_SIZE + Control_HEADER_SIZE) {
                Log_info(context->logger,
                          "error packet from [%s] containing runt cause packet",
                          labelStr);
                return Error_NONE;
            }
            struct Control* causeCtrl = (struct Control*) &(&ctrl->content.error.cause)[1];
            if (causeCtrl->type_be != Control_PING_be) {
                #ifdef Log_INFO
                    uint32_t errorType =
                        Endian_bigEndianToHost32(ctrl->content.error.errorType_be);
                    Log_info(context->logger,
                              "error packet from [%s] caused by [%s] packet ([%s])",
                              labelStr,
                              Control_typeString(causeCtrl->type_be),
                              Error_strerror(errorType));
                #endif
            } else {
                if (LabelSplicer_isOneHop(label)
                    && ctrl->content.error.errorType_be
                        == Endian_hostToBigEndian32(Error_UNDELIVERABLE))
                {
                    // this is our own InterfaceController complaining
                    // because the node isn't responding to pings.
                    return Error_NONE;
                }
                Log_debug(context->logger,
                           "error packet from [%s] in response to ping, err [%u], length: [%u].",
                           labelStr,
                           Endian_bigEndianToHost32(ctrl->content.error.errorType_be),
                           message->length);
                // errors resulting from pings are forwarded back to the pinger.
                pong = true;
            }
        } else if (causeType != Headers_SwitchHeader_TYPE_DATA) {
            Log_info(context->logger,
                      "error packet from [%s] containing cause of unknown type [%u]",
                      labelStr, causeType);
        } else {
            uint32_t errorType = Endian_bigEndianToHost32(ctrl->content.error.errorType_be);
            if (errorType != Error_RETURN_PATH_INVALID) {
                // Error_RETURN_PATH_INVALID is impossible to prevent so will appear all the time.
                Log_info(context->logger,
                         "error packet from [%s] [%s]",
                         labelStr,
                         Error_strerror(errorType));
            }
        }
    } else if (ctrl->type_be == Control_PONG_be) {
        pong = true;
    } else if (ctrl->type_be == Control_PING_be) {

        Message_shift(message, -Control_HEADER_SIZE, NULL);

        if (message->length < Control_Ping_MIN_SIZE) {
            Log_info(context->logger, "DROP runt ping");
            return Error_INVALID;
        }
        struct Control_Ping* ping = (struct Control_Ping*) message->bytes;
        ping->magic = Control_Pong_MAGIC;
        ping->version_be = Endian_hostToBigEndian32(Version_CURRENT_PROTOCOL);
        Message_shift(message, Control_HEADER_SIZE, NULL);

        ctrl->type_be = Control_PONG_be;
        ctrl->checksum_be = 0;
        ctrl->checksum_be = Checksum_engine(message->bytes, message->length);
        Message_shift(message, Headers_SwitchHeader_SIZE, NULL);
        Log_debug(context->logger, "got switch ping from [%s]", labelStr);
        switchIf->receiveMessage(message, switchIf);

    } else if (ctrl->type_be == Control_KEYPONG_be) {
        pong = true;
    } else if (ctrl->type_be == Control_KEYPING_be) {

        Message_shift(message, -Control_HEADER_SIZE, NULL);

        if (message->length < Control_KeyPing_MIN_SIZE) {
            Log_info(context->logger, "DROP runt keyping");
            return Error_INVALID;
        }

        struct Control_KeyPing* keyPing = (struct Control_KeyPing*) message->bytes;

        #ifdef Log_DEBUG
            struct Address herAddr = {
                .protocolVersion = Endian_bigEndianToHost32(keyPing->version_be),
                .path = label
            };
            Bits_memcpyConst(herAddr.key, keyPing->key, 32);
            String* addrStr = Address_toString(&herAddr, message->alloc);
            Log_debug(context->logger, "got switch keyPing from [%s]", addrStr->bytes);
        #endif

        if (message->length > Control_KeyPing_MIN_SIZE + 64) {
            Log_debug(context->logger, "DROP oversize keyping message");
            return Error_INVALID;
        }

        keyPing->magic = Control_KeyPong_MAGIC;
        keyPing->version_be = Endian_hostToBigEndian32(Version_CURRENT_PROTOCOL);
        Bits_memcpyConst(keyPing->key, context->myAddr.key, 32);
        Message_shift(message, Control_HEADER_SIZE, NULL);

        ctrl->type_be = Control_KEYPONG_be;
        ctrl->checksum_be = 0;
        ctrl->checksum_be = Checksum_engine(message->bytes, message->length);
        Message_shift(message, Headers_SwitchHeader_SIZE, NULL);

        Interface_receiveMessage(switchIf, message);

    } else {
        Log_info(context->logger,
                  "control packet of unknown type from [%s], type [%d]",
                  labelStr, Endian_bigEndianToHost16(ctrl->type_be));
    }

    if (pong && context->pub.switchPingerIf.receiveMessage) {
        // Shift back over the header
        Message_shift(message, Headers_SwitchHeader_SIZE, NULL);
        context->pub.switchPingerIf.receiveMessage(
            message, &context->pub.switchPingerIf);
    }
    return Error_NONE;
}

#ifdef Version_2_COMPAT
static inline void translateVersion2(struct Message* message,
                                     struct Ducttape_MessageHeader* dtHeader)
{
    uint32_t handle = Endian_bigEndianToHost32(((uint32_t*)message->bytes)[0]);
    uint32_t nonce = Endian_bigEndianToHost32(((uint32_t*)message->bytes)[1]);
    dtHeader->currentSessionVersion = 2;
    if (handle & HANDLE_FLAG_BIT) {
        // We have to doctor their handles to make them conform to the new protocol.
        // see sendToSwitch() where they are un-doctored when being sent back.
        handle &= ~HANDLE_FLAG_BIT;
        ((uint32_t*)message->bytes)[0] = Endian_bigEndianToHost32(handle);
        return;
    }
    // This has a 4 / 4294967296 risk of a false positive, losing a traffic packet
    // between 2 version2 nodes because the first 4 bytes of the content are mistaken
    // for a nonce.
    if (nonce <= 3) {
        dtHeader->currentSessionSendHandle_be = Endian_bigEndianToHost32(handle);
        Message_shift(message, -4, NULL);
        return;
    }
    dtHeader->currentSessionVersion = 3;
}
#endif

/**
 * This is called as sendMessage() by the switch.
 * There is only one switch interface which sends all traffic.
 * message is aligned on the beginning of the switch header.
 */
static uint8_t incomingFromSwitch(struct Message* message, struct Interface* switchIf)
{
    struct Ducttape_pvt* context = Identity_check((struct Ducttape_pvt*)switchIf->senderContext);

    struct Ducttape_MessageHeader* dtHeader = getDtHeader(message, true);

    struct Headers_SwitchHeader* switchHeader = (struct Headers_SwitchHeader*) message->bytes;
    Message_shift(message, -Headers_SwitchHeader_SIZE, NULL);

    // The label comes in reversed from the switch because the switch doesn't know that we aren't
    // another switch ready to parse more bits, bit reversing the label yields the source address.
    switchHeader->label_be = Bits_bitReverse64(switchHeader->label_be);

    if (Headers_getMessageType(switchHeader) == Headers_SwitchHeader_TYPE_CONTROL) {
        return handleControlMessage(context, message, switchHeader, switchIf);
    }

    if (message->length < 8) {
        Log_info(context->logger, "runt");
        return Error_INVALID;
    }

    #ifdef Version_2_COMPAT
    translateVersion2(message, dtHeader);
    #endif

    // #1 try to get the session using the handle.

    uint32_t nonceOrHandle = Endian_bigEndianToHost32(((uint32_t*)message->bytes)[0]);

    struct SessionManager_Session* session = NULL;

    if (nonceOrHandle > 3) {
        // Run message, it's a handle.
        session = SessionManager_sessionForHandle(nonceOrHandle, context->sm);
        Message_shift(message, -4, NULL);
        if (session) {
            uint32_t nonce = Endian_bigEndianToHost32(((uint32_t*)message->bytes)[0]);
            if (nonce == ~0u) {
                Log_debug(context->logger, "DROP connectToMe packet at switch layer");
                return 0;
            }
            /*
            debugHandlesAndLabel(context->logger, session,
                                 Endian_bigEndianToHost64(switchHeader->label_be),
                                 "running session nonce[%u]",
                                 nonce);
            */
            dtHeader->receiveHandle = nonceOrHandle;
        } else {
            Log_debug(context->logger, "Got message with unrecognized handle");
        }
    } else if (message->length >= Headers_CryptoAuth_SIZE) {
        union Headers_CryptoAuth* caHeader = (union Headers_CryptoAuth*) message->bytes;
        uint8_t ip6[16];
        uint8_t* herKey = caHeader->handshake.publicKey;
        AddressCalc_addressForPublicKey(ip6, herKey);
        // a packet which claims to be "from us" causes problems
        if (AddressCalc_validAddress(ip6) && Bits_memcmp(ip6, &context->myAddr, 16)) {
            session = SessionManager_getSession(ip6, herKey, context->sm);
            debugHandlesAndLabel(context->logger, session,
                                 Endian_bigEndianToHost64(switchHeader->label_be),
                                 "new session nonce[%d]", nonceOrHandle);
            dtHeader->receiveHandle = Endian_bigEndianToHost32(session->receiveHandle_be);
        } else {
            Log_debug(context->logger, "Got message with invalid ip addr");
        }
    }

    if (!session) {
        #ifdef Log_INFO
            uint8_t path[20];
            AddrTools_printPath(path, Endian_bigEndianToHost64(switchHeader->label_be));
            Log_info(context->logger, "DROP traffic packet from unknown node. [%s]", path);
        #endif
        return 0;
    }

    // This is needed so that the priority and other information
    // from the switch header can be passed on properly.
    dtHeader->switchHeader = switchHeader;

    // This goes to incomingFromCryptoAuth()
    // then incomingFromRouter() then core()
    dtHeader->layer = Ducttape_SessionLayer_OUTER;

    if (Interface_receiveMessage(&session->external, message) == Error_AUTHENTICATION) {
        debugHandlesAndLabel(context->logger, session,
                             Endian_bigEndianToHost64(switchHeader->label_be),
                             "DROP Failed decrypting message NoH[%d] state[%s]",
                             nonceOrHandle,
                             CryptoAuth_stateString(CryptoAuth_getState(session->internal)));
        return Error_AUTHENTICATION;
    }

    return 0;
}

static uint8_t incomingFromPinger(struct Message* message, struct Interface* iface)
{
    struct Ducttape_pvt* context = Identity_check((struct Ducttape_pvt*)iface->senderContext);
    return context->switchInterface.receiveMessage(message, &context->switchInterface);
}

struct Ducttape* Ducttape_register(uint8_t privateKey[32],
                                   struct DHTModuleRegistry* registry,
                                   struct RouterModule* routerModule,
                                   struct SearchRunner* searchRunner,
                                   struct SwitchCore* switchCore,
                                   struct EventBase* eventBase,
                                   struct Allocator* allocator,
                                   struct Log* logger,
                                   struct IpTunnel* ipTun,
                                   struct Random* rand)
{
    struct Ducttape_pvt* context = Allocator_calloc(allocator, sizeof(struct Ducttape_pvt), 1);
    context->registry = registry;
    context->routerModule = routerModule;
    context->logger = logger;
    context->eventBase = eventBase;
    context->alloc = allocator;
    context->searchRunner = searchRunner;
    Bits_memcpyConst(&context->pub.magicInterface, (&(struct Interface) {
        .sendMessage = magicInterfaceSendMessage,
        .allocator = allocator
    }), sizeof(struct Interface));
    Identity_set(context);

    context->ipTunnel = ipTun;

    ipTun->nodeInterface.receiveMessage = sendToNode;
    ipTun->nodeInterface.receiverContext = context;
    ipTun->tunInterface.receiveMessage = sendToTun;
    ipTun->tunInterface.receiverContext = context;

    struct CryptoAuth* cryptoAuth =
        CryptoAuth_new(allocator, privateKey, eventBase, logger, rand);
    Bits_memcpyConst(context->myAddr.key, cryptoAuth->publicKey, 32);
    Address_getPrefix(&context->myAddr);

    context->sm = SessionManager_new(incomingFromCryptoAuth,
                                     outgoingFromCryptoAuth,
                                     context,
                                     eventBase,
                                     cryptoAuth,
                                     rand,
                                     allocator);
    context->pub.sessionManager = context->sm;

    Bits_memcpyConst(&context->module, (&(struct DHTModule) {
        .name = "Ducttape",
        .context = context,
        .handleOutgoing = handleOutgoing
    }), sizeof(struct DHTModule));

    Bits_memcpyConst(&context->switchInterface, (&(struct Interface) {
        .sendMessage = incomingFromSwitch,
        .senderContext = context,
        .allocator = allocator
    }), sizeof(struct Interface));

    if (DHTModuleRegistry_register(&context->module, context->registry)
        || SwitchCore_setRouterInterface(&context->switchInterface, switchCore))
    {
        return NULL;
    }

    // setup the switch pinger interface.
    Bits_memcpyConst(&context->pub.switchPingerIf, (&(struct Interface) {
        .sendMessage = incomingFromPinger,
        .senderContext = context
    }), sizeof(struct Interface));

    return &context->pub;
}

void Ducttape_setUserInterface(struct Ducttape* dt, struct Interface* userIf)
{
    struct Ducttape_pvt* context = Identity_check((struct Ducttape_pvt*) dt);
    context->userIf = userIf;
    userIf->receiveMessage = incomingFromTun;
    userIf->receiverContext = context;
}
