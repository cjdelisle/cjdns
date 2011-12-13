/*#include "libbenc/benc.h"*/
#include "dht/DHTModules.h"
#include "net/NetworkTools.h"
#include "memory/MemAllocator.h"
#include "memory/BufferAllocator.h"

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
    const struct DHTModuleRegistry* const registry;

    const struct SwitchCore* const switchCore;

    /** A memory allocator which will be reset after each message is sent. */
    const struct MemAllocator* const perMessageAllocator;

    /** The allocator for this module. */
    const struct MemAllocator* const allocator;

    uint8_t myAddress[16];

    /** This is set by incomingFromSwitch. */
    struct Headers_SwitchHeader* switchHeader;

    /** This is set in decryptedIncoming() and expected by incomingForMe(). */
    struct Headers_Ip6Header* ip6Header;

    struct Interface* contentSm;
};

/*--------------------Prototypes--------------------*/
static int handleOutgoing(struct DHTMessage* message,
                          void* vcontext);

/*--------------------Interface--------------------*/

int SwitchConnectorModule_register(uint8_t privateKey[32],
                                   struct DHTModuleRegistry* dhtRegistry,
                                   struct SwitchCore* switchCore,
                                   struct MemAllocator* allocator)
{
    #define PER_MESSAGE_BUF_SZ 16384
    char* messageBuffer = allocator->malloc(PER_MESSAGE_BUF_SZ, allocator);
    struct MemAllocator* perMessageAllocator =
        BufferAllocator_new(messageBuffer, PER_MESSAGE_BUF_SZ);

    struct Context* context =
        allocator->clone(sizeof(struct Context), allocator, &(struct Context) {
            .module = {
                .name = "SwitchConnectorModule",
                .context = context,
                .handleOutgoing = handleOutgoing
            },
            .switchInterface = {
                .sendMessage = sendMessage,
                .senderContext = senderContext,
                .allocator = allocator
            },
            .registry = registry,
            .switchCore = switchCore,
            .perMessageAllocator = perMessageAllocator,
            .allocator = allocator,
            // Switch labels are used for keys.
            .sessionManager = SessionManager_wrapInterface(
            .cryptoSessionMap = InterfaceMap_new(8, allocator),
            .cryptoAuth = CryptoAuth_new(privateKey, allocator)
        });

    return DHTModules_register(context->module, dhtRegistry)
        | SwitchCore_setRouterInterface(context->switchInterface, switchCore);
}

static int outgoingToSwitch(struct DHTMessage* message, void* vcontext);
{

}

static inline void incomingIP6(struct Message* message,
                               struct Interface* interface,
                               struct Headers_SwitchHeader* header,
                               struct Context context)
{

}

static inline void incomingDHT(struct Message* message,
                               struct Address* addr,
                               struct Context* context)
{
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

