#include "crypto/CryptoAuth.h"
#include "log/Log.h"
#include "dht/Address.h"
#include "dht/DHTModules.h"
#include "dht/dhtcore/Node.h"
#include "dht/dhtcore/RouterModule.h"
#include "dht/Ducttape.h"
#include "interface/Interface.h"
#include "interface/InterfaceMap.h"
#include "interface/SessionManager.h"
#include "log/Log.h"
#include "memory/MemAllocator.h"
#include "memory/BufferAllocator.h"
#include "switch/SwitchCore.h"
#include "util/Bits.h"
#include "wire/Headers.h"

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
    /** The network module. */
    struct DHTModule module;

    struct Interface switchInterface;

    /** The registry to call when a message comes in. */
    struct DHTModuleRegistry* registry;

    struct SwitchCore* switchCore;

    /** A memory allocator which will be reset after each message is sent. */
    struct MemAllocator* perMessageAllocator;

    /** The allocator for this module. */
    struct MemAllocator* allocator;

    struct Address myAddr;

    struct CryptoAuth* cryptoAuth;

    struct RouterModule* routerModule;

    /** The interface which is used by the operator of the node to communicate in the network. */
    struct Interface* routerIf;

    /**
     * A map of interfaces by switch label.
     * Used for CryptoAuth sessions which are in the process of being setup.
     * This is for the outer layer of crypto (router-to-router)
     */
    struct InterfaceMap* ifMap;

    /** This is set by incomingFromSwitch. */
    struct Headers_SwitchHeader* switchHeader;

    /** This is set in decryptedIncoming() and expected by incomingForMe(). */
    struct Headers_IP6Header* ip6Header;

    /** Catch an incoming message after it runs through the crypto authenticator. */
    struct Message* messageFromCryptoAuth;
    uint8_t* herPublicKey;

    struct Interface contentSmOutside;
    struct Interface* contentSmInside;

    struct Interface* contentSession;

    struct event_base* eventBase;

    struct Log* logger;
};

/*--------------------Prototypes--------------------*/
static int handleOutgoing(struct DHTMessage* message,
                          void* vcontext);

static inline void incomingDHT(struct Message* message,
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
    dht.allocator = context->perMessageAllocator;
    // This is a bufferAllocator, free resets it to 0.
    context->perMessageAllocator->free(context->perMessageAllocator);

    DHTModules_handleIncoming(&dht, context->registry);
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
        .hopLimit = 255
    };

    memcpy(&header.destinationAddr,
           dmessage->address->ip6.bytes,
           Address_SEARCH_TARGET_SIZE);

    memcpy(&header.sourceAddr,
           context->myAddr.ip6.bytes,
           Address_SEARCH_TARGET_SIZE);

    context->ip6Header = &header;

    struct Headers_SwitchHeader switchHeader;
    memset(&switchHeader, 0, sizeof(struct Headers_SwitchHeader));
    context->switchHeader = &switchHeader;

    // Create a pseudoheader behind the message header.
    memcpy(message.bytes - 16,
           dmessage->address->ip6.bytes,
           Address_SEARCH_TARGET_SIZE);

    SessionManager_setKey(&message, dmessage->address->key, true, context->contentSmInside);
    // This comes out at outgoingFromMe()
    assert(!context->contentSmInside->sendMessage(&message, context->contentSmInside));
    return 0;
}

// Aligned on the beginning of the content.
static inline bool isRouterTraffic(struct Message* message, struct Headers_IP6Header* ip6)
{
    if (ip6->nextHeader != 17) {
        return false;
    }
    // TODO: validate the checksum
    struct Headers_UDPHeader* uh = (struct Headers_UDPHeader*) message->bytes;
    return uh->sourceAndDestPorts == 0
        && Endian_bigEndianToHost16(uh->length_be) == message->length - Headers_UDPHeader_SIZE;
}

/**
 * Message which is for us, message is aligned on the beginning of the content.
 * this is called from decryptedIncoming() which calls through an interfaceMap.
 */
