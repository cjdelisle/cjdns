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
#include "interface/Interface.h"
#include "benc/Dict.h"
#include "benc/List.h"
#include "benc/String.h"
#include "util/log/Log.h"
#include "memory/Allocator.h"
#include "util/Assert.h"
#include "util/AddrTools.h"
#include "util/Bits.h"
#include "util/Endian.h"
#include "util/Hex.h"
#include "util/events/Time.h"
#include "wire/Error.h"
#include "wire/Headers.h"
#include "wire/Message.h"

#include "crypto_box_curve25519xsalsa20poly1305.h"
#include "crypto_core_hsalsa20.h"
#include "crypto_hash_sha256.h"
#include "crypto_scalarmult_curve25519.h"
#include "crypto_stream_salsa20.h"
#include "crypto_stream_xsalsa20.h"

#include <stdint.h>
#include <stdbool.h>

#ifdef win32
    #undef interface
#endif

#ifdef Log_KEYS
static inline void printHexKey(uint8_t output[65], uint8_t key[32])
{
    if (key) {
        Hex_encode(output, 65, key, 32);
    } else {
        Bits_memcpyConst(output, "NULL", 5);
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
#endif

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
        Bits_memcpyConst(buff.components.passwd, passwordHash, 32);
        crypto_hash_sha256(outputSecret, buff.bytes, 64);
    }
    #ifdef Log_KEYS
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
    #endif
}

static inline void hashPassword_sha256(struct CryptoAuth_Auth* auth, const String* password)
{
    uint8_t tempBuff[32];
    crypto_hash_sha256(auth->secret, (uint8_t*) password->bytes, password->len);
    crypto_hash_sha256(tempBuff, auth->secret, 32);
    Bits_memcpyConst(auth->challenge.bytes, tempBuff, Headers_AuthChallenge_SIZE);
    Headers_setAuthChallengeDerivations(&auth->challenge, 0);
    auth->challenge.challenge.type = 1;
}

static inline uint8_t* hashPassword(struct CryptoAuth_Auth* auth,
                                    const String* password,
                                    const uint8_t authType)
{
    switch (authType) {
        case 1:
            hashPassword_sha256(auth, password);
            break;
        default:
            Assert_true(!"Unsupported auth type.");
    };
    return auth->secret;
}

/**
 * Search the authorized passwords for one matching this auth header.
 *
 * @param auth the auth header.
 * @param context the CryptoAuth engine to search in.
 * @return an Auth struct with a if one is found, otherwise NULL.
 */
static inline struct CryptoAuth_Auth* getAuth(union Headers_AuthChallenge auth,
                                              struct CryptoAuth_pvt* context)
{
    if (auth.challenge.type != 1) {
        return NULL;
    }
    for (uint32_t i = 0; i < context->passwordCount; i++) {
        if (Bits_memcmp(auth.bytes, &context->passwords[i], Headers_AuthChallenge_KEYSIZE) == 0) {
            return &context->passwords[i];
        }
    }
    Log_debug(context->logger, "Got unrecognized auth, password count = [%d]",
              context->passwordCount);
    return NULL;
}

static inline void getPasswordHash_typeOne(uint8_t output[32],
                                           uint16_t derivations,
                                           struct CryptoAuth_Auth* auth)
{
    Bits_memcpyConst(output, auth->secret, 32);
    if (derivations) {
        union {
            uint8_t bytes[2];
            uint8_t asShort;
        } deriv = { .asShort = derivations };

        output[0] ^= deriv.bytes[0];
        output[1] ^= deriv.bytes[1];

        crypto_hash_sha256(output, output, 32);
    }
}

