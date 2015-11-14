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
#include "crypto/CryptoAuth_pvt.h"
#include "crypto/AddressCalc.h"
#include "crypto/ReplayProtector.h"
#include "crypto/random/Random.h"
#include "benc/Dict.h"
#include "benc/List.h"
#include "benc/String.h"
#include "util/log/Log.h"
#include "memory/Allocator.h"
#include "util/Assert.h"
#include "util/AddrTools.h"
#include "util/Bits.h"
#include "util/Defined.h"
#include "util/Endian.h"
#include "util/Hex.h"
#include "util/events/Time.h"
#include "wire/Error.h"
#include "wire/Headers.h"
#include "wire/Message.h"

#include "crypto_box_curve25519xsalsa20poly1305.h"
#include "crypto_hash_sha256.h"
#include "crypto_scalarmult_curve25519.h"

#include <stdint.h>
#include <stdbool.h>

static inline void printHexKey(uint8_t output[65], uint8_t key[32])
{
    if (key) {
        Hex_encode(output, 65, key, 32);
    } else {
        Bits_memcpy(output, "NULL", 5);
    }
}

static inline void printHexPubKey(uint8_t output[65], uint8_t privateKey[32])
{
    if (privateKey) {
        uint8_t publicKey[32];
        crypto_scalarmult_curve25519_base(publicKey, privateKey);
        printHexKey(output, publicKey);
    } else {
        printHexKey(output, NULL);
    }
}

/**
 * Get a shared secret.
 *
 * @param outputSecret an array to place the shared secret in.
 * @param myPrivateKey
 * @param herPublicKey
 * @param logger
 * @param passwordHash a 32 byte value known to both ends, this must be provably pseudorandom
 *                     the first 32 bytes of a sha256 output from hashing a password is ok,
 *                     whatever she happens to send me in the Auth field is NOT ok.
 *                     If this field is null, the secret will be generated without the password.
 */
static inline void getSharedSecret(uint8_t outputSecret[32],
                                   uint8_t myPrivateKey[32],
                                   uint8_t herPublicKey[32],
                                   uint8_t passwordHash[32],
                                   struct Log* logger)
{
    if (passwordHash == NULL) {
        crypto_box_curve25519xsalsa20poly1305_beforenm(outputSecret, herPublicKey, myPrivateKey);
    } else {
        union {
            struct {
                uint8_t key[32];
                uint8_t passwd[32];
            } components;
            uint8_t bytes[64];
        } buff;

        crypto_scalarmult_curve25519(buff.components.key, myPrivateKey, herPublicKey);
        Bits_memcpy(buff.components.passwd, passwordHash, 32);
        crypto_hash_sha256(outputSecret, buff.bytes, 64);
    }
    if (Defined(Log_KEYS)) {
        uint8_t myPublicKeyHex[65];
        printHexPubKey(myPublicKeyHex, myPrivateKey);
        uint8_t herPublicKeyHex[65];
        printHexKey(herPublicKeyHex, herPublicKey);
        uint8_t passwordHashHex[65];
        printHexKey(passwordHashHex, passwordHash);
        uint8_t outputSecretHex[65] = "NULL";
        printHexKey(outputSecretHex, outputSecret);
        Log_keys(logger,
                  "Generated a shared secret:\n"
                  "     myPublicKey=%s\n"
                  "    herPublicKey=%s\n"
                  "    passwordHash=%s\n"
                  "    outputSecret=%s\n",
                  myPublicKeyHex, herPublicKeyHex, passwordHashHex, outputSecretHex);
    }
}

static inline void hashPassword(uint8_t secretOut[32],
                                struct CryptoHeader_Challenge* challengeOut,
                                const String* login,
                                const String* password,
                                const uint8_t authType)
{
    crypto_hash_sha256(secretOut, (uint8_t*) password->bytes, password->len);
    uint8_t tempBuff[32];
    if (authType == 1) {
        crypto_hash_sha256(tempBuff, secretOut, 32);
    } else if (authType == 2) {
        crypto_hash_sha256(tempBuff, (uint8_t*) login->bytes, login->len);
    } else {
        Assert_failure("Unsupported auth type [%u]", authType);
    }
    Bits_memcpy(challengeOut, tempBuff, CryptoHeader_Challenge_SIZE);
    CryptoHeader_setAuthChallengeDerivations(challengeOut, 0);
    challengeOut->type = authType;
    challengeOut->additional = 0;
}

/**
 * Search the authorized passwords for one matching this auth header.
 *
 * @param auth the auth header.
 * @param context the CryptoAuth engine to search in.
 * @return an Auth struct with a if one is found, otherwise NULL.
 */
