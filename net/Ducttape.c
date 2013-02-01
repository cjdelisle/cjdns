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
#include "interface/TUNMessageType.h"
#include "interface/Interface.h"
#include "interface/SessionManager.h"
#include "util/log/Log.h"
#include "memory/Allocator.h"
#include "memory/BufferAllocator.h"
#include "net/Ducttape_pvt.h"
#include "switch/SwitchCore.h"
#include "switch/LabelSplicer.h"
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
#include <setjmp.h>

/** Size of the per-message workspace. */
#define PER_MESSAGE_BUF_SZ 8192

/**
 * In order to easily tell the incoming connection requests from messages which
 * are addressed to a specific interface by its handle, the most significant bit
 * in the big endian representation of the handle shall be cleared to indicate
 * that a session is new and set otherwise.
 */
#define HANDLE_FLAG_BIT_be Endian_hostToBigEndian32(0x80000000)

/*--------------------Prototypes--------------------*/
static int handleOutgoing(struct DHTMessage* message,
                          void* vcontext);

static void outOfMemory(void* towel)
{
    longjmp(*((jmp_buf*) towel), 0);
}

static inline uint8_t incomingDHT(struct Message* message,
                                  struct Address* addr,
                                  struct Ducttape_pvt* context)
{
    struct DHTMessage dht;
    Bits_memset(&dht, 0, sizeof(struct DHTMessage));

    // TODO: These copies are not necessary at all.
    const uint32_t length = (message->length < DHTMessage_MAX_SIZE)
        ? message->length
        : DHTMessage_MAX_SIZE;
    Bits_memcpy(dht.bytes, message->bytes, length);

    dht.address = addr;

    uint8_t buffer[PER_MESSAGE_BUF_SZ];
    dht.allocator = BufferAllocator_new(buffer, PER_MESSAGE_BUF_SZ);

    jmp_buf towel;
    if (!setjmp(towel)) {
        BufferAllocator_onOOM(dht.allocator, outOfMemory, &towel);

        DHTModuleRegistry_handleIncoming(&dht, context->registry);
    }
    // TODO: return something meaningful.
    return Error_NONE;
}

/** Header must not be encrypted and must be aligned on the beginning of the ipv6 header. */
static inline uint8_t sendToRouter(struct Node* node,
                                   struct Message* message,
                                   struct Ducttape_pvt* context)
{
    struct Address* addr = &node->address;

    // We have to copy out the switch header because it
    // will probably be clobbered by the crypto headers.
    struct Headers_SwitchHeader header;
    if (context->switchHeader) {
        Bits_memcpyConst(&header, context->switchHeader, Headers_SwitchHeader_SIZE);
    } else {
        Bits_memset(&header, 0, Headers_SwitchHeader_SIZE);
    }
    header.label_be = Endian_hostToBigEndian64(addr->path);
    context->switchHeader = &header;

    // This comes out in outgoingFromCryptoAuth() then sendToSwitch()
    struct SessionManager_Session* session =
        SessionManager_getSession(addr->ip6.bytes, addr->key, context->sm);

    // Whichever has a declared version should transfer it to the other.
    // Since 0 is the default version, if the router doesn't know the version
    // it should not set the session's version to 0.
    if (session->version) {
        node->version = session->version;
    } else if (node->version) {
        // If the session is already running, we have to reset it otherwise we
        // will have a version 1 session but not know the handle for the other
        // node.
        session->version = node->version;
        CryptoAuth_reset(&session->iface);
    }

    context->session = session;
    context->layer = Ducttape_SessionLayer_OUTER;
    return session->iface.sendMessage(message, &session->iface);
}

