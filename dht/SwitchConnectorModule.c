/*#include "libbenc/benc.h"*/
#include "crypto/CryptoAuth.h"
#include "dht/Address.h"
#include "dht/DHTModules.h"
#include "dht/dhtcore/Node.h"
#include "dht/dhtcore/RouterModule.h"
#include "interface/Interface.h"
#include "interface/InterfaceMap.h"
#include "interface/SessionManager.h"
#include "memory/MemAllocator.h"
#include "memory/BufferAllocator.h"
#include "switch/SwitchCore.h"
#include "util/Assert.h"
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
    const struct DHTModuleRegistry* registry;

    const struct SwitchCore* switchCore;

    /** A memory allocator which will be reset after each message is sent. */
    const struct MemAllocator* perMessageAllocator;

    /** The allocator for this module. */
    const struct MemAllocator* allocator;

    uint8_t myAddress[16];

    struct CryptoAuth* cryptoAuth;

    struct RouterModule* routerModule;

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

    struct Interface contentSmOutside;
    struct Interface* contentSmInside;

    struct event_base* eventBase;
};

/*--------------------Prototypes--------------------*/
static int handleOutgoing(struct DHTMessage* message,
                          void* vcontext);

/*--------------------Interface--------------------*/

/*
static inline void incomingIP6(struct Message* message,
                               struct Interface* interface,
                               struct Headers_SwitchHeader* header,
                               struct Context context)
{

}*/

static inline void incomingDHT(struct Message* message,
                               struct Address* addr,
                               struct Context* context)
{

printf("yay we got a dht message!!11\n");
message = message;
addr = addr;
context = context;

/*
    struct DHTMessage dht;
    memset(&dht, 0, sizeof(struct DHTMessage));

    // TODO: These copies are not necessary at all.
    const uint32_t length =
        (message->length < MAX_MESSAGE_SIZE) ? message->length : MAX_MESSAGE_SIZE;
    memcpy(&dht->peerAddress, header->label_be, 8);
    dht.addressLength = 8;
    memcpy(&dht->bytes, message->bytes, length);

    dht.allocator = context->perMessageAllocator;
    // This is a bufferAllocator, free resets it to 0.
    context->perMessageAllocator->free(context->perMessageAllocator);

    DHTModules_handleIncoming(&dht, context->registry);*/
}

struct UDPHeader {
    uint32_t sourceAndDestPorts;
    uint16_t length;
    uint16_t checksum;
};
Assert_assertTrue(sizeof(struct UDPHeader) == 8);

static int handleOutgoing(struct DHTMessage* dmessage,
                          void* vcontext)
{
    struct Context* context = (struct Context*) vcontext;

    printf("Yay, the router tried to send something!\n");

    struct Message message =
        { .length = dmessage->length, .bytes = (uint8_t*) dmessage->bytes, .padding = 512 };
assert(message.length == 43);
    Message_shift(&message, sizeof(struct UDPHeader));
    struct UDPHeader* uh = (struct UDPHeader*) message.bytes;
    uh->sourceAndDestPorts = 0;
    uh->length = Endian_hostToBigEndian16(dmessage->length);
    uh->checksum = 0;

    struct Headers_IP6Header header =
    {
        // Length will be set after the crypto.
        //.payloadLength_be = Endian_hostToBigEndian16(dmessage->length + sizeof(struct UDPHeader)),
        .nextHeader = 17,
        .hopLimit = 255
    };

    memcpy(&header.destinationAddr,
           dmessage->address->ip6.bytes,
           Address_SEARCH_TARGET_SIZE);

    memcpy(&header.sourceAddr,
           context->myAddress,
           Address_SEARCH_TARGET_SIZE);

    context->ip6Header = &header;

    struct Headers_SwitchHeader switchHeader;
    memset(&switchHeader, 0, sizeof(struct Headers_SwitchHeader));
    context->switchHeader = &switchHeader;
assert(message.length == 51);

    SessionManager_setKey(&message, dmessage->address->key, context->contentSmInside);
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
    struct UDPHeader* uh = (struct UDPHeader*) (ip6 + 1);
    return uh->sourceAndDestPorts == 0
        && uh->length == (message->length - sizeof(struct UDPHeader));
}

/**
 * Message which is for us, message is aligned on the beginning of the content.
 * this is called from decryptedIncoming() which calls through an interfaceMap.
 */