    DHTModules_handleIncoming(&dht, context->registry);
}

// Aligned on the beginning of the content.
static inline bool isRouterTraffic(struct Message* message, struct Headers_IP6Header* ip6)
{
    if (ip6->nextHeader != 17) {
        return false;
    }
    // TODO: validate the checksum
    struct UDPHeader {
        uint32_t sourceAndDestPorts;
        uint16_t length;
        uint16_t checksum;
    };
    ASSERT(sizeof(struct UDPHeader) == 8);

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

    if (isRouterTraffic(message, context->ip6Header)) {
        struct Address addr;
        memcpy(addr.ip6.bytes, context->ip6Header->sourceAddr, 16);
        memcpy(addr.networkAddress, context->switchHeader->label_be, 8);
        incomingDHT(message, &addr, context);
        return;
    }
    printf("Dropped a message which should have been sent to the TUN device...");
}

static inline bool validIP6(struct Message* message)
{
    struct Headers_IP6Header* header = message->bytes;
    uint16_t length = Endian_bigEndianToHost16(header->payloadLength_be);
    return header->sourceAddr[0] == 0xFC
        && header->destinationAddr[0] == 0xFC
        && length == message->length - Headers_IP6Header_SIZE;
}

static inline bool isForMe(struct Message* message, struct Context* context)
{
    struct Headers_IP6Header* header = message->bytes;
    return (memcmp(header->destinationAddr, context->myAddress, 16) == 0);
}

/** Message must not be encrypted and must be aligned on the beginning of the ipv6 header. */
static inline uint8_t sendToRouter(struct Message* message, struct Context* context)
{
}

/**
 * after being decrypted, message is sent here.
 * Message is aligned on the beginning of the ipv6 header.
 */
static inline uint8_t decryptedIncoming(struct Message* message, struct Context* context)
{
    context->ip6Header = (struct Headers_IP6Header*) message->bytes

    if (!validIP6(message)) {
        DEBUG("Dropping message because of invalid ipv6 header.");
        return;
    }

    if (isForMe(message)) {
        Message_shift(message, -Headers_ip6Header_SIZE);
        // This call goes to incomingForMe()
        context->contentSm->receiveMessage(message, context->contentSm);
        return;
    }

    struct Node* nextBest = RouterModule_getNextBest(addr, context->routerModule);
    memcpy(switchHeader->label, nextBest->address.networkAddress, 8);
    

    return;
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
#define decrypt(nonce msg secret isInitiator) encrypt(nonce, msg, secret, !isInitiator)

// for responses coming back from the CryptoAuth session.
// Will cause receiveMessage() to be called on the switch if.
static uint8_t sendToSwitch(struct Message* message, struct Interface* iface)
{
    struct Context* context = iface->senderContext;
    if (context->switchHeader != (message->bytes - sizeof(Headers_SwitchHeader))) {
        assert(message->padding >= sizeof(Headers_SwitchHeader));
        memmove(message->bytes - sizeof(Headers_SwitchHeader),
                context->switchHeader,
                sizeof(Headers_SwitchHeader));
    }
    Message_shift(message, sizeof(Headers_SwitchHeader));

    context->switchInterface.receiveMessage(message, &context->switchInterface);

    return 0;
}

static void receivedFromCryptoAuth(struct Message* message, struct Interface* iface)
{
    struct Context* context = iface->receiverContext;
    context->messageFromCryptoAuth = message;
}

static inline struct Interface* getCaSession(struct Headers_SwitchHeader* header,
                                             struct Context* context)
{
    int index = InterfaceMap_indexOf(header, context->ifMap);
    if (index > -1) {
        return context->ifMap.interfaces[index];
    }

    struct Interface* iface;
    // TODO: this can be done faster.
    // This is just a dirty way of giving CryptoAuth what it wants, a pair of interfaces.
    // All we really want is a way to pipe messages through a given CryptoAuth session.
    struct MemAllocator* child = context->allocator->child(context->allocator);
    struct Interface* outerIf =
        child->clone(sizeof(struct Interface), child, &(struct Interface) {
            .sendMessage = sendToSwitch,
            .senderContext = context
            .allocator = child,
        });
    struct Interface* innerIf =
        CryptoAuth_wrapInterface(outerIf, NULL, false, false, context->cryptoAuth);
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
    InterfaceMap_put(header, iface, now.tv_sec, context->ifMap);

    return iface;
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

    uint32_t messageType = Headers_getMessageType(header);
    if (messageType != 0) {
        // TODO: handle this
        fprintf(stderr,
                "A message with a non-zero type, perhaps an error. messageType=%d",
                messageType);
        return 0;
    }

    uint32_t nonce = *((uint32_t*)message->bytes);
    if (nonce != 0) {
        struct Node* node = RouterModule_getNode(switchHeader->label_be,
                                                 context->routerModule);
        if (node->session.exists) {
            Message_shift(message, -4);
            decrypt(nonce, message, &node->session.sharedSecret, node->session.isInitiator);
            return decryptedIncoming(message, node, context);
        }
        // Got a message which we can't handle, drop it and ping,
        // causing a session to be negotiated.
        RouterModule_pingNode(switchHeader->label_be, context->routerModule)
        return 0;
    }

    // Nonce is 0, this is a crypto negotiation.
    struct Interface* iface = getCaSession(context);

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