static int handleOutgoing(struct DHTMessage* dmessage,
                          void* vcontext)
{
    struct Ducttape_pvt* context = Identity_cast((struct Ducttape_pvt*) vcontext);

    struct Message message =
        { .length = dmessage->length, .bytes = (uint8_t*) dmessage->bytes, .padding = 512 };

    Message_shift(&message, Headers_UDPHeader_SIZE);
    struct Headers_UDPHeader* uh = (struct Headers_UDPHeader*) message.bytes;
    uh->sourceAndDestPorts = 0;
    uh->length_be = Endian_hostToBigEndian16(dmessage->length);
    uh->checksum_be = 0;

    uint16_t payloadLength = message.length;

    Message_shift(&message, Headers_IP6Header_SIZE);
    struct Headers_IP6Header* header = (struct Headers_IP6Header*) message.bytes;
    header->versionClassAndFlowLabel = 0;
    header->flowLabelLow_be = 0;
    header->nextHeader = 17;
    header->hopLimit = 0;
    header->payloadLength_be = Endian_hostToBigEndian16(payloadLength);

    Bits_memcpyConst(header->sourceAddr,
                     context->myAddr.ip6.bytes,
                     Address_SEARCH_TARGET_SIZE);

    Bits_memcpyConst(header->destinationAddr,
                     dmessage->address->ip6.bytes,
                     Address_SEARCH_TARGET_SIZE);

    #ifdef Log_DEBUG
        Assert_true(!((uintptr_t)dmessage->bytes % 4) || !"alignment fault");
    #endif

    uh->checksum_be =
        Checksum_udpIp6(header->sourceAddr, (uint8_t*) uh, message.length - Headers_IP6Header_SIZE);

    context->ip6Header = header;
    context->switchHeader = NULL;

    struct Node* n = RouterModule_getNode(dmessage->address->path, context->routerModule);
    struct Node ns;
    if (!n) {
        // TODO: this is a mess
        Bits_memcpyConst(&ns.address, &dmessage->address, sizeof(struct Address));
        if (dmessage->replyTo) {
            int64_t* verPtr = Dict_getInt(dmessage->replyTo->asDict, String_CONST("p"));
            ns.version = (verPtr) ? *verPtr : 0;
        }
        n = &ns;
    }

    sendToRouter(n, &message, context);

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
        && uh->sourceAndDestPorts == 0
        && (int) Endian_bigEndianToHost16(uh->length_be) ==
            (message->length - Headers_UDPHeader_SIZE);
}

/**
 * Message which is for us, message is aligned on the beginning of the content.
 * this is called from core() which calls through an interfaceMap.
 */
static inline uint8_t incomingForMe(struct Message* message,
                                    struct Ducttape_pvt* context,
                                    uint8_t herPubKey[32])
{
    struct Address addr;
    Bits_memcpyConst(addr.ip6.bytes, context->session->ip6, 16);
    //AddressCalc_addressForPublicKey(addr.ip6.bytes, herPubKey);

    if (Bits_memcmp(addr.ip6.bytes, context->ip6Header->sourceAddr, 16)) {
        #ifdef Log_DEBUG
            uint8_t keyAddr[40];
            Address_printIp(keyAddr, &addr);
            Bits_memcpyConst(addr.ip6.bytes, context->ip6Header->sourceAddr, 16);
            uint8_t srcAddr[40];
            Address_printIp(srcAddr, &addr);
            Log_debug(context->logger,
                       "Dropped packet because source address is not same as key.\n"
                       "    %s source addr\n"
                       "    %s hash of key\n",
                       srcAddr,
                       keyAddr);
        #endif
        return Error_INVALID;
    }

    if (message->length == 0) {
        #ifdef Log_WARN
            uint8_t keyAddr[40];
            uint8_t ipv6Hex[80];
            Address_printIp(keyAddr, &addr);
            Hex_encode(ipv6Hex, 80, (uint8_t*) context->ip6Header, 40);
            Log_warn(context->logger,
                      "Got ipv6 packet from %s which has no content!\nIPv6 Header: [%s]",
                      keyAddr, ipv6Hex);
        #endif
        return Error_INVALID;
    }

    if (isRouterTraffic(message, context->ip6Header)) {
        // Check the checksum.
        struct Headers_UDPHeader* uh = (struct Headers_UDPHeader*) message->bytes;

        if (uh->checksum_be == 0) {
            #ifdef Version_0_COMPAT
                uint8_t keyAddr[40];
                Address_printIp(keyAddr, &addr);
                Log_warn(context->logger, "Router packet with blank checksum from [%s].", keyAddr);
            #else
                #ifdef Log_DEBUG
                uint8_t keyAddr[40];
                Address_printIp(keyAddr, &addr);
                Log_debug(context->logger, "Router packet with blank checksum from [%s].", keyAddr);
                #endif
            #endif
        } else if (Checksum_udpIp6(context->ip6Header->sourceAddr, (uint8_t*)uh, message->length)) {
            #ifdef Log_DEBUG
                uint8_t keyAddr[40];
                Address_printIp(keyAddr, &addr);
                Log_debug(context->logger,
                          "Router packet with incorrect checksum, from [%s]", keyAddr);
            #endif
            return Error_INVALID;
        }

        // Shift off the UDP header.
        Message_shift(message, -Headers_UDPHeader_SIZE);
        addr.path = Endian_bigEndianToHost64(context->switchHeader->label_be);
        Bits_memcpyConst(addr.key, herPubKey, 32);
        return incomingDHT(message, &addr, context);
    }

    if (!context->userIf) {
        Log_warn(context->logger,
                 "Dropping message because there is no router interface configured.\n");
        return Error_UNDELIVERABLE;
    }

    // prevent router advertizement schenanigans
    if (context->ip6Header->hopLimit == 255) {
        context->ip6Header->hopLimit--;
    }

    // Now write a message to the TUN device.
    // Need to move the ipv6 header forward up to the content because there's a crypto header
    // between the ipv6 header and the content which just got eaten.
    Message_shift(message, Headers_IP6Header_SIZE);
    uint16_t sizeDiff = message->bytes - (uint8_t*)context->ip6Header;
    if (sizeDiff) {
        context->ip6Header->payloadLength_be =
            Endian_hostToBigEndian16(
                Endian_bigEndianToHost16(context->ip6Header->payloadLength_be) - sizeDiff);
        Bits_memmoveConst(message->bytes, context->ip6Header, Headers_IP6Header_SIZE);
    }

    TUNMessageType_push(message, Ethernet_TYPE_IP6);

    context->userIf->sendMessage(message, context->userIf);
    return Error_NONE;
}

