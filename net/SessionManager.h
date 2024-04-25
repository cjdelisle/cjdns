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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef SessionManager_H
#define SessionManager_H

#include "crypto/random/Random.h"
#include "crypto/Ca.h"
#include "memory/Allocator.h"
#include "wire/PFChan.h"
#include "net/EventEmitter.h"
#include "wire/SwitchHeader.h"
#include "wire/CryptoHeader.h"
#include "util/Linker.h"
Linker_require("net/SessionManager.c")

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
     * Number of milliseconds after which a new DHT search will be run to verify the path.
     * This is guaged off of lastSearchTime so it need not be less than sessionTimeoutMilliseconds
     * which is guaged off of time of last incoming message (timeOfLastIn).
     */
    #define SessionManager_SESSION_SEARCH_AFTER_MILLISECONDS_DEFAULT 30000
    int64_t sessionSearchAfterMilliseconds;
};

typedef struct SessionManager_Path_s
{
    int64_t timeLastValidated;
    uint64_t label;
    uint32_t metric;
} SessionManager_Path_t;

#define SessionManager_PATH_COUNT 3

struct SessionManager_Session
{
    Ca_Session_t* caSession;

    SessionManager_Path_t paths[SessionManager_PATH_COUNT];

    // This is used to know when we should stop maintaining the session.
    // It is only flagged for real traffic, not router traffic.
    int64_t timeOfLastUsage;

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
};

struct SessionManager_HandleList
{
    int length;
    uint32_t* handles;
};

uint32_t SessionManager_effectiveMetric(struct SessionManager_Session* sess);

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

int SessionManager_handleCount(struct SessionManager* sm);

/**
 * Get the list of all handles.
 */
struct SessionManager_HandleList* SessionManager_getHandleList(struct SessionManager* sm,
                                                               struct Allocator* alloc);

struct SessionManager* SessionManager_new(struct Allocator* alloc,
                                          EventBase_t* eventBase,
                                          Ca_t* cryptoAuth,
                                          struct Random* rand,
                                          struct Log* log,
                                          struct EventEmitter* ee);

#endif
