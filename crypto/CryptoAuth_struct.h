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
#ifndef CryptoAuth_struct_H
#define CryptoAuth_struct_H

#include "crypto/ReplayProtector.h"
#include "interface/Interface.h"
#include "benc/Object.h"
#include "util/Log.h"
#include "memory/Allocator.h"
#include "wire/Headers.h"
#include "wire/Message.h"

#include <stdint.h>
#include <event2/event.h>

struct CryptoAuth_Auth {
    union Headers_AuthChallenge challenge;

    uint8_t secret[32];

    void* user;
};

struct CryptoAuth
{
    uint8_t privateKey[32];

    uint8_t publicKey[32];

    struct CryptoAuth_Auth* passwords;
    uint32_t passwordCount;
    uint32_t passwordCapacity;

    struct Log* logger;
    struct event_base* eventBase;

    /**
     * After this number of seconds of inactivity,
     * a connection will be reset to prevent them hanging in a bad state.
     */
    uint32_t resetAfterInactivitySeconds;

    struct Allocator* allocator;
};

/**
 * What the "secret" and "tempKey" fields hold during different stages of the handshake.
 * |  secret  | tempKey |     message     |  secret  | tempKey | encryptedWith
 * |+tmpPvtA  |+tmpPubA | ---- hello ---->|    0     |+tmpPubA | prmPvtA-prmPubB-passA
 * | tmpPvtA  | tmpPubA | --dupe hello -->|    0     | tmpPubA | prmPvtA-prmPubB-passA
 * | tmpPvtA  | tmpPubA | <---- key ----- | +tmpPvtB | tmpPubA | prmPvtB-tmpPubA-passB
 * | tmpPvtA  | tmpPubA | <--dupe key---- |  tmpPvtB | tmpPubA | prmPvtB-tmpPubA-passB
 * | finalSec |    0    | ---- data ----->|+finalSec |    0    | tmpPvtA-tmpPubB
 */
struct CryptoAuth_Wrapper
{
    /** The public key of the other node. */
    uint8_t herPerminentPubKey[32];

    /**
     * If an object was associated with a password and the remote host authed
     * with the password this will be the object, otherwise it will be null.
     */
    void* user;

    /** The shared secret. */
    uint8_t secret[32];

    /** Used during handshake to hold her public key and my private key at different times. */
    uint8_t tempKey[32];

    /** An outgoing message which is buffered in the event that a reverse handshake is required. */
    struct Message* bufferedMessage;

    /** A password to use for authing with the other party. */
    String* password;

    /** Used for preventing replay attacks. */
    struct ReplayProtector replayProtector;

    /** The next nonce to use. */
    uint32_t nextNonce;

    /** Used to reset the connection if it's in a bad state (no traffic coming in). */
    uint32_t timeOfLastPacket;

    /** The method to use for trying to auth with the server. */
    uint8_t authType;

    /** True if this node began the conversation. */
    bool isInitiator : 1;

    /** If true then the packets sent through this interface must be authenticated. */
    bool authenticatePackets : 1;

    /** If true and the other end is connecting, do not respond until a valid password is sent. */
    bool requireAuth : 1;

    /** True if there is a message which is buffered until a connection is setup. */
    bool hasBufferedMessage : 1;

    /** A pointer back to the main cryptoauth context. */
    struct CryptoAuth* const context;

    /** The internal interface which we are wrapping. */
    struct Interface* const wrappedInterface;

    /** The interface which this wrapper provides. */
    struct Interface externalInterface;
};

#endif
