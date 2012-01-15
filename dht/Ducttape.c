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
#include "util/Log.h"
#include "dht/Address.h"
#include "dht/AddressMapper.h"
#include "dht/DHTModules.h"
#include "dht/dhtcore/Node.h"
#include "dht/dhtcore/RouterModule.h"
#include "dht/Ducttape.h"
#include "interface/Interface.h"
#include "interface/InterfaceMap.h"
#include "interface/SessionManager.h"
#include "util/Log.h"
#include "memory/MemAllocator.h"
#include "memory/BufferAllocator.h"
#include "switch/SwitchCore.h"
#include "util/Bits.h"
#include "wire/Control.h"
#include "wire/Error.h"
#include "wire/Headers.h"
#include "wire/MessageType.h"

#include "crypto_stream_salsa20.h"

#include <assert.h>
#include <stdint.h>
#include <event2/event.h>

/**
 * A network module which connects the DHT router to the SwitchCore.
 * This module's job is to grab messages off of the switch,
 * determine the peer's address,
 * map the message to the appropriate CryptoAuth obj and decrypt,
 * and send the message toward the DHT core.
 */

struct Context
{
    /** The network module for the DHT. */
    struct DHTModule module;

    /** The registry to call when a message comes in. */
    struct DHTModuleRegistry* registry;

    /** The DHT router module. */
    struct RouterModule* routerModule;


    /** The interface which interacts with the switch core. */
    struct Interface switchInterface;

    /** The interface which is used by the operator of the node to communicate in the network. */
    struct Interface* routerIf;

    struct Address myAddr;

    struct SessionManager* sm;

    struct AddressMapper addrMap;

    struct event_base* eventBase;

    struct Log* logger;

    // Changes on a message-by-message basis.

    /** This is set by incomingFromSwitch. */
    struct Headers_SwitchHeader* switchHeader;

    /** This is set in core() and expected by incomingForMe(). */
    struct Headers_IP6Header* ip6Header;

    /**
     * NULL unless this is router-to-router traffic.
     * router-to-router traffic MUST NOT be forwarded, therefor it must be sent to the switch.
     */
    struct Address* forwardTo;

    struct Interface* contentSession;

    /** whether we are encrypting/decrypting the inner layer or the outer layer. */
    int layer;
};

/** Used as a sanity check on the layer code. */
#define INVALID_LAYER 0

/** Encrypting/decrypting the content. */
#define INNER_LAYER 1

/** Encrypting/decrypting the ipv6 header. */
#define OUTER_LAYER 2

/** Size of the per-message workspace. */
#define PER_MESSAGE_BUF_SZ 8192

/*--------------------Prototypes--------------------*/
static int handleOutgoing(struct DHTMessage* message,
                          void* vcontext);

static inline uint8_t incomingDHT(struct Message* message,
                                  struct Address* addr,
                                  struct Context* context)
{
    struct DHTMessage dht;
    memset(&dht, 0, sizeof(struct DHTMessage));

    // TODO: These copies are not necessary at all.
    const uint32_t length =
        (message->length < MAX_MESSAGE_SIZE) ? message->length : MAX_MESSAGE_SIZE;
    memcpy(dht.bytes, message->bytes, length);

    dht.address = addr;

    uint8_t buffer[PER_MESSAGE_BUF_SZ];
    dht.allocator = BufferAllocator_new(buffer, PER_MESSAGE_BUF_SZ);;

    DHTModules_handleIncoming(&dht, context->registry);

    // TODO: return something meaningful.
    return Error_NONE;
}

static int handleOutgoing(struct DHTMessage* dmessage,
                          void* vcontext)
{
    struct Context* context = (struct Context*) vcontext;

    struct Message message =
        { .length = dmessage->length, .bytes = (uint8_t*) dmessage->bytes, .padding = 512 };

    Message_shift(&message, Headers_UDPHeader_SIZE);
    struct Headers_UDPHeader* uh = (struct Headers_UDPHeader*) message.bytes;
    uh->sourceAndDestPorts = 0;
    uh->length_be = Endian_hostToBigEndian16(dmessage->length);
    uh->checksum_be = 0;

    struct Headers_IP6Header header =
    {
        // Length will be set after the crypto.
        .nextHeader = 17,

        // control messages MUST NOT be forwarded.
        .hopLimit = 1
    };

    memcpy(header.destinationAddr,
           dmessage->address->ip6.bytes,
           Address_SEARCH_TARGET_SIZE);

    memcpy(header.sourceAddr,
           context->myAddr.ip6.bytes,
           Address_SEARCH_TARGET_SIZE);

    context->ip6Header = &header;
    context->forwardTo = dmessage->address;
    context->switchHeader = NULL;

    struct Interface* session =
        SessionManager_getSession(dmessage->address->ip6.bytes, dmessage->address->key, context->sm);

    // This comes out at outgoingFromMe()
    context->layer = INNER_LAYER;
    session->sendMessage(&message, session);

    context->forwardTo = NULL;
    return 0;
}

