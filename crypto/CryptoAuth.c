#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "crypto_box_curve25519xsalsa20poly1305.h"
#include "crypto_core_hsalsa20.h"
#include "crypto_hash_sha256.h"
#include "crypto_scalarmult_curve25519.h"
#include "crypto_stream_salsa20.h"
#include "crypto_stream_xsalsa20.h"

#include "crypto/Crypto.h"
#include "memory/MemAllocator.h"
#include "libbenc/benc.h"
#include "wire/Headers.h"

/** The constant used in nacl. */
static const uint8_t keyHashSigma[16] = "expand 32-byte k";
static const uint8_t keyHashNonce[16] = {0};

struct CryptoAuth_Session
{
    void* user;

    /**
     * Usually the shared secret.
     * Between genHandshake() and readHandshake2(), this will be our temporary private key.
     * Between readHandshake() and genHandshake2(), this will be the peer's temporary public key.
     */
    uint8_t secret[32];

    /** The next nonce to use. */
    uint32_t nextNonce;

    /** True if this node began the conversation. */
    bool isClient;
};

struct Auth {
    union Headers_AuthChallenge challenge;

    uint8_t secret[32];

    void* user;
};

struct CryptoAuth
{
    uint8_t privateKey[32];

    uint8_t publicKey[32];

    struct Auth* passwords;

    uint32_t passwordCapacity;

    uint32_t passwordCount;

    struct CryptoAuth_Session* sessions;

    uint32_t sessionCapacity;

    uint32_t sessionCount;

    struct MemAllocator* allocator;
};

struct CryptoAuth* CryptoAuth_new(struct MemAllocator* allocator)
{
    struct CryptoAuth* ca = allocator->calloc(sizeof(struct CryptoAuth), 1, allocator);
    ca->allocator = allocator;

    ca->passwords = allocator->calloc(sizeof(struct Auth), 256, allocator);
    ca->passwordCapacity = 256;

    ca->sessions = allocator->calloc(sizeof(struct CryptoAuth_Session), 256, allocator);
    ca->sessionCapacity = 256;

    crypto_box_curve25519xsalsa20poly1305_keypair(ca->publicKey, ca->privateKey);
    return ca;
}

/**
 * Get a shared secret.
 *
 * @param outputSecret an array to place the shared secret in.
 * @param myPrivateKey
 * @param herPublicKey
 * @param passwordHash a 16 byte value known to both ends, this must be provably pseudorandom
 *                     the first 16 bytes of a sha256 output from hashing a password is ok,
 *                     whatever she happens to send me in the Auth field is NOT ok.
 *                     If this field is null, the secret will be generated without the password.
 */
static void getSharedSecret(uint8_t outputSecret[32],
                            uint8_t myPrivateKey[32],
                            uint8_t herPublicKey[32],
                            uint8_t passwordHash[32])
{
    uint8_t tempBuff[64];
    crypto_scalarmult_curve25519(tempBuff, myPrivateKey, herPublicKey);
    if (passwordHash == NULL) {
        crypto_core_hsalsa20(outputSecret, keyHashNonce, tempBuff, keyHashSigma);
    } else {
        memcpy(&tempBuff[32], passwordHash, 32);
        crypto_hash_sha256(outputSecret, tempBuff, 64);
    }
}

static void hashPassword_sha256(struct Auth* auth, String* password)
{
    uint8_t tempBuff[32];
    crypto_hash_sha256(auth->secret, (uint8_t*) password->bytes, password->len);
    crypto_hash_sha256(tempBuff, auth->secret, 32);
    memcpy(auth->challenge.bytes, tempBuff, 16);
    auth->challenge.challenge.derivations = 0;
    auth->challenge.challenge.type = 1;
}

/**
 * Search the authorized passwords for one matching this auth header.
 *
 * @param auth the auth header.
 * @param context the CryptoAuth engine to search in.
 * @return an Auth struct with a if one is found, otherwise NULL.
 */
static struct Auth* getAuth(union Headers_AuthChallenge auth, struct CryptoAuth* context)
{
    if (auth.challenge.type != 1) {
        return NULL;
    }
    for (uint32_t i = 0; i < context->passwordCount; i++) {
        if (memcmp(auth.bytes, &context->passwords[i], 15) == 0) {
            return &context->passwords[i];
        }
    }
    return NULL;
}

