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
#include "crypto/AddressCalc.h"
#include "interface/Interface.h"
#include "memory/Allocator.h"
#include "util/Bits.h"
#include "util/events/Time.h"
#include "util/events/Timeout.h"
#include "util/version/Version.h"
#include "wire/Error.h"
#include "wire/Headers.h"
#include "wire/Message.h"

#include <stdint.h>

/** The number of seconds of inactivity before a session should expire. */
#define SESSION_TIMEOUT_SECONDS 600

/** The number of seconds between cleanup cycles. */
#define CLEANUP_CYCLE_SECONDS 20

/** Number of milliseconds between state change announcements. */
#define STATE_UPDATE_TIME 8000

/** Handle numbers 0-3 are reserved for CryptoAuth nonces. */
#define MIN_FIRST_HANDLE 4

#define MAX_FIRST_HANDLE 100000

struct SessionManager_Session_pvt
{
    struct SessionManager_Session pub;
    struct SessionManager* const sm;
    Identity
};

struct Ip6
{
    uint8_t bytes[16];
};
#define Map_NAME OfSessionsByIp6
#define Map_KEY_TYPE struct Ip6
#define Map_VALUE_TYPE struct SessionManager_Session_pvt*
#define Map_ENABLE_HANDLES
#include "util/Map.h"


/**
 * A SessionManager is a mechanism for getting a crypto session based on a given key.
 */
struct SessionManager
{
    /** Trick interface which is used for receiving and sending to the inside/outside world. */
    struct Interface iface;

    Interface_CONST_CALLBACK(encryptedOutgoing);

    void* const interfaceContext;

    struct EventBase* const eventBase;

    struct Map_OfSessionsByIp6 ifaceMap;

    struct Allocator* const allocator;

    struct Timeout* cleanupInterval;

    struct CryptoAuth* cryptoAuth;

    /** The first handle number to start with, randomized at startup to reduce collisions. */
    uint32_t first;
};

static void cleanup(void* vsm)
{
    struct SessionManager* sm = (struct SessionManager*) vsm;
    uint64_t now = Time_currentTimeMilliseconds(sm->eventBase);
    for (uint32_t i = 0; i < sm->ifaceMap.count; i++) {
        uint64_t lastMsg = sm->ifaceMap.values[i]->pub.timeOfLastIn;
        if (sm->ifaceMap.values[i]->pub.timeOfLastOut > lastMsg) {
            lastMsg = sm->ifaceMap.values[i]->pub.timeOfLastOut;
        }
        if (lastMsg < (now - (SESSION_TIMEOUT_SECONDS * 1000))) {
            struct Allocator* ifAllocator = sm->ifaceMap.values[i]->pub.external.allocator;
            Map_OfSessionsByIp6_remove(i, &sm->ifaceMap);
            Allocator_free(ifAllocator);
            i--;
        }
    }
}

static void check(struct SessionManager* sm, int mapIndex)
{
    Assert_true(sm->ifaceMap.keys[mapIndex].bytes[0] == 0xfc);
    uint8_t* herPubKey = CryptoAuth_getHerPublicKey(sm->ifaceMap.values[mapIndex]->pub.internal);
    if (!Bits_isZero(herPubKey, 32)) {
        uint8_t ip6[16];
        AddressCalc_addressForPublicKey(ip6, herPubKey);
        Assert_true(!Bits_memcmp(&sm->ifaceMap.keys[mapIndex], ip6, 16));
    }
}

static void stateChange(struct SessionManager_Session_pvt* ss,
                        uint64_t prevTimeOfLastIn,
                        uint64_t prevTimeOfLastOut,
                        int prevCryptoAuthState)
{
}

static uint8_t sendMessage(struct Message* msg, struct Interface* iface)
{
    struct SessionManager_Session_pvt* ss =
        Identity_check((struct SessionManager_Session_pvt*)iface);

    uint64_t timeOfLastOut = ss->pub.timeOfLastOut;
    ss->pub.timeOfLastOut = Time_currentTimeMilliseconds(ss->sm->eventBase);
    int prevState = ss->pub.cryptoAuthState;
    ss->pub.cryptoAuthState = CryptoAuth_getState(ss->pub.internal);
    if ((ss->pub.timeOfLastOut - timeOfLastOut) > STATE_UPDATE_TIME
        || prevState != ss->pub.cryptoAuthState)
    {
        stateChange(ss, ss->pub.timeOfLastIn, timeOfLastOut, prevState);
    }

    return Interface_sendMessage(&ss->sm->iface, msg);
}

static uint8_t receiveMessage(struct Message* msg, struct Interface* iface)
{
    struct SessionManager_Session_pvt* ss =
        Identity_check((struct SessionManager_Session_pvt*)iface->receiverContext);

    uint64_t timeOfLastIn = ss->pub.timeOfLastIn;
    ss->pub.timeOfLastIn = Time_currentTimeMilliseconds(ss->sm->eventBase);
    int prevState = ss->pub.cryptoAuthState;
    ss->pub.cryptoAuthState = CryptoAuth_getState(ss->pub.internal);
    if ((ss->pub.timeOfLastIn - timeOfLastIn) > STATE_UPDATE_TIME
        || prevState != ss->pub.cryptoAuthState)
    {
        stateChange(ss, timeOfLastIn, ss->pub.timeOfLastOut, prevState);
    }

    return Interface_receiveMessage(&ss->sm->iface, msg);
}