static void incomingForMe(struct Message* message, struct Interface* iface)
{
    struct Context* context = (struct Context*) iface->receiverContext;

    // See "Hack" below.
    Message_shift(message, 16);

    if (isRouterTraffic(message, context->ip6Header)) {
        struct Address addr;
        memcpy(addr.ip6.bytes, context->ip6Header->sourceAddr, 16);
        memcpy(addr.networkAddress, &context->switchHeader->label_be, 8);
        incomingDHT(message, &addr, context);
        return;
    }
    printf("Dropped a message which should have been sent to the TUN device...\n");
}

/**
 * Send a message to another switch.
 * Switchheader will precede the message.
 */
static inline uint8_t sendToSwitch(struct Message* message,
                                   struct Headers_SwitchHeader* destinationSwitchHeader,
                                   struct Context* context)
{
    struct Headers_SwitchHeader* switchHeaderLocation =
        (struct Headers_SwitchHeader*) (message->bytes - Headers_SwitchHeader_SIZE);
    if (destinationSwitchHeader != switchHeaderLocation) {
        assert(message->padding >= Headers_SwitchHeader_SIZE);
        memmove(message->bytes - Headers_SwitchHeader_SIZE,
                destinationSwitchHeader,
                sizeof(struct Headers_SwitchHeader));
    }
    Message_shift(message, Headers_SwitchHeader_SIZE);
printf("Sending message to switch\n");
assert(message->length == 51+120+40+120+12);
    context->switchInterface.receiveMessage(message, &context->switchInterface);

    return 0;
}

// for responses coming back from the CryptoAuth session.
// Will cause receiveMessage() to be called on the switch interface.
static uint8_t sendToSwitchFromCryptoAuth(struct Message* message, struct Interface* iface)
{
assert(message->length == 51+120+40+120);
    struct Context* context = (struct Context*) iface->senderContext;
    return sendToSwitch(message, context->switchHeader, context);
}

static void receivedFromCryptoAuth(struct Message* message, struct Interface* iface)
{
    struct Context* context = iface->receiverContext;
    context->messageFromCryptoAuth = message;
}

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
        memcpy(message->bytes, &sendTo->session.nextNonce, 4);
        sendTo->session.nextNonce++;

        memcpy(&context->switchHeader->label_be,
               sendTo->address.networkAddress,
               Address_NETWORK_ADDR_SIZE);
        return sendToSwitch(message, context->switchHeader, context);
    }

    // We have to copy out the switch header because it
    // will probably be clobbered by the crypto headers.
    struct Headers_SwitchHeader header;
    memcpy(&header, context->switchHeader, sizeof(struct Headers_SwitchHeader));
    memcpy(&header.label_be, sendTo->address.networkAddress, Address_NETWORK_ADDR_SIZE);
    context->switchHeader = &header;
    struct Interface* session = getCaSession(&header, sendTo->address.key, context);
assert(message->length == 51+120+40);
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
    return (memcmp(header->destinationAddr, context->myAddress, 16) == 0);
}

/**
 * after being decrypted, message is sent here.
 * Message is aligned on the beginning of the ipv6 header.
 */
static inline uint8_t decryptedIncoming(struct Message* message, struct Context* context)
{
    context->ip6Header = (struct Headers_IP6Header*) message->bytes;

    if (!validIP6(message)) {
        printf("Dropping message because of invalid ipv6 header.");
        return 0;
    }

    if (isForMe(message, context)) {
        // Hack: align the message on a point 16 bytes behind the end of the ipv6 header
        //       then tell the SessionManager that the key offset is -16 making it use the
        //       source address as the key.
        // TODO: fix the SessionManager so it can heve different offsets for incoming and outgoing.
        Message_shift(message, -(Headers_IP6Header_SIZE - 16));

        // This call goes to incomingForMe()
        context->contentSmOutside.receiveMessage(message, &context->contentSmOutside);
        return 0;
    }
assert(message->length == 51+120+40);
    struct Node* nextBest = RouterModule_getNextBest(context->ip6Header->destinationAddr,
                                                     context->routerModule);
    return sendToRouter(nextBest, message, context);
}

/**
 * When we send a message it goes into the SessionManager.
 * for the content level crypto then it comes here.
 * Message is aligned on the beginning of the crypto header, ip6 header must be reapplied.
 */
