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
#include "crypto/CryptoAuth.h"
#include "util/log/Log.h"
#include "dht/Address.h"
#include "dht/DHTMessage.h"
#include "dht/DHTModule.h"
#include "dht/DHTModuleRegistry.h"
#include "dht/dhtcore/Node.h"
#include "dht/dhtcore/RouterModule.h"
#include "interface/Interface.h"
#include "interface/InterfaceMap.h"
#include "interface/SessionManager.h"
#include "util/log/Log.h"
#include "memory/Allocator.h"
#include "memory/BufferAllocator.h"
#include "net/Ducttape_pvt.h"
#include "switch/SwitchCore.h"
#include "switch/LabelSplicer.h"
#include "util/Bits.h"
#include "util/checksum/Checksum.h"
#include "util/version/Version.h"
#include "util/Assert.h"
#include "wire/Control.h"
#include "wire/Error.h"
#include "wire/Headers.h"

#include <stdint.h>
#include <event2/event.h>
#include <setjmp.h>

/** Used as a sanity check on the layer code. */
#define INVALID_LAYER 0

/** Encrypting/decrypting the content. */
#define INNER_LAYER 1

/** Encrypting/decrypting the ipv6 header. */
#define OUTER_LAYER 2

/** Size of the per-message workspace. */
#define PER_MESSAGE_BUF_SZ 8192

/**
 * In order to easily tell the incoming connection requests from messages which
 * are addressed to a specific interface by it's handle, the most significant bit
 * in the big endian representation of the handle shall be reserved for indicating
 * that a session is new.
 */
#define HANDLE_FLAG_BIT Endian_hostToBigEndian32(0x80000000)

/*--------------------Prototypes--------------------*/
static int handleOutgoing(struct DHTMessage* message,
                          void* vcontext);

static void outOfMemory(void* towel)
{
    longjmp(*((jmp_buf*) towel), 0);
}

static inline uint8_t incomingDHT(struct Message* message,
                                  struct Address* addr,
                                  struct Ducttape_Private* context)
{
    struct DHTMessage dht;
    memset(&dht, 0, sizeof(struct DHTMessage));

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
                                   struct Ducttape_Private* context)
{
    struct Address* addr = &node->address;

    // We have to copy out the switch header because it
    // will probably be clobbered by the crypto headers.
    struct Headers_SwitchHeader header;
    if (context->switchHeader) {
        Bits_memcpyConst(&header, context->switchHeader, Headers_SwitchHeader_SIZE);
    } else {
        memset(&header, 0, Headers_SwitchHeader_SIZE);
    }
    header.label_be = Endian_hostToBigEndian64(addr->path);
    context->switchHeader = &header;

    // This comes out in outgoingFromCryptoAuth() then sendToSwitch()
    struct SessionManager_Session* session =
        SessionManager_getSession(addr->ip6.bytes, addr->key, context->sm);
    session->version = node->version;

    context->layer = OUTER_LAYER;
    context->session = session;
    return session->iface.sendMessage(message, &session->iface);
}