static inline struct CryptoAuth_User* getAuth(struct CryptoHeader_Challenge* auth,
                                              struct CryptoAuth_pvt* ca)
{
    if (auth->type == 0) {
        return NULL;
    }
    int count = 0;
    for (struct CryptoAuth_User* u = ca->users; u; u = u->next) {
        count++;
        if (auth->type == 1 &&
            !Bits_memcmp(auth, u->passwordHash, CryptoHeader_Challenge_KEYSIZE))
        {
            return u;
        } else if (auth->type == 2 &&
            !Bits_memcmp(auth, u->userNameHash, CryptoHeader_Challenge_KEYSIZE))
        {
            return u;
        }
    }
    Log_debug(ca->logger, "Got unrecognized auth, password count = [%d]", count);
    return NULL;
}

/**
 * Decrypt and authenticate.
 *
 * @param nonce a 24 byte number, may be random, cannot repeat.
 * @param msg a message to encipher and authenticate.
 * @param secret a shared secret.
 * @return 0 if decryption is succeddful, otherwise -1.
 */
static inline Gcc_USE_RET int decryptRndNonce(uint8_t nonce[24],
                                              struct Message* msg,
                                              uint8_t secret[32])
{
    if (msg->length < 16) {
        return -1;
    }
    Assert_true(msg->padding >= 16);
    uint8_t* startAt = msg->bytes - 16;
    uint8_t paddingSpace[16];
    Bits_memcpy(paddingSpace, startAt, 16);
    Bits_memset(startAt, 0, 16);
    if (!Defined(NSA_APPROVED)) {
        if (crypto_box_curve25519xsalsa20poly1305_open_afternm(
                startAt, startAt, msg->length + 16, nonce, secret) != 0)
        {
            return -1;
        }
    }

    Bits_memcpy(startAt, paddingSpace, 16);
    Message_shift(msg, -16, NULL);
    return 0;
}

/**
 * Encrypt and authenticate.
 * Shifts the message by 16 bytes.
 *
 * @param nonce a 24 byte number, may be random, cannot repeat.
 * @param msg a message to encipher and authenticate.
 * @param secret a shared secret.
 */
static inline void encryptRndNonce(uint8_t nonce[24],
                                   struct Message* msg,
                                   uint8_t secret[32])
{
    Assert_true(msg->padding >= 32);
    uint8_t* startAt = msg->bytes - 32;
    // This function trashes 16 bytes of the padding so we will put it back
    uint8_t paddingSpace[16];
    Bits_memcpy(paddingSpace, startAt, 16);
    Bits_memset(startAt, 0, 32);
    if (!Defined(NSA_APPROVED)) {
        crypto_box_curve25519xsalsa20poly1305_afternm(
            startAt, startAt, msg->length + 32, nonce, secret);
    }

    Bits_memcpy(startAt, paddingSpace, 16);
    Message_shift(msg, 16, NULL);
}

/**
 * Decrypt a packet.
 *
 * @param nonce a counter.
 * @param msg the message to decrypt, decrypted in place.
 * @param secret the shared secret.
 * @param isInitiator true if we started the connection.
 */
static inline Gcc_USE_RET int decrypt(uint32_t nonce,
                                      struct Message* msg,
                                      uint8_t secret[32],
                                      bool isInitiator)
{
    union {
        uint32_t ints[2];
        uint8_t bytes[24];
    } nonceAs = { .ints = {0, 0} };
    nonceAs.ints[!isInitiator] = Endian_hostToLittleEndian32(nonce);

    return decryptRndNonce(nonceAs.bytes, msg, secret);
}

/**
 * Encrypt a packet.
 *
 * @param nonce a counter.
 * @param msg the message to decrypt, decrypted in place.
 * @param secret the shared secret.
 * @param isInitiator true if we started the connection.
 */
static inline void encrypt(uint32_t nonce,
                           struct Message* msg,
                           uint8_t secret[32],
                           bool isInitiator)
{
    union {
        uint32_t ints[2];
        uint8_t bytes[24];
    } nonceAs = { .ints = {0, 0} };
    nonceAs.ints[isInitiator] = Endian_hostToLittleEndian32(nonce);

    encryptRndNonce(nonceAs.bytes, msg, secret);
}

static inline bool knowHerKey(struct CryptoAuth_Session_pvt* session)
{
    return !Bits_isZero(session->pub.herPublicKey, 32);
}

static void getIp6(struct CryptoAuth_Session_pvt* session, uint8_t* addr)
{
    Assert_true(knowHerKey(session));
    uint8_t ip6[16];
    AddressCalc_addressForPublicKey(ip6, session->pub.herPublicKey);
    AddrTools_printIp(addr, ip6);
}

#define cryptoAuthDebug(wrapper, format, ...) \
    {                                                                                            \
        uint8_t addr[40] = "unknown";                                                            \
        getIp6((session), addr);                                                                 \
        String* dn = (session)->pub.displayName;                                                 \
        Log_debug((session)->context->logger, "%p %s [%s]: " format, (void*)(session),           \
                  dn ? dn->bytes : "", addr, __VA_ARGS__);                                       \
    }