static void incomingForMe(struct Message* message, struct Interface* iface)
{
    struct Context* context = (struct Context*) iface->receiverContext;
    if (isRouterTraffic(message, context->ip6Header)) {
        struct Address addr;
        memcpy(addr.ip6.bytes, context->ip6Header->sourceAddr, 16);
        addr.networkAddress_be = context->switchHeader->label_be;
        uint8_t* key = CryptoAuth_getHerPublicKey(context->contentSession);
        memcpy(addr.key, key, 32);
        // Shift off the UDP header.
        Message_shift(message, -Headers_UDPHeader_SIZE);
        incomingDHT(message, &addr, context);
        return;
    }

    if (context->routerIf) {
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
    } else {
        Log_warn(context->logger,
                 "Dropping message because there is no router interface configured.\n");
    }
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

    context->switchInterface.receiveMessage(message, &context->switchInterface);

    return 0;
}

// for responses coming back from the CryptoAuth session.
// Will cause receiveMessage() to be called on the switch interface.
static uint8_t sendToSwitchFromCryptoAuth(struct Message* message, struct Interface* iface)
{
    struct Context* context = (struct Context*) iface->senderContext;
    return sendToSwitch(message, context->switchHeader, context);
}

static void receivedFromCryptoAuth(struct Message* message, struct Interface* iface);

static inline struct Interface* getCaSession(struct Headers_SwitchHeader* header,
                                             uint8_t key[32],
                                             struct Context* context)
{
    int index = InterfaceMap_indexOf((uint8_t*) &header->label_be, context->ifMap);
    if (index > -1) {
        return context->ifMap->interfaces[index];
    }

    struct Interface* iface;
    // TODO: this can be done faster.
    // This is just a dirty way of giving CryptoAuth what it wants, a pair of interfaces.
    // All we really want is a way to pipe messages through a given CryptoAuth session.
    struct MemAllocator* child = context->allocator->child(context->allocator);
    struct Interface* outerIf =
        child->clone(sizeof(struct Interface), child, &(struct Interface) {
            .sendMessage = sendToSwitchFromCryptoAuth,
            .senderContext = context,
            .allocator = child,
        });
    struct Interface* innerIf =
        CryptoAuth_wrapInterface(outerIf, key, false, false, context->cryptoAuth);
    innerIf->receiveMessage = receivedFromCryptoAuth;
    innerIf->receiverContext = context;
    iface = child->clone(sizeof(struct Interface), child, &(struct Interface) {
        .sendMessage = innerIf->sendMessage,
        .senderContext = innerIf->senderContext,
        .receiveMessage = outerIf->receiveMessage,
        .receiverContext = outerIf->receiverContext,
        .allocator = child,
    });

    struct timeval now;
    event_base_gettimeofday_cached(context->eventBase, &now);
    InterfaceMap_put((uint8_t*) &header->label_be, iface, now.tv_sec, context->ifMap);

    return iface;
}

static inline int encrypt(uint32_t nonce,
                          struct Message* msg,
                          uint8_t secret[32],
                          bool isInitiator)
{
    union {
        uint32_t ints[2];
        uint8_t bytes[24];
    } nonceAs = { .ints = {0, 0} };
    nonceAs.ints[isInitiator] = nonce;

    return crypto_stream_salsa20_xor(msg->bytes, msg->bytes, msg->length, nonceAs.bytes, secret);
}
#define decrypt(nonce, msg, secret, isInitiator) encrypt(nonce, msg, secret, !(isInitiator))

/** Message must not be encrypted and must be aligned on the beginning of the ipv6 header. */
static inline uint8_t sendToRouter(struct Node* sendTo,
                                   struct Message* message,
                                   struct Context* context)
{
    if (sendTo->session.exists) {
        encrypt(sendTo->session.nextNonce,
                message,
                sendTo->session.sharedSecret,
                sendTo->session.isInitiator);
        Message_shift(message, 4);

        ((union Headers_CryptoAuth*) message->bytes)->nonce =
            Endian_hostToBigEndian32(sendTo->session.nextNonce);

        sendTo->session.nextNonce++;

        context->switchHeader->label_be = sendTo->address.networkAddress_be;

        return sendToSwitch(message, context->switchHeader, context);
    }

