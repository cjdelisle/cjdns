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
#include "interface/SessionManager.h"
#include "crypto/CryptoAuth.h"
#include "interface/Interface.h"
#include "memory/Allocator.h"
#include "util/Bits.h"
#include "util/events/Time.h"
#include "util/events/Timeout.h"
#include "wire/Error.h"
#include "wire/Headers.h"
#include "wire/Message.h"

#include <stdint.h>

/** The number of seconds of inactivity before a session should expire. */
#define SESSION_TIMEOUT_SECONDS 600

/** The number of seconds between cleanup cycles. */
#define CLEANUP_CYCLE_SECONDS 20


struct Ip6
{
    uint8_t bytes[16];
};
#define Map_NAME OfSessionsByIp6
#define Map_KEY_TYPE struct Ip6
#define Map_VALUE_TYPE struct SessionManager_Session
#define Map_ENABLE_HANDLES
#include "util/Map.h"


/**
 * A SessionManager is a mechanism for getting a crypto session based on a given key.
 */
struct SessionManager
{
    Interface_CONST_CALLBACK(decryptedIncoming);

    Interface_CONST_CALLBACK(encryptedOutgoing);

    void* const interfaceContext;

    struct EventBase* const eventBase;

    struct Map_OfSessionsByIp6 ifaceMap;

    struct Allocator* const allocator;

    struct Timeout* cleanupInterval;

    struct CryptoAuth* cryptoAuth;
};

static void cleanup(void* vsm)
{
    struct SessionManager* sm = (struct SessionManager*) vsm;
    uint64_t nowSecs = Time_currentTimeSeconds(sm->eventBase);
    for (uint32_t i = 0; i < sm->ifaceMap.count; i++) {
        if (sm->ifaceMap.values[i].lastMessageTime < (nowSecs - SESSION_TIMEOUT_SECONDS)) {
            struct Allocator* ifAllocator = sm->ifaceMap.values[i].iface.allocator;
            ifAllocator->free(ifAllocator);
            Map_OfSessionsByIp6_remove(i, &sm->ifaceMap);
            i--;
        }
    }
}

struct SessionManager_Session* SessionManager_getSession(uint8_t* lookupKey,
                                                         uint8_t cryptoKey[32],
                                                         struct SessionManager* sm)
{
    int ifaceIndex = Map_OfSessionsByIp6_indexForKey((struct Ip6*)lookupKey, &sm->ifaceMap);
    if (ifaceIndex == -1) {
        // Make sure cleanup() doesn't get behind.
        cleanup(sm);

        struct Allocator* ifAllocator = Allocator_child(sm->allocator);
        struct Interface* outsideIf =
            ifAllocator->clone(sizeof(struct Interface), ifAllocator, &(struct Interface) {
                .sendMessage = sm->encryptedOutgoing,
                .senderContext = sm->interfaceContext,
                .allocator = ifAllocator
            });
        struct Interface* insideIf =
            CryptoAuth_wrapInterface(outsideIf, cryptoKey, false, true, sm->cryptoAuth);
        insideIf->receiveMessage = sm->decryptedIncoming;
        insideIf->receiverContext = sm->interfaceContext;

        struct SessionManager_Session s = {
            .lastMessageTime = Time_currentTimeSeconds(sm->eventBase),

            // Create a trick interface which pretends to be on both sides of the crypto.
            .iface = {
                .sendMessage = insideIf->sendMessage,
                .senderContext = insideIf->senderContext,
                .receiveMessage = outsideIf->receiveMessage,
                .receiverContext = outsideIf->receiverContext,
                .allocator = ifAllocator
            }
        };

        int index = Map_OfSessionsByIp6_put((struct Ip6*)lookupKey, &s, &sm->ifaceMap);
        struct SessionManager_Session* sp = &sm->ifaceMap.values[index];
        sp->receiveHandle_be = Endian_hostToBigEndian32(sm->ifaceMap.handles[index]);
        Bits_memcpyConst(sp->ip6, lookupKey, 16);
        return sp;
    } else {
        // Interface already exists, set the time of last message to "now".
        sm->ifaceMap.values[ifaceIndex].lastMessageTime = Time_currentTimeSeconds(sm->eventBase);
        uint8_t* herPubKey = CryptoAuth_getHerPublicKey(&sm->ifaceMap.values[ifaceIndex].iface);
        if (Bits_isZero(herPubKey, 32) && cryptoKey) {
            Bits_memcpyConst(herPubKey, cryptoKey, 32);
        }
    }

    return &sm->ifaceMap.values[ifaceIndex];
}

struct SessionManager_Session* SessionManager_sessionForHandle(uint32_t handle,
                                                               struct SessionManager* sm)
{
    int index = Map_OfSessionsByIp6_indexForHandle(handle, &sm->ifaceMap);
    return (index == -1) ? NULL : &sm->ifaceMap.values[index];
}

struct SessionManager* SessionManager_new(Interface_CALLBACK(decryptedIncoming),
                                          Interface_CALLBACK(encryptedOutgoing),
                                          void* interfaceContext,
                                          struct EventBase* eventBase,
                                          struct CryptoAuth* cryptoAuth,
                                          struct Allocator* allocator)
{
    struct SessionManager* sm = allocator->malloc(sizeof(struct SessionManager), allocator);
    Bits_memcpyConst(sm, (&(struct SessionManager) {
        .decryptedIncoming = decryptedIncoming,
        .encryptedOutgoing = encryptedOutgoing,
        .interfaceContext = interfaceContext,
        .eventBase = eventBase,
        .ifaceMap = {
            .allocator = allocator
        },
        .cryptoAuth = cryptoAuth,
        .allocator = allocator,
        .cleanupInterval =
            Timeout_setInterval(cleanup, sm, 1000 * CLEANUP_CYCLE_SECONDS, eventBase, allocator)
    }), sizeof(struct SessionManager));

    return sm;
}