#define cryptoAuthDebug0(wrapper, format) \
    cryptoAuthDebug(session, format "%s", "")

static void reset(struct CryptoAuth_Session_pvt* session)
{
    session->nextNonce = 0;
    session->isInitiator = false;

    Bits_memset(session->ourTempPrivKey, 0, 32);
    Bits_memset(session->ourTempPubKey, 0, 32);
    Bits_memset(session->herTempPubKey, 0, 32);
    Bits_memset(session->sharedSecret, 0, 32);
    session->established = false;

    Bits_memset(&session->pub.replayProtector, 0, sizeof(struct ReplayProtector));
}

static void resetIfTimeout(struct CryptoAuth_Session_pvt* session)
{
    if (session->nextNonce == 1) {
        // Lets not reset the session, we just sent one or more hello packets and
        // have not received a response, if they respond after we reset then we'll
        // be in a tough state.
        return;
    }

    uint64_t nowSecs = Time_currentTimeSeconds(session->context->eventBase);
    if (nowSecs - session->timeOfLastPacket > session->context->pub.resetAfterInactivitySeconds) {
        cryptoAuthDebug(session, "No traffic in [%d] seconds, resetting connection.",
                  (int) (nowSecs - session->timeOfLastPacket));

        session->timeOfLastPacket = nowSecs;
        reset(session);
    }
}

static void encryptHandshake(struct Message* message,
                             struct CryptoAuth_Session_pvt* session,
                             int setupMessage)
{
    Message_shift(message, CryptoHeader_SIZE, NULL);

    struct CryptoHeader* header = (struct CryptoHeader*) message->bytes;

    // garbage the auth challenge and set the nonce which follows it
    Random_bytes(session->context->rand, (uint8_t*) &header->auth,
                 CryptoHeader_Challenge_SIZE + 24);

    // set the permanent key
    Bits_memcpy(header->publicKey, session->context->pub.publicKey, 32);

    Assert_true(knowHerKey(session));

    // Password auth
    uint8_t* passwordHash = NULL;
    uint8_t passwordHashStore[32];
    if (session->password != NULL) {
        hashPassword(passwordHashStore,
                     &header->auth,
                     session->login,
                     session->password,
                     session->authType);
        passwordHash = passwordHashStore;
    } else {
        header->auth.type = session->authType;
        header->auth.additional = 0;
    }

    // Set the session state
    header->nonce = Endian_hostToBigEndian32(session->nextNonce);

    if (session->nextNonce == 0 || session->nextNonce == 2) {
        // If we're sending a hello or a key
        // Here we make up a temp keypair
        Random_bytes(session->context->rand, session->ourTempPrivKey, 32);
        crypto_scalarmult_curve25519_base(session->ourTempPubKey, session->ourTempPrivKey);

        if (Defined(Log_KEYS)) {
            uint8_t tempPrivateKeyHex[65];
            Hex_encode(tempPrivateKeyHex, 65, session->ourTempPrivKey, 32);
            uint8_t tempPubKeyHex[65];
            Hex_encode(tempPubKeyHex, 65, session->ourTempPubKey, 32);
            Log_keys(session->context->logger, "Generating temporary keypair\n"
                                                "    myTempPrivateKey=%s\n"
                                                "     myTempPublicKey=%s\n",
                      tempPrivateKeyHex, tempPubKeyHex);
        }
    }

    Bits_memcpy(header->encryptedTempKey, session->ourTempPubKey, 32);

    if (Defined(Log_KEYS)) {
        uint8_t tempKeyHex[65];
        Hex_encode(tempKeyHex, 65, header->encryptedTempKey, 32);
        Log_keys(session->context->logger,
                  "Wrapping temp public key:\n"
                  "    %s\n",
                  tempKeyHex);
    }

    cryptoAuthDebug(session, "Sending %s%s packet",
                    ((session->nextNonce & 1) ? "repeat " : ""),
                    ((session->nextNonce < 2) ? "hello" : "key"));

    uint8_t sharedSecret[32];
    if (session->nextNonce < 2) {
        getSharedSecret(sharedSecret,
                        session->context->privateKey,
                        session->pub.herPublicKey,
                        passwordHash,
                        session->context->logger);

        session->isInitiator = true;

        Assert_true(session->nextNonce <= 1);
        session->nextNonce = 1;
    } else {
        // Handshake2
        // herTempPubKey was set by decryptHandshake()
        Assert_ifParanoid(!Bits_isZero(session->herTempPubKey, 32));
        getSharedSecret(sharedSecret,
                        session->context->privateKey,
                        session->herTempPubKey,
                        passwordHash,
                        session->context->logger);

        Assert_true(session->nextNonce <= 3);
        session->nextNonce = 3;

        if (Defined(Log_KEYS)) {
            uint8_t tempKeyHex[65];
            Hex_encode(tempKeyHex, 65, session->herTempPubKey, 32);
            Log_keys(session->context->logger,
                      "Using their temp public key:\n"
                      "    %s\n",
                      tempKeyHex);
        }
    }

    // Shift message over the encryptedTempKey field.
    Message_shift(message, 32 - CryptoHeader_SIZE, NULL);

    encryptRndNonce(header->handshakeNonce, message, sharedSecret);

    if (Defined(Log_KEYS)) {
        uint8_t sharedSecretHex[65];
        printHexKey(sharedSecretHex, sharedSecret);
        uint8_t nonceHex[49];
        Hex_encode(nonceHex, 49, header->handshakeNonce, 24);
        uint8_t cipherHex[65];
        printHexKey(cipherHex, message->bytes);
        Log_keys(session->context->logger,
                  "Encrypting message with:\n"
                  "    nonce: %s\n"
                  "   secret: %s\n"
                  "   cipher: %s\n",
                  nonceHex, sharedSecretHex, cipherHex);
    }

    // Shift it back -- encryptRndNonce adds 16 bytes of authenticator.
    Message_shift(message, CryptoHeader_SIZE - 32 - 16, NULL);
}