    // We have to copy out the switch header because it
    // will probably be clobbered by the crypto headers.
    struct Headers_SwitchHeader header;
    memcpy(&header, context->switchHeader, sizeof(struct Headers_SwitchHeader));
    header.label_be = sendTo->address.networkAddress_be;
    context->switchHeader = &header;
    struct Interface* session = getCaSession(&header, sendTo->address.key, context);
    // This comes out in sendToSwitchFromCryptoAuth()
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
static inline void ip6FromTun(struct Message* message,
                              struct Interface* interface)
{
    struct Context* context = (struct Context*) interface->receiverContext;

    if (!validIP6(message)) {
        Log_debug(context->logger, "dropped message from TUN because it was not valid IPv6.\n");
        return;
    }

    struct Headers_IP6Header header;
    memcpy(&header, message->bytes, Headers_IP6Header_SIZE);

    if (memcmp(&header.sourceAddr, context->myAddr.ip6.bytes, Address_SEARCH_TARGET_SIZE)) {
        Log_warn(context->logger, "dropped message because only one address is allowed to be used "
                                  "and the source address was different.\n");
        return;
    }

    context->ip6Header = &header;

    struct Headers_SwitchHeader switchHeader;
    memset(&switchHeader, 0, sizeof(struct Headers_SwitchHeader));
    context->switchHeader = &switchHeader;

    Message_shift(message, -Headers_IP6Header_SIZE);

