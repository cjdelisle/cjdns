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
#ifndef SessionManager_H
#define SessionManager_H

#include "crypto/CryptoAuth.h"
#include "crypto/random/Random.h"
#include "interface/Interface.h"
#include "memory/Allocator.h"
#include "util/events/EventBase.h"
#include "util/Linker.h"
Linker_require("interface/SessionManager.c")

#include <stdint.h>

struct SessionManager;

struct SessionManager_Session
{
    struct Interface external;

    struct Interface* internal;

    /** When the last message was received on this session (milliseconds since epoch). */
    uint64_t timeOfLastIn;

    /** When the last message was sent on this session (milliseconds since epoch). */
    uint64_t timeOfLastOut;

    /** The CryptoAuth state as of the last message. See: CryptoAuth_getState() */
    int cryptoAuthState;

    /** The handle which will be used to lookup this session on our side, big endian. */
    uint32_t receiveHandle_be;

    /** The handle which we are expected to send to identify ourselves, big endian. */
    uint32_t sendHandle_be;

    /** The version of the other node. */
    uint32_t version;

    /** The IPv6 address of the other node. */
    uint8_t ip6[16];
};

struct SessionManager_HandleList
{
    uint32_t count;
    uint32_t* handles;
};

/**
 * Create a new session manager for keeping track of and expiring crypto sessions.
 * The typical use case is discriminating between packets by ip address, keyOffset is the number
 * of bytes between the beginning of each message and the beginning of the ip address and keySize
 * is the number of bytes in the address.
 *
 * @param decryptedIncoming the callback to call with incoming data after it has been decrypted.
 * @param encryptedOutgoing the callback to call with outgoing data after it has been encrypted.
 * @param interfaceContext the context which will become the senderContext and receiverContext for
 *                         encryptedOutgoing and decryptedIncoming respectively.
 * @param eventBase the libevent event base.
 * @param cryptoAuth the cryptoauthenticator for the sessions.
 * @param allocator means of getting memory.
 * @return a session manager.
 */

struct SessionManager* SessionManager_new(Interface_CALLBACK(decryptedIncoming),
                                          Interface_CALLBACK(encryptedOutgoing),
                                          void* interfaceContext,
                                          struct EventBase* eventBase,
                                          struct CryptoAuth* cryptoAuth,
                                          struct Random* rand,
                                          struct Allocator* allocator);

/**
 * Get a session from the session manager.
 * If there is no session for the lookup key, it will be created.
 *
 * @param lookupKey this must be the size given by keySize in SessionManager_new().
 * @param cryptoKey optional encryption key if it is known, otherwise NULL.
 * @param sm the session manager.
 */
struct SessionManager_Session* SessionManager_getSession(uint8_t* lookupKey,
                                                         uint8_t cryptoKey[32],
                                                         struct SessionManager* sm);

/**
 * Get a session by its handle.
 *
 * @param handle an opaque handle associated with the session.
 * @param sm the session manager.
 * @return the sesssion if there is one by that handle or null.
 */
struct SessionManager_Session* SessionManager_sessionForHandle(uint32_t handle,
                                                               struct SessionManager* sm);

/**
 * Get the IPv6 address for a session.
 *
 * @param handle the handle for the session
 * @param sm the session manager
 * @return a binary ipv6 address or NULL.
 */
uint8_t* SessionManager_getIp6(uint32_t handle, struct SessionManager* sm);

/**
 * Get the list of all handles.
 */
struct SessionManager_HandleList* SessionManager_getHandleList(struct SessionManager* sm,
                                                               struct Allocator* alloc);

#endif
