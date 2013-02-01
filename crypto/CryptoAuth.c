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
#include "crypto/ReplayProtector.h"
#include "crypto/random/Random.h"
#include "interface/Interface.h"
#include "benc/Object.h"
#include "util/log/Log.h"
#include "memory/Allocator.h"
#include "util/Assert.h"
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

#ifdef WIN32
    #undef interface
#endif

/** The constant used in nacl. */
static const uint8_t keyHashSigma[16] = "expand 32-byte k";
static const uint8_t keyHashNonce[16] = {0};

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
            // Unsupported auth type.
            abort();
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
                               void** userPtr)
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
    wrapper->authenticatePackets |= Headers_isPacketAuthRequired(&cauth->handshake.auth);

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
    Message_shift(msg, -16);
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
    Message_shift(msg, 16);
}

/**
 * Encipher the content without any authentication.
 * Encryption is the same function as decryption.
 *
 * @param nonce a number which is used only once.
 * @param msg a message to encipher.
 * @param secret a shared secret.
 */
static inline int cipher(uint8_t nonce[8],
                          struct Message* msg,
                          uint8_t secret[20])
{
    return crypto_stream_salsa20_xor(msg->bytes, msg->bytes, msg->length, nonce, secret);
}

/**
 * Decrypt a packet.
 *
 * @param nonce a counter.
 * @param msg the message to decrypt, decrypted in place.
 * @param secret the shared secret.
 * @param isInitiator true if we started the connection.
 * @param authenticate if true then the packet will be authenticated as well at a 16 byte cost.
 *                     The packet must have been encrypted using authenticate.
 */
static inline int decrypt(uint32_t nonce,
                          struct Message* msg,
                          uint8_t secret[32],
                          bool isInitiator,
                          bool authenticate)
{
    union {
        uint32_t ints[2];
        uint8_t bytes[24];
    } nonceAs = { .ints = {0, 0} };
    nonceAs.ints[!isInitiator] = Endian_hostToLittleEndian32(nonce);

    if (authenticate) {
        return decryptRndNonce(nonceAs.bytes, msg, secret);
    } else {
        return cipher(nonceAs.bytes, msg, secret);
    }
}

/**
 * Encrypt a packet.
 *
 * @param nonce a counter.
 * @param msg the message to decrypt, decrypted in place.
 * @param secret the shared secret.
 * @param isInitiator true if we started the connection.
 * @param authenticate if true then the packet will be authenticated as well at a 16 byte cost.
 *                     The packet must have been encrypted using authenticate.
 */