uint8_t Ducttape_injectIncomingForMe(struct Message* message,
                                     struct Ducttape* dt,
                                     uint8_t herPublicKey[32])
{
    struct Ducttape_pvt* context = Identity_cast((struct Ducttape_pvt*)dt);
    struct Headers_SwitchHeader sh;
    Bits_memcpyConst(&sh, message->bytes, Headers_SwitchHeader_SIZE);
    context->switchHeader = &sh;
    Message_shift(message, -Headers_SwitchHeader_SIZE);

    struct Headers_IP6Header ip6;
    Bits_memcpyConst(&ip6, message->bytes, Headers_IP6Header_SIZE);
    context->ip6Header = &ip6;
    Message_shift(message, -Headers_IP6Header_SIZE);

    struct SessionManager_Session s;
    AddressCalc_addressForPublicKey(s.ip6, herPublicKey);

    context->session = &s;
    uint8_t ret = incomingForMe(message, context, herPublicKey);
    context->session = NULL;

    return ret;
}

static void debugHandles(struct Log* logger, struct SessionManager_Session* session, char* message)
{
    uint8_t ip[40];
    AddrTools_printIp(ip, session->ip6);
    Log_debug(logger, "%s ver[%u] send[%u] recv[%u] ip[%s]",
              message,
              session->version,
              Endian_hostToBigEndian32(session->sendHandle_be & ~HANDLE_FLAG_BIT_be),
              Endian_hostToBigEndian32(session->receiveHandle_be),
              ip);
}

/**
 * Send a message to another switch.
 * Switchheader will precede the message.
 */
static inline uint8_t sendToSwitch(struct Message* message,
                                   struct Headers_SwitchHeader* destinationSwitchHeader,
                                   struct Ducttape_pvt* context)
{
    // Tag the message with the proper handle.
    struct SessionManager_Session* session = context->session;
    if (session->version > 0) {
        // If the session is established, we send their handle for the session,
        // otherwise we send ours.
        int state = CryptoAuth_getState(&session->iface);
        if (state >= CryptoAuth_HANDSHAKE3) {
            debugHandles(context->logger, session, "Sending run message");

            Message_shift(message, 4);
            ((uint32_t*)message->bytes)[0] = session->sendHandle_be;
        } else if (state < CryptoAuth_HANDSHAKE3) {
            debugHandles(context->logger, session, "Sending start message");

            // the most significant bit in a handle is reserved to tell the recipient if it is
            // an initiation handle or a running handle which they should look up in their map.
            Message_shift(message, 4);
            ((uint32_t*)message->bytes)[0] = session->receiveHandle_be;
        }
    } else {
        debugHandles(context->logger, session, "Sending protocol 0 message");
    }

    Message_shift(message, Headers_SwitchHeader_SIZE);
    struct Headers_SwitchHeader* switchHeaderLocation =
        (struct Headers_SwitchHeader*) message->bytes;

    // This will be false if an incoming connect-to-me packet caused the cryptoAuth to send
    // back a hello packet.
    if (destinationSwitchHeader != switchHeaderLocation) {
        Bits_memmoveConst(message->bytes, destinationSwitchHeader, Headers_SwitchHeader_SIZE);
    }

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

// Called by the TUN device.
static inline uint8_t incomingFromTun(struct Message* message,
                                      struct Interface* iface)
{
    struct Ducttape_pvt* context = Identity_cast((struct Ducttape_pvt*) iface->receiverContext);

    uint16_t ethertype = TUNMessageType_pop(message);

    struct Headers_IP6Header* header = (struct Headers_IP6Header*) message->bytes;

    int version = Headers_getIpVersion(message->bytes);
    if ((ethertype == Ethernet_TYPE_IP4 && version != 4)
        || (ethertype == Ethernet_TYPE_IP6 && version != 6))
    {
        Log_warn(context->logger, "dropped packet because ip version [%d] "
                 "doesn't match ethertype [%u].", version, Endian_bigEndianToHost16(ethertype));
        return Error_INVALID;
    }