struct SessionManager_Session* SessionManager_getSession(uint8_t* lookupKey,
                                                         uint8_t cryptoKey[32],
                                                         struct SessionManager* sm)
{
    int ifaceIndex = Map_OfSessionsByIp6_indexForKey((struct Ip6*)lookupKey, &sm->ifaceMap);
    if (ifaceIndex == -1) {
        // Make sure cleanup() doesn't get behind.
        cleanup(sm);

        struct Allocator* ifAlloc = Allocator_child(sm->allocator);
        struct SessionManager_Session_pvt* ss =
            Allocator_clone(ifAlloc, (&(struct SessionManager_Session_pvt) {
                .pub = {
                    .version = Version_DEFAULT_ASSUMPTION,
                    .external = {
                        .sendMessage = sendMessage,
                        .allocator = ifAlloc
                    },
                },
                .sm = sm
            }));
        Identity_set(ss);

        // const hack
        Bits_memcpyConst((void*)&ss->pub.external.senderContext, &ss, sizeof(char*));

        Bits_memcpyConst(ss->pub.ip6, lookupKey, 16);
        ss->pub.internal = CryptoAuth_wrapInterface(&ss->pub.external,
                                                    cryptoKey,
                                                    lookupKey,
                                                    false,
                                                    "inner",
                                                    sm->cryptoAuth);

        ss->pub.internal->receiveMessage = receiveMessage;
        ss->pub.internal->receiverContext = ss;

        ifaceIndex = Map_OfSessionsByIp6_put((struct Ip6*)lookupKey, &ss, &sm->ifaceMap);

        ss->pub.receiveHandle_be =
            Endian_hostToBigEndian32(sm->ifaceMap.handles[ifaceIndex] + sm->first);

    } else {
        uint8_t* herPubKey =
            CryptoAuth_getHerPublicKey(sm->ifaceMap.values[ifaceIndex]->pub.internal);
        if (Bits_isZero(herPubKey, 32) && cryptoKey) {
            Bits_memcpyConst(herPubKey, cryptoKey, 32);
        }
    }

    check(sm, ifaceIndex);

    return &Identity_check(sm->ifaceMap.values[ifaceIndex])->pub;
}

struct SessionManager_Session* SessionManager_sessionForHandle(uint32_t handle,
                                                               struct SessionManager* sm)
{
    int index = Map_OfSessionsByIp6_indexForHandle(handle - sm->first, &sm->ifaceMap);
    if (index < 0) { return NULL; }
    check(sm, index);
    return &Identity_check(sm->ifaceMap.values[index])->pub;
}

uint8_t* SessionManager_getIp6(uint32_t handle, struct SessionManager* sm)
{
    int index = Map_OfSessionsByIp6_indexForHandle(handle - sm->first, &sm->ifaceMap);
    if (index < 0) { return NULL; }
    check(sm, index);
    return sm->ifaceMap.keys[index].bytes;
}

struct SessionManager_HandleList* SessionManager_getHandleList(struct SessionManager* sm,
                                                               struct Allocator* alloc)
{
    struct SessionManager_HandleList* out =
        Allocator_malloc(alloc, sizeof(struct SessionManager_HandleList));
    uint32_t* buff = Allocator_malloc(alloc, 4 * sm->ifaceMap.count);
    Bits_memcpy(buff, sm->ifaceMap.handles, 4 * sm->ifaceMap.count);
    out->handles = buff;
    out->count = sm->ifaceMap.count;
    for (int i = 0; i < (int)out->count; i++) {
        buff[i] += sm->first;
    }
    return out;
}

struct SessionManager* SessionManager_new(Interface_CALLBACK(decryptedIncoming),
                                          Interface_CALLBACK(encryptedOutgoing),
                                          void* interfaceContext,
                                          struct EventBase* eventBase,
                                          struct CryptoAuth* cryptoAuth,
                                          struct Random* rand,
                                          struct Allocator* allocator)
{
    struct SessionManager* sm = Allocator_malloc(allocator, sizeof(struct SessionManager));
    Bits_memcpyConst(sm, (&(struct SessionManager) {
        .iface = {
            .receiveMessage = decryptedIncoming,
            .receiverContext = interfaceContext,
            .sendMessage = encryptedOutgoing,
            .senderContext = interfaceContext
        },
        .eventBase = eventBase,
        .ifaceMap = {
            .allocator = allocator
        },
        .cryptoAuth = cryptoAuth,
        .allocator = allocator,
        .first = (Random_uint32(rand) % (MAX_FIRST_HANDLE - MIN_FIRST_HANDLE)) + MIN_FIRST_HANDLE,
        .cleanupInterval =
            Timeout_setInterval(cleanup, sm, 1000 * CLEANUP_CYCLE_SECONDS, eventBase, allocator)
    }), sizeof(struct SessionManager));

    return sm;
}