static inline void encrypt(uint32_t nonce,
                           struct Message* msg,
                           uint8_t secret[32],
                           bool isInitiator,
                           bool authenticate)
{
    union {
        uint32_t ints[2];
        uint8_t bytes[24];
    } nonceAs = { .ints = {0, 0} };
    nonceAs.ints[isInitiator] = Endian_hostToLittleEndian32(nonce);

    if (authenticate) {
        encryptRndNonce(nonceAs.bytes, msg, secret);
    } else {
        cipher(nonceAs.bytes, msg, secret);
    }
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

/**
 * If we don't know her key, the handshake has to be done backwards.
 * Reverse handshake requests are signaled by sending a non-obfuscated zero nonce.
 */
static uint8_t genReverseHandshake(struct Message* message,
                                   struct CryptoAuth_Wrapper* wrapper,
                                   union Headers_CryptoAuth* header)
{
    wrapper->nextNonce = 0;
    Message_shift(message, -Headers_CryptoAuth_SIZE);

    // Buffer the packet so it can be sent ASAP
    if (wrapper->bufferedMessage == NULL) {
        Log_debug(wrapper->context->logger, "Buffered a message.\n");
        wrapper->bufferedMessage =
            Message_clone(message, wrapper->externalInterface.allocator);
        Assert_true(wrapper->nextNonce == 0);
    } else {
        Log_debug(wrapper->context->logger,
                  "Expelled a message because a session has not yet been setup.\n");
        Message_copyOver(wrapper->bufferedMessage,
                         message,
                         wrapper->externalInterface.allocator);
        Assert_true(wrapper->nextNonce == 0);
    }
    wrapper->hasBufferedMessage = true;

    Message_shift(message, Headers_CryptoAuth_SIZE);
    header = (union Headers_CryptoAuth*) message->bytes;
    header->nonce = UINT32_MAX;
    message->length = Headers_CryptoAuth_SIZE;

    // sessionState must be 0, auth and 24 byte nonce are garbaged and public key is set
    // now garbage the authenticator and the encrypted key which are not used.
    Random_bytes(wrapper->context->rand, (uint8_t*) &header->handshake.authenticator, 48);

    return wrapper->wrappedInterface->sendMessage(message, wrapper->wrappedInterface);
}

static uint8_t encryptHandshake(struct Message* message, struct CryptoAuth_Wrapper* wrapper)
{
    Message_shift(message, sizeof(union Headers_CryptoAuth));

    union Headers_CryptoAuth* header = (union Headers_CryptoAuth*) message->bytes;

    // garbage the auth field to frustrate DPI and set the nonce (next 24 bytes after the auth)
    Random_bytes(wrapper->context->rand,
                 (uint8_t*) &header->handshake.auth,
                 sizeof(union Headers_AuthChallenge) + 24);
    Bits_memcpyConst(&header->handshake.publicKey, wrapper->context->pub.publicKey, 32);

    if (!knowHerKey(wrapper)) {
        return genReverseHandshake(message, wrapper, header);
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

    Headers_setPacketAuthRequired(&header->handshake.auth, wrapper->authenticatePackets);

    // set the session state
    uint32_t sessionState_be = Endian_hostToBigEndian32(wrapper->nextNonce);
    header->nonce = sessionState_be;

    if (wrapper->nextNonce == 0 || wrapper->nextNonce == 2) {
        // If we're sending a hello or a key
        Random_bytes(wrapper->context->rand, wrapper->secret, 32);
        crypto_scalarmult_curve25519_base(header->handshake.encryptedTempKey,  wrapper->secret);

        #ifdef Log_KEYS
            uint8_t tempPrivateKeyHex[65];
            Hex_encode(tempPrivateKeyHex, 65, wrapper->secret, 32);
            uint8_t tempPubKeyHex[65];
            Hex_encode(tempPubKeyHex, 65, header->handshake.encryptedTempKey, 32);
            Log_keys(wrapper->context->logger, "Generating temporary keypair\n"
                                                "    myTempPrivateKey=%s\n"
                                                "     myTempPublicKey=%s\n",
                      tempPrivateKeyHex, tempPubKeyHex);
        #endif
        if (wrapper->nextNonce == 0) {
            Bits_memcpyConst(wrapper->tempKey, header->handshake.encryptedTempKey, 32);
        }
        #ifdef Log_DEBUG
            Assert_true(!Bits_isZero(header->handshake.encryptedTempKey, 32));
            Assert_true(!Bits_isZero(wrapper->secret, 32));
        #endif
    } else if (wrapper->nextNonce == 3) {
        // Dupe key
        // If nextNonce is 1 then we have our pubkey stored in wrapper->tempKey,
        // If nextNonce is 3 we need to recalculate it each time
        // because tempKey the final secret.
        crypto_scalarmult_curve25519_base(header->handshake.encryptedTempKey,
                                          wrapper->secret);
    } else {
        // Dupe hello
        // wrapper->nextNonce == 1
        // Our public key is cached in wrapper->tempKey so lets copy it out.
        Bits_memcpyConst(header->handshake.encryptedTempKey, wrapper->tempKey, 32);
    }
    #ifdef Log_KEYS
        uint8_t tempKeyHex[65];
        Hex_encode(tempKeyHex, 65, header->handshake.encryptedTempKey, 32);
        Log_keys(wrapper->context->logger,
                  "Wrapping temp public key:\n"
                  "    %s\n",
                  tempKeyHex);
    #endif

    uint8_t sharedSecret[32];
    if (wrapper->nextNonce < 2) {
        if (wrapper->nextNonce == 0) {
            Log_debug(wrapper->context->logger, "@%p Sending hello packet\n", (void*) wrapper);
        } else {
            Log_debug(wrapper->context->logger, "@%p Sending repeat hello packet\n",
                       (void*) wrapper);
        }
        getSharedSecret(sharedSecret,
                        wrapper->context->privateKey,
                        wrapper->herPerminentPubKey,
                        passwordHash,
                        wrapper->context->logger);

        wrapper->isInitiator = true;
        wrapper->nextNonce = 1;
    } else {
        if (wrapper->nextNonce == 2) {
            Log_debug(wrapper->context->logger, "@%p Sending key packet\n", (void*) wrapper);
        } else {
            Log_debug(wrapper->context->logger, "@%p Sending repeat key packet\n",
                       (void*) wrapper);
        }
        // Handshake2 wrapper->tempKey holds her public temp key.
        // it was put there by receiveMessage()
        getSharedSecret(sharedSecret,
                        wrapper->context->privateKey,
                        wrapper->tempKey,
                        passwordHash,
                        wrapper->context->logger);
        wrapper->nextNonce = 3;

        #ifdef Log_KEYS
            uint8_t tempKeyHex[65];
            Hex_encode(tempKeyHex, 65, wrapper->tempKey, 32);
            Log_keys(wrapper->context->logger,
                      "Using their temp public key:\n"
                      "    %s\n",
                      tempKeyHex);
        #endif
    }

    // Shift message over the encryptedTempKey field.
    Message_shift(message, 32 - Headers_CryptoAuth_SIZE);

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
    #ifdef Log_DEBUG
        Assert_true(!Bits_isZero(header->handshake.encryptedTempKey, 32));
    #endif

    // Shift it back -- encryptRndNonce adds 16 bytes of authenticator.
    Message_shift(message, Headers_CryptoAuth_SIZE - 32 - 16);

    return wrapper->wrappedInterface->sendMessage(message, wrapper->wrappedInterface);
}

static inline uint8_t encryptMessage(struct Message* message,
                                     struct CryptoAuth_Wrapper* wrapper)
{
    Assert_true(message->padding >= 36 || !"not enough padding");

    encrypt(wrapper->nextNonce,
            message,
            wrapper->secret,
            wrapper->isInitiator,
            wrapper->authenticatePackets);

    Message_shift(message, 4);

    union Headers_CryptoAuth* header = (union Headers_CryptoAuth*) message->bytes;
    header->nonce = Endian_hostToBigEndian32(wrapper->nextNonce);
    wrapper->nextNonce++;

    return wrapper->wrappedInterface->sendMessage(message, wrapper->wrappedInterface);
}

static uint8_t sendMessage(struct Message* message, struct Interface* interface)
{
    struct CryptoAuth_Wrapper* wrapper =
        Identity_cast((struct CryptoAuth_Wrapper*) interface->senderContext);

    // If there has been no incoming traffic for a while, reset the connection to state 0.
    // This will prevent "connection in bad state" situations from lasting forever.
    uint64_t nowSecs = Time_currentTimeSeconds(wrapper->context->eventBase);

    if (nowSecs - wrapper->timeOfLastPacket > wrapper->context->pub.resetAfterInactivitySeconds) {
        Log_debug(wrapper->context->logger, "No traffic in [%d] seconds, resetting connection.",
                  (int) (nowSecs - wrapper->timeOfLastPacket));

        wrapper->timeOfLastPacket = nowSecs;
        CryptoAuth_reset(interface);
        return encryptHandshake(message, wrapper);
    }

    #ifdef Log_DEBUG
        Assert_true(!((uintptr_t)message->bytes % 4) || !"alignment fault");
    #endif

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
            return encryptHandshake(message, wrapper);
        } else {
            Log_debug(wrapper->context->logger,
                       "@%p Doing final step to send message. nonce=4\n", (void*) wrapper);
            uint8_t secret[32];
            getSharedSecret(secret,
                            wrapper->secret,
                            wrapper->tempKey,
                            NULL,
                            wrapper->context->logger);
            Bits_memcpyConst(wrapper->secret, secret, 32);
        }
    }

    return encryptMessage(message, wrapper);
}

