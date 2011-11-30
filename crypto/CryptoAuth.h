#ifndef CRYPTO_AUTH_H
#define CRYPTO_AUTH_H

#include <stdint.h>
#include <stdbool.h>

#include "interface/Interface.h"
#include "libbenc/benc.h"
#include "memory/MemAllocator.h"

struct CryptoAuth;

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
 * @param context The CryptoAuth context.
 * @return 0 if all goes well, -1 if the authentication method is not supported and -2 if there is
 *         not enough space to store the user.
 */
int32_t CryptoAuth_addUser(String* password,
                           uint8_t authType,
                           void* user,
                           struct CryptoAuth* context);

/**
 * Get the user object associated with the authenticated session or NULL if there is none.
 * Please make sure to only call this on interfaces which were actually returned by
 * CryptoAuth_wrapInterface() as strange and interesting bugs will result otherwise.
 *
 * @param interface an interface as returned by CryptoAuth_wrapInterface().
 * @return the user object added by calling CryptoAuth_addUser() or NULL if this session is not
 *         authenticated.
 */
void* CryptoAuth_getUser(struct Interface* interface);

/**
 * Create a new crypto authenticator.
 *
 * @param allocator the means of aquiring memory.
 * @param privateKey the private key to use for this CryptoAuth or null if one should be generated.
 * @return a new CryptoAuth context.
 */
struct CryptoAuth* CryptoAuth_new(struct MemAllocator* allocator, const uint8_t privateKey[32]);

/**
 * Wrap an interface with crypto authentication.
 *
 * @param toWarp the interface to wrap
 * @param herPublicKey the public key of the other party or NULL if unknown.
 * @param password the password to use for authenticating, this must match the password given to
 *                 CryptoAuth_addUser() at the other end of the connection.
 * @param authType this must match CryptoAuth_addUser() at the other end of the connection.
 * @param authenticatePackets if true, all packets will be protected against forgery and replay
 *                            attacks, this is a seperate system from password and authType.
 * @param context the CryptoAuth context.
 */
struct Interface* CryptoAuth_wrapInterface(struct Interface* toWrap,
                                           const uint8_t herPublicKey[32],
                                           String* password,
                                           uint8_t authType,
                                           bool authenticatePackets,
                                           struct CryptoAuth* context);

#endif
