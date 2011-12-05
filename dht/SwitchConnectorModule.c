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


    struct Interface* sessionManager;
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

static int handleIncomingPostDeserialization(struct DHTMessage* message, void* vcontext)
{
}

static int handleOutgoingPreSerialization(struct DHTMessage* message, void* vcontext)
{
    String* argumentsKey =
        (message->replyTo == NULL) ? &DHTConstants_arguments : &DHTConstants_reply;
    Dict* args = benc_lookupDictionary(message->asDict, &DHTConstants_arguments);
    benc_removeEntry(args, &DHTConstants_myId);
}

static inline void incomingDHT(struct Message* message,
                               struct Interface* interface,
                               struct Headers_SwitchHeader* header,
                               struct Context context)
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

static inline bool validIP6(struct Message* message)
{
    struct Headers_IP6Header* header = message->bytes;
    uint16_t length = Endian_bigEndianToHost16(header->payloadLength_be);
    return header->sourceAddr[0] == 0xFC
        && header->destinationAddr[0] == 0xFC
        && length == message->length - Headers_IP6Header_SIZE;
}

/**
 * the bottom of incomingFromSwitch() calls an interface which goes through
 * CryptoAuth and then calls this. At this point there is a different interface for each
 * router which we are communicating with.
 */
static void authedIncomingFromSwitch(struct Message* message, struct Interface* iface)
{
    struct Context* context = defaultInterface->receiverContext;

    if (!validIP6(message)) {
        return;
    }

    if (isForMe(message)) {
        
    }

    uint8_t route[8];
    memcpy(route, header, 8);

    uint32_t messageType = Headers_getMessageType(header);
    switch (messageType) {
        case 0:;
            incomingIP6(message, interface, header, context);
            return;

        case 1:;
            incomingDHT(message, interface, header, context);
            return;

        default:
            return;
    };
    
}

/**
 * Get the first message from a newly discovered router, this will add a crypto session
 * for the router to the InterfaceMap.
 */
static void receiveFirstMessage(struct Message* message, struct Interface* defaultInterface)
{
    struct Context* context = defaultInterface->receiverContext;

    struct Headers_SwitchHeader* header = (struct Headers_SwitchHeader*) message->bytes
        - sizeof(struct Headers_SwitchHeader)
        - sizeof(struct Headers_CryptoAuth);

    // insert this interface into the map and make it not the default interface.
    InterfaceMap_put((uint8_t*) &header->label_be, defaultInterface, context->cryptoSessionMap);
    context->defaultInterface == NULL;

    // Set the receiver to be authedIncomingFromSwitch which will do the routing or DHT.
    defaultInterface->receiveMessage = authedIncomingFromSwitch;

    // Now send the message on so the first message isn't lost.
    authedIncomingFromSwitch(message, defaultInterface
}

/**
 * This is called as sendMessage() by the switch.
 * There is only one switch interface which sends all traffic.
 */
static uint8_t incomingFromSwitch(struct Message* message, struct Interface* switchIf)
{
    struct Headers_SwitchHeader* header =
        (struct Headers_SwitchHeader*) message->bytes - sizeof(struct Headers_SwitchHeader);

    struct Context* context = switchIf->senderContext;

    uint32_t messageType = Headers_getMessageType(header);
    if (messageType != 0) {
        // TODO: handle this
        fprintf(stderr,
                "A message with a non-zero type, perhaps an error. messageType=%d",
                messageType);
        return 0;
    }

    


    // Get the CryptoAuth to send this to by it's label.
    struct Interface* iface =
        InterfaceMap_get((uint8_t*) &header->label_be, context->cryptoSessionMap);

    if (iface == NULL) {
        if (context->defaultInterface == NULL) {
            // We create an interface which authenticates incoming connections
            // after their message is authenticated, receiveFirstMessage() forks the interface
            // off into a position in the interface map so we can communicate with this node.
            struct MemAllocator* ifAllocator = context->allocator->child(context->allocator);
            struct Interface* defaultIf =
                ifAllocator->clone(sizeof(struct Interface), ifAllocator, &(struct Interface) {
                    // Make this inteface send directly to the switch.
                    .sendMessage = switchIf->sendMessage,
                    .senderContext = switchIf->senderContext,
                    .allocator = ifAllocator
                });
            context->defaultInterface =
                CryptoAuth_wrapInterface(defaultIf, NULL, false, false, context->cryptoAuth);
            context->defaultInterface->receiveMessage = receiveFirstMessage;
            context->defaultInterface->receiverContext = context;
        }
        iface = context->defaultInterface;
    }

    // This goes into a CryptoAuth and then to authedIncomingFromSwitch
    iface->receiveMessage(message, iface);
}






    switch (messageType) {
        case 0: return incomingIp6(message, route, )
    };
    



    struct DHTMessage message;
    memset(&message, 0, sizeof(struct DHTMessage));

    memcpy(message->peerAddress, message - sizeof(struct Headers_SwitchHeader), 8);
    message->addressLength = 0;

    


    int rc = recvfrom(socket,
                      message.bytes,
                      MAX_MESSAGE_SIZE,
                      0,
                      (struct sockaddr*) &addrStore,
                      &addrLength);

    assert(addrLength <= sizeof(struct sockaddr_storage));
    assert(rc <= MAX_MESSAGE_SIZE);

    if (rc < 0) {
        return;
    }

    message.length = rc;

    struct LibeventNetworkModule_Context* context = (struct LibeventNetworkModule_Context*) vcontext;

    context->perMessageAllocator->free(context->perMessageAllocator);
    message.allocator = context->perMessageAllocator;

    int32_t length = NetworkTools_addressFromSockaddr(&addrStore, message.peerAddress);
    if (length > -1) {
        message.addressLength = length;
        DHTModules_handleIncoming(&message, ((struct LibeventNetworkModule_Context*) vcontext)->registry);
    }
}




