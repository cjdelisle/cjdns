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
#ifndef CryptoAuth_pvt_H
#define CryptoAuth_pvt_H
#include "crypto/CryptoAuth.h"
#include "crypto/ReplayProtector.h"
#include "interface/Interface.h"
#include "benc/Object.h"
#include "util/log/Log.h"
#include "memory/Allocator.h"
#include "util/events/EventBase.h"
#include "wire/Headers.h"
#include "wire/Message.h"
#include "util/Identity.h"

#include <stdint.h>

struct CryptoAuth_Auth {
    union Headers_AuthChallenge challenge;

    uint8_t secret[32];

    String* user;
};

struct CryptoAuth_pvt
{
    struct CryptoAuth pub;

    uint8_t privateKey[32];

    struct CryptoAuth_Auth* passwords;
    uint32_t passwordCount;
    uint32_t passwordCapacity;

    struct Log* logger;
    struct EventBase* eventBase;

    struct Allocator* allocator;
    struct Random* rand;

    Identity
};


struct CryptoAuth_Wrapper
{
    /** The public key of the other node, all zeros is taken to mean "don't know" */
    uint8_t herPerminentPubKey[32];

    /**
     * Bind this CryptoAuth session to the other node's ip6 address,
     * any packet avertizing a key which doesn't hash to this will be dropped.
     */
    uint8_t herIp6[16];

    /**
     * If an object was associated with a password and the remote host authed
     * with the password this will be the object, otherwise it will be null.
     */
    String* user;

    /** The shared secret. */
    uint8_t sharedSecret[32];

    uint8_t herTempPubKey[32];

    uint8_t ourTempPrivKey[32];

    uint8_t ourTempPubKey[32];

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

    /** If true and the other end is connecting, do not respond until a valid password is sent. */
    bool requireAuth : 1;

    bool established : 1;

    /** A pointer back to the main cryptoauth context. */
    struct CryptoAuth_pvt* const context;

    /** The internal interface which we are wrapping. */
    struct Interface* const wrappedInterface;

    /** The interface which this wrapper provides. */
    struct Interface externalInterface;

    /** A name for the wrapper which will appear in logs. */
    char* name;

    Identity
};


uint8_t CryptoAuth_receiveMessage(struct Message* received, struct Interface* interface);

uint8_t CryptoAuth_encryptHandshake(struct Message* message,
                                    struct CryptoAuth_Wrapper* wrapper,
                                    int setupMessage);

int CryptoAuth_decryptRndNonce(uint8_t nonce[24], struct Message* msg, uint8_t secret[32]);

void CryptoAuth_encryptRndNonce(uint8_t nonce[24], struct Message* msg, uint8_t secret[32]);

#endif
