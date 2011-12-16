#include "crypto/CryptoAuth.h"
#include "interface/Interface.h"
#include "interface/InterfaceMap.h"
#include "memory/MemAllocator.h"
#include "util/Timeout.h"
#include "wire/Error.h"
#include "wire/Headers.h"
#include "wire/Message.h"

#include <stdint.h>
#include <event2/event.h>

#ifdef DEBUG
    #include <stdio.h>
    #define DEBUG(x) printf(x);
#else
    #define DEBUG(x)
#endif

/** The number of seconds of inactivity before a session should expire. */
#define SESSION_TIMEOUT_SECONDS 120

/** The number of seconds between cleanup cycles. */
#define CLEANUP_CYCLE_SECONDS 10

/**
 * A SessionManager is a special interface which chooses which CryptoAuth session to use
 * based on part of the data in the incoming or outgoing message.
 */
struct SessionManager
{
    struct Interface outgoing;

    struct Interface* const incoming;

    const uint16_t keySize;

    const int32_t keyOffset;

    struct event_base* const eventBase;

    struct InterfaceMap ifaceMap;

    struct MemAllocator* const allocator;

    struct Timeout* cleanupInterval;

    struct CryptoAuth* cryptoAuth;
};

/**
 * This additional step of indirection is needed because the user may change
 * the receiver or receiverContext later on.
 */
static void receiveMessageTwo(struct Message* message, struct Interface* iface)
{
    struct SessionManager* sm = (struct SessionManager*) iface->receiverContext;
    if (sm->outgoing.receiveMessage != NULL) {
        sm->outgoing.receiveMessage(message, &sm->outgoing);
    }
}

static inline struct Interface* getSession(struct Message* message,
                                           uint8_t key[32],
                                           struct SessionManager* sm)
{
    struct timeval now;
    event_base_gettimeofday_cached(sm->eventBase, &now);

    int ifaceIndex = InterfaceMap_indexOf(&message->bytes[sm->keyOffset], &sm->ifaceMap);
    if (ifaceIndex == -1) {
        struct MemAllocator* ifAllocator = sm->allocator->child(sm->allocator);
        struct Interface* outsideIf =
            ifAllocator->clone(sizeof(struct Interface), ifAllocator, &(struct Interface) {
                .sendMessage = sm->incoming->sendMessage,
                .senderContext = sm->incoming->senderContext,
                .allocator = ifAllocator
            });
        struct Interface* insideIf =
            CryptoAuth_wrapInterface(outsideIf, key, false, false, sm->cryptoAuth);
        insideIf->receiveMessage = receiveMessageTwo;
        insideIf->receiverContext = sm;

        // Create a trick interface which pretends to be on both sides of the crypto.
        struct Interface* combinedIf =
            ifAllocator->clone(sizeof(struct Interface), ifAllocator, &(struct Interface) {
                .sendMessage = insideIf->sendMessage,
                .senderContext = insideIf->senderContext,
                .receiveMessage = outsideIf->receiveMessage,
                .receiverContext = outsideIf->receiverContext,
                .allocator = ifAllocator
            });

        InterfaceMap_put(&message->bytes[sm->keyOffset], combinedIf, now.tv_sec, &sm->ifaceMap);
        return combinedIf;
    } else {
        // Interface already exists, set the time of last message to "now".
        sm->ifaceMap.lastMessageTimes[ifaceIndex] = now.tv_sec;
    }

    return sm->ifaceMap.interfaces[ifaceIndex];
}

static bool runt(struct Message* message, struct SessionManager* sm)
{
    if (sm->keyOffset > 0) {
        return sm->keyOffset + sm->keySize > message->length;
    } else {
        return (sm->keyOffset * -1) >= message->padding;
    }
}

// This is messages being crypto'd so they can be sent out.
static uint8_t sendMessage(struct Message* message, struct Interface* iface)
{
    struct SessionManager* sm = (struct SessionManager*) iface->senderContext;
    if (runt(message, sm)) {
        return Error_UNDERSIZE_MESSAGE;
    }
    struct Interface* outIface = getSession(message, NULL, sm);
    return outIface->sendMessage(message, outIface);
}

// This is messages coming in from the big bad world.
static void receiveMessage(struct Message* message, struct Interface* iface)
{
    struct SessionManager* sm = (struct SessionManager*) iface->receiverContext;
    if (runt(message, sm)) {
        DEBUG("dropped incoming runt message");
        return;
    }
    struct Interface* inIface = getSession(message, NULL, sm);
    inIface->receiveMessage(message, inIface);
}

static void cleanup(void* vsm)
{
    struct SessionManager* sm = (struct SessionManager*) vsm;
    struct timeval now;
    event_base_gettimeofday_cached(sm->eventBase, &now);
    uint32_t nowSeconds = now.tv_sec;
    for (uint32_t i = 0; i < sm->ifaceMap.count; i++) {
        if (sm->ifaceMap.lastMessageTimes[i] < (nowSeconds - SESSION_TIMEOUT_SECONDS)) {
            struct MemAllocator* ifAllocator = sm->ifaceMap.interfaces[i]->allocator;
            ifAllocator->free(ifAllocator);
            InterfaceMap_remove(i, &sm->ifaceMap);
            i--;
        }
    }
}

struct Interface* SessionManager_wrapInterface(uint16_t keySize,
                                               int32_t keyOffset,
                                               struct Interface* toWrap,
                                               struct event_base* eventBase,
                                               struct CryptoAuth* cryptoAuth,
                                               struct MemAllocator* allocator)
{
    struct SessionManager* sm = allocator->malloc(sizeof(struct SessionManager), allocator);
    memcpy(sm, &(struct SessionManager) {
        .outgoing = {
            .senderContext = sm,
            .sendMessage = sendMessage,
            .maxMessageLength = toWrap->maxMessageLength - Headers_CryptoAuth_SIZE,
            .requiredPadding = toWrap->requiredPadding + Headers_CryptoAuth_SIZE
        },
        .incoming = toWrap,
        .keySize = keySize,
        .keyOffset = keyOffset,
        .eventBase = eventBase,
        .ifaceMap = {
            .keySize = keySize,
            .allocator = allocator,
        },
        .cryptoAuth = cryptoAuth,
        .allocator = allocator,
        .cleanupInterval =
            Timeout_setInterval(cleanup, sm, 1000 * CLEANUP_CYCLE_SECONDS, eventBase, allocator)
    }, sizeof(struct SessionManager));
    toWrap->receiveMessage = receiveMessage;
    toWrap->receiverContext = sm;

    return &sm->outgoing;
}

void SessionManager_setKey(struct Message* message,
                           uint8_t key[32],
                           struct Interface* sessionManagerIface)
{
    struct SessionManager* sm = (struct SessionManager*) sessionManagerIface->senderContext;
    getSession(message, key, sm);
}
