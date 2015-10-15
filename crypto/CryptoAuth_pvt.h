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

    uint8_t privateKey[32];

    struct CryptoAuth_User* users;

    struct Log* logger;
    struct EventBase* eventBase;

    struct Allocator* allocator;
    struct Random* rand;

    Identity
};


struct CryptoAuth_Session_pvt
{
    struct CryptoAuth_Session pub;

    struct Allocator* alloc;

    /** The shared secret. */
    uint8_t sharedSecret[32];

    uint8_t herTempPubKey[32];

    uint8_t ourTempPrivKey[32];

    uint8_t ourTempPubKey[32];

    /** A password to use for authing with the other party. */
    String* password;

    /** The login name to auth with the other party. */
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


//uint8_t CryptoAuth_receiveMessage(struct Message* received, struct Iface* interface);

//uint8_t CryptoAuth_encryptHandshake(struct Message* message,
//                                    struct CryptoAuth_Wrapper* wrapper,
//                                    int setupMessage);

int CryptoAuth_decryptRndNonce(uint8_t nonce[24], struct Message* msg, uint8_t secret[32]);

void CryptoAuth_encryptRndNonce(uint8_t nonce[24], struct Message* msg, uint8_t secret[32]);

#endif