static void getPasswordHash(uint8_t output[32], uint8_t derivations, struct Auth* auth)
{
    uint8_t tempBuff[32];
    memcpy(output, auth->secret, 32);
    for (uint32_t i = 0; i < derivations; i++) {
        crypto_hash_sha256(tempBuff, output, 32);
        crypto_hash_sha256(output, tempBuff, 32);
    }
}
/*
static int32_t hashPassword_sha256_derive(struct Auth* newAuth, struct Auth* existingAuth)
{
    if (existingAuth->challenge.challenge.derivations == 255) {
        return -1;
    }
    uint8_t tempBuff[32];
    getPasswordHash(tempBuff, (existingAuth->challenge.challenge.derivations - 1), existingAuth);
    crypto_hash_sha256(newAuth->secret, tempBuff, 32);
    existingAuth->challenge.challenge.derivations++;
    memcpy(newAuth->challenge.bytes,
           existingAuth->challenge.bytes,
           sizeof(union Headers_AuthChallenge));
}*/

static inline uint8_t* tryAuth(struct Headers_Handshake* handshake,
                               uint8_t hashOutput[32],
                               struct CryptoAuth* context,
                               struct CryptoAuth_Session* session)
{
    struct Auth* auth = getAuth(handshake->auth, context);
    if (auth != NULL) {
        getPasswordHash(hashOutput, handshake->auth.challenge.derivations, auth);
        if (handshake->auth.challenge.derivations == 0) {
            session->user = auth->user;
        }
        return hashOutput;
    }
    return NULL;
}

static inline int32_t decryptKey(struct Headers_Handshake* handshake,
                                 uint8_t sharedSecret[32],
                                 uint8_t output[32])
{
    uint8_t key[crypto_box_curve25519xsalsa20poly1305_ZEROBYTES + 32];
    uint8_t boxedKey[crypto_box_curve25519xsalsa20poly1305_BOXZEROBYTES + 48];
    memcpy(&boxedKey[crypto_box_curve25519xsalsa20poly1305_BOXZEROBYTES],
           handshake->encryptedTempKey, 48);
    uint64_t cipherLength = crypto_box_curve25519xsalsa20poly1305_BOXZEROBYTES + 48;
    if (crypto_box_curve25519xsalsa20poly1305_open_afternm(
        key, boxedKey, cipherLength, handshake->nonce, sharedSecret) != 0)
    {
        return -1;
    }
    memcpy(output, &key[crypto_box_curve25519xsalsa20poly1305_ZEROBYTES], 32);
    return 0;
}

static inline int32_t encryptKey(struct Headers_Handshake* handshake,
                                 uint8_t sharedSecret[32],
                                 uint8_t toEncrypt[32])
{
    uint8_t key[crypto_box_curve25519xsalsa20poly1305_ZEROBYTES + 32];
    uint8_t boxedKey[crypto_box_curve25519xsalsa20poly1305_BOXZEROBYTES + 48];
    memcpy(&key[crypto_box_curve25519xsalsa20poly1305_ZEROBYTES], toEncrypt, 32);
    uint64_t cipherLength = crypto_box_curve25519xsalsa20poly1305_BOXZEROBYTES + 48;
    if (crypto_box_curve25519xsalsa20poly1305_afternm(
        key, boxedKey, cipherLength, handshake->nonce, sharedSecret) != 0)
    {
        return -1;
    }
    memcpy(handshake->encryptedTempKey,
           &boxedKey[crypto_box_curve25519xsalsa20poly1305_BOXZEROBYTES], 48);
    return 0;
}

int32_t CryptoAuth_addUser(String* password,
                           uint8_t authType,
                           void* user,
                           struct CryptoAuth* context)
{
    if (authType != 1) {
        return -1;
    }
    if (context->passwordCount == context->passwordCapacity) {
        // TODO: realloc password space and increase buffer.
        return -2;
    }
    hashPassword_sha256(&context->passwords[context->passwordCount], password);
    context->passwords[context->passwordCount].user = user;
    context->passwordCount++;
    return 0;
}