    if (ethertype != Ethernet_TYPE_IP6 || !AddressCalc_validAddress(header->sourceAddr)) {
        return context->ipTunnel->tunInterface.sendMessage(message,
                                                           &context->ipTunnel->tunInterface);
    }

    if (Bits_memcmp(header->sourceAddr, context->myAddr.ip6.bytes, 16)) {
        uint8_t expectedSource[40];
        AddrTools_printIp(expectedSource, context->myAddr.ip6.bytes);
        uint8_t packetSource[40];
        AddrTools_printIp(packetSource, header->sourceAddr);
        Log_warn(context->logger,
                 "dropped packet from [%s] because all messages must have source address [%s]",
                 (char*) packetSource, (char*) expectedSource);
        return Error_INVALID;
    }
    if (!Bits_memcmp(header->destinationAddr, context->myAddr.ip6.bytes, 16)) {
        // I'm Gonna Sit Right Down and Write Myself a Letter
        iface->sendMessage(message, iface);
        return Error_NONE;
    }

    context->switchHeader = NULL;

    struct Node* bestNext = RouterModule_lookup(header->destinationAddr, context->routerModule);
    context->forwardTo = bestNext;
    if (bestNext) {
        if (!Bits_memcmp(header->destinationAddr, bestNext->address.ip6.bytes, 16)) {
            // Direct send, skip the innermost layer of encryption.
            #ifdef Log_DEBUG
                uint8_t nhAddr[60];
                Address_print(nhAddr, &bestNext->address);
                Log_debug(context->logger, "Forwarding data to %s (last hop)\n", nhAddr);
            #endif
            return sendToRouter(bestNext, message, context);
        }
        // else { the message will need to be 3 layer encrypted but since we already did a lookup
        // of the best node to forward to, we can skip doing another lookup by storing a pointer
        // to that node in the context (forwardTo).
    } else {
        #ifdef Log_WARN
            uint8_t thisAddr[40];
            uint8_t destAddr[40];
            AddrTools_printIp(thisAddr, context->myAddr.ip6.bytes);
            AddrTools_printIp(destAddr, header->destinationAddr);
            Log_warn(context->logger,
                     "Dropped message from TUN because this node [%s] is closest to dest [%s]",
                     thisAddr, destAddr);
        #endif
        return Error_UNDELIVERABLE;
    }

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
    // NOTE: We can't check its state now, we have to assume it will use the 120 byte
    //       header because we might be in a currently established session which has
    //       timed out and will be in state HANDSHALE1 after we call sendMessage().

    // shift, copy, shift because shifting asserts that there is enough buffer space.
    Message_shift(message, Headers_CryptoAuth_SIZE);
    Bits_memcpyConst(message->bytes, header, Headers_IP6Header_SIZE);
    Message_shift(message, -(Headers_IP6Header_SIZE + Headers_CryptoAuth_SIZE));
    // The message is now aligned on the content.

    #ifdef Log_DEBUG
        uint8_t destAddr[40];
        AddrTools_printIp(destAddr, header->destinationAddr);
        uint8_t nhAddr[60];
        Address_print(nhAddr, &bestNext->address);
        Log_debug(context->logger, "Sending to [%s] via [%s]", destAddr, nhAddr);
    #endif