    // This comes out at outgoingFromMe()
    assert(!context->contentSmInside->sendMessage(message, context->contentSmInside));
}

/**
 * after being decrypted, message is sent here.
 * Message is aligned on the beginning of the ipv6 header.
 */
static inline uint8_t decryptedIncoming(struct Message* message, struct Context* context)
{
    context->ip6Header = (struct Headers_IP6Header*) message->bytes;

    if (!validIP6(message)) {
        Log_debug(context->logger, "Dropping message because of invalid ipv6 header.\n");
        return 0;
    }

    if (isForMe(message, context)) {
        Message_shift(message, -Headers_IP6Header_SIZE);
        // This call goes to incomingForMe()
        context->contentSession =
            SessionManager_getSession(message, false, context->contentSmInside);
        context->contentSession->receiveMessage(message, context->contentSession);
        return 0;
    }

    if (context->ip6Header->hopLimit == 0) {
        Log_debug(context->logger, "dropped message because hop limit has been exceeded.\n");
    }
    context->ip6Header->hopLimit--;

    struct Node* nextBest = RouterModule_getNextBest(context->ip6Header->destinationAddr,
                                                     context->routerModule);
    if (nextBest) {
        #ifdef Log_DEBUG
            uint8_t netAddr[20];
            Address_printNetworkAddress(netAddr, &nextBest->address);
            Log_debug1(context->logger, "Forwarding packet to %s\n", netAddr);
        #endif
        return sendToRouter(nextBest, message, context);
    }
    Log_debug(context->logger, "Dropped message because this node is the closest known "
                               "node to the destination.\n");
    return 0;
}

/**
 * When we send a message it goes into the SessionManager.
 * for the content level crypto then it comes here.
 * Message is aligned on the beginning of the crypto header, ip6 header must be reapplied.
 */
static uint8_t outgoingFromMe(struct Message* message, struct Interface* iface)
{
    struct Context* context = (struct Context*) iface->senderContext;

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

    // Forward this call to decryptedIncoming() which will check it's validity
    // and since it's not to us, forward it to the correct node.
    return decryptedIncoming(message, context);
}

static void receivedFromCryptoAuth(struct Message* message, struct Interface* iface)
{
    struct Context* context = iface->receiverContext;
    context->messageFromCryptoAuth = message;
    RouterModule_addNode(context->herPublicKey, 
                         context->switchHeader->label_be,
                         context->routerModule);
    decryptedIncoming(message, context);
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

    uint32_t messageType = Headers_getMessageType(switchHeader);
    if (messageType != 0) {
        // TODO: handle this
        fprintf(stderr,
                "A message with a non-zero type, perhaps an error. messageType=%d\n",
                messageType);
        return 0;
    }

    // The label comes in reversed from the switch because the switch doesn't know that we aren't
    // another switch ready to parse more bits, bit reversing the label yields the source address.
    switchHeader->label_be = Bits_bitReverse64(switchHeader->label_be);

    struct Node* node = RouterModule_getNode(switchHeader->label_be, context->routerModule);

    union Headers_CryptoAuth* caHeader = (union Headers_CryptoAuth*) message->bytes;

    uint8_t* herKey;
    if (node) {
        herKey = node->address.key;
    } else if (message->length < Headers_CryptoAuth_SIZE) {
        Log_debug(context->logger, "Dropped runt packet.\n");
        return 0;
    } else {
        herKey = caHeader->handshake.publicKey;
    }

    uint32_t nonce = Endian_bigEndianToHost32(caHeader->nonce);

    // The address is extracted from the switch header later.
    context->switchHeader = switchHeader;

    if (nonce > 4 && node && node->session.exists) {
        Message_shift(message, -4);
        decrypt(nonce, message, node->session.sharedSecret, node->session.isInitiator);
        return decryptedIncoming(message, context);
    }

    // Nonce is <4, this is a crypto negotiation.
    struct Interface* iface = getCaSession(switchHeader, NULL, context);

    // If it's past negotiation then copy the session into the node
    // then the ca session can be freed.
    if (nonce > 4 && node) {
        CryptoAuth_getSession(&node->session, iface);
    }

    // Null the message in the context then call cryptoAuth and if
    // it's nolonger null then the message is valid :/
    context->messageFromCryptoAuth = NULL;
    context->herPublicKey = herKey;

    // This goes to receivedFromCryptoAuth()
    // then decryptedIncoming()
    iface->receiveMessage(message, iface);

    if (!context->messageFromCryptoAuth) {
        Log_debug(context->logger, "invalid (?) message was eaten by the cryptoAuth\n");
    }

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
    struct Context* context = allocator->malloc(sizeof(struct Context), allocator);
    context->ifMap = InterfaceMap_new(8, allocator);
    context->registry = registry;
    context->routerModule = routerModule;
    context->switchCore = switchCore;
    context->allocator = allocator;
    context->logger = logger;
    context->cryptoAuth = CryptoAuth_new(config, allocator, privateKey, eventBase, logger);
    CryptoAuth_getPublicKey(context->myAddr.key, context->cryptoAuth);
    Address_getPrefix(&context->myAddr);

    if (routerIf) {
        context->routerIf = routerIf;
        routerIf->receiveMessage = ip6FromTun;
        routerIf->receiverContext = context;
    }

    #define PER_MESSAGE_BUF_SZ 16384
    uint8_t* messageBuffer = allocator->malloc(PER_MESSAGE_BUF_SZ, allocator);
    context->perMessageAllocator = BufferAllocator_new(messageBuffer, PER_MESSAGE_BUF_SZ);

    // This is the sessionManager which encrypts and decrypts content which is going from or to
    // This node, this is the innermost level of crypto.
    memcpy(&context->contentSmOutside, &(struct Interface) {
        .sendMessage = outgoingFromMe,
        .senderContext = context,
        .allocator = allocator
    }, sizeof(struct Interface));
    // The key is 16 bytes long (ipv6 address) and it is at index -32 for incoming
    // and -16 for outgoing traffic.
    context->contentSmInside =
        SessionManager_wrapInterface(16,
                                     -32,
                                     -16,
                                     &context->contentSmOutside,
                                     eventBase,
                                     context->cryptoAuth,
                                     allocator);
    context->contentSmInside->receiveMessage = incomingForMe;
    context->contentSmInside->receiverContext = context;

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