// Aligned on the beginning of the content.
static inline bool isRouterTraffic(struct Message* message, struct Headers_IP6Header* ip6)
{
    if (ip6->nextHeader != 17 || ip6->hopLimit != 0) {
        return false;
    }

    struct Headers_UDPHeader* uh = (struct Headers_UDPHeader*) message->bytes;
    return uh->sourceAndDestPorts == 0
        && Endian_bigEndianToHost16(uh->length_be) == message->length - Headers_UDPHeader_SIZE;
}

/**
 * Message which is for us, message is aligned on the beginning of the content.
 * this is called from core() which calls through an interfaceMap.
 */
static inline uint8_t incomingForMe(struct Message* message, struct Context* context)
{
    struct Address addr;
    uint8_t* key = CryptoAuth_getHerPublicKey(context->contentSession);
    AddressCalc_addressForPublicKey(addr.ip6.bytes, key);

    if (memcmp(addr.ip6.bytes, context->ip6Header->sourceAddr, 16)) {
        #ifdef Log_DEBUG
            uint8_t keyAddr[40];
            Address_printIp(keyAddr, &addr);
            memcpy(addr.ip6.bytes, context->ip6Header->sourceAddr, 16);
            uint8_t srcAddr[40];
            Address_printIp(srcAddr, &addr);
            Log_debug2(context->logger,
                       "Dropped packet because source address is not same as key.\n"
                       "    %s source addr\n"
                       "    %s hash of key\n",
                       srcAddr,
                       keyAddr);
        #endif
        return Error_INVALID;
    }

    if (isRouterTraffic(message, context->ip6Header)) {
        // Shift off the UDP header.
        Message_shift(message, -Headers_UDPHeader_SIZE);
        addr.networkAddress_be = context->switchHeader->label_be;
        memcpy(addr.key, key, 32);
        return incomingDHT(message, &addr, context);
    }

    // RouterModule_addNode(&addr, context->routerModule);

    if (!context->routerIf) {
        Log_warn(context->logger,
                 "Dropping message because there is no router interface configured.\n");
        return Error_UNDELIVERABLE;
    }

    // Now write a message to the TUN device.
    // Need to move the ipv6 header forward up to the content because there's a crypto header
    // between the ipv6 header and the content which just got eaten.
    Message_shift(message, Headers_IP6Header_SIZE);
    uint16_t sizeDiff = message->bytes - (uint8_t*)context->ip6Header;
    context->ip6Header->payloadLength_be =
        Endian_hostToBigEndian16(
            Endian_bigEndianToHost16(context->ip6Header->payloadLength_be) - sizeDiff);
    memmove(message->bytes, context->ip6Header, Headers_IP6Header_SIZE);
    context->routerIf->sendMessage(message, context->routerIf);
    return Error_NONE;
}

/**
 * Send a message to another switch.
 * Switchheader will precede the message.
 */
static inline uint8_t sendToSwitch(struct Message* message,
                                   struct Headers_SwitchHeader* destinationSwitchHeader,
                                   struct Context* context)
{
    Message_shift(message, Headers_SwitchHeader_SIZE);
    struct Headers_SwitchHeader* switchHeaderLocation =
        (struct Headers_SwitchHeader*) message->bytes;

    if (destinationSwitchHeader != switchHeaderLocation) {
        memmove(message->bytes, destinationSwitchHeader, Headers_SwitchHeader_SIZE);
    }

    // This is a test to trap a bug.
    assert(Headers_getPriority(destinationSwitchHeader) == 0);

    return context->switchInterface.receiveMessage(message, &context->switchInterface);
}