    // This comes out at outgoingFromCryptoAuth() then outgoingFromMe()
    context->session = session;
    context->layer = Ducttape_SessionLayer_INNER;
    return session->iface.sendMessage(message, &session->iface);
}

/**
 * Send an arbitrary message to a node.
 *
 * @param message to be sent, must be prefixed with IpTunnel_PacketInfoHeader.
 * @param iface an interface for which receiverContext is the ducttape.
 */
static uint8_t sendToNode(struct Message* message, struct Interface* iface)
{
    struct Ducttape_pvt* context = Identity_cast((struct Ducttape_pvt*)iface->receiverContext);
    context->switchHeader = NULL;
    struct IpTunnel_PacketInfoHeader* header = (struct IpTunnel_PacketInfoHeader*) message->bytes;
    Message_shift(message, -IpTunnel_PacketInfoHeader_SIZE);
    struct Node* n = RouterModule_lookup(header->nodeIp6Addr, context->routerModule);
    if (n) {
        if (!Bits_memcmp(header->nodeKey, n->address.key, 32)) {
            // Found the node.
            #ifdef Log_DEBUG
                uint8_t nhAddr[60];
                Address_print(nhAddr, &n->address);
                Log_debug(context->logger, "Sending arbitrary data to [%s]", nhAddr);
            #endif
            return sendToRouter(n, message, context);
        }
    }

    #ifdef Log_DEBUG
        uint8_t printedIp6[40];
        AddrTools_printIp(printedIp6, header->nodeIp6Addr);
        Log_debug(context->logger, "Couldn't find node [%s] for sending to.", printedIp6);
    #endif

    // Now lets trigger a search for this node.
    uint64_t now = Time_currentTimeMilliseconds(context->eventBase);
    if (context->timeOfLastSearch + context->timeBetweenSearches < now) {
        context->timeOfLastSearch = now;
        RouterModule_beginSearch(header->nodeIp6Addr, NULL, NULL, context->routerModule);
    }
    return 0;
}

/**
 * Send an arbitrary message to the tun device.
 *
 * @param message to be sent, must be prefixed with IpTunnel_PacketInfoHeader.
 * @param iface an interface for which receiverContext is the ducttape.
 */
static uint8_t sendToTun(struct Message* message, struct Interface* iface)
{
    struct Ducttape_pvt* context = Identity_cast((struct Ducttape_pvt*)iface->receiverContext);
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
static inline int core(struct Message* message, struct Ducttape_pvt* context)
{
    context->ip6Header = (struct Headers_IP6Header*) message->bytes;

    if (isForMe(message, context)) {
        Message_shift(message, -Headers_IP6Header_SIZE);

        if (Bits_memcmp(context->session->ip6, context->ip6Header->sourceAddr, 16)) {
            // triple encrypted
            // This call goes to incomingForMe()
            struct SessionManager_Session* session =
                SessionManager_getSession(context->ip6Header->sourceAddr, NULL, context->sm);

            if (!session->sendHandle_be && message->length > Headers_CryptoAuth_SIZE + 4) {
                uint32_t nonce_be = ((uint32_t*)message->bytes)[1];
                if (Endian_bigEndianToHost32(nonce_be) < 4) {
                    uint32_t handle_be = ((uint32_t*)message->bytes)[0];
                    session->sendHandle_be = handle_be | HANDLE_FLAG_BIT_be;
                    session->version = (session->version) ? session->version : 1;
                    debugHandles(context->logger, session, "New session, incoming layer3");
                }
            }

            #ifdef Log_DEBUG
                uint8_t addr[40];
                AddrTools_printIp(addr, context->ip6Header->sourceAddr);
                Log_debug(context->logger, "Incoming layer3 message, ostensibly from [%s]", addr);
            #endif

            context->session = session;
            context->layer = Ducttape_SessionLayer_INNER;
            return session->iface.receiveMessage(message, &session->iface);
        } else {
            // double encrypted, inner layer plaintext.
            // The session is still set from the router-to-router traffic and that is the one we use
            // to determine the node's id.
            return incomingForMe(message,
                                 context,
                                 CryptoAuth_getHerPublicKey(&context->session->iface));
        }
    }

    if (context->ip6Header->hopLimit == 0) {
        Log_debug(context->logger, "dropped message because hop limit has been exceeded.\n");
        // TODO: send back an error message in response.
        return Error_UNDELIVERABLE;
    }
    context->ip6Header->hopLimit--;

    struct Node* nextHop = context->forwardTo;
    context->forwardTo = NULL;
    if (!nextHop) {
        nextHop = RouterModule_lookup(context->ip6Header->destinationAddr, context->routerModule);
    }

    if (nextHop) {
        #ifdef Log_DEBUG
            struct Address* addr = &nextHop->address;
            uint8_t nhAddr[60];
            Address_print(nhAddr, addr);
            if (Bits_memcmp(context->ip6Header->destinationAddr, addr->ip6.bytes, 16)) {
                // Potentially forwarding for ourselves.
                struct Address destination;
                Bits_memcpyConst(destination.ip6.bytes, context->ip6Header->destinationAddr, 16);
                uint8_t ipAddr[40];
                Address_printIp(ipAddr, &destination);
                Log_debug(context->logger, "Forwarding data to %s via %s\n", ipAddr, nhAddr);
            } else {
                // Definitely forwarding on behalf of someone else.
                Log_debug(context->logger, "Forwarding data to %s (last hop)\n", nhAddr);
            }
        #endif
        return sendToRouter(nextHop, message, context);
    }

    #ifdef Log_INFO
        struct Address destination;
        Bits_memcpyConst(destination.ip6.bytes, context->ip6Header->destinationAddr, 16);
        uint8_t ipAddr[40];
        Address_printIp(ipAddr, &destination);
        Log_info(context->logger, "Dropped message because this node is the closest known "
                                   "node to the destination %s.", ipAddr);
    #endif
    return Error_UNDELIVERABLE;
}

/**
 * When we send a message it goes into the CryptoAuth.
 * for the content level crypto then it goes to outgoingFromCryptoAuth then comes here.
 * Message is aligned on the beginning of the CryptoAuth header.
 */
static inline uint8_t outgoingFromMe(struct Message* message, struct Ducttape_pvt* context)
{
    // incomingFromTun puts the IP6 header Headers_CryptoAuth_SIZE bytes behind
    // the beginning of the data, depending on the state of the CA, it might need to be moved.

    if (CryptoAuth_getState(&context->session->iface) >= CryptoAuth_HANDSHAKE3) {
        //     [ ip6 ][        CA header  [  actual CA header ][ content.....
        //                                ^-- you are here
        // actual CA header == 20

        uint8_t* beginningOfCaHeader = message->bytes;

        Message_shift(message, Headers_IP6Header_SIZE + Headers_CryptoAuth_SIZE - 20);

        //     [ ip6 ][        CA header  [  actual CA header ][ content.....
        //     ^-- moved

        Bits_memcpyConst(beginningOfCaHeader - Headers_IP6Header_SIZE,
                         message->bytes,
                         Headers_IP6Header_SIZE);

        //                         [ ip6 ][  actual CA header ][ content.....
        //     ^-- still here but ip6 header moved

        Message_shift(message, -(Headers_CryptoAuth_SIZE - 20));

        //                         [ ip6 ][  actual CA header ][ content.....
        //                         ^-- you are here
    } else {
        Message_shift(message, Headers_IP6Header_SIZE);
    }

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
        Assert_true(!Bits_memcmp(header->sourceAddr, context->myAddr.ip6.bytes, 16));
    }

    // Need to set the length field to take into account
    // the crypto headers which are hidden under the ipv6 packet.
    header->payloadLength_be =
        Endian_hostToBigEndian16(message->length - Headers_IP6Header_SIZE);


    // Forward this call to core() which will check its validity
    // and since it's not to us, forward it to the correct node.
    return core(message, context);
}

static inline int incomingFromRouter(struct Message* message, struct Ducttape_pvt* context)
{
    if (!validEncryptedIP6(message)) {
        // Not valid cjdns IPv6, we'll try it as an IPv4 or ICANN-IPv6 packet
        // and check if we have an agreement with the node who sent it.
        Message_shift(message, IpTunnel_PacketInfoHeader_SIZE);
        struct IpTunnel_PacketInfoHeader* header =
            (struct IpTunnel_PacketInfoHeader*) message->bytes;

        uint8_t* pubKey = CryptoAuth_getHerPublicKey(&context->session->iface);
        uint8_t* addr = context->session->ip6;
        Bits_memcpyConst(header->nodeIp6Addr, addr, 16);
        Bits_memcpyConst(header->nodeKey, pubKey, 32);

        struct Interface* ipTun = &context->ipTunnel->nodeInterface;
        return ipTun->sendMessage(message, ipTun);
    }

    //Log_debug(context->logger, "Got message from router.\n");
    return core(message, context);
}

static uint8_t incomingFromCryptoAuth(struct Message* message, struct Interface* iface)
{
    struct Ducttape_pvt* context = Identity_cast((struct Ducttape_pvt*) iface->receiverContext);
    enum Ducttape_SessionLayer layer = context->layer;
    context->layer = Ducttape_SessionLayer_INVALID;
    switch (layer) {
        case Ducttape_SessionLayer_OUTER:
            return incomingFromRouter(message, context);
        case Ducttape_SessionLayer_INNER:
            return incomingForMe(message,
                                 context,
                                 CryptoAuth_getHerPublicKey(&context->session->iface));
        default:
            Assert_always(false);
    }
    // never reached.
    return 0;
}

static uint8_t outgoingFromCryptoAuth(struct Message* message, struct Interface* iface)
{
    struct Ducttape_pvt* context = Identity_cast((struct Ducttape_pvt*) iface->senderContext);

    enum Ducttape_SessionLayer layer = context->layer;
    context->layer = Ducttape_SessionLayer_INVALID;

    if (layer == Ducttape_SessionLayer_OUTER) {
        return sendToSwitch(message, context->switchHeader, context);
    } else {
        Log_debug(context->logger, "Sending layer3 message");
        return outgoingFromMe(message, context);
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
        Log_info(context->logger, "dropped runt ctrl packet from [%s]", labelStr);
        return Error_NONE;
    }
    struct Control* ctrl = (struct Control*) message->bytes;

    if (Checksum_engine(message->bytes, message->length)) {
        Log_info(context->logger, "ctrl packet from [%s] with invalid checksum.", labelStr);
        #ifndef Version_0_COMPAT
            return Error_NONE;
        #endif
        // This will break error responses since they were
        // not sending proper checksums as of 5610464f7bc44ec09ffac81b3507d4df905d6d98
    }

    bool pong = false;
    if (ctrl->type_be == Control_ERROR_be) {
        if (message->length < Control_Error_MIN_SIZE) {
            Log_info(context->logger, "dropped runt error packet from [%s]", labelStr);
            return Error_NONE;
        }

        RouterModule_brokenPath(Endian_bigEndianToHost64(switchHeader->label_be),
                                context->routerModule);

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
                Log_info(context->logger,
                          "error packet from [%s] caused by [%s] packet ([%u])",
                          labelStr,
                          Control_typeString(causeCtrl->type_be),
                          Endian_bigEndianToHost16(causeCtrl->type_be));
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
            Log_info(context->logger,
                      "error packet from [%s], error type [%u]",
                      labelStr,
                      Endian_bigEndianToHost32(ctrl->content.error.errorType_be));
        }
    } else if (ctrl->type_be == Control_PONG_be) {
        pong = true;
    } else if (ctrl->type_be == Control_PING_be) {

        Message_shift(message, -Control_HEADER_SIZE);
        #ifdef Version_0_COMPAT
            if (message->length >= Control_Ping_MIN_SIZE) {
        #endif
        if (message->length < Control_Ping_MIN_SIZE) {
            Log_info(context->logger, "dropped runt ping");
            return Error_INVALID;
        }
        struct Control_Ping* ping = (struct Control_Ping*) message->bytes;
        ping->magic = Control_Pong_MAGIC;
        ping->version_be = Endian_hostToBigEndian32(Version_CURRENT_PROTOCOL);
        #ifdef Version_0_COMPAT
            }
        #endif
        Message_shift(message, Control_HEADER_SIZE);

        ctrl->type_be = Control_PONG_be;
        ctrl->checksum_be = 0;
        ctrl->checksum_be = Checksum_engine(message->bytes, message->length);
        Message_shift(message, Headers_SwitchHeader_SIZE);
        Log_info(context->logger, "got switch ping from [%s]", labelStr);
        switchIf->receiveMessage(message, switchIf);
    } else {
        Log_info(context->logger,
                  "control packet of unknown type from [%s], type [%d]",
                  labelStr, Endian_bigEndianToHost16(ctrl->type_be));
    }

