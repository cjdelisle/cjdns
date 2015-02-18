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

#error poisoned

#include "crypto/CryptoAuth.h"
#include "crypto/random/Random.h"
#include "memory/Allocator.h"
#include "util/events/EventBase.h"
#include "util/Linker.h"
Linker_require("net/SessionManager.c")

#include <stdint.h>

struct SessionManager;

struct SessionManager_Session
{
    struct CryptoAuth_Session* caSession;

    /** The handle which will be used to lookup this session on our side. */
    uint32_t receiveHandle;

    //struct Allocator* alloc;


    /** When the last message was received on this session (milliseconds since epoch). */
    //uint64_t timeOfLastIn;

    /** When the last message was sent on this session (milliseconds since epoch). */
    //uint64_t timeOfLastOut;

    /** When this session was created. */
    //uint64_t timeOfCreation;

    /** The CryptoAuth state as of the last message. See: CryptoAuth_getState() */
    //int cryptoAuthState;

    /** The handle which we are expected to send to identify ourselves */
    //uint32_t sendHandle;

    /** The version of the other node. */
    //uint32_t version;

    /** The best known switch label for reaching this node. */
    //uint64_t sendSwitchLabel;

    /** The switch label which this node uses for reaching us. */
    //uint64_t recvSwitchLabel;
};

struct SessionManager_HandleList
{
    int count;
    uint32_t* handles;
};

/**
 * Create a new session manager for keeping track of and expiring crypto sessions.
 * The typical use case is discriminating between packets by ip address, keyOffset is the number
 * of bytes between the beginning of each message and the beginning of the ip address and keySize
 * is the number of bytes in the address.
 *
 * @param eventBase the libevent event base.
 * @param cryptoAuth the cryptoauthenticator for the sessions.
 * @param allocator means of getting memory.
 * @return a session manager.
 */
struct SessionManager* SessionManager_new(struct CryptoAuth* cryptoAuth,
                                      struct Random* rand,
                                      struct Allocator* allocator);

void SessionManager_regSession(uint8_t* ip6,
                             uint8_t cryptoKey[32],
                             struct SessionManager_Session* session,
                             struct Allocator* alloc,
                             struct SessionManager* st);

/**
 * Get a session by its handle.
 *
 * @param handle an opaque handle associated with the session.
 * @param sm the session manager.
 * @return the sesssion if there is one by that handle or null.
 */
struct SessionManager_Session* SessionManager_sessionForHandle(uint32_t handle,
                                                               struct SessionManager* sm);

struct SessionManager_Session* SessionManager_sessionForIp6(uint8_t* lookupKey,
                                                            struct SessionManager* sm);

/**
 * Get the list of all handles.
 */
struct SessionManager_HandleList* SessionManager_getHandleList(struct SessionManager* sm,
                                                           struct Allocator* alloc);

#endif
