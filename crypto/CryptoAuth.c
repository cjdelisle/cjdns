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

#include <sodium/crypto_box_curve25519xsalsa20poly1305.h>
#include <sodium/crypto_hash_sha256.h>
#include <sodium/crypto_scalarmult_curve25519.h>

#include <stdint.h>
#include <stdbool.h>

enum Nonce {
    Nonce_HELLO = 0,
    Nonce_REPEAT_HELLO = 1,
    Nonce_KEY = 2,
    Nonce_REPEAT_KEY = 3,
    Nonce_FIRST_TRAFFIC_PACKET = 4
};

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
        Assert_true(!crypto_box_curve25519xsalsa20poly1305_beforenm(
            outputSecret, herPublicKey, myPrivateKey));
    } else {
        union {
            struct {
                uint8_t key[32];
                uint8_t passwd[32];
            } components;
            uint8_t bytes[64];
        } buff;

        Assert_true(!crypto_scalarmult_curve25519(buff.components.key, myPrivateKey, herPublicKey));
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
static inline Gcc_USE_RET int decryptRndNonce(const uint8_t nonce[24],
                                              Message_t* msg,
                                              const uint8_t secret[32])
{
    if (Message_getLength(msg) < 16) {
        return -1;
    }
    Assert_true(Message_getPadding(msg) >= 16);
    uint8_t* startAt = Message_bytes(msg) - 16;
    uint8_t paddingSpace[16];
    Bits_memcpy(paddingSpace, startAt, 16);
    Bits_memset(startAt, 0, 16);
    if (!Defined(NSA_APPROVED)) {
        if (crypto_box_curve25519xsalsa20poly1305_open_afternm(
                startAt, startAt, Message_getLength(msg) + 16, nonce, secret) != 0)
        {
            return -1;
        }
    }

    Bits_memcpy(startAt, paddingSpace, 16);
    Er_assert(Message_eshift(msg, -16));
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
static inline void encryptRndNonce(const uint8_t nonce[24],
                                   Message_t* msg,
                                   const uint8_t secret[32])
{
    Assert_true(Message_getPadding(msg) >= 32);
    uint8_t* startAt = Message_bytes(msg) - 32;
    // This function trashes 16 bytes of the padding so we will put it back
    uint8_t paddingSpace[16];
    Bits_memcpy(paddingSpace, startAt, 16);
    Bits_memset(startAt, 0, 32);
    if (!Defined(NSA_APPROVED)) {
        crypto_box_curve25519xsalsa20poly1305_afternm(
            startAt, startAt, Message_getLength(msg) + 32, nonce, secret);
    }

    Bits_memcpy(startAt, paddingSpace, 16);
    Er_assert(Message_eshift(msg, 16));
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
                                      Message_t* msg,
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
                           Message_t* msg,
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
    return !Bits_isZero(session->herPublicKey, 32);
}

static void getIp6(struct CryptoAuth_Session_pvt* session, uint8_t* addr)
{
    Assert_true(knowHerKey(session));
    uint8_t ip6[16];
    AddressCalc_addressForPublicKey(ip6, session->herPublicKey);
    AddrTools_printIp(addr, ip6);
}

#define cryptoAuthDebug(wrapper, format, ...) \
    do {                                                                                         \
        if (!Defined(Log_DEBUG)) { break; }                                                      \
        uint8_t addr[40] = "unknown";                                                            \
        getIp6((session), addr);                                                                 \
        String* dn = (session)->displayName;                                                     \
        Log_debug((session)->context->logger, "%p %s [%s] state[%d]: " format, (void*)(session), \
                  dn ? dn->bytes : "", addr, (session)->nextNonce, __VA_ARGS__);                 \
    } while (0)
// CHECKFILES_IGNORE missing ;

#define cryptoAuthDebug0(wrapper, format) \
    cryptoAuthDebug(session, format "%s", "")

static void reset(struct CryptoAuth_Session_pvt* session)
{
    session->nextNonce = CryptoAuth_State_INIT;
    session->isInitiator = false;

    Bits_memset(session->ourTempPrivKey, 0, 32);
    Bits_memset(session->ourTempPubKey, 0, 32);
    Bits_memset(session->herTempPubKey, 0, 32);
    Bits_memset(session->sharedSecret, 0, 32);
    session->established = false;

    Bits_memset(&session->replayProtector, 0, sizeof(struct ReplayProtector));
}

static void resetIfTimeout(struct CryptoAuth_Session_pvt* session)
{
    if (session->nextNonce == CryptoAuth_State_SENT_HELLO) {
        // Lets not reset the session, we just sent one or more hello packets and
        // have not received a response, if they respond after we reset then we'll
        // be in a tough state.
        return;
    }

    uint64_t nowSecs = Time_currentTimeSeconds();
    if (nowSecs - session->timeOfLastPacket < session->setupResetAfterInactivitySeconds) {
        return;
    } else if (nowSecs - session->timeOfLastPacket < session->resetAfterInactivitySeconds) {
        if (session->established) { return; }
    }

    cryptoAuthDebug(session, "No traffic in [%d] seconds, resetting connection.",
              (int) (nowSecs - session->timeOfLastPacket));

    session->timeOfLastPacket = nowSecs;
    reset(session);
}

static void encryptHandshake(Message_t* message,
                             struct CryptoAuth_Session_pvt* session,
                             int setupMessage)
{
    Er_assert(Message_eshift(message, CryptoHeader_SIZE));

    struct CryptoHeader* header = (struct CryptoHeader*) Message_bytes(message);

    // garbage the auth challenge and set the nonce which follows it
    Random_bytes(session->context->rand, (uint8_t*) &header->auth,
                 CryptoHeader_Challenge_SIZE + 24);

    // set the permanent key
    Bits_memcpy(header->publicKey, session->context->pubKey, 32);

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

    if (session->nextNonce == CryptoAuth_State_INIT ||
        session->nextNonce == CryptoAuth_State_RECEIVED_HELLO)
    {
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

    cryptoAuthDebug(session, "Sending %s%s packet (auth: %d)",
                    ((session->nextNonce & 1) ? "repeat " : ""),
                    ((session->nextNonce < CryptoAuth_State_RECEIVED_HELLO) ? "hello" : "key"),
                    (passwordHash != NULL));

    uint8_t sharedSecret[32];
    if (session->nextNonce < CryptoAuth_State_RECEIVED_HELLO) {
        getSharedSecret(sharedSecret,
                        session->context->privateKey,
                        session->herPublicKey,
                        passwordHash,
                        session->context->logger);

        session->isInitiator = true;

        Assert_true(session->nextNonce <= CryptoAuth_State_SENT_HELLO);
        session->nextNonce = CryptoAuth_State_SENT_HELLO;
    } else {
        // Handshake2
        // herTempPubKey was set by decryptHandshake()
        Assert_ifParanoid(!Bits_isZero(session->herTempPubKey, 32));
        getSharedSecret(sharedSecret,
                        session->context->privateKey,
                        session->herTempPubKey,
                        passwordHash,
                        session->context->logger);

        Assert_true(session->nextNonce <= CryptoAuth_State_SENT_KEY);
        session->nextNonce = CryptoAuth_State_SENT_KEY;

        if (Defined(Log_KEYS)) {
            uint8_t tempKeyHex[65];
            Hex_encode(tempKeyHex, 65, session->herTempPubKey, 32);
            Log_keys(session->context->logger,
                      "Using their temp public key:\n"
                      "    %s\n",
                      tempKeyHex);
        }
    }

    Assert_true((session->nextNonce < CryptoAuth_State_RECEIVED_HELLO) ==
                Bits_isZero(session->herTempPubKey, 32));

    // Shift message over the encryptedTempKey field.
    Er_assert(Message_eshift(message, 32 - CryptoHeader_SIZE));

    encryptRndNonce(header->handshakeNonce, message, sharedSecret);

    if (Defined(Log_KEYS)) {
        uint8_t sharedSecretHex[65];
        printHexKey(sharedSecretHex, sharedSecret);
        uint8_t nonceHex[49];
        Hex_encode(nonceHex, 49, header->handshakeNonce, 24);
        uint8_t cipherHex[65];
        printHexKey(cipherHex, Message_bytes(message));
        Log_keys(session->context->logger,
                  "Encrypting message with:\n"
                  "    nonce: %s\n"
                  "   secret: %s\n"
                  "   cipher: %s\n",
                  nonceHex, sharedSecretHex, cipherHex);
    }

    // Shift it back -- encryptRndNonce adds 16 bytes of authenticator.
    Er_assert(Message_eshift(message, CryptoHeader_SIZE - 32 - 16));
}

/** @return 0 on success, -1 otherwise. */
static int encryptPacket(struct CryptoAuth_Session_pvt* session, Message_t* msg)
{
    // If there has been no incoming traffic for a while, reset the connection to state 0.
    // This will prevent "connection in bad state" situations from lasting forever.
    // this will reset the session if it has timed out.
    resetIfTimeout(session);

    // If the nonce wraps, start over.
    if (session->nextNonce >= 0xfffffff0) {
        reset(session);
    }

    Assert_true(!((uintptr_t)Message_bytes(msg) % 4) || !"alignment fault");

    // nextNonce 0: sending hello, we are initiating connection.
    // nextNonce 1: sending another hello, nothing received yet.
    // nextNonce 2: sending key, hello received.
    // nextNonce 3: sending key again, no data packet recieved yet.
    // nextNonce >3: handshake complete
    //
    // if it's a blind handshake, every message will be empty and nextNonce will remain
    // zero until the first message is received back.
    if (session->nextNonce <= CryptoAuth_State_RECEIVED_KEY) {
        if (session->nextNonce < CryptoAuth_State_RECEIVED_KEY) {
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

    Assert_true(Message_getLength(msg) > 0 && "Empty packet during handshake");
    Assert_true(Message_getPadding(msg) >= 36 || !"not enough padding");

    encrypt(session->nextNonce, msg, session->sharedSecret, session->isInitiator);

    Er_assert(Message_epush32be(msg, session->nextNonce));
    session->nextNonce++;
    return 0;
}

/** @return 0 on success, -1 otherwise. */ // Now only used in unit tests on Rust side
int CryptoAuth_encrypt(struct CryptoAuth_Session* sessionPub, Message_t* msg) {
    struct CryptoAuth_Session_pvt *session =
            Identity_check((struct CryptoAuth_Session_pvt *) sessionPub);
    return encryptPacket(session, msg);
}

/** Call the external interface and tell it that a message has been received. */
static inline void updateTime(struct CryptoAuth_Session_pvt* session, Message_t* message)
{
    session->timeOfLastPacket = Time_currentTimeSeconds();
}

static inline enum CryptoAuth_DecryptErr decryptMessage(struct CryptoAuth_Session_pvt* session,
                                                        uint32_t nonce,
                                                        Message_t* content,
                                                        uint8_t secret[32])
{
    // Decrypt with authentication and replay prevention.
    if (decrypt(nonce, content, secret, session->isInitiator)) {
        cryptoAuthDebug0(session, "DROP authenticated decryption failed");
        return CryptoAuth_DecryptErr_DECRYPT;
    }
    if (!ReplayProtector_checkNonce(nonce, &session->replayProtector)) {
        cryptoAuthDebug(session, "DROP nonce checking failed nonce=[%u]", nonce);
        return CryptoAuth_DecryptErr_REPLAY;
    }
    return 0;
}

static bool ip6MatchesKey(uint8_t ip6[16], uint8_t key[32])
{
    uint8_t calculatedIp6[16];
    AddressCalc_addressForPublicKey(calculatedIp6, key);
    return !Bits_memcmp(ip6, calculatedIp6, 16);
}

static enum CryptoAuth_DecryptErr decryptHandshake(struct CryptoAuth_Session_pvt* session,
                                                   const uint32_t nonce,
                                                   Message_t* message,
                                                   struct CryptoHeader* header)
{
    if (Message_getLength(message) < CryptoHeader_SIZE) {
        cryptoAuthDebug0(session, "DROP runt");
        return CryptoAuth_DecryptErr_RUNT;
    }

    // handshake
    // nextNonce 0: recieving hello.
    // nextNonce 1: recieving key, we sent hello.
    // nextNonce 2: recieving first data packet or duplicate hello.
    // nextNonce 3: recieving first data packet.
    // nextNonce >3: handshake complete

    Assert_true(knowHerKey(session));
    if (Bits_memcmp(session->herPublicKey, header->publicKey, 32)) {
        cryptoAuthDebug0(session, "DROP a packet with different public key than this session");
        return CryptoAuth_DecryptErr_WRONG_PERM_PUBKEY;
    }

    Assert_true((session->nextNonce < CryptoAuth_State_RECEIVED_HELLO) ==
                Bits_isZero(session->herTempPubKey, 32));

    struct CryptoAuth_User* userObj = getAuth(&header->auth, session->context);
    uint8_t* restrictedToip6 = NULL;
    uint8_t* passwordHash = NULL;
    if (userObj) {
        passwordHash = userObj->secret;
        if (userObj->restrictedToip6[0]) {
            restrictedToip6 = userObj->restrictedToip6;
            if (!ip6MatchesKey(restrictedToip6, session->herPublicKey)) {
                cryptoAuthDebug0(session, "DROP packet with key not matching restrictedToip6");
                return CryptoAuth_DecryptErr_IP_RESTRICTED;
            }
        }
    }
    if (session->requireAuth && !userObj) {
        cryptoAuthDebug0(session, "DROP message because auth was not given");
        return CryptoAuth_DecryptErr_AUTH_REQUIRED;
    }
    if (!userObj && header->auth.type != 0) {
        cryptoAuthDebug0(session, "DROP message with unrecognized authenticator");
        return CryptoAuth_DecryptErr_UNRECOGNIZED_AUTH;
    }
    // What the nextNonce will become if this packet is valid.
    uint32_t nextNonce;

    // The secret for decrypting this message.
    uint8_t sharedSecret[32];

    if (nonce < Nonce_KEY) { // HELLO or REPEAT_HELLO
        cryptoAuthDebug(session, "Received a %shello packet, using auth: %d",
            (nonce == Nonce_REPEAT_HELLO) ? "repeat " : "",
            (userObj != NULL));

        getSharedSecret(sharedSecret,
                        session->context->privateKey,
                        session->herPublicKey,
                        passwordHash,
                        session->context->logger);
        nextNonce = CryptoAuth_State_RECEIVED_HELLO;
    } else {
        if (nonce == Nonce_KEY) {
            cryptoAuthDebug0(session, "Received a key packet");
        } else {
            Assert_true(nonce == Nonce_REPEAT_KEY);
            cryptoAuthDebug0(session, "Received a repeat key packet");
        }
        if (!session->isInitiator) {
            cryptoAuthDebug0(session, "DROP a stray key packet");
            return CryptoAuth_DecryptErr_STRAY_KEY;
        }
        // We sent the hello, this is a key
        getSharedSecret(sharedSecret,
                        session->ourTempPrivKey,
                        session->herPublicKey,
                        passwordHash,
                        session->context->logger);
        nextNonce = CryptoAuth_State_RECEIVED_KEY;
    }

    // Shift it on top of the authenticator before the encrypted public key
    Er_assert(Message_eshift(message, 48 - CryptoHeader_SIZE));

    if (Defined(Log_KEYS)) {
        uint8_t sharedSecretHex[65];
        printHexKey(sharedSecretHex, sharedSecret);
        uint8_t nonceHex[49];
        Hex_encode(nonceHex, 49, header->handshakeNonce, 24);
        uint8_t cipherHex[65];
        printHexKey(cipherHex, Message_bytes(message));
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
        return CryptoAuth_DecryptErr_HANDSHAKE_DECRYPT_FAILED;
    }

    if (Bits_isZero(header->encryptedTempKey, 32)) {
        // we need to reject 0 public keys outright because they will be confused with "unknown"
        cryptoAuthDebug0(session, "DROP message with zero as temp public key");
        return CryptoAuth_DecryptErr_WISEGUY;
    }

    if (Defined(Log_KEYS)) {
        uint8_t tempKeyHex[65];
        Hex_encode(tempKeyHex, 65, header->encryptedTempKey, 32);
        Log_keys(session->context->logger,
                  "Unwrapping temp public key:\n"
                  "    %s\n",
                  tempKeyHex);
    }

    Er_assert(Message_eshift(message, -32));

    // Post-decryption checking
    if (nonce == Nonce_HELLO) {
        // A new hello packet
        if (!Bits_memcmp(session->herTempPubKey, header->encryptedTempKey, 32)) {
            // possible replay attack or duped packet
            cryptoAuthDebug0(session, "DROP dupe hello packet with same temp key");
            return CryptoAuth_DecryptErr_INVALID_PACKET;
        }
    } else if (nonce == Nonce_KEY && session->nextNonce >= CryptoAuth_State_RECEIVED_KEY) {
        // we accept a new key packet and let it change the session since the other end might have
        // killed off the session while it was in the midst of setting up.
        // This is NOT a repeat key packet because it's nonce is 2, not 3
        if (!Bits_memcmp(session->herTempPubKey, header->encryptedTempKey, 32)) {
            Assert_true(!Bits_isZero(session->herTempPubKey, 32));
            cryptoAuthDebug0(session, "DROP dupe key packet with same temp key");
            return CryptoAuth_DecryptErr_INVALID_PACKET;
        }

    } else if (nonce == Nonce_REPEAT_KEY && session->nextNonce >= CryptoAuth_State_RECEIVED_KEY) {
        // Got a repeat key packet, make sure the temp key is the same as the one we know.
        if (Bits_memcmp(session->herTempPubKey, header->encryptedTempKey, 32)) {
            Assert_true(!Bits_isZero(session->herTempPubKey, 32));
            cryptoAuthDebug0(session, "DROP repeat key packet with different temp key");
            return CryptoAuth_DecryptErr_INVALID_PACKET;
        }
    }

    // If Alice sent a hello packet then Bob sent a hello packet and they crossed on the wire,
    // somebody has to yield and the other has to stand firm otherwise they will either deadlock
    // each believing their hello packet is superior or they will livelock, each switching to the
    // other's session and never synchronizing.
    // In this event whoever has the lower permanent public key wins.

    // If we receive a (possibly repeat) key packet
    if (nextNonce == CryptoAuth_State_RECEIVED_KEY) {
        Assert_true(nonce == Nonce_KEY || nonce == Nonce_REPEAT_KEY);
        switch (session->nextNonce) {
            case CryptoAuth_State_INIT:
            case CryptoAuth_State_RECEIVED_HELLO:
            case CryptoAuth_State_SENT_KEY: {
                cryptoAuthDebug0(session, "DROP stray key packet");
                return CryptoAuth_DecryptErr_STRAY_KEY;
            }
            case CryptoAuth_State_SENT_HELLO: {
                Bits_memcpy(session->herTempPubKey, header->encryptedTempKey, 32);
                break;
            }
            case CryptoAuth_State_RECEIVED_KEY: {
                if (nonce == Nonce_KEY) {
                    Bits_memcpy(session->herTempPubKey, header->encryptedTempKey, 32);
                } else {
                    Assert_true(!Bits_memcmp(session->herTempPubKey, header->encryptedTempKey, 32));
                }
                break;
            }
            default: {
                Assert_true(!session->established);
                if (nonce == Nonce_KEY) {
                    Bits_memcpy(session->herTempPubKey, header->encryptedTempKey, 32);
                    cryptoAuthDebug0(session, "New key packet, recalculating shared secret");
                    Assert_ifParanoid(!Bits_isZero(session->ourTempPrivKey, 32));
                    Assert_ifParanoid(!Bits_isZero(session->herTempPubKey, 32));
                    getSharedSecret(session->sharedSecret,
                                    session->ourTempPrivKey,
                                    session->herTempPubKey,
                                    NULL,
                                    session->context->logger);
                } else {
                    Assert_true(!Bits_memcmp(session->herTempPubKey, header->encryptedTempKey, 32));
                }
                nextNonce = session->nextNonce + 1;
                cryptoAuthDebug0(session, "New key packet but we are already sending data");
            }
        }

    } else if (nextNonce == CryptoAuth_State_RECEIVED_HELLO) {
        Assert_true(nonce == Nonce_HELLO || nonce == Nonce_REPEAT_HELLO);
        if (Bits_memcmp(session->herTempPubKey, header->encryptedTempKey, 32)) {
            // fresh new hello packet, we should reset the session.
            switch (session->nextNonce) {
                case CryptoAuth_State_SENT_HELLO: {
                    if (Bits_memcmp(session->herPublicKey,
                                    session->context->pubKey, 32) < 0)
                    {
                        // It's a hello and we are the initiator but their permant public key is
                        // numerically lower than ours, this is so that in the event of two hello
                        // packets crossing on the wire, the nodes will agree on who is the
                        // initiator.
                        cryptoAuthDebug0(session,
                            "Incoming hello from node with lower key, resetting");
                        reset(session);

                        Bits_memcpy(session->herTempPubKey, header->encryptedTempKey, 32);
                        break;
                    } else {
                        // We are the initiator and thus we are sending HELLO packets, however they
                        // have sent a hello to us and we already sent a HELLO
                        // We accept the packet (return 0) but we do not alter the state because
                        // we have our own state and we will respond with our (key) packet.
                        cryptoAuthDebug0(session,
                            "Incoming hello from node with higher key, not resetting");
                        return 0;
                    }
                }
                case CryptoAuth_State_INIT: {
                    Bits_memcpy(session->herTempPubKey, header->encryptedTempKey, 32);
                    break;
                }
                default: {
                    cryptoAuthDebug0(session, "Incoming hello packet resetting session");
                    reset(session);
                    Bits_memcpy(session->herTempPubKey, header->encryptedTempKey, 32);
                    break;
                }
            }
        } else {
            // received a hello packet with the same key as the session we already know...
            switch (session->nextNonce) {
                case CryptoAuth_State_RECEIVED_HELLO:
                case CryptoAuth_State_SENT_KEY: {
                    nextNonce = session->nextNonce;
                    break;
                }
                default: {
                    cryptoAuthDebug0(session, "DROP Incoming repeat hello");
                    // We already know the key which is being used for this hello packet and
                    // our state has advanced past RECEIVED_HELLO or SENT_KEY or perhaps we
                    // are the initiator of this session and they're sending us what should
                    // be a key packet but is marked as hello, it's all invalid.
                    return CryptoAuth_DecryptErr_INVALID_PACKET;
                }
            }
        }
    } else {
        Assert_failure("should never happen");
    }

    // Nonces can never go backward and can only "not advance" if they're 0,1,2,3,4 session state.
    Assert_true(session->nextNonce < nextNonce ||
        (session->nextNonce <= CryptoAuth_State_RECEIVED_KEY && nextNonce == session->nextNonce)
    );
    session->nextNonce = nextNonce;

    Bits_memset(&session->replayProtector, 0, sizeof(struct ReplayProtector));

    return 0;
}

/** @return 0 on success, -1 otherwise. */
static enum CryptoAuth_DecryptErr decryptPacket(struct CryptoAuth_Session_pvt* session,
                                                Message_t* msg)
{
    struct CryptoHeader* header = (struct CryptoHeader*) Message_bytes(msg);

    if (Message_getLength(msg) < 20) {
        cryptoAuthDebug0(session, "DROP runt");
        return CryptoAuth_DecryptErr_RUNT;
    }
    Assert_true(Message_getPadding(msg) >= 12 || "need at least 12 bytes of padding in incoming message");
    Assert_true(!((uintptr_t)Message_bytes(msg) % 4) || !"alignment fault");
    Assert_true(!(Message_getCapacity(msg) % 4) || !"length fault");

    Er_assert(Message_eshift(msg, -4));

    uint32_t nonce = Endian_bigEndianToHost32(header->nonce);

    if (!session->established) {
        if (nonce >= Nonce_FIRST_TRAFFIC_PACKET) {
            if (session->nextNonce < CryptoAuth_State_SENT_KEY) {
                // This is impossible because we have not exchanged hello and key messages.
                cryptoAuthDebug0(session, "DROP Received a run message to an un-setup session");
                return CryptoAuth_DecryptErr_NO_SESSION;
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

            enum CryptoAuth_DecryptErr ret = decryptMessage(session, nonce, msg, secret);

            // This prevents a few "ghost" dropped packets at the beginning of a session.
            session->replayProtector.baseOffset = nonce + 1;
            session->replayProtector.bitfield = 0;

            if (!ret) {
                cryptoAuthDebug0(session, "Final handshake step succeeded");
                Bits_memcpy(session->sharedSecret, secret, 32);

                // Now we're in run mode, no more handshake packets will be accepted
                session->established = true;
                session->nextNonce += 3;
                updateTime(session, msg);
                return 0;
            }
            cryptoAuthDebug0(session, "DROP Final handshake step failed");
            return ret;
        }

        Er_assert(Message_eshift(msg, 4));
        return decryptHandshake(session, nonce, msg, header);

    } else if (nonce >= Nonce_FIRST_TRAFFIC_PACKET) {
        Assert_ifParanoid(!Bits_isZero(session->sharedSecret, 32));
        enum CryptoAuth_DecryptErr ret = decryptMessage(session, nonce, msg, session->sharedSecret);
        if (!ret) {
            updateTime(session, msg);
            return 0;
        } else {
            cryptoAuthDebug(session, "DROP Failed to [%s] message",
                ((ret == CryptoAuth_DecryptErr_REPLAY) ? "replay check" : "decrypt"));
            return ret;
        }
    } else if (nonce <= Nonce_REPEAT_HELLO) {
        cryptoAuthDebug(session, "hello packet during established session nonce=[%d]", nonce);
        Er_assert(Message_eshift(msg, 4));
        return decryptHandshake(session, nonce, msg, header);
    } else {
        cryptoAuthDebug(session, "DROP key packet during established session nonce=[%d]", nonce);
        return CryptoAuth_DecryptErr_KEY_PKT_ESTABLISHED_SESSION;
    }
    Assert_failure("unreachable");
}

/** @return 0 on success, -1 otherwise. */ // Now only used in unit tests on Rust side
enum CryptoAuth_DecryptErr CryptoAuth_decrypt(struct CryptoAuth_Session* sessionPub,
                                              Message_t* msg) {
    struct CryptoAuth_Session_pvt *session =
            Identity_check((struct CryptoAuth_Session_pvt *) sessionPub);
    return decryptPacket(session, msg);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

struct CryptoAuth* CryptoAuth_new(struct Allocator* allocator,
                                  const uint8_t* privateKey,
                                  EventBase_t* eventBase,
                                  struct Log* logger,
                                  struct Random* rand)
{
    struct CryptoAuth_pvt* ca = Allocator_calloc(allocator, sizeof(struct CryptoAuth_pvt), 1);
    Identity_set(ca);
    ca->allocator = allocator;
    ca->eventBase = eventBase;
    ca->logger = logger;
    ca->rand = rand;

    if (privateKey != NULL) {
        Bits_memcpy(ca->privateKey, privateKey, 32);
    } else {
        Random_bytes(rand, ca->privateKey, 32);
    }
    crypto_scalarmult_curve25519_base(ca->pubKey, ca->privateKey);

    if (Defined(Log_KEYS)) {
        uint8_t publicKeyHex[65];
        printHexKey(publicKeyHex, ca->pubKey);
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

RTypes_StrList_t* CryptoAuth_getUsers(const struct CryptoAuth* context, struct Allocator* alloc)
{
    struct CryptoAuth_pvt* ca = Identity_check((struct CryptoAuth_pvt*) context);

    int count = 0;
    for (struct CryptoAuth_User* u = ca->users; u; u = u->next) {
        count++;
    }

    RTypes_StrList_t* out = Allocator_calloc(alloc, sizeof(RTypes_StrList_t), 1);
    out->len = count;
    out->items = Allocator_calloc(alloc, sizeof(String*), count);
    int i = 0;
    for (struct CryptoAuth_User* u = ca->users; u; u = u->next) {
        out->items[i] = String_clone(u->login, alloc);
        i++;
    }

    return out;
}

static Iface_DEFUN plaintextMsg(Message_t* msg, struct Iface* iface)
{
    struct CryptoAuth_Session_pvt* sess =
        Identity_containerOf(iface, struct CryptoAuth_Session_pvt, pub.plaintext);
    if (encryptPacket(sess, msg)) {
        return Error(msg, "INTERNAL");
    }
    return Iface_next(&sess->pub.ciphertext, msg);
}

static Iface_DEFUN ciphertextMsg(Message_t* msg, struct Iface* iface)
{
    struct CryptoAuth_Session_pvt* sess =
        Identity_containerOf(iface, struct CryptoAuth_Session_pvt, pub.ciphertext);
    if (Message_getLength(msg) < 32) {
        return Error(msg, "RUNT");
    }
    // Address is pushed on top of the message
    Er_assert(Message_epop(msg, NULL, 16));
    uint8_t firstSixteen[16];
    Bits_memcpy(firstSixteen, Message_bytes(msg), 16);
    enum CryptoAuth_DecryptErr e = decryptPacket(sess, msg);
    if (e == CryptoAuth_DecryptErr_NONE) {
        Er_assert(Message_epush32be(msg, CryptoAuth_DecryptErr_NONE));
        return Iface_next(&sess->pub.plaintext, msg);
    }
    Er_assert(Message_epop(msg, NULL, Message_getLength(msg)));
    Er_assert(Message_epush32be(msg, CryptoAuth_getState(&sess->pub)));
    Er_assert(Message_epush32be(msg, e));
    Er_assert(Message_epush(msg, firstSixteen, 16));
    Er_assert(Message_epush32h(msg, e));
    return Iface_next(&sess->pub.plaintext, msg);
}

struct CryptoAuth_Session* CryptoAuth_newSession(struct CryptoAuth* ca,
                                                 struct Allocator* alloc,
                                                 const uint8_t herPublicKey[32],
                                                 const bool requireAuth,
                                                 const char* displayName,
                                                 bool useNoise)
{
    Assert_true(!useNoise && "Noise protocol not implemented in old CryptoAuth");
    struct CryptoAuth_pvt* context = Identity_check((struct CryptoAuth_pvt*) ca);
    struct CryptoAuth_Session_pvt* session =
        Allocator_calloc(alloc, sizeof(struct CryptoAuth_Session_pvt), 1);
    Identity_set(session);
    session->pub.plaintext.send = plaintextMsg;
    session->pub.ciphertext.send = ciphertextMsg;
    session->context = context;
    session->requireAuth = requireAuth;
    session->displayName = displayName ? String_new(displayName, alloc) : NULL;
    session->timeOfLastPacket = Time_currentTimeSeconds();
    session->alloc = alloc;

    session->resetAfterInactivitySeconds = CryptoAuth_DEFAULT_RESET_AFTER_INACTIVITY_SECONDS;
    session->setupResetAfterInactivitySeconds =
        CryptoAuth_DEFAULT_SETUP_RESET_AFTER_INACTIVITY_SECONDS;

    Assert_true(herPublicKey);
    Bits_memcpy(session->herPublicKey, herPublicKey, 32);
    uint8_t calculatedIp6[16];
    AddressCalc_addressForPublicKey(calculatedIp6, herPublicKey);
    Bits_memcpy(session->herIp6, calculatedIp6, 16);

    return &session->pub;
}

void CryptoAuth_setAuth(const String* password,
                        const String* login,
                        struct CryptoAuth_Session* caSession)
{
    struct CryptoAuth_Session_pvt* session =
        Identity_check((struct CryptoAuth_Session_pvt*)caSession);

    if (!password && (session->password || session->authType)) {
        if (session->passwdAlloc) {
            Allocator_free(session->passwdAlloc);
            session->passwdAlloc = NULL;
        }
        session->password = NULL;
        session->authType = 0;
    } else if (!session->password || !String_equals(session->password, password)) {
        if (session->passwdAlloc) {
            Allocator_free(session->passwdAlloc);
        }
        session->passwdAlloc = Allocator_child(session->alloc);
        session->password = String_clone(password, session->passwdAlloc);
        session->authType = 1;
        if (login) {
            session->authType = 2;
            if (session->loginAlloc) {
                Allocator_free(session->loginAlloc);
            }
            session->loginAlloc = Allocator_child(session->alloc);
            session->login = String_clone(login, session->loginAlloc);
        }
    } else {
        return;
    }
    reset(session);
}

RTypes_CryptoAuth_State_t CryptoAuth_getState(struct CryptoAuth_Session* caSession)
{
    struct CryptoAuth_Session_pvt* session =
        Identity_check((struct CryptoAuth_Session_pvt*)caSession);

    if (session->nextNonce <= CryptoAuth_State_RECEIVED_KEY) {
        return (RTypes_CryptoAuth_State_t) session->nextNonce;
    }
    return (session->established) ? CryptoAuth_State_ESTABLISHED : CryptoAuth_State_RECEIVED_KEY;
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

void CryptoAuth_getHerPubKey(const struct CryptoAuth_Session* session, uint8_t* pkOut)
{
    const struct CryptoAuth_Session_pvt* s =
        Identity_check((struct CryptoAuth_Session_pvt*)session);
    Bits_memcpy(pkOut, s->herPublicKey, 32);
}

void CryptoAuth_getHerIp6(const struct CryptoAuth_Session* session, uint8_t* ipOut)
{
    const struct CryptoAuth_Session_pvt* s =
        Identity_check((struct CryptoAuth_Session_pvt*)session);
    Bits_memcpy(ipOut, s->herIp6, 16);
}

void CryptoAuth_getPubKey(const struct CryptoAuth* ca, uint8_t* pkOut)
{
    const struct CryptoAuth_pvt* context = Identity_check((struct CryptoAuth_pvt*) ca);
    Bits_memcpy(pkOut,  context->pubKey, 32);
}

String_t *CryptoAuth_getName(const struct CryptoAuth_Session* session, Allocator_t* alloc)
{
    const struct CryptoAuth_Session_pvt* s =
        Identity_check((struct CryptoAuth_Session_pvt*)session);
    if (s->displayName) {
        return String_clone(s->displayName, alloc);
    } else {
        return NULL;
    }
}

void CryptoAuth_stats(const struct CryptoAuth_Session* session, RTypes_CryptoStats_t* statsOut)
{
    const struct CryptoAuth_Session_pvt* s =
        Identity_check((struct CryptoAuth_Session_pvt*)session);
    statsOut->received_packets = s->replayProtector.baseOffset +
        Bits_popCountx64(s->replayProtector.bitfield);
    statsOut->lost_packets = s->replayProtector.lostPackets;
    statsOut->received_unexpected = s->replayProtector.receivedOutOfRange;
    statsOut->duplicate_packets = s->replayProtector.duplicates;
    statsOut->noise_proto = false;
}

// For testing:
void CryptoAuth_encryptRndNonce(const uint8_t nonce[24], Message_t* msg, const uint8_t secret[32])
{
    encryptRndNonce(nonce, msg, secret);
}

int CryptoAuth_decryptRndNonce(const uint8_t nonce[24], Message_t* msg, const uint8_t secret[32])
{
    return decryptRndNonce(nonce, msg, secret);
}

