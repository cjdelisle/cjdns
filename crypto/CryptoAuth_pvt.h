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
#ifndef CryptoAuth_pvt_H
#define CryptoAuth_pvt_H
#include "crypto/CryptoAuth.h"
#include "crypto/ReplayProtector.h"
#include "benc/Object.h"
#include "util/log/Log.h"
#include "memory/Allocator.h"
#include "util/events/EventBase.h"
#include "wire/CryptoHeader.h"
#include "wire/Message.h"
#include "util/Identity.h"

#include <stdint.h>

struct CryptoAuth_User;
struct CryptoAuth_User {
    /** Double-hash of password for authType 1 */
    uint8_t passwordHash[CryptoHeader_Challenge_KEYSIZE];

    /** Hash of username for authType 2 */
    uint8_t userNameHash[CryptoHeader_Challenge_KEYSIZE];

    uint8_t secret[32];

    String* login;

    uint8_t restrictedToip6[16];

    struct CryptoAuth_User* next;

    struct Allocator* alloc;

    Identity
};

struct CryptoAuth_pvt
{
    struct CryptoAuth pub;

    uint8_t pubKey[32];

    uint8_t privateKey[32];

    struct CryptoAuth_User* users;

    struct Log* logger;
    EventBase_t* eventBase;

    struct Allocator* allocator;
    struct Random* rand;

    Identity
};


struct CryptoAuth_Session_pvt
{
    struct CryptoAuth_Session pub;

    uint8_t herPublicKey[32];

    String* displayName;

    struct ReplayProtector replayProtector;

    /**
     * Bind this CryptoAuth session to the other node's ip6 address,
     * any packet avertizing a key which doesn't hash to this will be dropped.
     */
    uint8_t herIp6[16];

    /**
     * After this number of seconds of inactivity,
     * a connection will be reset to prevent them hanging in a bad state.
     */
    uint32_t resetAfterInactivitySeconds;

    /** If a session is not completely setup, reset it after this many seconds of inactivity. */
    uint32_t setupResetAfterInactivitySeconds;

    struct Allocator* alloc;

    /** The shared secret. */
    uint8_t sharedSecret[32];

    uint8_t herTempPubKey[32];

    uint8_t ourTempPrivKey[32];

    uint8_t ourTempPubKey[32];

    /** A password to use for authing with the other party. */
    struct Allocator* passwdAlloc;
    String* password;

    /** The login name to auth with the other party. */
    struct Allocator* loginAlloc;
    String* login;

    /** The next nonce to use. */
    uint32_t nextNonce;

    /** Used to reset the connection if it's in a bad state (no traffic coming in). */
    uint32_t timeOfLastPacket;

    /** The method to use for trying to auth with the server. */
    int authType : 8;

    /** True if this node began the conversation. */
    bool isInitiator : 1;

    /** If true and the other end is connecting, do not respond until a valid password is sent. */
    bool requireAuth : 1;

    bool established : 1;

    /** A pointer back to the main cryptoauth context. */
    struct CryptoAuth_pvt* context;

    Identity
};


//uint8_t CryptoAuth_receiveMessage(Message_t* received, struct Iface* interface);

//uint8_t CryptoAuth_encryptHandshake(Message_t* message,
//                                    struct CryptoAuth_Wrapper* wrapper,
//                                    int setupMessage);

int CryptoAuth_decryptRndNonce(const uint8_t nonce[24], Message_t* msg, const uint8_t secret[32]);

void CryptoAuth_encryptRndNonce(const uint8_t nonce[24], Message_t* msg, const uint8_t secret[32]);

#endif