static inline uint8_t* tryAuth(union Headers_CryptoAuth* cauth,
                               uint8_t hashOutput[32],
                               struct CryptoAuth_Wrapper* wrapper,
                               String** userPtr)
{
    struct CryptoAuth_Auth* auth = getAuth(cauth->handshake.auth, wrapper->context);
    if (auth != NULL) {
        uint16_t deriv = Headers_getAuthChallengeDerivations(&cauth->handshake.auth);
        getPasswordHash_typeOne(hashOutput, deriv, auth);
        if (deriv == 0) {
            *userPtr = auth->user;
        }
        return hashOutput;
    }

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
static inline int decryptRndNonce(uint8_t nonce[24],
                                  struct Message* msg,
                                  uint8_t secret[32])
{
    if (msg->length < 16) {
        return -1;
    }
    Assert_true(msg->padding >= 16);
    uint8_t* startAt = msg->bytes - 16;
    uint8_t paddingSpace[16];
    Bits_memcpyConst(paddingSpace, startAt, 16);
    Bits_memset(startAt, 0, 16);
    if (crypto_box_curve25519xsalsa20poly1305_open_afternm(
            startAt, startAt, msg->length + 16, nonce, secret) != 0)
    {
        return -1;
    }

    Bits_memcpyConst(startAt, paddingSpace, 16);
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
    Bits_memcpyConst(paddingSpace, startAt, 16);
    Bits_memset(startAt, 0, 32);
    crypto_box_curve25519xsalsa20poly1305_afternm(
        startAt, startAt, msg->length + 32, nonce, secret);

    Bits_memcpyConst(startAt, paddingSpace, 16);
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
static inline int decrypt(uint32_t nonce,
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

static inline void setRequiredPadding(struct CryptoAuth_Wrapper* wrapper)
{
    uint32_t padding = (wrapper->nextNonce < 4) ? 36 : sizeof(union Headers_CryptoAuth) + 32;
    wrapper->externalInterface.requiredPadding =
        wrapper->wrappedInterface->requiredPadding + padding;
    wrapper->externalInterface.maxMessageLength =
        wrapper->wrappedInterface->maxMessageLength - padding;
}

static inline bool knowHerKey(struct CryptoAuth_Wrapper* wrapper)
{
    return !Bits_isZero(wrapper->herPerminentPubKey, 32);
}

static void getIp6(struct CryptoAuth_Wrapper* wrapper, uint8_t* addr)
{
    if (knowHerKey(wrapper)) {
        uint8_t ip6[16];
        AddressCalc_addressForPublicKey(ip6, wrapper->herPerminentPubKey);
        AddrTools_printIp(addr, ip6);
    }
}

#define cryptoAuthDebug(wrapper, format, ...) \
    {                                                                                            \
        uint8_t addr[40] = "unknown";                                                            \
        getIp6((wrapper), addr);                                                                 \
        Log_debug((wrapper)->context->logger,                                                    \
                  "%p %s [%s]: " format, (void*)(wrapper), (wrapper)->name, addr, __VA_ARGS__);  \
    }

#define cryptoAuthDebug0(wrapper, format) \
    cryptoAuthDebug(wrapper, format "%s", "")

static void reset(struct CryptoAuth_Wrapper* wrapper)
{
    wrapper->nextNonce = 0;
    wrapper->isInitiator = false;

    Bits_memset(wrapper->ourTempPrivKey, 0, 32);
    Bits_memset(wrapper->ourTempPubKey, 0, 32);
    Bits_memset(wrapper->herTempPubKey, 0, 32);
    Bits_memset(wrapper->sharedSecret, 0, 32);
    wrapper->established = false;

    Bits_memset(&wrapper->replayProtector, 0, sizeof(struct ReplayProtector));
}

/**
 * If we don't know her key, the handshake has to be done backwards.
 * Reverse handshake requests are signaled by sending a non-obfuscated zero nonce.
 */
static uint8_t genReverseHandshake(struct Message* message,
                                   struct CryptoAuth_Wrapper* wrapper,
                                   union Headers_CryptoAuth* header)
{
    reset(wrapper);
    Message_shift(message, -Headers_CryptoAuth_SIZE, NULL);

    // Buffer the packet so it can be sent ASAP
    if (wrapper->bufferedMessage != NULL) {
        // Not exactly a drop but a message is not going to reach the destination.
        cryptoAuthDebug0(wrapper,
            "DROP Expelled a message because a session has not yet been setup");
        Allocator_free(wrapper->bufferedMessage->alloc);
    }

    cryptoAuthDebug0(wrapper, "Buffered a message");
    struct Allocator* bmalloc = Allocator_child(wrapper->externalInterface.allocator);
    wrapper->bufferedMessage = Message_clone(message, bmalloc);
    Assert_ifParanoid(wrapper->nextNonce == 0);

    Message_shift(message, Headers_CryptoAuth_SIZE, NULL);
    header = (union Headers_CryptoAuth*) message->bytes;
    header->nonce = UINT32_MAX;
    message->length = Headers_CryptoAuth_SIZE;

    // sessionState must be 0, auth and 24 byte nonce are garbaged and public key is set
    // now garbage the authenticator and the encrypted key which are not used.
    Random_bytes(wrapper->context->rand, (uint8_t*) &header->handshake.authenticator, 48);

    // This is a special packet which the user should never see.
    Headers_setSetupPacket(&header->handshake.auth, 1);

    return wrapper->wrappedInterface->sendMessage(message, wrapper->wrappedInterface);
}

static uint8_t sendMessage(struct Message* message, struct Interface* interface);

static uint8_t encryptHandshake(struct Message* message,
                                struct CryptoAuth_Wrapper* wrapper,
                                int setupMessage)
{
    Message_shift(message, sizeof(union Headers_CryptoAuth), NULL);

    union Headers_CryptoAuth* header = (union Headers_CryptoAuth*) message->bytes;

    // garbage the auth challenge and set the nonce which follows it
    Random_bytes(wrapper->context->rand, (uint8_t*) &header->handshake.auth,
                 sizeof(union Headers_AuthChallenge) + 24);

    // set the permanent key
    Bits_memcpyConst(&header->handshake.publicKey, wrapper->context->pub.publicKey, 32);

    if (!knowHerKey(wrapper)) {
        return genReverseHandshake(message, wrapper, header);
    } else if (!Bits_isZero(wrapper->herIp6, 16)) {
        // If someone starts a CA session and then discovers the key later and memcpy's it into the
        // result of getHerPublicKey() then we want to make sure they didn't memcpy in an invalid
        // key.
        uint8_t calculatedIp6[16];
        AddressCalc_addressForPublicKey(calculatedIp6, wrapper->herPerminentPubKey);
        Assert_true(!Bits_memcmp(wrapper->herIp6, calculatedIp6, 16));
    }

    if (wrapper->bufferedMessage) {
        // We wanted to send a message but we didn't know the peer's key so we buffered it
        // and sent a connectToMe.
        // Now we just discovered their key and we're sending a hello packet.
        // Lets send 2 hello packets instead and on one will attach our buffered message.

        // This can never happen when the machine is beyond the first hello packet because
        // it should have been sent either by this or in the recipet of a hello packet from
        // the other node.
        Assert_true(wrapper->nextNonce == 0);

        struct Message* bm = wrapper->bufferedMessage;
        wrapper->bufferedMessage = NULL;
        cryptoAuthDebug0(wrapper, "Sending buffered message");
        sendMessage(bm, &wrapper->externalInterface);
        Allocator_free(bm->alloc);
    }

    // Password auth
    uint8_t* passwordHash = NULL;
    struct CryptoAuth_Auth auth;
    if (wrapper->password != NULL) {
        passwordHash = hashPassword(&auth, wrapper->password, wrapper->authType);
        Bits_memcpyConst(header->handshake.auth.bytes,
                         &auth.challenge,
                         sizeof(union Headers_AuthChallenge));
    }
    header->handshake.auth.challenge.type = wrapper->authType;

    // Packet authentication option is deprecated, it must always be enabled.
    Headers_setPacketAuthRequired(&header->handshake.auth, 1);

    // This is a special packet which the user should never see.
    Headers_setSetupPacket(&header->handshake.auth, setupMessage);

    // Set the session state
    uint32_t sessionState_be = Endian_hostToBigEndian32(wrapper->nextNonce);
    header->nonce = sessionState_be;

    if (wrapper->nextNonce == 0 || wrapper->nextNonce == 2) {
        // If we're sending a hello or a key
        // Here we make up a temp keypair
        Random_bytes(wrapper->context->rand, wrapper->ourTempPrivKey, 32);
        crypto_scalarmult_curve25519_base(wrapper->ourTempPubKey, wrapper->ourTempPrivKey);

        #ifdef Log_KEYS
            uint8_t tempPrivateKeyHex[65];
            Hex_encode(tempPrivateKeyHex, 65, wrapper->ourTempPrivKey, 32);
            uint8_t tempPubKeyHex[65];
            Hex_encode(tempPubKeyHex, 65, header->handshake.encryptedTempKey, 32);
            Log_keys(wrapper->context->logger, "Generating temporary keypair\n"
                                                "    myTempPrivateKey=%s\n"
                                                "     myTempPublicKey=%s\n",
                      tempPrivateKeyHex, tempPubKeyHex);
        #endif
    }

    Bits_memcpyConst(header->handshake.encryptedTempKey, wrapper->ourTempPubKey, 32);

    #ifdef Log_KEYS
        uint8_t tempKeyHex[65];
        Hex_encode(tempKeyHex, 65, header->handshake.encryptedTempKey, 32);
        Log_keys(wrapper->context->logger,
                  "Wrapping temp public key:\n"
                  "    %s\n",
                  tempKeyHex);
    #endif

    cryptoAuthDebug(wrapper, "Sending %s%s packet",
                    ((wrapper->nextNonce & 1) ? "repeat " : ""),
                    ((wrapper->nextNonce < 2) ? "hello" : "key"));

    uint8_t sharedSecret[32];
    if (wrapper->nextNonce < 2) {
        getSharedSecret(sharedSecret,
                        wrapper->context->privateKey,
                        wrapper->herPerminentPubKey,
                        passwordHash,
                        wrapper->context->logger);

        wrapper->isInitiator = true;

        Assert_true(wrapper->nextNonce <= 1);
        wrapper->nextNonce = 1;
    } else {
        // Handshake2
        // herTempPubKey was set by receiveMessage()
        Assert_ifParanoid(!Bits_isZero(wrapper->herTempPubKey, 32));
        getSharedSecret(sharedSecret,
                        wrapper->context->privateKey,
                        wrapper->herTempPubKey,
                        passwordHash,
                        wrapper->context->logger);

        Assert_true(wrapper->nextNonce <= 3);
        wrapper->nextNonce = 3;

        #ifdef Log_KEYS
            uint8_t tempKeyHex[65];
            Hex_encode(tempKeyHex, 65, wrapper->herTempPubKey, 32);
            Log_keys(wrapper->context->logger,
                      "Using their temp public key:\n"
                      "    %s\n",
                      tempKeyHex);
        #endif
    }

    // Shift message over the encryptedTempKey field.
    Message_shift(message, 32 - Headers_CryptoAuth_SIZE, NULL);

    encryptRndNonce(header->handshake.nonce, message, sharedSecret);

    #ifdef Log_KEYS
        uint8_t sharedSecretHex[65];
        printHexKey(sharedSecretHex, sharedSecret);
        uint8_t nonceHex[49];
        Hex_encode(nonceHex, 49, header->handshake.nonce, 24);
        uint8_t cipherHex[65];
        printHexKey(cipherHex, message->bytes);
        Log_keys(wrapper->context->logger,
                  "Encrypting message with:\n"
                  "    nonce: %s\n"
                  "   secret: %s\n"
                  "   cipher: %s\n",
                  nonceHex, sharedSecretHex, cipherHex);
    #endif

    // Shift it back -- encryptRndNonce adds 16 bytes of authenticator.
    Message_shift(message, Headers_CryptoAuth_SIZE - 32 - 16, NULL);

    return wrapper->wrappedInterface->sendMessage(message, wrapper->wrappedInterface);
}

static inline uint8_t encryptMessage(struct Message* message,
                                     struct CryptoAuth_Wrapper* wrapper)
{
    Assert_true(message->padding >= 36 || !"not enough padding");

    encrypt(wrapper->nextNonce,
            message,
            wrapper->sharedSecret,
            wrapper->isInitiator);

    Message_shift(message, 4, NULL);

    union Headers_CryptoAuth* header = (union Headers_CryptoAuth*) message->bytes;
    header->nonce = Endian_hostToBigEndian32(wrapper->nextNonce);
    wrapper->nextNonce++;

    return wrapper->wrappedInterface->sendMessage(message, wrapper->wrappedInterface);
}

static uint8_t sendMessage(struct Message* message, struct Interface* interface)
{
    struct CryptoAuth_Wrapper* wrapper =
        Identity_check((struct CryptoAuth_Wrapper*) interface->senderContext);

    // If there has been no incoming traffic for a while, reset the connection to state 0.
    // This will prevent "connection in bad state" situations from lasting forever.
    // this will reset the session if it has timed out.
    CryptoAuth_resetIfTimeout(interface);

    // If the nonce wraps, start over.
    if (wrapper->nextNonce >= 0xfffffff0) {
        reset(wrapper);
    }

    Assert_true(!((uintptr_t)message->bytes % 4) || !"alignment fault");

    // nextNonce 0: sending hello, we are initiating connection.
    // nextNonce 1: sending another hello, nothing received yet.
    // nextNonce 2: sending key, hello received.
    // nextNonce 3: sending key again, no data packet recieved yet.
    // nextNonce >3: handshake complete
    //
    // if it's a blind handshake, every message will be empty and nextNonce will remain
    // zero until the first message is received back.
    if (wrapper->nextNonce < 5) {
        if (wrapper->nextNonce < 4) {
            return encryptHandshake(message, wrapper, 0);
        } else {
            cryptoAuthDebug0(wrapper, "Doing final step to send message. nonce=4");
            Assert_ifParanoid(!Bits_isZero(wrapper->ourTempPrivKey, 32));
            Assert_ifParanoid(!Bits_isZero(wrapper->herTempPubKey, 32));
            getSharedSecret(wrapper->sharedSecret,
                            wrapper->ourTempPrivKey,
                            wrapper->herTempPubKey,
                            NULL,
                            wrapper->context->logger);
        }
    }

    Assert_true(message->length > 0 && "Empty packet during handshake");

    return encryptMessage(message, wrapper);
}

/** Call the external interface and tell it that a message has been received. */
static inline uint8_t callReceivedMessage(struct CryptoAuth_Wrapper* wrapper,
                                          struct Message* message)
{
    wrapper->timeOfLastPacket = Time_currentTimeSeconds(wrapper->context->eventBase);

    uint8_t ret = 0;
    if (wrapper->externalInterface.receiveMessage != NULL) {
        ret = wrapper->externalInterface.receiveMessage(message, &wrapper->externalInterface);
    }

    return ret;
}

static inline bool decryptMessage(struct CryptoAuth_Wrapper* wrapper,
                                  uint32_t nonce,
                                  struct Message* content,
                                  uint8_t secret[32])
{
    // Decrypt with authentication and replay prevention.
    if (decrypt(nonce, content, secret, wrapper->isInitiator)) {
        cryptoAuthDebug0(wrapper, "DROP authenticated decryption failed");
        return false;
    }
    if (!ReplayProtector_checkNonce(nonce, &wrapper->replayProtector)) {
        cryptoAuthDebug(wrapper, "DROP nonce checking failed nonce=[%u]", nonce);
        return false;
    }
    return true;
}

static uint8_t decryptHandshake(struct CryptoAuth_Wrapper* wrapper,
                                const uint32_t nonce,
                                struct Message* message,
                                union Headers_CryptoAuth* header)
{
    if (message->length < Headers_CryptoAuth_SIZE) {
        cryptoAuthDebug0(wrapper, "DROP runt");
        return Error_UNDERSIZE_MESSAGE;
    }

    // handshake
    // nextNonce 0: recieving hello.
    // nextNonce 1: recieving key, we sent hello.
    // nextNonce 2: recieving first data packet or duplicate hello.
    // nextNonce 3: recieving first data packet.
    // nextNonce >3: handshake complete

    if (knowHerKey(wrapper)) {
        if (Bits_memcmp(wrapper->herPerminentPubKey, header->handshake.publicKey, 32)) {
            cryptoAuthDebug0(wrapper, "DROP a packet with different public key than this session");
            return Error_AUTHENTICATION;
        }
    } else if (!Bits_isZero(wrapper->herIp6, 16)) {
        uint8_t calculatedIp6[16];
        AddressCalc_addressForPublicKey(calculatedIp6, header->handshake.publicKey);
        if (Bits_memcmp(wrapper->herIp6, calculatedIp6, 16)) {
            cryptoAuthDebug0(wrapper, "DROP packet with public key not matching ip6 for session");
            return Error_AUTHENTICATION;
        }
    }

    if (wrapper->nextNonce < 2 && nonce == UINT32_MAX && !wrapper->requireAuth) {
        // Reset without knowing key is allowed until state reaches 2.
        // this is because we don't know that the other end knows our key until we
        // have received a valid packet from them.
        // We can't allow the upper layer to see this message because it's not authenticated.
        if (!knowHerKey(wrapper)) {
            Bits_memcpyConst(wrapper->herPerminentPubKey, header->handshake.publicKey, 32);
        }
        Message_shift(message, -Headers_CryptoAuth_SIZE, NULL);
        message->length = 0;
        reset(wrapper);
        wrapper->user = NULL;
        cryptoAuthDebug0(wrapper, "Got a connect-to-me message, sending a hello");
        // Send an empty response (to initiate the connection).
        encryptHandshake(message, wrapper, 1);
        return Error_NONE;
    }

    String* user = NULL;
    uint8_t passwordHashStore[32];
    uint8_t* passwordHash = tryAuth(header, passwordHashStore, wrapper, &user);
    if (wrapper->requireAuth && !user) {
        cryptoAuthDebug0(wrapper, "DROP message because auth was not given");
        return Error_AUTHENTICATION;
    }
    if (passwordHash == NULL && header->handshake.auth.challenge.type != 0) {
        cryptoAuthDebug0(wrapper, "DROP message with unrecognized authenticator");
        return Error_AUTHENTICATION;
    }

    // What the nextNonce will become if this packet is valid.
    uint32_t nextNonce;

    // The secret for decrypting this message.
    uint8_t sharedSecret[32];

    uint8_t* herPermKey = NULL;
    if (nonce < 2) {
        if (nonce == 0) {
            cryptoAuthDebug(wrapper, "Received a hello packet, using auth: %d",
                            (passwordHash != NULL));
        } else {
            cryptoAuthDebug0(wrapper, "Received a repeat hello packet");
        }

        // Decrypt message with perminent keys.
        if (!knowHerKey(wrapper) || wrapper->nextNonce == 0) {
            herPermKey = header->handshake.publicKey;
            #ifdef Log_DEBUG
                if (Bits_isZero(header->handshake.publicKey, 32)) {
                    cryptoAuthDebug0(wrapper, "Node sent public key of ZERO!");
                }
            #endif
        } else {
            herPermKey = wrapper->herPerminentPubKey;
            if (Bits_memcmp(header->handshake.publicKey, herPermKey, 32)) {
                cryptoAuthDebug0(wrapper, "DROP packet contains different perminent key");
                return Error_AUTHENTICATION;
            }
        }

        getSharedSecret(sharedSecret,
                        wrapper->context->privateKey,
                        herPermKey,
                        passwordHash,
                        wrapper->context->logger);
        nextNonce = 2;
    } else {
        if (nonce == 2) {
            cryptoAuthDebug0(wrapper, "Received a key packet");
        } else if (nonce == 3) {
            cryptoAuthDebug0(wrapper, "Received a repeat key packet");
        } else {
            cryptoAuthDebug(wrapper, "Received a packet of unknown type! nonce=%u", nonce);
        }
        if (Bits_memcmp(header->handshake.publicKey, wrapper->herPerminentPubKey, 32)) {
            cryptoAuthDebug0(wrapper, "DROP packet contains different perminent key");
            return Error_AUTHENTICATION;
        }
        if (!wrapper->isInitiator) {
            cryptoAuthDebug0(wrapper, "DROP a stray key packet");
            return Error_AUTHENTICATION;
        }
        // We sent the hello, this is a key
        getSharedSecret(sharedSecret,
                        wrapper->ourTempPrivKey,
                        wrapper->herPerminentPubKey,
                        passwordHash,
                        wrapper->context->logger);
        nextNonce = 4;
    }

    // Shift it on top of the authenticator before the encrypted public key
    Message_shift(message, 48 - Headers_CryptoAuth_SIZE, NULL);

    #ifdef Log_KEYS
        uint8_t sharedSecretHex[65];
        printHexKey(sharedSecretHex, sharedSecret);
        uint8_t nonceHex[49];
        Hex_encode(nonceHex, 49, header->handshake.nonce, 24);
        uint8_t cipherHex[65];
        printHexKey(cipherHex, message->bytes);
        Log_keys(wrapper->context->logger,
                  "Decrypting message with:\n"
                  "    nonce: %s\n"
                  "   secret: %s\n"
                  "   cipher: %s\n",
                  nonceHex, sharedSecretHex, cipherHex);
    #endif

    // Decrypt her temp public key and the message.
    if (decryptRndNonce(header->handshake.nonce, message, sharedSecret) != 0) {
        // just in case
        Bits_memset(header, 0, Headers_CryptoAuth_SIZE);
        cryptoAuthDebug(wrapper, "DROP message with nonce [%d], decryption failed", nonce);
        return Error_AUTHENTICATION;
    }

    Assert_ifParanoid(!Bits_isZero(header->handshake.encryptedTempKey, 32));

    #ifdef Log_KEYS
        uint8_t tempKeyHex[65];
        Hex_encode(tempKeyHex, 65, header->handshake.encryptedTempKey, 32);
        Log_keys(wrapper->context->logger,
                  "Unwrapping temp public key:\n"
                  "    %s\n",
                  tempKeyHex);
    #endif

    Message_shift(message, -32, NULL);

    // Post-decryption checking
    if (nonce == 0) {
        // A new hello packet
        if (!Bits_memcmp(wrapper->herTempPubKey, header->handshake.encryptedTempKey, 32)) {
            // possible replay attack or duped packet
            cryptoAuthDebug0(wrapper, "DROP dupe hello packet with same temp key");
            return Error_AUTHENTICATION;
        }
    } else if (nonce == 2 && wrapper->nextNonce >= 4) {
        // we accept a new key packet and let it change the session since the other end might have
        // killed off the session while it was in the midst of setting up.
        if (!Bits_memcmp(wrapper->herTempPubKey, header->handshake.encryptedTempKey, 32)) {
            Assert_true(!Bits_isZero(wrapper->herTempPubKey, 32));
            cryptoAuthDebug0(wrapper, "DROP dupe key packet with same temp key");
            return Error_AUTHENTICATION;
        }

    } else if (nonce == 3 && wrapper->nextNonce >= 4) {
        // Got a repeat key packet, make sure the temp key is the same as the one we know.
        if (Bits_memcmp(wrapper->herTempPubKey, header->handshake.encryptedTempKey, 32)) {
            Assert_true(!Bits_isZero(wrapper->herTempPubKey, 32));
            cryptoAuthDebug0(wrapper, "DROP repeat key packet with different temp key");
            return Error_AUTHENTICATION;
        }
    }

    // If Alice sent a hello packet then Bob sent a hello packet and they crossed on the wire,
    // somebody has to yield and the other has to stand firm otherwise they will either deadlock
    // each believing their hello packet is superior or they will livelock, each switching to the
    // other's session and never synchronizing.
    // In this event whoever has the lower permanent public key wins.

    // If we receive a (possibly repeat) key packet
    if (nextNonce == 4) {
        if (wrapper->nextNonce <= 4) {
            // and have not yet begun sending "run" data
            Assert_true(wrapper->nextNonce <= nextNonce);
            wrapper->nextNonce = nextNonce;

            wrapper->user = user;
            Bits_memcpyConst(wrapper->herTempPubKey, header->handshake.encryptedTempKey, 32);
        } else {
            // It's a (possibly repeat) key packet and we have begun sending run data.
            // We will change the shared secret to the one specified in the new key packet but
            // intentionally avoid de-incrementing the nonce just in case
            getSharedSecret(wrapper->sharedSecret,
                            wrapper->ourTempPrivKey,
                            header->handshake.encryptedTempKey,
                            NULL,
                            wrapper->context->logger);
            cryptoAuthDebug0(wrapper, "New key packet but we are already sending data");
        }

    } else if (nextNonce == 2 && (!wrapper->isInitiator || wrapper->established)) {
        // This is a hello packet and we are either in ESTABLISHED state or we are
        // not the initiator of the connection.
        // If the case is that we are in ESTABLISHED state, the other side tore down the session
        // and we have not so lets tear it down.
        // If we are not in ESTABLISHED state then we don't allow resetting of the session unless
        // they are the sender of the hello packet or their permanent public key is lower.
        // this is a tie-breaker in case hello packets cross on the wire.
        if (wrapper->established) {
            reset(wrapper);
        }
        // We got a (possibly repeat) hello packet and we have not sent any hello packet,
        // new session.
        if (wrapper->nextNonce == 3 && nextNonce == 2) {
            // We sent a key packet so the next packet is a repeat key but we got another hello
            // We'll just keep steaming along sending repeat key packets
            nextNonce = 3;
        }

        Assert_true(wrapper->nextNonce <= nextNonce);
        wrapper->nextNonce = nextNonce;
        wrapper->user = user;
        Bits_memcpyConst(wrapper->herTempPubKey, header->handshake.encryptedTempKey, 32);

    } else if (nextNonce == 2
        && Bits_memcmp(header->handshake.publicKey, wrapper->context->pub.publicKey, 32) < 0)
    {
        // It's a hello and we are the initiator but their permant public key is numerically lower
        // than ours, this is so that in the event of two hello packets crossing on the wire, the
        // nodes will agree on who is the initiator.
        cryptoAuthDebug0(wrapper, "Incoming hello from node with lower key, resetting");
        reset(wrapper);

        Assert_true(wrapper->nextNonce <= nextNonce);
        wrapper->nextNonce = nextNonce;
        wrapper->user = user;
        Bits_memcpyConst(wrapper->herTempPubKey, header->handshake.encryptedTempKey, 32);

    } else {
        cryptoAuthDebug0(wrapper, "Incoming hello from node with higher key, not resetting");
    }

    if (herPermKey && herPermKey != wrapper->herPerminentPubKey) {
        Bits_memcpyConst(wrapper->herPerminentPubKey, herPermKey, 32);
    }

    // If this is a handshake which was initiated in reverse because we
    // didn't know the other node's key, now send what we were going to send.

    if (wrapper->bufferedMessage) {
        // This can only happen when we have received a (maybe repeat) hello packet.
        Assert_true(wrapper->nextNonce == 2);

        struct Message* bm = wrapper->bufferedMessage;
        wrapper->bufferedMessage = NULL;
        cryptoAuthDebug0(wrapper, "Sending buffered message");
        sendMessage(bm, &wrapper->externalInterface);
        Allocator_free(bm->alloc);
    }

    if (message->length == 0 && Headers_isSetupPacket(&header->handshake.auth)) {
        return Error_NONE;
    }

    Bits_memset(&wrapper->replayProtector, 0, sizeof(struct ReplayProtector));

    setRequiredPadding(wrapper);
    return callReceivedMessage(wrapper, message);
}

static uint8_t receiveMessage(struct Message* received, struct Interface* interface)
{
    struct CryptoAuth_Wrapper* wrapper =
        Identity_check((struct CryptoAuth_Wrapper*) interface->receiverContext);

    union Headers_CryptoAuth* header = (union Headers_CryptoAuth*) received->bytes;

    if (received->length < 20) {
        cryptoAuthDebug0(wrapper, "DROP runt");
        return Error_UNDERSIZE_MESSAGE;
    }
    Assert_true(received->padding >= 12 || "need at least 12 bytes of padding in incoming message");
    Assert_true(!((uintptr_t)received->bytes % 4) || !"alignment fault");

    Message_shift(received, -4, NULL);

    uint32_t nonce = Endian_bigEndianToHost32(header->nonce);

    if (!wrapper->established) {
        if (nonce > 3 && nonce != UINT32_MAX) {
            if (wrapper->nextNonce < 3) {
                // This is impossible because we have not exchanged hello and key messages.
                cryptoAuthDebug0(wrapper, "DROP Received a run message to an un-setup session");
                return Error_UNDELIVERABLE;
            }
            cryptoAuthDebug(wrapper, "Trying final handshake step, nonce=%u\n", nonce);
            uint8_t secret[32];
            Assert_ifParanoid(!Bits_isZero(wrapper->ourTempPrivKey, 32));
            Assert_ifParanoid(!Bits_isZero(wrapper->herTempPubKey, 32));
            getSharedSecret(secret,
                            wrapper->ourTempPrivKey,
                            wrapper->herTempPubKey,
                            NULL,
                            wrapper->context->logger);

            // We'll optimistically advance the nextNonce value because decryptMessage()
            // passes the message on to the upper level and if this message causes a
            // response, we want the CA to be in ESTABLISHED state.
            // if the decryptMessage() call fails, we CryptoAuth_reset() it back.
            wrapper->nextNonce += 3;

            if (decryptMessage(wrapper, nonce, received, secret)) {
                cryptoAuthDebug0(wrapper, "Final handshake step succeeded");
                Bits_memcpyConst(wrapper->sharedSecret, secret, 32);

                // Now we're in run mode, no more handshake packets will be accepted
                Bits_memset(wrapper->ourTempPrivKey, 0, 32);
                Bits_memset(wrapper->ourTempPubKey, 0, 32);
                Bits_memset(wrapper->herTempPubKey, 0, 32);
                wrapper->established = true;

                return callReceivedMessage(wrapper, received);
            }
            CryptoAuth_reset(&wrapper->externalInterface);
            cryptoAuthDebug0(wrapper, "DROP Final handshake step failed");
            return Error_UNDELIVERABLE;
        }

        Message_shift(received, 4, NULL);
        return decryptHandshake(wrapper, nonce, received, header);

    } else if (nonce > 3 && nonce != UINT32_MAX) {
        Assert_ifParanoid(!Bits_isZero(wrapper->sharedSecret, 32));
        if (decryptMessage(wrapper, nonce, received, wrapper->sharedSecret)) {
            return callReceivedMessage(wrapper, received);
        } else {
            cryptoAuthDebug0(wrapper, "DROP Failed to decrypt message");
            return Error_UNDELIVERABLE;
        }
    } else if (nonce < 2) {
        cryptoAuthDebug(wrapper, "hello packet during established session nonce=[%d]", nonce);
        Message_shift(received, 4, NULL);
        return decryptHandshake(wrapper, nonce, received, header);
    } else {
        // setup keys are already zeroed, not much we can do here.
        cryptoAuthDebug(wrapper, "DROP key packet during established session nonce=[%d]", nonce);
        return Error_UNDELIVERABLE;
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
    ca->allocator = allocator;

    ca->passwords = Allocator_calloc(allocator, sizeof(struct CryptoAuth_Auth), 256);
    ca->passwordCount = 0;
    ca->passwordCapacity = 256;
    ca->eventBase = eventBase;
    ca->logger = logger;
    ca->pub.resetAfterInactivitySeconds = CryptoAuth_DEFAULT_RESET_AFTER_INACTIVITY_SECONDS;
    ca->rand = rand;
    Identity_set(ca);

    if (privateKey != NULL) {
        Bits_memcpyConst(ca->privateKey, privateKey, 32);
    } else {
        Random_bytes(rand, ca->privateKey, 32);
    }
    crypto_scalarmult_curve25519_base(ca->pub.publicKey, ca->privateKey);

    #ifdef Log_KEYS
        uint8_t publicKeyHex[65];
        printHexKey(publicKeyHex, ca->pub.publicKey);
        uint8_t privateKeyHex[65];
        printHexKey(privateKeyHex, ca->privateKey);
        Log_keys(logger,
                  "Initialized CryptoAuth:\n    myPrivateKey=%s\n     myPublicKey=%s\n",
                  privateKeyHex,
                  publicKeyHex);
    #endif

    return &ca->pub;
}

int32_t CryptoAuth_addUser(String* password,
                           uint8_t authType,
                           String* user,
                           struct CryptoAuth* ca)
{
    struct CryptoAuth_pvt* context = Identity_check((struct CryptoAuth_pvt*) ca);
    if (authType != 1) {
        return CryptoAuth_addUser_INVALID_AUTHTYPE;
    }
    if (context->passwordCount == context->passwordCapacity) {
        // TODO(cjd): realloc password space and increase buffer.
        return CryptoAuth_addUser_OUT_OF_SPACE;
    }
    struct CryptoAuth_Auth a;
    hashPassword_sha256(&a, password);
    for (uint32_t i = 0; i < context->passwordCount; i++) {
        if (!Bits_memcmp(a.secret, context->passwords[i].secret, 32) ||
            String_equals(user, context->passwords[i].user)) {
            return CryptoAuth_addUser_DUPLICATE;
        }
    }
    a.user = String_new(user->bytes, context->allocator);
    Bits_memcpyConst(&context->passwords[context->passwordCount],
                     &a,
                     sizeof(struct CryptoAuth_Auth));
    context->passwordCount++;
    return 0;
}

int CryptoAuth_removeUsers(struct CryptoAuth* context, String* user)
{
    struct CryptoAuth_pvt* ctx = Identity_check((struct CryptoAuth_pvt*) context);
    if (!user) {
        int count = ctx->passwordCount;
        Log_debug(ctx->logger, "Flushing [%d] users", count);
        ctx->passwordCount = 0;
        return count;
    }
    int count = 0;
    int i = 0;
    while (i < (int)ctx->passwordCount) {
        if (String_equals(ctx->passwords[i].user, user)) {
            Bits_memcpyConst(&ctx->passwords[i],
                             &ctx->passwords[ctx->passwordCount--],
                             sizeof(struct CryptoAuth_Auth));
            count++;
        } else {
            i++;
        }
    }
    Log_debug(ctx->logger, "Removing [%d] user(s) identified by [%s]", count, user->bytes);
    return count;
}

List* CryptoAuth_getUsers(struct CryptoAuth* context, struct Allocator* alloc)
{
    struct CryptoAuth_pvt* ctx = Identity_check((struct CryptoAuth_pvt*) context);
    uint32_t count = ctx->passwordCount;

    List* users = NULL;

    for (uint32_t i = 0; i < count; i++ )
    {
        users = List_addString(users, String_clone(ctx->passwords[i].user, alloc), alloc);
    }

    return users;
}

String* CryptoAuth_getUser(struct Interface* interface)
{
    struct CryptoAuth_Wrapper* wrapper =
        Identity_check((struct CryptoAuth_Wrapper*)interface->senderContext);

    String* user = wrapper->user;
    if (user) {
        // If the user was lost in flushusers, then we need to return null.
        for (uint32_t i = 0; i < wrapper->context->passwordCount; i++) {
            if (user == wrapper->context->passwords[i].user) {
                return user;
            }
        }
        // Null it since it's been removed.
        wrapper->user = NULL;
    }
    return NULL;
}

struct Interface* CryptoAuth_wrapInterface(struct Interface* toWrap,
                                           const uint8_t herPublicKey[32],
                                           const uint8_t herIp6[16],
                                           const bool requireAuth,
                                           char* name,
                                           struct CryptoAuth* ca)
{
    struct CryptoAuth_pvt* context = Identity_check((struct CryptoAuth_pvt*) ca);
    struct CryptoAuth_Wrapper* wrapper = Allocator_clone(toWrap->allocator,
        (&(struct CryptoAuth_Wrapper) {
            .user = NULL,
            .nextNonce = 0,
            .context = context,
            .wrappedInterface = toWrap,
            .requireAuth = requireAuth,
            .name = name
        }));

    wrapper->timeOfLastPacket = Time_currentTimeSeconds(context->eventBase);
    Identity_set(wrapper);
    toWrap->receiverContext = wrapper;
    toWrap->receiveMessage = receiveMessage;

    struct Interface iface = {
        .senderContext = wrapper,
        .sendMessage = sendMessage,
        .allocator = toWrap->allocator
    };
    Bits_memcpyConst(&wrapper->externalInterface, &iface, sizeof(struct Interface));

    if (herPublicKey != NULL) {
        Bits_memcpyConst(wrapper->herPerminentPubKey, herPublicKey, 32);
        uint8_t calculatedIp6[16];
        AddressCalc_addressForPublicKey(calculatedIp6, herPublicKey);
        Bits_memcpyConst(wrapper->herIp6, calculatedIp6, 16);
        if (herIp6 != NULL) {
            Assert_true(!Bits_memcmp(calculatedIp6, herIp6, 16));
        }
    } else if (herIp6) {
        Bits_memcpyConst(wrapper->herIp6, herIp6, 16);
    }

    return &wrapper->externalInterface;
}

void CryptoAuth_setAuth(const String* password,
                        const uint8_t authType,
                        struct Interface* wrappedInterface)
{
    struct CryptoAuth_Wrapper* wrapper =
        Identity_check((struct CryptoAuth_Wrapper*)wrappedInterface->senderContext);
    wrapper->password = (password != NULL)
        ? String_newBinary(password->bytes, password->len, wrappedInterface->allocator)
        : NULL;
    wrapper->authType = (password != NULL) ? authType : 0;
}

uint8_t* CryptoAuth_getHerPublicKey(struct Interface* interface)
{
    return ((struct CryptoAuth_Wrapper*) interface->senderContext)->herPerminentPubKey;
}

void CryptoAuth_reset(struct Interface* interface)
{
    struct CryptoAuth_Wrapper* wrapper =
        Identity_check((struct CryptoAuth_Wrapper*)interface->senderContext);
    reset(wrapper);
}

int CryptoAuth_getState(struct Interface* interface)
{
    struct CryptoAuth_Wrapper* wrapper =
        Identity_check((struct CryptoAuth_Wrapper*)interface->senderContext);

    switch (wrapper->nextNonce) {
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
            return (wrapper->established) ? CryptoAuth_ESTABLISHED : CryptoAuth_HANDSHAKE3;
    }
}

void CryptoAuth_resetIfTimeout(struct Interface* iface)
{
    struct CryptoAuth_Wrapper* wrapper =
        Identity_check((struct CryptoAuth_Wrapper*)iface->senderContext);

    if (wrapper->nextNonce == 1) {
        // Lets not reset the session, we just sent one or more hello packets and
        // have not received a response, if they respond after we reset then we'll
        // be in a tough state.
        return;
    }

    uint64_t nowSecs = Time_currentTimeSeconds(wrapper->context->eventBase);
    if (nowSecs - wrapper->timeOfLastPacket > wrapper->context->pub.resetAfterInactivitySeconds) {
        cryptoAuthDebug(wrapper, "No traffic in [%d] seconds, resetting connection.",
                  (int) (nowSecs - wrapper->timeOfLastPacket));

        wrapper->timeOfLastPacket = nowSecs;
        reset(wrapper);
    }
}

struct Interface* CryptoAuth_getConnectedInterface(struct Interface* iface)
{
    if (iface->sendMessage == sendMessage) {
        // internal (plaintext side)
        struct CryptoAuth_Wrapper* wrapper =
            Identity_check((struct CryptoAuth_Wrapper*)iface->senderContext);
        return wrapper->wrappedInterface;
    } else if (iface->receiveMessage == receiveMessage) {
        struct CryptoAuth_Wrapper* wrapper =
            Identity_check((struct CryptoAuth_Wrapper*)iface->receiverContext);
        return &wrapper->externalInterface;
    }
    return NULL;
}

struct ReplayProtector* CryptoAuth_getReplayProtector(struct Interface* iface)
{
    struct CryptoAuth_Wrapper* wrapper =
        Identity_check((struct CryptoAuth_Wrapper*)iface->senderContext);
    return &wrapper->replayProtector;
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

uint8_t CryptoAuth_encryptHandshake(struct Message* message,
                                    struct CryptoAuth_Wrapper* wrapper,
                                    int setupMessage)
{
    return encryptHandshake(message, wrapper, setupMessage);
}

uint8_t CryptoAuth_receiveMessage(struct Message* received, struct Interface* interface)
{
    return receiveMessage(received, interface);
}
