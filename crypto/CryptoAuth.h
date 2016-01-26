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
#ifndef CryptoAuth_H
#define CryptoAuth_H

#include "benc/Object.h"
#include "crypto/random/Random.h"
#include "crypto/ReplayProtector.h"
#include "memory/Allocator.h"
#include "util/Endian.h"
#include "util/log/Log.h"
#include "util/events/EventBase.h"
#include "wire/Message.h"
#include "util/Linker.h"
Linker_require("crypto/CryptoAuth.c");

#include <stdint.h>
#include <stdbool.h>

#define CryptoAuth_DEFAULT_RESET_AFTER_INACTIVITY_SECONDS 60

struct CryptoAuth
{
    uint8_t publicKey[32];

    /**
     * After this number of seconds of inactivity,
     * a connection will be reset to prevent them hanging in a bad state.
     */
    uint32_t resetAfterInactivitySeconds;
};

struct CryptoAuth_Session
{
    uint8_t herPublicKey[32];

    String* displayName;

    struct ReplayProtector replayProtector;

    /**
     * Bind this CryptoAuth session to the other node's ip6 address,
     * any packet avertizing a key which doesn't hash to this will be dropped.
     */
    uint8_t herIp6[16];
};

/**
 * Associate a password:authtype pair with a user object.
 * Calling CryptoAuth_getUser() on any interface which has established a connection with
 * the same password:authType pair will return the same user object.
 *
 * @param password This should be a key derived from the password using a good key derivation
 *                 function, using a plaintext password here is NOT recommended.
 * @param authType The method of authenticating the user, only option currently is 1 for sha256
 *                 based authentication.
 * @param user The thing to associate with this user, will be returned by CryptoAuth_getUser().
 *             If this is NULL and requireAuthentication is enabled, authentication will fail.
 *             Duplicate user entires are OK.
 * @param ipv6 If not NULL, only allow connections to this CryptoAuth from the key which hashes
 *             to the given IPv6 address.
 * @param context The CryptoAuth context.
 * @return 0 if all goes well,
 *         CryptoAuth_addUser_DUPLICATE if the same *password* already exists.
 */
#define CryptoAuth_addUser_DUPLICATE         -3
int CryptoAuth_addUser_ipv6(String* password,
                            String* login,
                            uint8_t ipv6[16],
                            struct CryptoAuth* ca);

static inline int CryptoAuth_addUser(String* password, String* login, struct CryptoAuth* ca)
{
    return CryptoAuth_addUser_ipv6(password, login, NULL, ca);
}

/**
 * Remove all users registered with this CryptoAuth.
 *
 * @param context the context to remove users for.
 * @param user the identifier which was passed to addUser(), all users with this id will be removed.
 * @return the number of users removed.
 */
int CryptoAuth_removeUsers(struct CryptoAuth* context, String* user);

/**
 * Get a list of all the users added via addUser.
 *
 * @param context the context used to call addUser.
 * @param alloc the Allocator to use to create the usersOut array.
 * @returns List* containing the user String's
 */
List* CryptoAuth_getUsers(struct CryptoAuth* context, struct Allocator* alloc);

/**
 * Create a new crypto authenticator.
 *
 * @param allocator the means of aquiring memory.
 * @param privateKey the private key to use for this CryptoAuth or null if one should be generated.
 * @param eventBase the libevent context for handling timeouts.
 * @param logger the mechanism for logging output from the CryptoAuth.
 *               if NULL then no logging will be done.
 * @param rand random number generator.
 * @return a new CryptoAuth context.
 */
struct CryptoAuth* CryptoAuth_new(struct Allocator* allocator,
                                  const uint8_t* privateKey,
                                  struct EventBase* eventBase,
                                  struct Log* logger,
                                  struct Random* rand);

/**
 * Wrap an interface with crypto authentication.
 *
 * NOTE2: Every received packet is prefixed by the 4 byte *nonce* for that packet
 *        in host endian order.
 *
 * @param toWarp the interface to wrap
 * @param herPublicKey the public key of the other party or NULL if unknown.
 * @param requireAuth if the remote end of this interface begins the connection, require
 *                    them to present valid authentication credentials to connect.
 *                    If this end begins the connection, this parameter has no effect.
 * @param name a name for this CA which will appear in logs.
 * @param context the CryptoAuth context.
 */
struct CryptoAuth_Session* CryptoAuth_newSession(struct CryptoAuth* ca,
                                                 struct Allocator* alloc,
                                                 const uint8_t herPublicKey[32],
                                                 const bool requireAuth,
                                                 char* name);

/** @return 0 on success, -1 otherwise. */
int CryptoAuth_encrypt(struct CryptoAuth_Session* session, struct Message* msg);

/** @return 0 on success, -1 otherwise. */
int CryptoAuth_decrypt(struct CryptoAuth_Session* session, struct Message* msg);

/**
 * Choose the authentication credentials to use.
 * WARNING: Even if the remote end begins the connection, these credentials will be presented which
 *          will cause the connection initiation to fail if the remote end does not know of them.
 *
 * @param password the password to use for authenticating, this must match the password given to
 *                 CryptoAuth_addUser() at the other end of the connection.
 * @param login the username to use for logging in with the other node.
 *              if null then the authtype will be type 1 (password only).
 * @param wrappedInterface this MUST be the output from CryptoAuth_wrapInterface().
 */
void CryptoAuth_setAuth(const String* password,
                        const String* login,
                        struct CryptoAuth_Session* caSession);

/** Reset the session's state to CryptoAuth_NEW, a new connection will be negotiated. */
//void CryptoAuth_reset(struct CryptoAuth_Session* session);

void CryptoAuth_resetIfTimeout(struct CryptoAuth_Session* session);

void CryptoAuth_reset(struct CryptoAuth_Session* caSession);

/** New CryptoAuth session, has not sent or received anything. */
#define CryptoAuth_NEW         0

/** Sent a hello message, waiting for reply. */
#define CryptoAuth_HANDSHAKE1  1

/** Received a hello message, sent a key message, waiting for the session to complete. */
#define CryptoAuth_HANDSHAKE2  2

/** Sent a hello message and received a key message but have not gotten a data message yet. */
#define CryptoAuth_HANDSHAKE3  3

/** The CryptoAuth session has successfully done a handshake and received at least one message. */
#define CryptoAuth_ESTABLISHED 4

/** The number of states */
#define CryptoAuth_STATE_COUNT 5

static inline char* CryptoAuth_stateString(int state)
{
    switch (state) {
        case CryptoAuth_NEW:         return "CryptoAuth_NEW";
        case CryptoAuth_HANDSHAKE1:  return "CryptoAuth_HANDSHAKE1";
        case CryptoAuth_HANDSHAKE2:  return "CryptoAuth_HANDSHAKE2";
        case CryptoAuth_HANDSHAKE3:  return "CryptoAuth_HANDSHAKE3";
        case CryptoAuth_ESTABLISHED: return "CryptoAuth_ESTABLISHED";
        default: return "INVALID";
    }
}

/**
 * Get the state of the CryptoAuth session.
 *
 * @param interface a CryptoAuth wrapper.
 * @return one of CryptoAuth_NEW,
 *                CryptoAuth_HANDSHAKE1,
 *                CryptoAuth_HANDSHAKE2 or
 *                CryptoAuth_ESTABLISHED
 */
int CryptoAuth_getState(struct CryptoAuth_Session* session);

#endif