static int handleOutgoing(struct DHTMessage* dmessage,
                          void* vcontext)
{
    struct Ducttape_Private* context = (struct Ducttape_Private*) vcontext;

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
                                    struct Ducttape_Private* context,
                                    uint8_t herPubKey[32])
{
    struct Address addr;
    Bits_memcpyConst(addr.ip6.bytes, context->session->ip6, 16);
    //AddressCalc_addressForPublicKey(addr.ip6.bytes, herPubKey);

    if (memcmp(addr.ip6.bytes, context->ip6Header->sourceAddr, 16)) {
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

    // Now write a message to the TUN device.
    // Need to move the ipv6 header forward up to the content because there's a crypto header
    // between the ipv6 header and the content which just got eaten.
    Message_shift(message, Headers_IP6Header_SIZE);
    uint16_t sizeDiff = message->bytes - (uint8_t*)context->ip6Header;
    if (sizeDiff) {
        context->ip6Header->payloadLength_be =
            Endian_hostToBigEndian16(
                Endian_bigEndianToHost16(context->ip6Header->payloadLength_be) - sizeDiff);
        memmove(message->bytes, context->ip6Header, Headers_IP6Header_SIZE);
    }
    context->userIf->sendMessage(message, context->userIf);
    return Error_NONE;
}

uint8_t Ducttape_injectIncomingForMe(struct Message* message,
                                     struct Ducttape* dt,
                                     uint8_t herPublicKey[32])
{
    struct Ducttape_Private* context = (struct Ducttape_Private*) dt;
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

/**
 * Send a message to another switch.
 * Switchheader will precede the message.
 */
static inline uint8_t sendToSwitch(struct Message* message,
                                   struct Headers_SwitchHeader* destinationSwitchHeader,
                                   struct Ducttape_Private* context)
{
    struct SessionManager_Session* session = context->session;
    if (session->version > 0) {
        Message_shift(message, 4);
        // If the session is established, we send their handle for the session,
        // otherwise we send ours.
        if (CryptoAuth_getState(&session->iface) >= CryptoAuth_HANDSHAKE3) {
            Log_debug(context->logger, "Sending protocol [%d] run message.", session->version);
            ((uint32_t*)message->bytes)[0] = session->sendHandle;
        } else {
            Log_debug(context->logger, "Sending protocol [%d] start message with handle [%u].",
                      session->version,
                      Endian_hostToBigEndian32(session->receiveHandle));
            // the most significant bit in a handle is reserved to tell the recipient if it is
            // an initiation handle or a running handle which they should look up in their map.
            ((uint32_t*)message->bytes)[0] =
                Endian_hostToBigEndian32(session->receiveHandle) | HANDLE_FLAG_BIT;
        }
    } else {
        Log_debug(context->logger, "Sending protocol 0 message.");
    }

    Message_shift(message, Headers_SwitchHeader_SIZE);
    struct Headers_SwitchHeader* switchHeaderLocation =
        (struct Headers_SwitchHeader*) message->bytes;

    // This will be false if an incoming connect-to-me packet caused the cryptoAuth to send
    // back a hello packet.
    if (destinationSwitchHeader != switchHeaderLocation) {
        memmove(message->bytes, destinationSwitchHeader, Headers_SwitchHeader_SIZE);
    }

    return context->switchInterface.receiveMessage(message, &context->switchInterface);
}

static inline bool validEncryptedIP6(struct Message* message)
{
    struct Headers_IP6Header* header = (struct Headers_IP6Header*) message->bytes;
    // Empty ipv6 headers are tolerated at this stage but dropped later.
    return message->length >= Headers_IP6Header_SIZE
        && header->sourceAddr[0] == 0xFC
        && header->destinationAddr[0] == 0xFC;
}

static inline bool isForMe(struct Message* message, struct Ducttape_Private* context)
{
    struct Headers_IP6Header* header = (struct Headers_IP6Header*) message->bytes;
    return (memcmp(header->destinationAddr, context->myAddr.ip6.bytes, 16) == 0);
}

// Called by the TUN device.
static inline uint8_t ip6FromTun(struct Message* message,
                                 struct Interface* interface)
{
    struct Ducttape_Private* context = (struct Ducttape_Private*) interface->receiverContext;

    struct Headers_IP6Header* header = (struct Headers_IP6Header*) message->bytes;

    if (memcmp(header->sourceAddr, context->myAddr.ip6.bytes, 16)) {
        uint8_t expectedSource[40];
        AddrTools_printIp(expectedSource, context->myAddr.ip6.bytes);
        uint8_t packetSource[40];
        AddrTools_printIp(packetSource, header->sourceAddr);
        Log_warn(context->logger,
                 "dropped packet from [%s] because all messages must have source address [%s]",
                 (char*) packetSource, (char*) expectedSource);
        return Error_INVALID;
    }
    if (!memcmp(header->destinationAddr, context->myAddr.ip6.bytes, 16)) {
        // I'm Gonna Sit Right Down and Write Myself a Letter
        interface->sendMessage(message, interface);
        return Error_NONE;
    }

    context->switchHeader = NULL;

    struct Node* bestNext = RouterModule_lookup(header->destinationAddr, context->routerModule);
    context->forwardTo = bestNext;
    if (bestNext) {
        if (!memcmp(header->destinationAddr, bestNext->address.ip6.bytes, 16)) {
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
    }
    // else { we don't have *that* node in our store, proceed with forwardTo set to null and
    // encrypt the message with a session corrisponding to the destination addr then send it to
    // outgoingFromMe() where the best node to handle it will be looked up. }

    // Grab out the header so it doesn't get clobbered.
    struct Headers_IP6Header headerStore;
    Bits_memcpyConst(&headerStore, header, Headers_IP6Header_SIZE);
    context->ip6Header = &headerStore;

    // Shift over the content.
    Message_shift(message, -Headers_IP6Header_SIZE);

    struct SessionManager_Session* session =
        SessionManager_getSession(headerStore.destinationAddr, NULL, context->sm);

    // This comes out at outgoingFromMe()
    context->layer = INNER_LAYER;
    context->session = session;
    return session->iface.sendMessage(message, &session->iface);
}

/**
 * Messages with content encrypted and header decrypted are sent here to be forwarded.
 * they may come from us, or from another node and may be to us or to any other node.
 * Message is aligned on the beginning of the ipv6 header.
 */
static inline int core(struct Message* message, struct Ducttape_Private* context)
{
    context->ip6Header = (struct Headers_IP6Header*) message->bytes;

    if (isForMe(message, context)) {
        Message_shift(message, -Headers_IP6Header_SIZE);

        if (memcmp(context->session->ip6, context->ip6Header->sourceAddr, 16)) {
            // triple encrypted
            // This call goes to incomingForMe()
            context->layer = INNER_LAYER;
            context->session =
                SessionManager_getSession(context->ip6Header->sourceAddr, NULL, context->sm);
            return context->session->iface.receiveMessage(message, &context->session->iface);
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
            if (memcmp(context->ip6Header->destinationAddr, addr->ip6.bytes, 16)) {
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
 * Message is aligned on the beginning of the crypto header, ip6 header must be reapplied.
 */
static inline uint8_t outgoingFromMe(struct Message* message, struct Ducttape_Private* context)
{
    // Need to set the length field to take into account
    // the crypto headers which are hidden under the ipv6 packet.
    context->ip6Header->payloadLength_be = Endian_hostToBigEndian16(message->length);

    Message_shift(message, Headers_IP6Header_SIZE);

    // If this message is addressed to us, it means the cryptoauth kicked back a response
    // message when we asked it to decrypt a message for us and the ipv6 addresses need to
    // be flipped to send it back to the other node.
    if (isForMe(message, context)) {
        struct Headers_IP6Header* ip6 = (struct Headers_IP6Header*) message->bytes;
        Assert_true(context->ip6Header == ip6);
        Bits_memcpyConst(ip6->destinationAddr, ip6->sourceAddr, 16);
        Bits_memcpyConst(ip6->sourceAddr, &context->myAddr.ip6.bytes, 16);
        // Say it came from us...
        //context->routerAddress = context->myAddr.ip6.bytes; TODO should not be needed
    } else {
        Bits_memcpyConst(message->bytes, context->ip6Header, Headers_IP6Header_SIZE);
    }

    // Forward this call to core() which will check it's validity
    // and since it's not to us, forward it to the correct node.
    return core(message, context);
}

static inline int incomingFromRouter(struct Message* message, struct Ducttape_Private* context)
{
    if (!validEncryptedIP6(message)) {
        Log_debug(context->logger, "Dropping message because of invalid ipv6 header.\n");
        return Error_INVALID;
    }

    //Log_debug(context->logger, "Got message from router.\n");
    return core(message, context);
}

static uint8_t incomingFromCryptoAuth(struct Message* message, struct Interface* iface)
{
    struct Ducttape_Private* context = iface->receiverContext;
    int layer = context->layer;
    context->layer = INVALID_LAYER;
    if (layer == INNER_LAYER) {
        return incomingForMe(message,
                             context,
                             CryptoAuth_getHerPublicKey(&context->session->iface));
    } else if (layer == OUTER_LAYER) {
        return incomingFromRouter(message, context);
    }
    Assert_true(false);
    return 0;
}

static uint8_t outgoingFromCryptoAuth(struct Message* message, struct Interface* iface)
{
    struct Ducttape_Private* context = (struct Ducttape_Private*) iface->senderContext;
    int layer = context->layer;
    context->layer = INVALID_LAYER;
    if (layer == INNER_LAYER) {
        return outgoingFromMe(message, context);
    } else if (layer == OUTER_LAYER) {
        return sendToSwitch(message, context->switchHeader, context);
    }
    Assert_true(false);
    return 0;
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
static uint8_t handleControlMessage(struct Ducttape_Private* context,
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
                          "error packet from [%s] caused by [%s] packet ([%d])",
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
                           "error packet from [%s] in response to ping, err [%d], length: [%d].",
                           labelStr,
                           Endian_bigEndianToHost32(ctrl->content.error.errorType_be),
                           message->length);
                // errors resulting from pings are forwarded back to the pinger.
                pong = true;
            }
        } else if (causeType != Headers_SwitchHeader_TYPE_DATA) {
            Log_info(context->logger,
                      "error packet from [%s] containing cause of unknown type [%d]",
                      labelStr, causeType);
        } else {
            Log_info(context->logger,
                      "error packet from [%s], error type [%d]",
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
    if (ip6[0] != 0xfc) {
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
    struct Ducttape_Private* context = switchIf->senderContext;
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
    uint32_t handle = ((uint32_t*)message->bytes)[0];
    struct SessionManager_Session* session = NULL;

    if (!(handle & HANDLE_FLAG_BIT)) {
        session = SessionManager_sessionForHandle(Endian_bigEndianToHost32(handle), context->sm);
        if (session) {
            Log_debug(context->logger, "Got session using protocol 1 handle");
            Message_shift(message, -4);
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
            session->sendHandle = handle & ~HANDLE_FLAG_BIT;
            session->version = version;
            Log_debug(context->logger, "Got session with protocol version [%d] handle [%u]",
                      version, session->sendHandle);
        }
    }

    // #3 try the message as a protocol version 0 message.
    #ifdef Version_0_COMPAT
        if (!session) {
            int herAddrIndex = AddressMapper_indexOf(switchHeader->label_be, &context->addrMap);
            uint8_t* herKey = NULL;
            if (herAddrIndex == -1) {
                uint64_t label = Endian_bigEndianToHost64(switchHeader->label_be);
                struct Node* n = RouterModule_getNode(label, context->routerModule);
                if (n) {
                    herAddrIndex = AddressMapper_put(switchHeader->label_be,
                                                     n->address.ip6.bytes,
                                                     &context->addrMap);
                    herKey = n->address.key;
                }
            }
            if (herAddrIndex != -1) {
                Log_debug(context->logger, "Handling packet from legacy protocol version 0 node.");
                session = SessionManager_getSession(context->addrMap.entries[herAddrIndex].address,
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

    // This is needed so that the priority and other information
    // from the switch header can be passed on properly.
    context->switchHeader = switchHeader;

    // This goes to incomingFromCryptoAuth()
    // then incomingFromRouter() then core()
    context->layer = OUTER_LAYER;
    context->session = session;
    session->iface.receiveMessage(message, &session->iface);

    return 0;
}

static uint8_t incomingFromPinger(struct Message* message, struct Interface* iface)
{
    struct Ducttape_Private* context = iface->senderContext;
    return context->switchInterface.receiveMessage(message, &context->switchInterface);
}

struct Ducttape* Ducttape_register(uint8_t privateKey[32],
                                   struct DHTModuleRegistry* registry,
                                   struct RouterModule* routerModule,
                                   struct SwitchCore* switchCore,
                                   struct event_base* eventBase,
                                   struct Allocator* allocator,
                                   struct Log* logger,
                                   struct Admin* admin)
{
    struct Ducttape_Private* context =
        allocator->calloc(sizeof(struct Ducttape_Private), 1, allocator);
    context->registry = registry;
    context->routerModule = routerModule;
    context->logger = logger;
    context->forwardTo = NULL;
    #ifdef Version_0_COMPAT
        AddressMapper_init(&context->addrMap);
    #endif

    struct CryptoAuth* cryptoAuth =
        CryptoAuth_new(allocator, privateKey, eventBase, logger);
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
    struct Ducttape_Private* context = (struct Ducttape_Private*) dt;
    context->userIf = userIf;
    userIf->receiveMessage = ip6FromTun;
    userIf->receiverContext = context;
}