    if (pong && context->public.switchPingerIf.receiveMessage) {
        // Shift back over the header
        Message_shift(message, Headers_SwitchHeader_SIZE);
        context->public.switchPingerIf.receiveMessage(
            message, &context->public.switchPingerIf);
    }
    return Error_NONE;
}

static inline uint8_t* extractPublicKey(struct Message* message,
                                        uint32_t* version,
                                        uint8_t ip6[16])
{
    if (*version == 0xFFFFFFFF) {
        return NULL;
    }
    if (*version == 0) {
        #ifndef Version_0_COMPAT
            return NULL;
        #endif
        // version 0 nodes are missing the handle
        Message_shift(message, 4);
    }

    union Headers_CryptoAuth* caHeader = (union Headers_CryptoAuth*) message->bytes;
    uint32_t nonce = Endian_bigEndianToHost32(caHeader->nonce);

    if (nonce > 3 && nonce < UINT32_MAX) {
        *version = *version - 1;
        return extractPublicKey(message, version, ip6);
    }

    if (message->length < Headers_CryptoAuth_SIZE) {
        *version = *version - 1;
        return extractPublicKey(message, version, ip6);
    }

    AddressCalc_addressForPublicKey(ip6, caHeader->handshake.publicKey);
    if (!AddressCalc_validAddress(ip6)) {
        *version = *version - 1;
        return extractPublicKey(message, version, ip6);
    }

    return caHeader->handshake.publicKey;
}