/** @return 0 on success, -1 otherwise. */
int CryptoAuth_encrypt(struct CryptoAuth_Session* sessionPub, struct Message* msg)
{
    struct CryptoAuth_Session_pvt* session =
        Identity_check((struct CryptoAuth_Session_pvt*) sessionPub);

    // If there has been no incoming traffic for a while, reset the connection to state 0.
    // This will prevent "connection in bad state" situations from lasting forever.
    // this will reset the session if it has timed out.
    resetIfTimeout(session);

    // If the nonce wraps, start over.
    if (session->nextNonce >= 0xfffffff0) {
        reset(session);
    }

    Assert_true(!((uintptr_t)msg->bytes % 4) || !"alignment fault");

    // nextNonce 0: sending hello, we are initiating connection.
    // nextNonce 1: sending another hello, nothing received yet.
    // nextNonce 2: sending key, hello received.
    // nextNonce 3: sending key again, no data packet recieved yet.
    // nextNonce >3: handshake complete
    //
    // if it's a blind handshake, every message will be empty and nextNonce will remain
    // zero until the first message is received back.
    if (session->nextNonce < 5) {
        if (session->nextNonce < 4) {
            encryptHandshake(msg, session, 0);
            return 0;
        } else {
            cryptoAuthDebug0(session, "Doing final step to send message. nonce=4");
            Assert_ifParanoid(!Bits_isZero(session->ourTempPrivKey, 32));
            Assert_ifParanoid(!Bits_isZero(session->herTempPubKey, 32));
            getSharedSecret(session->sharedSecret,
                            session->ourTempPrivKey,
                            session->herTempPubKey,
                            NULL,
                            session->context->logger);
        }
    }

    Assert_true(msg->length > 0 && "Empty packet during handshake");
    Assert_true(msg->padding >= 36 || !"not enough padding");

    encrypt(session->nextNonce, msg, session->sharedSecret, session->isInitiator);

    Message_push32(msg, session->nextNonce, NULL);
    session->nextNonce++;
    return 0;
}

/** Call the external interface and tell it that a message has been received. */
static inline void updateTime(struct CryptoAuth_Session_pvt* session, struct Message* message)
{
    session->timeOfLastPacket = Time_currentTimeSeconds(session->context->eventBase);
}

static inline Gcc_USE_RET bool decryptMessage(struct CryptoAuth_Session_pvt* session,
                                              uint32_t nonce,
                                              struct Message* content,
                                              uint8_t secret[32])
{
    // Decrypt with authentication and replay prevention.
    if (decrypt(nonce, content, secret, session->isInitiator)) {
        cryptoAuthDebug0(session, "DROP authenticated decryption failed");
        return false;
    }
    if (!ReplayProtector_checkNonce(nonce, &session->pub.replayProtector)) {
        cryptoAuthDebug(session, "DROP nonce checking failed nonce=[%u]", nonce);
        return false;
    }
    return true;
}

static bool ip6MatchesKey(uint8_t ip6[16], uint8_t key[32])
{
    uint8_t calculatedIp6[16];
    AddressCalc_addressForPublicKey(calculatedIp6, key);
    return !Bits_memcmp(ip6, calculatedIp6, 16);
}

static Gcc_USE_RET int decryptHandshake(struct CryptoAuth_Session_pvt* session,
                                        const uint32_t nonce,
                                        struct Message* message,
                                        struct CryptoHeader* header)
{
    if (message->length < CryptoHeader_SIZE) {
        cryptoAuthDebug0(session, "DROP runt");
        return -1;
    }

    // handshake
    // nextNonce 0: recieving hello.
    // nextNonce 1: recieving key, we sent hello.
    // nextNonce 2: recieving first data packet or duplicate hello.
    // nextNonce 3: recieving first data packet.
    // nextNonce >3: handshake complete

