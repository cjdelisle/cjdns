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

/** The number of seconds of inactivity before a session should expire. */
#define SESSION_TIMEOUT_SECONDS 600

/** The number of seconds between cleanup cycles. */
#define CLEANUP_CYCLE_SECONDS 20

/**
 * A SessionManager is a mechanism for getting a crypto session based on a given key.
 */
struct SessionManager
{
    Interface_CONST_CALLBACK(decryptedIncoming);

    Interface_CONST_CALLBACK(encryptedOutgoing);

    void* const interfaceContext;

    uint16_t keySize;

    struct event_base* const eventBase;

    struct InterfaceMap ifaceMap;

    struct MemAllocator* const allocator;

    struct Timeout* cleanupInterval;

    struct CryptoAuth* cryptoAuth;
};

struct Interface* SessionManager_getSession(uint8_t* lookupKey,
                                            uint8_t cryptoKey[32],
                                            struct SessionManager* sm)
{
    struct timeval now;
    event_base_gettimeofday_cached(sm->eventBase, &now);

    int ifaceIndex = InterfaceMap_indexOf(lookupKey, &sm->ifaceMap);
    if (ifaceIndex == -1) {
        struct MemAllocator* ifAllocator = sm->allocator->child(sm->allocator);
        struct Interface* outsideIf =
            ifAllocator->clone(sizeof(struct Interface), ifAllocator, &(struct Interface) {
                .sendMessage = sm->encryptedOutgoing,
                .senderContext = sm->interfaceContext,
                .allocator = ifAllocator
            });
        struct Interface* insideIf =
            CryptoAuth_wrapInterface(outsideIf, cryptoKey, false, false, sm->cryptoAuth);
        insideIf->receiveMessage = sm->decryptedIncoming;
        insideIf->receiverContext = sm->interfaceContext;

        // Create a trick interface which pretends to be on both sides of the crypto.
        struct Interface* combinedIf =
            ifAllocator->clone(sizeof(struct Interface), ifAllocator, &(struct Interface) {
                .sendMessage = insideIf->sendMessage,
                .senderContext = insideIf->senderContext,
                .receiveMessage = outsideIf->receiveMessage,
                .receiverContext = outsideIf->receiverContext,
                .allocator = ifAllocator
            });

        InterfaceMap_put(lookupKey, combinedIf, now.tv_sec, &sm->ifaceMap);
        return combinedIf;
    } else {
        // Interface already exists, set the time of last message to "now".
        sm->ifaceMap.lastMessageTimes[ifaceIndex] = now.tv_sec;
    }

    return sm->ifaceMap.interfaces[ifaceIndex];
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

struct SessionManager* SessionManager_new(uint16_t keySize,
                                          Interface_CALLBACK(decryptedIncoming),
                                          Interface_CALLBACK(encryptedOutgoing),
                                          void* interfaceContext,
                                          struct event_base* eventBase,
                                          struct CryptoAuth* cryptoAuth,
                                          struct MemAllocator* allocator)
{
    struct SessionManager* sm = allocator->malloc(sizeof(struct SessionManager), allocator);
    memcpy(sm, &(struct SessionManager) {
        .decryptedIncoming = decryptedIncoming,
        .encryptedOutgoing = encryptedOutgoing,
        .interfaceContext = interfaceContext,
        .keySize = keySize,
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

    return sm;
}
