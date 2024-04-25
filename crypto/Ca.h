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
#ifndef Ca_H
#define Ca_H

#include "rust/cjdns_sys/RTypes.h"
#include "benc/String.h"
#include "crypto/random/Random.h"
#include "memory/Allocator.h"
#include "util/log/Log.h"
#include "util/events/EventBase.h"

#ifndef OLD_CA
#include "rust/cjdns_sys/Rffi.h"
typedef RTypes_CryptoAuth2_t Ca_t;
typedef RTypes_CryptoAuth2_Session_t Ca_Session_t;
#define Ca_IMPL(x) Rffi_CryptoAuth2_ ## x
static inline Ca_t* Ca_new(struct Allocator* allocator,
                                  const uint8_t* privateKey,
                                  EventBase_t* eventBase,
                                  struct Log* logger,
                                  struct Random* rand)
{
    return Rffi_CryptoAuth2_new(allocator, privateKey, rand);
}
#else
#include "crypto/CryptoAuth.h"
typedef struct CryptoAuth Ca_t;
typedef struct CryptoAuth_Session Ca_Session_t;
#define Ca_IMPL(x) CryptoAuth_ ## x
#define Ca_new CryptoAuth_new
#endif

enum Ca_addUser_Res {
    Ca_addUser_DUPLICATE = -3,
};
static inline int Ca_addUser_ipv6(String* password,
                            String* login,
                            uint8_t ipv6[16],
                            Ca_t* ca)
{
    return Ca_IMPL(addUser_ipv6)(password, login, ipv6, ca);
}
static inline int Ca_addUser(String* password, String* login, Ca_t* ca)
{
    return Ca_addUser_ipv6(password, login, NULL, ca);
}
static inline int Ca_removeUsers(Ca_t* context, String* user)
{
    return Ca_IMPL(removeUsers)(context, user);
}
static inline RTypes_StrList_t* Ca_getUsers(const Ca_t* context, struct Allocator* alloc)
{
    return Ca_IMPL(getUsers)(context, alloc);
}
static inline Ca_Session_t* Ca_newSession(Ca_t* ca,
                                          struct Allocator* alloc,
                                          const uint8_t herPublicKey[32],
                                          const bool requireAuth,
                                          const char* name,
                                          bool useNoise)
{
    return Ca_IMPL(newSession)(ca, alloc, herPublicKey, requireAuth, name, useNoise);
}
static inline void Ca_setAuth(const String* password,
                              const String* login,
                              Ca_Session_t* caSession)
{
    Ca_IMPL(setAuth)(password, login, caSession);
}
static inline void Ca_resetIfTimeout(Ca_Session_t* session)
{
    Ca_IMPL(resetIfTimeout)(session);
}
static inline void Ca_reset(Ca_Session_t* caSession)
{
    Ca_IMPL(reset)(caSession);
}
static inline RTypes_CryptoAuth_State_t Ca_getState(Ca_Session_t* session)
{
    return Ca_IMPL(getState)(session);
}
static inline void Ca_getHerPubKey(const Ca_Session_t* session, uint8_t *pkOut)
{
    Ca_IMPL(getHerPubKey)(session, pkOut);
}
static inline void Ca_getHerIp6(const Ca_Session_t* session, uint8_t *ipOut)
{
    Ca_IMPL(getHerIp6)(session, ipOut);
}
static inline void Ca_getPubKey(const Ca_t* ca, uint8_t *pkOut)
{
    Ca_IMPL(getPubKey)(ca, pkOut);
}
static inline String_t *Ca_getName(const Ca_Session_t* session, Allocator_t *alloc)
{
    return Ca_IMPL(getName)(session, alloc);
}
static inline void Ca_stats(const Ca_Session_t* session, RTypes_CryptoStats_t *statsOut)
{
    Ca_IMPL(stats)(session, statsOut);
}


enum Ca_DecryptErr {
    Ca_DecryptErr_NONE = 0,

    // Packet too short
    Ca_DecryptErr_RUNT = 1,

    // Received a run message to an un-setup session
    Ca_DecryptErr_NO_SESSION = 2,

    Ca_DecryptErr_FINAL_SHAKE_FAIL = 3,

    Ca_DecryptErr_FAILED_DECRYPT_RUN_MSG = 4,

    Ca_DecryptErr_KEY_PKT_ESTABLISHED_SESSION = 5,

    Ca_DecryptErr_WRONG_PERM_PUBKEY = 6,

    // Only specific IPv6 can connect to this CA session and the request has the wrong one.
    Ca_DecryptErr_IP_RESTRICTED = 7,

    // Authentication is required and is missing.
    Ca_DecryptErr_AUTH_REQUIRED = 8,

    // Basically this means the login name doesn't exist, beware of giving this information up.
    Ca_DecryptErr_UNRECOGNIZED_AUTH = 9,

    // Key packet and we are not in a state to accept a key packet
    Ca_DecryptErr_STRAY_KEY = 10,

    Ca_DecryptErr_HANDSHAKE_DECRYPT_FAILED = 11,

    // Set zero as the temporary public key
    Ca_DecryptErr_WISEGUY = 12,

    // Duplicate hello or key packet (same temp key and not a repeat-packet type)
    // Or repeat key packet with different key than what is known
    // Or a repeat hello packet for which we already know the temp key (meaning it is associated
    // with an existing session) when we are not in a state to accept a repeat hello.
    Ca_DecryptErr_INVALID_PACKET = 13,

    // Replay checker could not validate this packet
    Ca_DecryptErr_REPLAY = 14,

    // Authenticated decryption failed
    Ca_DecryptErr_DECRYPT = 15
};



#define Ca_State_INIT           RTypes_CryptoAuth_State_t_Init
#define Ca_State_SENT_HELLO     RTypes_CryptoAuth_State_t_SentHello
#define Ca_State_RECEIVED_HELLO RTypes_CryptoAuth_State_t_ReceivedHello
#define Ca_State_SENT_KEY       RTypes_CryptoAuth_State_t_SentKey
#define Ca_State_RECEIVED_KEY   RTypes_CryptoAuth_State_t_ReceivedKey
#define Ca_State_ESTABLISHED    RTypes_CryptoAuth_State_t_Established

static inline char* Ca_stateString(RTypes_CryptoAuth_State_t state)
{
    switch (state) {
        case Ca_State_INIT:           return "INIT";
        case Ca_State_SENT_HELLO:     return "SENT_HELLO";
        case Ca_State_RECEIVED_HELLO: return "RECEIVED_HELLO";
        case Ca_State_SENT_KEY:       return "SENT_KEY";
        case Ca_State_RECEIVED_KEY:   return "RECEIVED_KEY";
        case Ca_State_ESTABLISHED:    return "ESTABLISHED";
        default: return "INVALID";
    }
}

#endif