    Assert_true(knowHerKey(session));
    if (Bits_memcmp(session->pub.herPublicKey, header->publicKey, 32)) {
        cryptoAuthDebug0(session, "DROP a packet with different public key than this session");
        return -1;
    }

    struct CryptoAuth_User* userObj = getAuth(&header->auth, session->context);
    uint8_t* restrictedToip6 = NULL;
    uint8_t* passwordHash = NULL;
    if (userObj) {
        passwordHash = userObj->secret;
        if (userObj->restrictedToip6[0]) {
            restrictedToip6 = userObj->restrictedToip6;
            if (!ip6MatchesKey(restrictedToip6, session->pub.herPublicKey)) {
                cryptoAuthDebug0(session, "DROP packet with key not matching restrictedToip6");
                return -1;
            }
        }
    }
    if (session->requireAuth && !userObj) {
        cryptoAuthDebug0(session, "DROP message because auth was not given");
        return -1;
    }
    if (!userObj && header->auth.type != 0) {
        cryptoAuthDebug0(session, "DROP message with unrecognized authenticator");
        return -1;
    }
    // What the nextNonce will become if this packet is valid.
    uint32_t nextNonce;

    // The secret for decrypting this message.
    uint8_t sharedSecret[32];

    if (nonce < 2) {
        if (nonce == 0) {
            cryptoAuthDebug(session, "Received a hello packet, using auth: %d",
                            (userObj != NULL));
        } else {
            cryptoAuthDebug0(session, "Received a repeat hello packet");
        }

        getSharedSecret(sharedSecret,
                        session->context->privateKey,
                        session->pub.herPublicKey,
                        passwordHash,
                        session->context->logger);
        nextNonce = 2;
    } else {
        if (nonce == 2) {
            cryptoAuthDebug0(session, "Received a key packet");
        } else {
            Assert_true(nonce == 3);
            cryptoAuthDebug0(session, "Received a repeat key packet");
        }
        if (!session->isInitiator) {
            cryptoAuthDebug0(session, "DROP a stray key packet");
            return -1;
        }
        // We sent the hello, this is a key
        getSharedSecret(sharedSecret,
                        session->ourTempPrivKey,
                        session->pub.herPublicKey,
                        passwordHash,
                        session->context->logger);
        nextNonce = 4;
    }

    // Shift it on top of the authenticator before the encrypted public key
    Message_shift(message, 48 - CryptoHeader_SIZE, NULL);

    if (Defined(Log_KEYS)) {
        uint8_t sharedSecretHex[65];
        printHexKey(sharedSecretHex, sharedSecret);
        uint8_t nonceHex[49];
        Hex_encode(nonceHex, 49, header->handshakeNonce, 24);
        uint8_t cipherHex[65];
        printHexKey(cipherHex, message->bytes);
        Log_keys(session->context->logger,
                  "Decrypting message with:\n"
                  "    nonce: %s\n"
                  "   secret: %s\n"
                  "   cipher: %s\n",
                  nonceHex, sharedSecretHex, cipherHex);
    }

    // Decrypt her temp public key and the message.
    if (decryptRndNonce(header->handshakeNonce, message, sharedSecret)) {
        // just in case
        Bits_memset(header, 0, CryptoHeader_SIZE);
        cryptoAuthDebug(session, "DROP message with nonce [%d], decryption failed", nonce);
        return -1;
    }

    if (Bits_isZero(header->encryptedTempKey, 32)) {
        // we need to reject 0 public keys outright because they will be confused with "unknown"
        cryptoAuthDebug0(session, "DROP message with zero as temp public key");
        return -1;
    }

    if (Defined(Log_KEYS)) {
        uint8_t tempKeyHex[65];
        Hex_encode(tempKeyHex, 65, header->encryptedTempKey, 32);
        Log_keys(session->context->logger,
                  "Unwrapping temp public key:\n"
                  "    %s\n",
                  tempKeyHex);
    }

    Message_shift(message, -32, NULL);

    // Post-decryption checking
    if (nonce == 0) {
        // A new hello packet
        if (!Bits_memcmp(session->herTempPubKey, header->encryptedTempKey, 32)) {
            // possible replay attack or duped packet
            cryptoAuthDebug0(session, "DROP dupe hello packet with same temp key");
            return -1;
        }
    } else if (nonce == 2 && session->nextNonce >= 4) {
        // we accept a new key packet and let it change the session since the other end might have
        // killed off the session while it was in the midst of setting up.
        // This is NOT a repeat key packet because it's nonce is 2, not 3
        if (!Bits_memcmp(session->herTempPubKey, header->encryptedTempKey, 32)) {
            Assert_true(!Bits_isZero(session->herTempPubKey, 32));
            cryptoAuthDebug0(session, "DROP dupe key packet with same temp key");
            return -1;
        }

    } else if (nonce == 3 && session->nextNonce >= 4) {
        // Got a repeat key packet, make sure the temp key is the same as the one we know.
        if (Bits_memcmp(session->herTempPubKey, header->encryptedTempKey, 32)) {
            Assert_true(!Bits_isZero(session->herTempPubKey, 32));
            cryptoAuthDebug0(session, "DROP repeat key packet with different temp key");
            return -1;
        }
    }