/** Header must not be encrypted and must be aligned on the beginning of the ipv6 header. */
static inline uint8_t sendToRouter(struct Address* sendTo,
                                   struct Message* message,
                                   struct Context* context)
{
    // We have to copy out the switch header because it
    // will probably be clobbered by the crypto headers.
    struct Headers_SwitchHeader header;
    if (context->switchHeader) {
        memcpy(&header, context->switchHeader, Headers_SwitchHeader_SIZE);
    } else {
        memset(&header, 0, Headers_SwitchHeader_SIZE);
    }
    header.label_be = sendTo->networkAddress_be;
    context->switchHeader = &header;
    struct Interface* session =
        SessionManager_getSession(sendTo->ip6.bytes, sendTo->key, context->sm);
    // This comes out in outgoingFromCryptoAuth() then sendToSwitch()
    context->layer = OUTER_LAYER;
    return session->sendMessage(message, session);
}

static inline bool validIP6(struct Message* message)
{
    struct Headers_IP6Header* header = (struct Headers_IP6Header*) message->bytes;
    uint16_t length = Endian_bigEndianToHost16(header->payloadLength_be);
    return header->sourceAddr[0] == 0xFC
        && header->destinationAddr[0] == 0xFC
        && length == message->length - Headers_IP6Header_SIZE;
}

static inline bool isForMe(struct Message* message, struct Context* context)
{
    struct Headers_IP6Header* header = (struct Headers_IP6Header*) message->bytes;
    return (memcmp(header->destinationAddr, context->myAddr.ip6.bytes, 16) == 0);
}

// Called by the TUN device.
static inline uint8_t ip6FromTun(struct Message* message,
                                 struct Interface* interface)
{
    struct Context* context = (struct Context*) interface->receiverContext;

    if (!validIP6(message)) {
        Log_debug(context->logger, "dropped message from TUN because it was not valid IPv6.\n");
        return Error_INVALID;
    }

    struct Headers_IP6Header header;
    memcpy(&header, message->bytes, Headers_IP6Header_SIZE);

    if (memcmp(&header.sourceAddr, context->myAddr.ip6.bytes, Address_SEARCH_TARGET_SIZE)) {
        Log_warn(context->logger, "dropped message because only one address is allowed to be used "
                                  "and the source address was different.\n");
        return Error_INVALID;
    }

    context->ip6Header = &header;

    struct Headers_SwitchHeader switchHeader;
    memset(&switchHeader, 0, sizeof(struct Headers_SwitchHeader));
    context->switchHeader = &switchHeader;

    Message_shift(message, -Headers_IP6Header_SIZE);

    struct Interface* session = SessionManager_getSession(header.destinationAddr, NULL, context->sm);

    // This comes out at outgoingFromMe()
    context->layer = INNER_LAYER;
    return session->sendMessage(message, session);
}

/**
 * Messages with content encrypted and header decrypted are sent here to be forwarded.
 * they may come from us, or from another node and may be to us or to any other node.
 * Message is aligned on the beginning of the ipv6 header.
 */
static inline int core(struct Message* message, struct Context* context)
{
    context->ip6Header = (struct Headers_IP6Header*) message->bytes;

    if (!validIP6(message)) {
        Log_debug(context->logger, "Dropping message because of invalid ipv6 header.\n");
        return Error_INVALID;
    }

    if (isForMe(message, context)) {
        Message_shift(message, -Headers_IP6Header_SIZE);
        // This call goes to incomingForMe()
        context->layer = INNER_LAYER;
        context->contentSession =
            SessionManager_getSession(context->ip6Header->sourceAddr, NULL, context->sm);
        return context->contentSession->receiveMessage(message, context->contentSession);
    }

    if (context->ip6Header->hopLimit == 0) {
        Log_debug(context->logger, "0 hop message not addressed to us, broken route.\n");
        return 0;
    }
    context->ip6Header->hopLimit--;

    if (context->forwardTo) {
        // Router traffic, we know where it is to be sent to.
        /*#ifdef Log_DEBUG
            uint8_t printedAddr[60];
            Address_print(printedAddr, context->forwardTo);
            Log_debug1(context->logger, "Sending router traffic to %s\n", printedAddr);
        #endif*/
        return sendToRouter(context->forwardTo, message, context);
    }

    struct Node* nextBest = RouterModule_getBest(context->ip6Header->destinationAddr,
                                                 context->routerModule);
    if (nextBest) {
        #ifdef Log_DEBUG
            uint8_t nhAddr[60];
            Address_print(nhAddr, &nextBest->address);
            if (memcmp(context->ip6Header->destinationAddr, nextBest->address.ip6.bytes, 16)) {
                struct Address destination;
                memcpy(destination.ip6.bytes, context->ip6Header->destinationAddr, 16);
                uint8_t ipAddr[40];
                Address_printIp(ipAddr, &destination);
                Log_debug2(context->logger, "Forwarding data to %s via %s\n", ipAddr, nhAddr);
            } else {
                Log_debug1(context->logger, "Forwarding data to %s (last hop)\n", nhAddr);
            }
        #endif
        return sendToRouter(&nextBest->address, message, context);
    }
    Log_debug(context->logger, "Dropped message because this node is the closest known "
                               "node to the destination.\n");
    return Error_UNDELIVERABLE;
}