/** Call the external interface and tell it that a message has been received. */
static inline uint8_t callReceivedMessage(struct CryptoAuth_Wrapper* wrapper,
                                          struct Message* message)
{
    uint8_t ret = 0;
    if (wrapper->externalInterface.receiveMessage != NULL) {
        ret = wrapper->externalInterface.receiveMessage(message, &wrapper->externalInterface);
    }

    // If the message is authenticated OR if the packet is considered valid by the next level,
    // then don't allow the connection to timeout.
    if (!ret || wrapper->authenticatePackets) {
        wrapper->timeOfLastPacket = Time_currentTimeSeconds(wrapper->context->eventBase);
    }

    return ret;
}

static inline bool decryptMessage(struct CryptoAuth_Wrapper* wrapper,
                                  uint32_t nonce,
                                  struct Message* content,
                                  uint8_t secret[32])
{
    if (wrapper->authenticatePackets) {
        // Decrypt with authentication and replay prevention.
        int ret = decrypt(nonce, content, secret, wrapper->isInitiator, true);
        if (ret) {
            Log_debug(wrapper->context->logger,
                       "Authenticated decryption failed returning %u\n",
                       ret);
            return false;
        }
        ret = !ReplayProtector_checkNonce(nonce, &wrapper->replayProtector);
        if (ret) {
            Log_debug(wrapper->context->logger, "Nonce checking failed.\n");
            return false;
        }
    } else {
        decrypt(nonce, content, secret, wrapper->isInitiator, false);
    }
    int ret = callReceivedMessage(wrapper, content);
    if (ret) {
        Log_debug(wrapper->context->logger,
                   "Call received message failed returning %u\n",
                   ret);
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
        Log_debug(wrapper->context->logger, "Dropped runt packet\n");
        return Error_UNDERSIZE_MESSAGE;
    }

    // handshake
    // nextNonce 0: recieving hello.
    // nextNonce 1: recieving key, we sent hello.
    // nextNonce 2: recieving first data packet or duplicate hello.
    // nextNonce 3: recieving first data packet.
    // nextNonce >3: handshake complete

    if (wrapper->nextNonce < 2 && nonce == UINT32_MAX && !wrapper->requireAuth) {
        // Reset without knowing key is allowed until state reaches 2.
        // this is because we don't know that the other end knows our key until we
        // have received a valid packet from them.
        // We can't allow the upper layer to see this message because it's not authenticated.
        if (!knowHerKey(wrapper)) {
            Bits_memcpyConst(wrapper->herPerminentPubKey, header->handshake.publicKey, 32);
        }
        Message_shift(message, -Headers_CryptoAuth_SIZE);
        message->length = 0;
        wrapper->nextNonce = 0;
        wrapper->user = NULL;
        Log_debug(wrapper->context->logger, "Got a connect-to-me message, sending a hello");
        // Send an empty response (to initiate the connection).
        encryptHandshake(message, wrapper);
        return Error_NONE;
    }

    void* user = NULL;
    uint8_t passwordHashStore[32];
    uint8_t* passwordHash = tryAuth(header, passwordHashStore, wrapper, &user);
    if (wrapper->requireAuth && !user) {
        Log_debug(wrapper->context->logger,
                  "Dropping message because auth was not given and is required.\n");
        return Error_AUTHENTICATION;
    }
    if (passwordHash == NULL && header->handshake.auth.challenge.type != 0) {
        Log_debug(wrapper->context->logger,
                  "Dropping message because it contans an authenticator which is unrecognized.\n");
        return Error_AUTHENTICATION;
    }

    // What the nextNonce will become if this packet is valid.
    uint32_t nextNonce;

    // The secret for decrypting this message.
    uint8_t sharedSecret[32];

    uint8_t* herPermKey = NULL;
    if (nonce < 2) {
        if (nonce == 0) {
            Log_debug(wrapper->context->logger,
                       "@%p Received a hello packet, using auth: %d\n",
                       (void*) wrapper, (passwordHash != NULL));
        } else {
            Log_debug(wrapper->context->logger, "@%p Received a repeat hello packet\n",
                       (void*) wrapper);
        }

        // Decrypt message with perminent keys.
        if (!knowHerKey(wrapper) || wrapper->nextNonce == 0) {
            herPermKey = header->handshake.publicKey;
            #ifdef Log_DEBUG
                if (Bits_isZero(header->handshake.publicKey, 32)) {
                    Log_debug(wrapper->context->logger, "Node sent public key of ZERO!\n");
                }
            #endif
        } else {
            herPermKey = wrapper->herPerminentPubKey;
            if (Bits_memcmp(header->handshake.publicKey, herPermKey, 32)) {
                Log_debug(wrapper->context->logger, "Packet contains different perminent key.\n");
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
            Log_debug(wrapper->context->logger, "@%p Received a key packet\n", (void*) wrapper);
        } else if (nonce == 3) {
            Log_debug(wrapper->context->logger, "@%p Received a repeat key packet\n",
                       (void*) wrapper);
        } else {
            Log_debug(wrapper->context->logger,
                       "Received a packet of unknown type! nonce=%u\n", nonce);
        }
        if (Bits_memcmp(header->handshake.publicKey, wrapper->herPerminentPubKey, 32)) {
            Log_debug(wrapper->context->logger, "Packet contains different perminent key.\n");
            return Error_AUTHENTICATION;
        }
        // We sent the hello, this is a key
        getSharedSecret(sharedSecret,
                        wrapper->secret,
                        wrapper->herPerminentPubKey,
                        passwordHash,
                        wrapper->context->logger);
        nextNonce = 4;
    }

    // Shift it on top of the authenticator before the encrypted public key
    Message_shift(message, 48 - Headers_CryptoAuth_SIZE);

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
        Log_debug(wrapper->context->logger,
                  "Dropped message because authenticated decryption failed.\n");
        return Error_AUTHENTICATION;
    }

    wrapper->user = user;
    Bits_memcpyConst(wrapper->tempKey, header->handshake.encryptedTempKey, 32);

    #ifdef Log_DEBUG
        Assert_true(!Bits_isZero(header->handshake.encryptedTempKey, 32));
    #endif
    #ifdef Log_KEYS
        uint8_t tempKeyHex[65];
        Hex_encode(tempKeyHex, 65, wrapper->tempKey, 32);
        Log_keys(wrapper->context->logger,
                  "Unwrapping temp public key:\n"
                  "    %s\n",
                  tempKeyHex);
    #endif

    Message_shift(message, -32);
    wrapper->nextNonce = nextNonce;
    if (nextNonce == 2) {
        wrapper->isInitiator = false;
    }
    if (herPermKey && herPermKey != wrapper->herPerminentPubKey) {
        Bits_memcpyConst(wrapper->herPerminentPubKey, herPermKey, 32);
    }

    // If this is a handshake which was initiated in reverse because we
    // didn't know the other node's key, now send what we were going to send.
    if (wrapper->hasBufferedMessage && message->length == 0) {
        Log_debug(wrapper->context->logger, "Sending buffered message.\n");
        sendMessage(wrapper->bufferedMessage, &wrapper->externalInterface);
        wrapper->hasBufferedMessage = false;
        return Error_NONE;
    } else if (wrapper->hasBufferedMessage) {
        Log_debug(wrapper->context->logger, "There is a buffered message.\n");
    }

    Bits_memset(&wrapper->replayProtector, 0, sizeof(struct ReplayProtector));

    setRequiredPadding(wrapper);
    return callReceivedMessage(wrapper, message);
}