static uint8_t outgoingFromMe(struct Message* message, struct Interface* iface)
{
    struct Context* context = (struct Context*) iface->senderContext;
assert(message->length == 51+120);
    // Need to set the length field to take into account
    // the crypto headers which are hidden under the ipv6 packet.
    context->ip6Header->payloadLength_be = Endian_hostToBigEndian16(message->length);

    Message_shift(message, Headers_IP6Header_SIZE);
    memcpy(message->bytes, context->ip6Header, Headers_IP6Header_SIZE);
    // Forward this call to decryptedIncoming() which will check it's validity
    // and since it's not to us, forward it to the correct node.
    return decryptedIncoming(message, context);
}

/**
 * This is called as sendMessage() by the switch.
 * There is only one switch interface which sends all traffic.
 * message is aligned on the end of the switch header.
 */
static uint8_t incomingFromSwitch(struct Message* message, struct Interface* switchIf)
{
    struct Context* context = switchIf->senderContext;
    struct Headers_SwitchHeader* switchHeader =
        (struct Headers_SwitchHeader*) message->bytes - sizeof(struct Headers_SwitchHeader);

    uint32_t messageType = Headers_getMessageType(switchHeader);
    if (messageType != 0) {
        // TODO: handle this
        fprintf(stderr,
                "A message with a non-zero type, perhaps an error. messageType=%d",
                messageType);
        return 0;
    }

    uint32_t nonce = *((uint32_t*)message->bytes);
    if (nonce != 0) {
        struct Node* node = RouterModule_getNode((uint8_t*) &switchHeader->label_be,
                                                 context->routerModule);
        if (node->session.exists) {
            Message_shift(message, -4);
            decrypt(nonce, message, node->session.sharedSecret, node->session.isInitiator);
            return decryptedIncoming(message, context);
        }
        // Got a message which we can't handle, drop it and ping,
        // causing a session to be negotiated.
        RouterModule_pingNode((uint8_t*) &switchHeader->label_be, context->routerModule);
        return 0;
    }

    // Nonce is 0, this is a crypto negotiation.
    struct Interface* iface = getCaSession(switchHeader, NULL, context);

    // Null the message in the context then call cryptoAuth and if
    // it's nolonger null then the message is valid :/
    context->messageFromCryptoAuth = NULL;

    // If the message causes CryptoAuth to want to send a response, it will call sendToSwitch
    // and sendToSwitch (as well as a bunch of other stuff) relies on the switchHeader being in
    // the context.
    context->switchHeader = switchHeader;

    iface->receiveMessage(message, iface);

    if (context->messageFromCryptoAuth) {
        return decryptedIncoming(context->messageFromCryptoAuth, context);
    } else {
        printf("invalid (?) message was eaten by the cryptoAuth");
    }

    return 0;
}

int SwitchConnectorModule_register(uint8_t privateKey[32],
                                   struct DHTModuleRegistry* registry,
                                   struct RouterModule* routerModule,
                                   struct SwitchCore* switchCore,
                                   struct event_base* eventBase,
                                   struct MemAllocator* allocator)
{
    struct Context* context = allocator->malloc(sizeof(struct Context), allocator);
    context->ifMap = InterfaceMap_new(8, allocator);
    context->registry = registry;
    context->routerModule = routerModule;
    context->switchCore = switchCore;
    context->allocator = allocator;
    context->cryptoAuth = CryptoAuth_new(allocator, privateKey);
    uint8_t myPubKey[32];
    CryptoAuth_getPublicKey(myPubKey, context->cryptoAuth);
    AddressCalc_addressForPublicKey(context->myAddress, myPubKey);

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
    // The key is 16 bytes long (ipv6 address) and it is at index -16, if it's incoming traffic
    // there's a hack which shifts the pointer back by another 16 so -16 is the source address
    // instead. See: "Hack" above.
    context->contentSmInside =
        SessionManager_wrapInterface(16,
                                     -16,
                                     &context->contentSmOutside,
                                     eventBase,
                                     context->cryptoAuth,
                                     allocator);
    context->contentSmInside->receiveMessage = incomingForMe;
    context->contentSmInside->receiverContext = context;

    memcpy(&context->module, &(struct DHTModule) {
        .name = "SwitchConnectorModule",
        .context = context,
        .handleOutgoing = handleOutgoing
    }, sizeof(struct DHTModule));

    memcpy(&context->switchInterface, &(struct Interface) {
        .sendMessage = incomingFromSwitch,
        .senderContext = context,
        .allocator = allocator
    }, sizeof(struct DHTModule));

    return DHTModules_register(&context->module, registry)
        | SwitchCore_setRouterInterface(&context->switchInterface, switchCore);
}