/**
 * This is called as sendMessage() by the switch.
 * There is only one switch interface which sends all traffic.
 * message is aligned on the beginning of the switch header.
 */
static uint8_t incomingFromSwitch(struct Message* message, struct Interface* switchIf)
{
    struct Ducttape_pvt* context = Identity_cast((struct Ducttape_pvt*)switchIf->senderContext);
    struct Headers_SwitchHeader* switchHeader = (struct Headers_SwitchHeader*) message->bytes;
    Message_shift(message, -Headers_SwitchHeader_SIZE);

    // The label comes in reversed from the switch because the switch doesn't know that we aren't
    // another switch ready to parse more bits, bit reversing the label yields the source address.
    switchHeader->label_be = Bits_bitReverse64(switchHeader->label_be);

    if (Headers_getMessageType(switchHeader) == Headers_SwitchHeader_TYPE_CONTROL) {
        return handleControlMessage(context, message, switchHeader, switchIf);
    }

    if (message->length < 4) {
        Log_info(context->logger, "runt");
        return Error_INVALID;
    }

    // #1 try to get the session using the handle.
    uint32_t version = 1;
    uint32_t handle_be = ((uint32_t*)message->bytes)[0];
    struct SessionManager_Session* session = NULL;

    if (handle_be & HANDLE_FLAG_BIT_be) {
        uint32_t realHandle = Endian_bigEndianToHost32(handle_be & ~HANDLE_FLAG_BIT_be);
        session = SessionManager_sessionForHandle(realHandle, context->sm);
        if (session) {
            if (session->version == 0) {
                uint8_t hex[9];
                Hex_encode(hex, 9, message->bytes, 4);
                Log_debug(context->logger, "0 version session input: [%s] [%u]", hex, realHandle);
                debugHandles(context->logger, session, "Got 0 version session");
                session = NULL;
                version = 0;
            } else {
                debugHandles(context->logger, session, "Got running session");
                Message_shift(message, -4);
            }
        } else {
            version = 0;
        }
    }

    // #2 no session, try the message as a handshake.
    if (!session && message->length >= Headers_CryptoAuth_SIZE) {
        Message_shift(message, -4);
        uint8_t ip6[16];
        uint8_t* herKey = extractPublicKey(message, &version, ip6);
        if (herKey) {
            session = SessionManager_getSession(ip6, herKey, context->sm);
            session->sendHandle_be = handle_be | HANDLE_FLAG_BIT_be;
            session->version = version;
            debugHandles(context->logger, session, "New session");
        }
    }

    // #3 try the message as a protocol version 0 message.
    #ifdef Version_0_COMPAT
        if (!session) {
            int herAddrIndex = AddressMapper_indexOf(switchHeader->label_be, context->addrMap);
            uint8_t* herKey = NULL;
            if (herAddrIndex == -1) {
                uint64_t label = Endian_bigEndianToHost64(switchHeader->label_be);
                struct Node* n = RouterModule_getNode(label, context->routerModule);
                if (n) {
                    herAddrIndex = AddressMapper_put(switchHeader->label_be,
                                                     n->address.ip6.bytes,
                                                     context->addrMap);
                    herKey = n->address.key;
                }
            }
            if (herAddrIndex != -1) {
                Log_debug(context->logger, "Handling packet from legacy protocol version 0 node.");
                session = SessionManager_getSession(context->addrMap->entries[herAddrIndex].address,
                                                    herKey,
                                                    context->sm);
                session->version = 0;
            }
        }
    #endif


    if (!session) {
        #ifdef Log_INFO
            uint8_t path[20];
            AddrTools_printPath(path, Endian_bigEndianToHost64(switchHeader->label_be));
            Log_info(context->logger, "Dropped traffic packet from unknown node. [%s]", path);
        #endif
        return 0;
    }

    if (((uint32_t*)message->bytes)[0] == 0xffffffff) {
        // Got a message from a node that doesn't know our key, this should never happen.
        #ifdef Log_INFO
            uint8_t path[20];
            AddrTools_printPath(path, Endian_bigEndianToHost64(switchHeader->label_be));
            Log_info(context->logger, "Dropped packet from node [%s] which doesn't know our key",
                     path);
        #endif
    }

    // This is needed so that the priority and other information
    // from the switch header can be passed on properly.
    context->switchHeader = switchHeader;

    // This goes to incomingFromCryptoAuth()
    // then incomingFromRouter() then core()
    context->session = session;
    context->layer = Ducttape_SessionLayer_OUTER;
    session->iface.receiveMessage(message, &session->iface);

    return 0;
}