/**
 * Works for both encryption and decryption.
 * only for the content of handshake messages.
 *
 * @param message the message to (de)crypt.
 * @param nonce a random number.
 * @param secret a shared secret.
 */
static inline int cryptHandshakeMessage(String* message, uint8_t nonce[24], uint8_t secret[32])
{
    return crypto_stream_xsalsa20_xor((uint8_t*) message->bytes,
                                      (uint8_t*) message->bytes,
                                      message->len,
                                      nonce,
                                      secret);
}

/**
 * Leaves the temporary private key in the session.secret field.
 */
static int32_t genHandshake(struct Headers_Handshake* handshake,
                            String* password,
                            String* message,
                            uint8_t herPublicKey[32],
                            struct CryptoAuth_Session* session,
                            struct CryptoAuth* context)
{
    uint8_t* passwordHash = NULL;
    if (password != NULL) {
        struct Auth auth;
        hashPassword_sha256(&auth, password);
        passwordHash = auth.secret;
        memcpy(handshake->auth.bytes, &auth.challenge, sizeof(union Headers_AuthChallenge));
    } else {
        // no auth.
        ((union Headers_AuthChallenge*) &handshake->auth)->challenge.type = 0;
    }

    Crypto_randomize(&(String) { .bytes = (char*)handshake->nonce, .len = 24 });
    memcpy(handshake->publicKey, context->publicKey, 32);

    getSharedSecret(session->secret, context->privateKey, herPublicKey, passwordHash);

    uint8_t tempPrivateKey[32];
    uint8_t tempPublicKey[32];
    crypto_box_curve25519xsalsa20poly1305_keypair(tempPublicKey, tempPrivateKey);
    if (encryptKey(handshake, session->secret, tempPublicKey) == 0) {
        cryptHandshakeMessage(message, handshake->nonce, session->secret);
        memcpy(session->secret, tempPrivateKey, 32);
        return 0;
    }
    return -1;
}

/**
 * Generate the first handshake cycle.
 * Send a public key cryptoauthed with my private key, her public key and any password if specified.
 * This encrypts the supplied message and a temporary public key.
 *
 * @param handshake a pointer to a handshake header which will be filled in.
 * @param herPublicKey
 * @param password if null, auth will be set to none.
 * @param message will be encrypted in place.
 * @param context the cryptoauth context.
 * @return a new session which may be used with CryptoAuth_readHandshake2().
 */
struct CryptoAuth_Session* CryptoAuth_genHandshake(struct Headers_Handshake* handshake,
                                                   uint8_t herPublicKey[32],
                                                   String* password,
                                                   String* message,
                                                   struct CryptoAuth* context)
{
    if (context->sessionCount >= context->sessionCapacity) {
        // TODO: expand buffer.
        return NULL;
    }

    struct CryptoAuth_Session* out = &context->sessions[context->sessionCount];
    out->nextNonce = 0;

    if (genHandshake(handshake, password, message, herPublicKey, out, context) != 0) {
        return NULL;
    }
    context->sessionCount++;

    return out;
}

/**
 * Decrypt the first handshake cycle.
 * This validates the password from CryptoAuth_genHandshake() and generates a key from the
 * password, our private key and his public key. This validates the success of the agreement
 * by decrypting an authenticated temporary public key for the foreign node. This is temporarily
 * stored as our shared secret.
 *
 * @param handshake the handshake header.
 * @param message, the content which will be decrypted in place.
 * @param context the cryptoauth context.
 * @return a new cryptoauth session or NULL if something goes wrong.
 */
struct CryptoAuth_Session* CryptoAuth_readHandshake(struct Headers_Handshake* handshake,
                                                    String* message,
                                                    struct CryptoAuth* context)
{
    if (context->sessionCount >= context->sessionCapacity) {
        // TODO: expand buffer.
        return NULL;
    }
    uint8_t tmpBuff[32];

    struct CryptoAuth_Session* out = &context->sessions[context->sessionCount];
    out->nextNonce = 1;
    uint8_t* passwordHash = tryAuth(handshake, tmpBuff, context, out);
    getSharedSecret(out->secret, context->privateKey, handshake->publicKey, passwordHash);