static uint8_t receiveMessage(struct Message* received, struct Interface* interface)
{
    struct CryptoAuth_Wrapper* wrapper =
        Identity_cast((struct CryptoAuth_Wrapper*) interface->receiverContext);

    union Headers_CryptoAuth* header = (union Headers_CryptoAuth*) received->bytes;

    if (received->length < (wrapper->authenticatePackets ? 20 : 4)) {
        Log_debug(wrapper->context->logger, "Dropped runt");
        return Error_UNDERSIZE_MESSAGE;
    }
    Assert_true(received->padding >= 12 || "need at least 12 bytes of padding in incoming message");
    #ifdef Log_DEBUG
        Assert_true(!((uintptr_t)received->bytes % 4) || !"alignment fault");
    #endif
    Message_shift(received, -4);

    uint32_t nonce = Endian_bigEndianToHost32(header->nonce);

    if (wrapper->nextNonce < 5) {
        if (nonce > 3 && nonce != UINT32_MAX && knowHerKey(wrapper)) {
            Log_debug(wrapper->context->logger,
                       "@%p Trying final handshake step, nonce=%u\n", (void*) wrapper, nonce);
            uint8_t secret[32];
            getSharedSecret(secret,
                            wrapper->secret,
                            wrapper->tempKey,
                            NULL,
                            wrapper->context->logger);

            // We'll optimistically advance the nextNonce value because decryptMessage()
            // passes the message on to the upper level and if this message causes a
            // response, we want the CA to be in ESTABLISHED state.
            // if the decryptMessage() call fails, we CryptoAuth_reset() it back.
            wrapper->nextNonce += 3;

            if (decryptMessage(wrapper, nonce, received, secret)) {
                Log_debug(wrapper->context->logger, "Final handshake step succeeded.\n");
                Bits_memcpyConst(wrapper->secret, secret, 32);
                return Error_NONE;
            }
            CryptoAuth_reset(&wrapper->externalInterface);
            Log_debug(wrapper->context->logger, "Final handshake step failed.\n");
        }
    } else if (nonce > 2 && decryptMessage(wrapper, nonce, received, wrapper->secret)) {
        // If decryptMessage returns false then we will try the packet as a handshake.
        return Error_NONE;
    } else {
        Log_debug(wrapper->context->logger, "Decryption failed, trying message as a handshake.\n");
    }
    Message_shift(received, 4);
    return decryptHandshake(wrapper, nonce, received, header);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

struct CryptoAuth* CryptoAuth_new(struct Allocator* allocator,
                                  const uint8_t* privateKey,
                                  struct EventBase* eventBase,
                                  struct Log* logger,
                                  struct Random* rand)
{
    struct CryptoAuth_pvt* ca = allocator->calloc(sizeof(struct CryptoAuth_pvt), 1, allocator);
    ca->allocator = allocator;

    ca->passwords = allocator->calloc(sizeof(struct CryptoAuth_Auth), 256, allocator);
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
                           void* user,
                           struct CryptoAuth* ca)
{
    struct CryptoAuth_pvt* context = Identity_cast((struct CryptoAuth_pvt*) ca);
    if (authType != 1) {
        return CryptoAuth_addUser_INVALID_AUTHTYPE;
    }
    if (context->passwordCount == context->passwordCapacity) {
        // TODO: realloc password space and increase buffer.
        return CryptoAuth_addUser_OUT_OF_SPACE;
    }
    struct CryptoAuth_Auth a;
    hashPassword_sha256(&a, password);
    for (uint32_t i = 0; i < context->passwordCount; i++) {
        if (!Bits_memcmp(a.secret, context->passwords[i].secret, 32)) {
            return CryptoAuth_addUser_DUPLICATE;
        }
    }
    a.user = user;
    Bits_memcpyConst(&context->passwords[context->passwordCount],
                     &a,
                     sizeof(struct CryptoAuth_Auth));
    context->passwordCount++;
    return 0;
}

int CryptoAuth_removeUsers(struct CryptoAuth* context, void* uid)
{
    struct CryptoAuth_pvt* ctx = Identity_cast((struct CryptoAuth_pvt*) context);
    if (!uid) {
        int count = ctx->passwordCount;
        Log_debug(ctx->logger, "Flushing [%d] users", count);
        ctx->passwordCount = 0;
        return count;
    }
    int count = 0;
    int i = 0;
    while (i < (int)ctx->passwordCount) {
        if (ctx->passwords[i].user == uid) {
            Bits_memcpyConst(&ctx->passwords[i],
                             &ctx->passwords[ctx->passwordCount--],
                             sizeof(struct CryptoAuth_Auth));
            count++;
        } else {
            i++;
        }
    }
    Log_debug(ctx->logger, "Removing [%d] user(s) identified by [%p]", count, uid);
    return count;
}

void* CryptoAuth_getUser(struct Interface* interface)
{
    struct CryptoAuth_Wrapper* wrapper =
        Identity_cast((struct CryptoAuth_Wrapper*)interface->senderContext);

    void* user = wrapper->user;
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
                                           const bool requireAuth,
                                           bool authenticatePackets,
                                           struct CryptoAuth* ca)
{
    struct CryptoAuth_pvt* context = Identity_cast((struct CryptoAuth_pvt*) ca);
    struct CryptoAuth_Wrapper* wrapper =
        toWrap->allocator->clone(sizeof(struct CryptoAuth_Wrapper), toWrap->allocator,
            &(struct CryptoAuth_Wrapper) {
                .user = NULL,
                .nextNonce = 0,
                .context = context,
                .wrappedInterface = toWrap,
                .requireAuth = requireAuth,
                .authenticatePackets = authenticatePackets,
                .timeOfLastPacket = Time_currentTimeSeconds(context->eventBase)
            });

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
    }

    return &wrapper->externalInterface;
}