    // If Alice sent a hello packet then Bob sent a hello packet and they crossed on the wire,
    // somebody has to yield and the other has to stand firm otherwise they will either deadlock
    // each believing their hello packet is superior or they will livelock, each switching to the
    // other's session and never synchronizing.
    // In this event whoever has the lower permanent public key wins.

    // If we receive a (possibly repeat) key packet
    if (nextNonce == 4) {
        if (session->nextNonce <= 4) {
            // and have not yet begun sending "run" data
            Bits_memcpy(session->herTempPubKey, header->encryptedTempKey, 32);
        } else {
            // It's a (possibly repeat) key packet and we have begun sending run data.
            // We will change the shared secret to the one specified in the new key packet but
            // intentionally avoid de-incrementing the nonce just in case
            getSharedSecret(session->sharedSecret,
                            session->ourTempPrivKey,
                            header->encryptedTempKey,
                            NULL,
                            session->context->logger);
            nextNonce = session->nextNonce + 1;
            cryptoAuthDebug0(session, "New key packet but we are already sending data");
        }

    } else if (nextNonce != 2) {

        Assert_true(!"should never happen");

    } else if (!session->isInitiator || session->established) {
        // This is a hello packet and we are either in ESTABLISHED state or we are
        // not the initiator of the connection.
        // If the case is that we are in ESTABLISHED state, the other side tore down the session
        // and we have not so lets tear it down.
        // If we are not in ESTABLISHED state then we don't allow resetting of the session unless
        // they are the sender of the hello packet or their permanent public key is lower.
        // this is a tie-breaker in case hello packets cross on the wire.
        if (session->established) {
            cryptoAuthDebug0(session, "new hello during established session, resetting");
            reset(session);
        }
        // We got a (possibly repeat) hello packet and we have not sent any hello packet,
        // new session.
        if (session->nextNonce == 3) {
            // We sent a key packet so the next packet is a repeat key but we got another hello
            // We'll just keep steaming along sending repeat key packets
            nextNonce = 3;
        }

        Bits_memcpy(session->herTempPubKey, header->encryptedTempKey, 32);

    } else if (Bits_memcmp(session->pub.herPublicKey, session->context->pub.publicKey, 32) < 0) {
        // It's a hello and we are the initiator but their permant public key is numerically lower
        // than ours, this is so that in the event of two hello packets crossing on the wire, the
        // nodes will agree on who is the initiator.
        cryptoAuthDebug0(session, "Incoming hello from node with lower key, resetting");
        reset(session);

        Bits_memcpy(session->herTempPubKey, header->encryptedTempKey, 32);

    } else {
        cryptoAuthDebug0(session, "DROP Incoming hello from node with higher key, not resetting");
        return -1;
    }

    // Nonces can never go backward and can only "not advance" if they're 0,1,2,3,4 session state.
    Assert_true(session->nextNonce < nextNonce ||
        (session->nextNonce <= 4 && nextNonce == session->nextNonce)
    );
    session->nextNonce = nextNonce;

    Bits_memset(&session->pub.replayProtector, 0, sizeof(struct ReplayProtector));

    return 0;
}