/**
 * @param base the libevent context.
 * @param socket an open bound nonblocking reusable socket.
 * @param addressLength since DHT only knows IPv4 from IPv6 by the number of
 *                      bytes for address and port, this module will handle
 *                      only packets which match the given address length.
 *                      6 for IPv4 and 18 for IPv6.
 * @param registry the module registry to send the incoming messages
 *                 to and send messages from.
 * @param allocator the means of getting memory for the module.
 * @return -1 if inputs are null. -2 if registering the event handler fails.
 */
int LibeventNetworkModule_register(struct event_base* base,
                                   evutil_socket_t socket,
                                   int addressLength,
                                   struct DHTModuleRegistry* registry,
                                   struct MemAllocator* allocator)
{
    if (registry == NULL || base == NULL || allocator == NULL) {
        return -1;
    }

    char* messageBuffer = allocator->malloc(16384, allocator);
    struct MemAllocator* perMessageAllocator = BufferAllocator_new(messageBuffer, 16384);

    struct LibeventNetworkModule_Context* context =
        allocator->malloc(sizeof(struct LibeventNetworkModule_Context), allocator);
    memcpy(context, &(struct LibeventNetworkModule_Context) {
        .module = {
            .name = "LibeventNetworkModule",
            .context = context,
            .handleOutgoing = handleOutgoing
        },
        .socket = socket,
        .addressLength = addressLength,
        .registry = registry,
        .perMessageAllocator = perMessageAllocator
    }, sizeof(struct LibeventNetworkModule_Context));

    context->incomingMessageEvent = event_new(base, socket, EV_READ | EV_PERSIST, handleEvent, context);

    if (context->incomingMessageEvent == NULL) {
        return -2;
    }

    event_add(context->incomingMessageEvent, NULL);

    allocator->onFree(freeEvent, context->incomingMessageEvent, allocator);

    return DHTModules_register(&(context->module), registry);
}

/*--------------------Internals--------------------*/

/**
 * Release the event used by this module.
 *
 * @param vevent a void pointer cast of the event structure.
 */
static void freeEvent(void* vevent)
{
    event_del((struct event*) vevent);
    event_free((struct event*) vevent);
}

/**
 * If MSG_CONFIRM is defined and this node has sent us a message recently
 * (within the last MESSAGE_RECENT_FOR seconds) then return the value of
 * MSG_CONFIRM. Otherwise return 0.
 *
 * @param addressAndPort the ip address in binary concatinated to port number.
 * @param addressAndPortLength how much of the buffer to read.
 * @param context the state of this module.
 * @return the value of MSG_CONFIRM or 0.
 */
static inline int confirmIfRecent(char* addressAndPort,
                                  int addressAndPortLength,
                                  struct LibeventNetworkModule_Context* context)
{
#ifndef MSG_CONFIRM
    return 0;
#endif
    // TODO implement me.
    addressAndPort = addressAndPort;
    addressAndPortLength = addressAndPortLength;
    context = context;
    return 0;
}

/**
 * Take an outgoing message and write it to the socket.
 *
 * @see DHTModule->handleOutgoing in DHTModules.h
 */
static int handleOutgoing(struct DHTMessage* message,
                          void* vcontext)
{
    struct LibeventNetworkModule_Context* context =
        (struct LibeventNetworkModule_Context*) vcontext;

    if (message->addressLength != context->addressLength) {
        /* This message is intended for another interface. */
        return 0;
    }

    struct sockaddr_storage addr;
    socklen_t length = NetworkTools_getPeerAddress(message->peerAddress, message->addressLength, &addr);

    if (length) {

        int flags = confirmIfRecent(message->peerAddress,
                                    message->addressLength,
                                    context);
        sendto(context->socket,
               message->bytes,
               message->length,
               flags,
               (struct sockaddr*) &addr,
               length);
    } else {
        /* Address of unknown length. TODO: error reporting. */
    }

    return 0;
}

/**
 * Handle input from Libevent.
 * This is a callback whihc is passed to libevent.
 *
 * @param socket the socket on which the input came.
 * @param eventType this should equal EV_READ
 * @param vcontext the LibeventNetworkModule_Context cast to
 *                 void* which libevent understands.
 */
static void handleEvent(evutil_socket_t socket,
                        short eventType,
                        void* vcontext)
{
    /* Only event which was registered. */
    assert(eventType == EV_READ);

    struct DHTMessage message;
    /* messageType MUST be set to 0. */
    memset(&message, 0, sizeof(struct DHTMessage));

    struct sockaddr_storage addrStore;
    socklen_t addrLength = sizeof(struct sockaddr_storage);

    int rc = recvfrom(socket,
                      message.bytes,
                      MAX_MESSAGE_SIZE,
                      0,
                      (struct sockaddr*) &addrStore,
                      &addrLength);

    assert(addrLength <= sizeof(struct sockaddr_storage));
    assert(rc <= MAX_MESSAGE_SIZE);

    if (rc < 0) {
        return;
    }

    message.length = rc;

    struct LibeventNetworkModule_Context* context = (struct LibeventNetworkModule_Context*) vcontext;

    context->perMessageAllocator->free(context->perMessageAllocator);
    message.allocator = context->perMessageAllocator;

    int32_t length = NetworkTools_addressFromSockaddr(&addrStore, message.peerAddress);
    if (length > -1) {
        message.addressLength = length;
        DHTModules_handleIncoming(&message, ((struct LibeventNetworkModule_Context*) vcontext)->registry);
    }
}
