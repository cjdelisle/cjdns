#ifndef CRYPTO_AUTH_H
#define CRYPTO_AUTH_H

#include <stdint.h>
#include <stdbool.h>

#include "crypto/Session.h"
#include "interface/Interface.h"
#include "libbenc/benc.h"
#include "memory/MemAllocator.h"
#include "util/Endian.h"

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
 *             If this is NULL and requireAuthentication is enabled, authentication will fail.
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
 * @param requireAuth if the remote end of this interface begins the connection, require
 *                    them to present valid authentication credentials to connect.
 *                    If this end begins the connection, this parameter has no effect.
 * @param authenticatePackets if true, all packets will be protected against forgery and replay
 *                            attacks, this is a seperate system from password and authType.
 * @param context the CryptoAuth context.
 */
struct Interface* CryptoAuth_wrapInterface(struct Interface* toWrap,
                                           uint8_t herPublicKey[32],
                                           const bool requireAuth,
                                           bool authenticatePackets,
                                           struct CryptoAuth* context);

/**
 * Choose the authentication credentials to use.
 * WARNING: Even if the remote end begins the connection, these credentials will be presented which
 *          will cause the connection initiation to fail if the remote end does not know of them.
 *
 * @param password the password to use for authenticating, this must match the password given to
 *                 CryptoAuth_addUser() at the other end of the connection.
 * @param authType this must match CryptoAuth_addUser() at the other end of the connection.
 * @param wrappedInterface this MUST be the output from CryptoAuth_wrapInterface().
 */
void CryptoAuth_setAuth(const String* password,
                        const uint8_t authType,
                        struct Interface* wrappedInterface);

void CryptoAuth_getPublicKey(uint8_t output[32], struct CryptoAuth* context);

uint8_t* CryptoAuth_getHerPublicKey(struct Interface* interface);

/**
 * Obfuscate the nonce so traffic type cannot be detected statelessly.
 * This does not guarantee that traffic type cannot be detected if packets are compared
 * to older packets or the intercepting box knows the public keys of the nodes.
 *
 * This function takes the nonce and a piece of salt data which is available at both ends, the
 * salt is used to extract information from the public keys of each node and that information,
 * as well as the salt itself, is XORd against the nonce to frustrate protocol analysis.
 *
 * The salt is read as a big endian number as follows.
 *
 *                       1               2               3
 *       0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *    0 |             unused            |  okRot  |  tkRot  | OKI | TKI |
 *      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * TKI: Their key index, the offset from the beginning of their key,
 *      measured in 4 byte increments.
 * OKI: Our key index, the offset from the beginning of our permanent public key,
 *      measured in 4 byte increments.
 * tkRot: How many bits to rotate the 4 bytes which were sampled from their key.
 * okRot: How many bits to rotate the 4 bytes which were sampled from our key.
 *
 * tks: "their key sample", 4 bytes sampled from their permanent public key at offset TKI * 4.
 * oks: "our key sample", 4 bytes sampled from our permanent public key at offset OKI * 4.
 *
 * NOTE: rotate() means rotate right on little endian architectures and rotate left on big endian.
 *       this allows for interoperability without needing to byte swap tks and oks for rotating,
 *       only to byte swap them back after.
 *
 * @param nonceAndSalt an array containing two 32 bit integers, the nonce to obfuscate
 *                     and a piece of unpredictable data for the salt.
 * @param theirKey the other node's public permanent key as an array of 32 bit integers.
 * @param ourKey this node's public permanent key as an array of 32 bit integers.
 * @return nonce ^ rotate(tks, tkRot) ^ rotate(oks, okRot) ^ salt;
 */
static inline uint32_t CryptoAuth_obfuscateNonce(uint32_t nonceAndSalt[2],
                                                 uint32_t theirKey[8],
                                                 uint32_t ourKey[8])
{
    #define CryptoAuth_rotate(number, bits) \
        (Endian_isBigEndian()                                    \
            ? ((number << (bits)) | (number >> (32 - (bits))))   \
            : ((number >> (bits)) | (number << (32 - (bits)))))

    uint32_t salt = Endian_bigEndianToHost32(nonceAndSalt[1]);
    uint32_t tkRot = (salt >> 6) % 32;
    uint32_t okRot = (salt >> 11) % 32;
    uint32_t tks = theirKey[salt % 8];
    uint32_t oks = ourKey[(salt >> 3) % 8];

    return nonceAndSalt[0]
        ^ CryptoAuth_rotate(tks, tkRot)
        ^ CryptoAuth_rotate(oks, okRot)
        ^ nonceAndSalt[1];

    #undef CryptoAuth_rotate
}
#define CryptoAuth_deobfuscateNonce(nonceAndData, theirKey, ourKey) \
    CryptoAuth_obfuscateNonce(nonceAndData, ourKey, theirKey)

void CryptoAuth_getSession(struct Session* output, struct Interface* interface);

#endif