/**
 * When we send a message it goes into the CryptoAuth.
 * for the content level crypto then it goes to outgoingFromCryptoAuth then comes here.
 * Message is aligned on the beginning of the crypto header, ip6 header must be reapplied.
 */
static inline uint8_t outgoingFromMe(struct Message* message, struct Context* context)
{
    // Need to set the length field to take into account
    // the crypto headers which are hidden under the ipv6 packet.
    context->ip6Header->payloadLength_be = Endian_hostToBigEndian16(message->length);

    Message_shift(message, Headers_IP6Header_SIZE);
    memcpy(message->bytes, context->ip6Header, Headers_IP6Header_SIZE);

    // If this message is addressed to us, it means the cryptoauth kicked back a response
    // message when we asked it to decrypt a message for us and the ipv6 addresses need to
    // be flipped to send it back to the other node.
    if (isForMe(message, context)) {
        struct Headers_IP6Header* ip6 = (struct Headers_IP6Header*) message->bytes;
        memcpy(ip6->destinationAddr, ip6->sourceAddr, 16);
        memcpy(ip6->sourceAddr, &context->myAddr.ip6.bytes, 16);
    }

    // Forward this call to core() which will check it's validity
    // and since it's not to us, forward it to the correct node.
    return core(message, context);
}

static inline int incomingFromRouter(struct Message* message, struct Context* context)
{
    if (!validIP6(message)) {
        Log_debug(context->logger, "Dropping message because of invalid ipv6 header.\n");
        return Error_INVALID;
    }

    // Do this here and check for 1 hop, not 0 because we want to differentiate between single
    // hop traffic and routed traffic as single hop traffic doesn't need 2 layers of crypto.
    if (((struct Headers_IP6Header*) message->bytes)->hopLimit == 1) {
        Log_debug(context->logger, "dropped message because hop limit has been exceeded.\n");
        // TODO: send back an error message in response.
        return Error_UNDELIVERABLE;
    }

    //Log_debug(context->logger, "Got message from router.\n");
    return core(message, context);
}

static uint8_t incomingFromCryptoAuth(struct Message* message, struct Interface* iface)
{
    struct Context* context = iface->receiverContext;
    int layer = context->layer;
    context->layer = INVALID_LAYER;
    if (layer == INNER_LAYER) {
        return incomingForMe(message, context);
    } else if (layer == OUTER_LAYER) {
        return incomingFromRouter(message, context);
    }
    assert(false);
}

static uint8_t outgoingFromCryptoAuth(struct Message* message, struct Interface* iface)
{
    struct Context* context = (struct Context*) iface->senderContext;
    int layer = context->layer;
    context->layer = INVALID_LAYER;
    if (layer == INNER_LAYER) {
        return outgoingFromMe(message, context);
    } else if (layer == OUTER_LAYER) {
        return sendToSwitch(message, context->switchHeader, context);
    }
    assert(false);
}

static inline uint8_t* extractPublicKey(struct Message* message,
                                        uint64_t switchLabel_be,
                                        struct Log* logger)
{
    union Headers_CryptoAuth* caHeader = (union Headers_CryptoAuth*) message->bytes;
    uint32_t nonce = Endian_bigEndianToHost32(caHeader->nonce);

    if (nonce > 3 && nonce < UINT32_MAX) {
        return NULL;
    }