static uint8_t incomingFromPinger(struct Message* message, struct Interface* iface)
{
    struct Ducttape_pvt* context = Identity_cast((struct Ducttape_pvt*)iface->senderContext);
    return context->switchInterface.receiveMessage(message, &context->switchInterface);
}

struct Ducttape* Ducttape_register(uint8_t privateKey[32],
                                   struct DHTModuleRegistry* registry,
                                   struct RouterModule* routerModule,
                                   struct SwitchCore* switchCore,
                                   struct EventBase* eventBase,
                                   struct Allocator* allocator,
                                   struct Log* logger,
                                   struct Admin* admin,
                                   struct IpTunnel* ipTun,
                                   struct Random* rand)
{
    struct Ducttape_pvt* context = allocator->calloc(sizeof(struct Ducttape_pvt), 1, allocator);
    context->registry = registry;
    context->routerModule = routerModule;
    context->logger = logger;
    context->forwardTo = NULL;
    context->eventBase = eventBase;
    Identity_set(context);

    #ifdef Version_0_COMPAT
        context->addrMap = Allocator_calloc(allocator, sizeof(struct AddressMapper), 1);
        AddressMapper_init(context->addrMap);
    #endif

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
                                     allocator);

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
    Bits_memcpyConst(&context->public.switchPingerIf, (&(struct Interface) {
        .sendMessage = incomingFromPinger,
        .senderContext = context
    }), sizeof(struct Interface));

    return &context->public;
}

void Ducttape_setUserInterface(struct Ducttape* dt, struct Interface* userIf)
{
    struct Ducttape_pvt* context = Identity_cast((struct Ducttape_pvt*) dt);
    context->userIf = userIf;
    userIf->receiveMessage = incomingFromTun;
    userIf->receiverContext = context;
}