    // Decrypt her temp public key and store it in session->secret.
    if (decryptKey(handshake, out->secret, tmpBuff) == 0) {
        cryptHandshakeMessage(message, handshake->nonce, out->secret);
        memcpy(out->secret, tmpBuff, 32);
        context->sessionCount++;
        return out;
    }
    return NULL;
}

/**
 * Generate the second handshake cycle.
 * Generate a temporary key and encrypt it using our perminent key and the other node's
 * temporary key, after this cycle completes, we switch our shared secret to the fully
 * temporary secret, any failures along this process cause it to drop all state and begin again.
 *
 * @param handshake a pointer to a handshake header which will be filled in.
 * @param password if null, auth will be set to none.
 * @param message will be encrypted in place.
 * @param session the session created by CryptoAuth_readHandshake().
 * @return 0 if successful, -1 if session in wrong state.
 */
int32_t CryptoAuth_genHandshake2(struct Headers_Handshake* handshake,
                                 String* password,
                                 String* message,
                                 struct CryptoAuth_Session* session,
                                 struct CryptoAuth* context)
{
    if (session->nextNonce != 1) {
        return -1;
    }
    session->nextNonce = 3;

    uint8_t herPublicKey[32];
    memcpy(herPublicKey, session->secret, 32);
    if (genHandshake(handshake, password, message, herPublicKey, session, context) != 0) {
        return -2;
    }

    return 0;
}

/**
 * Decrypt the second handshake cycle.
 *
 * @param handshake the handshake packet.
 * @param message, the content which will be decrypted in place.
 * @param session the session created by CryptoAuth_genHandshake().
 * @return 0 if successful, -1 if session in wrong state, -2 if sanity check fails.
 */
int32_t CryptoAuth_readHandshake2(struct Headers_Handshake* handshake,
                                  String* message,
                                  struct CryptoAuth_Session* session,
                                  struct CryptoAuth* context)
{
    if (session->nextNonce != 0) {
        return -1;
    }
    session->nextNonce = 2;

    // Load our session public key.
    uint8_t tempPrivateKey[32];
    memcpy(tempPrivateKey, session->secret, 32);

    uint8_t tmpBuff[32];
    uint8_t* passwordHash = tryAuth(handshake, tmpBuff, context, session);
    getSharedSecret(session->secret, tempPrivateKey, handshake->publicKey, passwordHash);

    // Decrypt her temp public key and store it in session->secret.
    if (decryptKey(handshake, session->secret, tmpBuff) == 0) {
        cryptHandshakeMessage(message, handshake->nonce, session->secret);
        memcpy(session->secret, tmpBuff, 32);
        return 0;
    }
    return -1;
}

static inline void cipher(struct Headers_Encrypted* header,
                          String* msg,
                          struct CryptoAuth_Session* session,
                          bool isClient)
{
    union {
        uint32_t ints[2];
        uint8_t bytes[8];
    } nonce = { .ints = {0, 0} };
    nonce.ints[isClient] = header->nonce;

    crypto_stream_salsa20_xor((uint8_t*)msg->bytes,
                              (uint8_t*)msg->bytes,
                              msg->len,
                              nonce.bytes,
                              session->secret);
}

/**
 * Encrypt a message.
 *
 * @param header a pointer to some space which will be filled with the header.
 * @param message the message which will be encrypted in place.
 * @param session the session as modified by CryptoAuth_genHandshake2()
 *                or CryptoAuth_readHandshake2().
 * @return 0 unless the nonce rolls over in which case return -1.
 */
int32_t CryptoAuth_encrypt(struct Headers_Encrypted* header,
                           String* message,
                           struct CryptoAuth_Session* session)
{
    if (session->nextNonce == 0) {
        return -1;
    }
    header->nonce = session->nextNonce;
    cipher(header, message, session, session->isClient);
    session->nextNonce++;
    return 0;
}

/**
 * Decrypt a message.
 *
 * @param header the header to read.
 * @param message the message which will be encrypted in place.
 * @param session the session as modified by CryptoAuth_genHandshake2()
 *                or CryptoAuth_readHandshake2().
 */
void CryptoAuth_decrypt(struct Headers_Encrypted* header,
                        String* message,
                        struct CryptoAuth_Session* session)
{
    cipher(header, message, session, !session->isClient);
}