    if (message->length < Headers_CryptoAuth_SIZE) {
        Log_debug(logger, "Dropped runt packet.\n");
        return NULL;
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
    struct Context* context = switchIf->senderContext;
    struct Headers_SwitchHeader* switchHeader = (struct Headers_SwitchHeader*) message->bytes;
    Message_shift(message, -Headers_SwitchHeader_SIZE);

    // The label comes in reversed from the switch because the switch doesn't know that we aren't
    // another switch ready to parse more bits, bit reversing the label yields the source address.
    switchHeader->label_be = Bits_bitReverse64(switchHeader->label_be);

    if (Headers_getMessageType(switchHeader) == MessageType_CONTROL) {
        struct Control* ctrl = (struct Control*) (switchHeader + 1);
        if (ctrl->type_be == Control_ERROR_be) {
            if (memcmp(&ctrl->content.error.cause.label_be, &switchHeader->label_be, 8)) {
                Log_warn(context->logger,
                         "Different label for cause than return packet, this shouldn't happen. "
                         "Perhaps a packet was corrupted.\n");
                return 0;
            }
            uint32_t errType_be = ctrl->content.error.errorType_be;
            if (errType_be == Endian_bigEndianToHost32(Error_MALFORMED_ADDRESS)) {
                Log_info(context->logger, "Got malformed-address error, removing route.\n");
                RouterModule_brokenPath(switchHeader->label_be, context->routerModule);
                return 0;
            }
            Log_info1(context->logger,
                      "Got error packet, error type: %d",
                      Endian_bigEndianToHost32(errType_be));
        }
        return 0;
    }

    uint8_t* herKey = extractPublicKey(message, switchHeader->label_be, context->logger);
    int herAddrIndex;
    if (herKey) {
        uint8_t herAddrStore[16];
        AddressCalc_addressForPublicKey(herAddrStore, herKey);
        if (herAddrStore[0] != 0xFC) {
            Log_debug(context->logger,
                      "Got message from peer whose address is not in fc00::/8 range.\n");
            return 0;
        }
        herAddrIndex = AddressMapper_put(switchHeader->label_be, herAddrStore, &context->addrMap);
    } else {
        herAddrIndex = AddressMapper_indexOf(switchHeader->label_be, &context->addrMap);
        if (herAddrIndex == -1) {
            struct Node* n = RouterModule_getNode(switchHeader->label_be, context->routerModule);
            if (n) {
                herAddrIndex = AddressMapper_put(switchHeader->label_be,
                                                 n->address.ip6.bytes,
                                                 &context->addrMap);
            } else {
                #ifdef Log_DEBUG
                    uint8_t switchAddr[20];
                    struct Address addr;
                    addr.networkAddress_be = switchHeader->label_be;
                    Address_printNetworkAddress(switchAddr, &addr);
                    Log_debug1(context->logger,
                               "Dropped traffic packet from unknown node. (%s)\n",
                               &switchAddr);
                #endif
                return 0;
            }
        }
    }
    uint8_t* herAddr = context->addrMap.addresses[herAddrIndex];

    // The address is extracted from the switch header later.
    context->switchHeader = switchHeader;

    struct Interface* session = SessionManager_getSession(herAddr, herKey, context->sm);

    // This goes to incomingFromCryptoAuth()
    // then incomingFromRouter() then core()
    context->layer = OUTER_LAYER;
    session->receiveMessage(message, session);

    return 0;
}

int Ducttape_register(Dict* config,
                      uint8_t privateKey[32],
                      struct DHTModuleRegistry* registry,
                      struct RouterModule* routerModule,
                      struct Interface* routerIf,
                      struct SwitchCore* switchCore,
                      struct event_base* eventBase,
                      struct MemAllocator* allocator,
                      struct Log* logger)
{
    struct Context* context = allocator->calloc(sizeof(struct Context), 1, allocator);
    context->registry = registry;
    context->routerModule = routerModule;
    context->logger = logger;
    context->forwardTo = NULL;

    struct CryptoAuth* cryptoAuth = CryptoAuth_new(config, allocator, privateKey, eventBase, logger);
    CryptoAuth_getPublicKey(context->myAddr.key, cryptoAuth);
    Address_getPrefix(&context->myAddr);

    context->sm = SessionManager_new(16,
                                     incomingFromCryptoAuth,
                                     outgoingFromCryptoAuth,
                                     context,
                                     eventBase,
                                     cryptoAuth,
                                     allocator);

    if (routerIf) {
        context->routerIf = routerIf;
        routerIf->receiveMessage = ip6FromTun;
        routerIf->receiverContext = context;
    }

    memcpy(&context->module, &(struct DHTModule) {
        .name = "Ducttape",
        .context = context,
        .handleOutgoing = handleOutgoing
    }, sizeof(struct DHTModule));

    memcpy(&context->switchInterface, &(struct Interface) {
        .sendMessage = incomingFromSwitch,
        .senderContext = context,
        .allocator = allocator
    }, sizeof(struct Interface));

    return DHTModules_register(&context->module, context->registry)
        | SwitchCore_setRouterInterface(&context->switchInterface, switchCore);
}
