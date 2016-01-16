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

#include "crypto/random/Random.h"
#include "crypto/CryptoAuth.h"
#include "memory/Allocator.h"
#include "wire/PFChan.h"
#include "net/EventEmitter.h"
#include "wire/SwitchHeader.h"
#include "wire/CryptoHeader.h"
#include "util/Linker.h"
Linker_require("net/SessionManager.c");

/**
 * Purpose of this module is to take packets from "the inside" which contain ipv6 address and
 * skeleton switch header and find an appropriate CryptoAuth session for them or begin one.
 * If a key for this node cannot be found then the packet will be blocked and a search will be
 * triggered. If the skeleton switch header contains "zero" as the switch label, the packet will
 * also be buffered and a search triggered. If a search is in progress (and another packet is
 * already buffered, the packet will be dropped instead).
 * Incoming messages from the outside will be decrypted and their key and path will be stored.
 */
struct SessionManager
{
    /** Sends and handles packets prepped to/from switch. */
    struct Iface switchIf;

    /**
     * Sends and handles packets with RouteHeader on top.
     * When sending a packet to SessionManager:
     *     header.sh.label_be may be zero
     *     version may be zero
     *     publicKey may be zero
     * If these values are not known, the packet will be taken from the cache or a search will
     * be triggered.
     */
    struct Iface insideIf;

    /**
     * Maximum number of packets to hold in buffer before summarily dropping...
     */
    #define SessionManager_MAX_BUFFERED_MESSAGES_DEFAULT 30
    int maxBufferedMessages;

    /** Number of milliseconds with no reply before a session should be timed out. */
    #define SessionManager_SESSION_TIMEOUT_MILLISECONDS_DEFAULT 120000
    int64_t sessionTimeoutMilliseconds;

    /**
     * Should be set to a number less than sessionSearchAfterMilliseconds, if no incoming nor
     * outgoing messages were sent on this session for this amount of time, re-running of the
     * search will be skipped when sessionSearchAfterMilliseconds is reached allowing the session
     * to be removed from the table after sessionTimeoutMilliseconds.
     */
    #define SessionManager_SESSION_IDLE_AFTER_MILLISECONDS_DEFAULT 50000
    int64_t sessionIdleAfterMilliseconds;

    /**
     * Number of milliseconds after which a new DHT search will be run to verify the path.
     * If the session is in "idle" state (sessionIdleAfterMilliseconds has elapsed without any
     * incoming or outgoing traffic) then the search will be skipped, although it will be triggered
     * again if more traffic is sent.
     * This is guaged off of lastSearchTime so it need not be less than sessionTimeoutMilliseconds
     * which is guaged off of time of last incoming message (timeOfLastIn).
     */
    #define SessionManager_SESSION_SEARCH_AFTER_MILLISECONDS_DEFAULT 100000
    int64_t sessionSearchAfterMilliseconds;
};

struct SessionManager_Session
{
    struct CryptoAuth_Session* caSession;

    /** When the last message was received on this session (milliseconds since epoch). */
    int64_t timeOfLastIn;

    /** When the last message was sent on this session (milliseconds since epoch). */
    int64_t timeOfLastOut;

    uint64_t bytesOut;

    uint64_t bytesIn;

    /** This is the time the last search was triggered for this session. */
    int64_t lastSearchTime;

    /** The handle which will be used to lookup this session on our side. */
    uint32_t receiveHandle;

    /** The handle which we are expected to send to identify ourselves */
    uint32_t sendHandle;

    /** The version of the other node. */
    uint32_t version;

    /** The best known switch label for reaching this node. */
    uint64_t sendSwitchLabel;

    /** The switch label which this node uses for reaching us. */
    uint64_t recvSwitchLabel;
};

struct SessionManager_HandleList
{
    int length;
    uint32_t* handles;
};

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

struct SessionManager* SessionManager_new(struct Allocator* alloc,
                                          struct EventBase* eventBase,
                                          struct CryptoAuth* cryptoAuth,
                                          struct Random* rand,
                                          struct Log* log,
                                          struct EventEmitter* ee);

#endif