void CryptoAuth_setAuth(const String* password,
                        const uint8_t authType,
                        struct Interface* wrappedInterface)
{
    struct CryptoAuth_Wrapper* wrapper =
        Identity_cast((struct CryptoAuth_Wrapper*)wrappedInterface->senderContext);
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
        Identity_cast((struct CryptoAuth_Wrapper*)interface->senderContext);
    wrapper->nextNonce = 0;
    wrapper->isInitiator = false;
    Bits_memset(&wrapper->replayProtector, 0, sizeof(struct ReplayProtector));
}

int CryptoAuth_getState(struct Interface* interface)
{
    struct CryptoAuth_Wrapper* wrapper =
        Identity_cast((struct CryptoAuth_Wrapper*)interface->senderContext);
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
            return CryptoAuth_HANDSHAKE3;
        default:
            return CryptoAuth_ESTABLISHED;
    }
}

struct Interface* CryptoAuth_getConnectedInterface(struct Interface* iface)
{
    if (iface->sendMessage == sendMessage) {
        // internal (plaintext side)
        struct CryptoAuth_Wrapper* wrapper =
            Identity_cast((struct CryptoAuth_Wrapper*)iface->senderContext);
        return wrapper->wrappedInterface;
    } else if (iface->receiveMessage == receiveMessage) {
        struct CryptoAuth_Wrapper* wrapper =
            Identity_cast((struct CryptoAuth_Wrapper*)iface->receiverContext);
        return &wrapper->externalInterface;
    }
    return NULL;
}