/** @return 0 on success, -1 otherwise. */
int CryptoAuth_decrypt(struct CryptoAuth_Session* sessionPub, struct Message* msg)
{
    struct CryptoAuth_Session_pvt* session =
        Identity_check((struct CryptoAuth_Session_pvt*) sessionPub);
    struct CryptoHeader* header = (struct CryptoHeader*) msg->bytes;

    if (msg->length < 20) {
        cryptoAuthDebug0(session, "DROP runt");
        return -1;
    }
    Assert_true(msg->padding >= 12 || "need at least 12 bytes of padding in incoming message");
    Assert_true(!((uintptr_t)msg->bytes % 4) || !"alignment fault");
    Assert_true(!(msg->capacity % 4) || !"length fault");

    Message_shift(msg, -4, NULL);

    uint32_t nonce = Endian_bigEndianToHost32(header->nonce);

    if (!session->established) {
        if (nonce > 3) {
            if (session->nextNonce < 3) {
                // This is impossible because we have not exchanged hello and key messages.
                cryptoAuthDebug0(session, "DROP Received a run message to an un-setup session");
                return -1;
            }
            cryptoAuthDebug(session, "Trying final handshake step, nonce=%u\n", nonce);
            uint8_t secret[32];
            Assert_ifParanoid(!Bits_isZero(session->ourTempPrivKey, 32));
            Assert_ifParanoid(!Bits_isZero(session->herTempPubKey, 32));
            getSharedSecret(secret,
                            session->ourTempPrivKey,
                            session->herTempPubKey,
                            NULL,
                            session->context->logger);

            if (decryptMessage(session, nonce, msg, secret)) {
                cryptoAuthDebug0(session, "Final handshake step succeeded");
                Bits_memcpy(session->sharedSecret, secret, 32);

                // Now we're in run mode, no more handshake packets will be accepted
                Bits_memset(session->ourTempPrivKey, 0, 32);
                Bits_memset(session->ourTempPubKey, 0, 32);
                Bits_memset(session->herTempPubKey, 0, 32);
                session->established = true;
                session->nextNonce += 3;
                updateTime(session, msg);
                return 0;
            }
            cryptoAuthDebug0(session, "DROP Final handshake step failed");
            return -1;
        }

        Message_shift(msg, 4, NULL);
        return decryptHandshake(session, nonce, msg, header);

    } else if (nonce > 3) {
        Assert_ifParanoid(!Bits_isZero(session->sharedSecret, 32));
        if (decryptMessage(session, nonce, msg, session->sharedSecret)) {
            updateTime(session, msg);
            return 0;
        } else {
            cryptoAuthDebug0(session, "DROP Failed to decrypt message");
            return -1;
        }
    } else if (nonce < 2) {
        cryptoAuthDebug(session, "hello packet during established session nonce=[%d]", nonce);
        Message_shift(msg, 4, NULL);
        return decryptHandshake(session, nonce, msg, header);
    } else {
        // setup keys are already zeroed, not much we can do here.
        cryptoAuthDebug(session, "DROP key packet during established session nonce=[%d]", nonce);
        return -1;
    }
    Assert_true(0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

struct CryptoAuth* CryptoAuth_new(struct Allocator* allocator,
                                  const uint8_t* privateKey,
                                  struct EventBase* eventBase,
                                  struct Log* logger,
                                  struct Random* rand)
{
    struct CryptoAuth_pvt* ca = Allocator_calloc(allocator, sizeof(struct CryptoAuth_pvt), 1);
    Identity_set(ca);
    ca->allocator = allocator;
    ca->eventBase = eventBase;
    ca->logger = logger;
    ca->pub.resetAfterInactivitySeconds = CryptoAuth_DEFAULT_RESET_AFTER_INACTIVITY_SECONDS;
    ca->rand = rand;

    if (privateKey != NULL) {
        Bits_memcpy(ca->privateKey, privateKey, 32);
    } else {
        Random_bytes(rand, ca->privateKey, 32);
    }
    crypto_scalarmult_curve25519_base(ca->pub.publicKey, ca->privateKey);

    if (Defined(Log_KEYS)) {
        uint8_t publicKeyHex[65];
        printHexKey(publicKeyHex, ca->pub.publicKey);
        uint8_t privateKeyHex[65];
        printHexKey(privateKeyHex, ca->privateKey);
        Log_keys(logger,
                  "Initialized CryptoAuth:\n    myPrivateKey=%s\n     myPublicKey=%s\n",
                  privateKeyHex,
                  publicKeyHex);
    }

    return &ca->pub;
}

int CryptoAuth_addUser_ipv6(String* password,
                            String* login,
                            uint8_t ipv6[16],
                            struct CryptoAuth* cryptoAuth)
{
    struct CryptoAuth_pvt* ca = Identity_check((struct CryptoAuth_pvt*) cryptoAuth);

    struct Allocator* alloc = Allocator_child(ca->allocator);
    struct CryptoAuth_User* user = Allocator_calloc(alloc, sizeof(struct CryptoAuth_User), 1);
    user->alloc = alloc;
    Identity_set(user);

    if (!login) {
        int i = 0;
        for (struct CryptoAuth_User* u = ca->users; u; u = u->next) { i++; }
        user->login = login = String_printf(alloc, "Anon #%d", i);
    } else {
        user->login = String_clone(login, alloc);
    }

    struct CryptoHeader_Challenge ac;
    // Users specified with a login field might want to use authType 1 still.
    hashPassword(user->secret, &ac, login, password, 2);
    Bits_memcpy(user->userNameHash, &ac, CryptoHeader_Challenge_KEYSIZE);
    hashPassword(user->secret, &ac, NULL, password, 1);
    Bits_memcpy(user->passwordHash, &ac, CryptoHeader_Challenge_KEYSIZE);

    for (struct CryptoAuth_User* u = ca->users; u; u = u->next) {
        if (Bits_memcmp(user->secret, u->secret, 32)) {
        } else if (!login) {
        } else if (String_equals(login, u->login)) {
            Allocator_free(alloc);
            return CryptoAuth_addUser_DUPLICATE;
        }
    }

    if (ipv6) {
        Bits_memcpy(user->restrictedToip6, ipv6, 16);
    }

    // Add the user to the *end* of the list
    for (struct CryptoAuth_User** up = &ca->users; ; up = &(*up)->next) {
        if (!*up) {
            *up = user;
            break;
        }
    }

    return 0;
}

int CryptoAuth_removeUsers(struct CryptoAuth* context, String* login)
{
    struct CryptoAuth_pvt* ca = Identity_check((struct CryptoAuth_pvt*) context);

    int count = 0;
    struct CryptoAuth_User** up = &ca->users;
    struct CryptoAuth_User* u = *up;
    while ((u = *up)) {
        if (!login || String_equals(login, u->login)) {
            *up = u->next;
            Allocator_free(u->alloc);
            count++;
        } else {
            up = &u->next;
        }
    }

    if (!login) {
        Log_debug(ca->logger, "Flushing [%d] users", count);
    } else {
        Log_debug(ca->logger, "Removing [%d] user(s) identified by [%s]", count, login->bytes);
    }
    return count;
}

List* CryptoAuth_getUsers(struct CryptoAuth* context, struct Allocator* alloc)
{
    struct CryptoAuth_pvt* ca = Identity_check((struct CryptoAuth_pvt*) context);

    List* users = List_new(alloc);
    for (struct CryptoAuth_User* u = ca->users; u; u = u->next) {
        List_addString(users, String_clone(u->login, alloc), alloc);
    }

    return users;
}

struct CryptoAuth_Session* CryptoAuth_newSession(struct CryptoAuth* ca,
                                                 struct Allocator* alloc,
                                                 const uint8_t herPublicKey[32],
                                                 const bool requireAuth,
                                                 char* displayName)
{
    struct CryptoAuth_pvt* context = Identity_check((struct CryptoAuth_pvt*) ca);
    struct CryptoAuth_Session_pvt* session =
        Allocator_calloc(alloc, sizeof(struct CryptoAuth_Session_pvt), 1);
    Identity_set(session);
    session->context = context;
    session->requireAuth = requireAuth;
    session->pub.displayName = displayName ? String_new(displayName, alloc) : NULL;
    session->timeOfLastPacket = Time_currentTimeSeconds(context->eventBase);
    session->alloc = alloc;

    Assert_true(herPublicKey);
    Bits_memcpy(session->pub.herPublicKey, herPublicKey, 32);
    uint8_t calculatedIp6[16];
    AddressCalc_addressForPublicKey(calculatedIp6, herPublicKey);
    Bits_memcpy(session->pub.herIp6, calculatedIp6, 16);

    return &session->pub;
}

void CryptoAuth_setAuth(const String* password,
                        const String* login,
                        struct CryptoAuth_Session* caSession)
{
    struct CryptoAuth_Session_pvt* session =
        Identity_check((struct CryptoAuth_Session_pvt*)caSession);

    if (!password && (session->password || session->authType)) {
        session->password = NULL;
        session->authType = 0;
    } else if (!session->password || !String_equals(session->password, password)) {
        session->password = String_clone(password, session->alloc);
        session->authType = 1;
        if (login) {
            session->authType = 2;
            session->login = String_clone(login, session->alloc);
        }
    } else {
        return;
    }
    reset(session);
}

int CryptoAuth_getState(struct CryptoAuth_Session* caSession)
{
    struct CryptoAuth_Session_pvt* session =
        Identity_check((struct CryptoAuth_Session_pvt*)caSession);

    switch (session->nextNonce) {
        case 0:
            return CryptoAuth_NEW;
        case 1: // Sent a hello, waiting for the key
            return CryptoAuth_HANDSHAKE1;
        case 2: // Received a hello, sent a key packet.
        case 3: // Received a hello, sent multiple key packets.
            return CryptoAuth_HANDSHAKE2;
        case 4:
            // state 4 = waiting for first data packet to prove the handshake succeeded.
            // At this point you have sent a challenge and received a response so it is safe
            // to assume you are not being hit with replay packets.
            //
            // Sent a hello, received one or more keys, waiting for data.
            // In this state data packets will be sent but no data packets have yet been received.
            return CryptoAuth_HANDSHAKE3;
        default:
            // Received data.
            return (session->established) ? CryptoAuth_ESTABLISHED : CryptoAuth_HANDSHAKE3;
    }
}

void CryptoAuth_resetIfTimeout(struct CryptoAuth_Session* caSession)
{
    struct CryptoAuth_Session_pvt* session =
        Identity_check((struct CryptoAuth_Session_pvt*)caSession);
    resetIfTimeout(session);
}

void CryptoAuth_reset(struct CryptoAuth_Session* caSession)
{
    struct CryptoAuth_Session_pvt* session =
        Identity_check((struct CryptoAuth_Session_pvt*)caSession);
    reset(session);
}

// For testing:
void CryptoAuth_encryptRndNonce(uint8_t nonce[24], struct Message* msg, uint8_t secret[32])
{
    encryptRndNonce(nonce, msg, secret);
}

int CryptoAuth_decryptRndNonce(uint8_t nonce[24], struct Message* msg, uint8_t secret[32])
{
    return decryptRndNonce(nonce, msg, secret);
}
