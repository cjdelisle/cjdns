#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

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
#include "wire/Message.h"
#include "interface/Interface.h"

/** The constant used in nacl. */
static const uint8_t keyHashSigma[16] = "expand 32-byte k";
static const uint8_t keyHashNonce[16] = {0};

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
    uint32_t passwordCount;
    uint32_t passwordCapacity;

    struct MemAllocator* allocator;
};

struct Wrapper
{
    /** How we tell sessions apart. */
    uint8_t herPerminentPubKey[32];

    /**
     * If an object was associated with a password and the remote host authed
     * with the password this will be the object, otherwise it will be null.
     */
    void* user;

    /** A password to use for authing with the other party. */
    String* password;

    /** The shared secret. */
    uint8_t secret[32];

    /** Used during handshake to hold her public key and my private key at different times. */
    uint8_t tempKey[32];

    /** An outgoing message which is buffered in the event that a reverse handshake is required. */
    struct Message* bufferedMessage;

    /** The next nonce to use. */
    uint32_t nextNonce;

    /** True if this node began the conversation. */
    bool isInitiator;

    /** A pointer back to the main cryptoauth context. */
    struct CryptoAuth* context;

    /** The internal interface which we are wrapping. */
    struct Interface* wrappedInterface;

    /** The interface which this wrapper provides. */
    struct Interface externalInterface;
};

struct CryptoAuth* CryptoAuth_new(struct MemAllocator* allocator)
{
    struct CryptoAuth* ca = allocator->calloc(sizeof(struct CryptoAuth), 1, allocator);
    ca->allocator = allocator;

    ca->passwords = allocator->calloc(sizeof(struct Auth), 256, allocator);
    ca->passwordCount = 0;
    ca->passwordCapacity = 256;

    crypto_box_curve25519xsalsa20poly1305_keypair(ca->publicKey, ca->privateKey);
    return ca;
}

/**
 * Get a shared secret.
 *
 * @param outputSecret an array to place the shared secret in.
 * @param myPrivateKey
 * @param herPublicKey
 * @param passwordHash a 32 byte value known to both ends, this must be provably pseudorandom
 *                     the first 32 bytes of a sha256 output from hashing a password is ok,
 *                     whatever she happens to send me in the Auth field is NOT ok.
 *                     If this field is null, the secret will be generated without the password.
 */
static inline void getSharedSecret(uint8_t outputSecret[32],
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

static inline void hashPassword_sha256(struct Auth* auth, String* password)
{
    uint8_t tempBuff[32];
    crypto_hash_sha256(auth->secret, (uint8_t*) password->bytes, password->len);
    crypto_hash_sha256(tempBuff, auth->secret, 32);
    memcpy(auth->challenge.bytes, tempBuff, Headers_AuthChallenge_SIZE);
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
static inline struct Auth* getAuth(union Headers_AuthChallenge auth, struct CryptoAuth* context)
{
    if (auth.challenge.type != 1) {
        return NULL;
    }
    for (uint32_t i = 0; i < context->passwordCount; i++) {
        if (memcmp(auth.bytes, &context->passwords[i], Headers_AuthChallenge_SIZE - 1) == 0) {
            return &context->passwords[i];
        }
    }
    return NULL;
}

static inline void getPasswordHash(uint8_t output[32], uint8_t derivations, struct Auth* auth)
{
    uint8_t tempBuff[32];
    memcpy(output, auth->secret, 32);
    for (uint32_t i = 0; i < derivations; i++) {
        crypto_hash_sha256(tempBuff, output, 32);
        crypto_hash_sha256(output, tempBuff, 32);
    }
}

static inline uint8_t* tryAuth(union Headers_CryptoAuth* cauth,
                               uint8_t hashOutput[32],
                               struct Wrapper* wrapper)
{
    struct Auth* auth = getAuth(cauth->handshake.auth, wrapper->context);
    if (auth != NULL) {
        getPasswordHash(hashOutput, cauth->handshake.auth.challenge.derivations, auth);
        if (cauth->handshake.auth.challenge.derivations == 0) {
            wrapper->user = auth->user;
        }
        return hashOutput;
    }
    return NULL;
}

static inline int32_t decryptKey(union Headers_CryptoAuth* cauth,
                                 uint8_t sharedSecret[32],
                                 uint8_t output[32])
{
    uint8_t key[crypto_box_curve25519xsalsa20poly1305_ZEROBYTES + 32];
    uint8_t boxedKey[crypto_box_curve25519xsalsa20poly1305_BOXZEROBYTES + 48];
    memcpy(&boxedKey[crypto_box_curve25519xsalsa20poly1305_BOXZEROBYTES],
           cauth->handshake.encryptedTempKey, 48);
    uint64_t cipherLength = crypto_box_curve25519xsalsa20poly1305_BOXZEROBYTES + 48;
    if (crypto_box_curve25519xsalsa20poly1305_open_afternm(
        key, boxedKey, cipherLength, cauth->handshake.nonce, sharedSecret) != 0)
    {
        return -1;
    }
    memcpy(output, &key[crypto_box_curve25519xsalsa20poly1305_ZEROBYTES], 32);
    return 0;
}

static inline int32_t encryptKey(union Headers_CryptoAuth* cauth,
                                 uint8_t sharedSecret[32],
                                 uint8_t toEncrypt[32])
{
    uint8_t key[crypto_box_curve25519xsalsa20poly1305_ZEROBYTES + 32];
    uint8_t boxedKey[crypto_box_curve25519xsalsa20poly1305_BOXZEROBYTES + 48];
    memcpy(&key[crypto_box_curve25519xsalsa20poly1305_ZEROBYTES], toEncrypt, 32);
    uint64_t cipherLength = crypto_box_curve25519xsalsa20poly1305_BOXZEROBYTES + 48;
    if (crypto_box_curve25519xsalsa20poly1305_afternm(
        key, boxedKey, cipherLength, cauth->handshake.nonce, sharedSecret) != 0)
    {
        return -1;
    }
    memcpy(cauth->handshake.encryptedTempKey,
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
static inline int cryptHandshakeMessage(struct Message* message,
                                        uint8_t nonce[24],
                                        uint8_t secret[32])
{
    return crypto_stream_xsalsa20_xor(message->bytes,
                                      message->bytes,
                                      message->length,
                                      nonce,
                                      secret);
}

/**
 * @param cauth the cryptoAuth header.
 * @param content the content of the message to (de)cipher.
 * @param secret the shared secret.
 * @param isInitiatorOrRecipient true if we are the initiator of the connection and encrypting
 *                               or if we are the recipient of the connection and decrypting.
 *                               Otherwise false.
 */
static inline void cipher(union Headers_CryptoAuth* cauth,
                          struct Message* content,
                          uint8_t secret[20],
                          bool isInitiatorOrRecipient)
{
    union {
        uint32_t ints[2];
        uint8_t bytes[8];
    } nonce = { .ints = {0, 0} };
    nonce.ints[!isInitiatorOrRecipient] = cauth->nonce;

    crypto_stream_salsa20_xor(content->bytes,
                              content->bytes,
                              content->length,
                              nonce.bytes,
                              secret);
}

static inline void setRequiredPadding(struct Wrapper* wrapper)
{
    uint32_t padding = (wrapper->nextNonce < 4) ? 4 : sizeof(union Headers_CryptoAuth);
    wrapper->externalInterface.requiredPadding =
        wrapper->wrappedInterface->requiredPadding + padding;
    wrapper->externalInterface.maxMessageLength =
        wrapper->wrappedInterface->maxMessageLength - padding;
}

static bool knowHerKey(struct Wrapper* wrapper)
{
    uint64_t* herKey = (uint64_t*) wrapper->herPerminentPubKey;
    return (herKey[0] | herKey[1] | herKey[2] | herKey[3]) != 0;
}

static uint16_t sendMessage(struct Interface* interface, struct Message* message)
{
    struct Wrapper* wrapper = (struct Wrapper*) interface->senderContext;

    // nextNonce 0: sending hello, we are initiating connection.
    // nextNonce 1: sending another hello, nothing received yet.
    // nextNonce 2: sending key, hello received.
    // nextNonce 3: sending key again, no data packet recieved yet.
    // nextNonce >3: handshake complete
    //
    // if it's a blind handshake, every message will be plaintext and nextNonce will remain
    // zero until the first message is received back.

    if (wrapper->nextNonce < 4) {
        assert(message->padding >= sizeof(union Headers_CryptoAuth) || !"not enough padding");

        union Headers_CryptoAuth* header =
            (union Headers_CryptoAuth*) message->bytes - sizeof(union Headers_CryptoAuth);
        struct Message toSend = {
            .bytes = (uint8_t*)header,
            .length = message->length + sizeof(union Headers_CryptoAuth),
            .padding = message->length - sizeof(union Headers_CryptoAuth)
        };

        // It's handshake 1 or 2.
        memcpy(header->handshake.publicKey, wrapper->context->publicKey, 32);

        // If the nonce is 1 it should become 0 because hellos all need to
        // have a nonce equal to her perm key.
        // NOTE: this nonce field is NOT used for these types of packets, only data packets.
        header->nonce = (wrapper->nextNonce >> 1) ^ *((uint32_t*)wrapper->herPerminentPubKey);

        // If we don't know her key, the handshake has to be done backwards.
        if (!knowHerKey(wrapper)) {
            wrapper->nextNonce = 0;

            // Buffer the packet so it can be sent ASAP
            if (wrapper->bufferedMessage == NULL) {
                wrapper->bufferedMessage =
                    Message_clone(message, wrapper->externalInterface.allocator);
            } else {
                Message_copyOver(wrapper->bufferedMessage,
                                 message,
                                 wrapper->externalInterface.allocator);
            }

            message->length = sizeof(union Headers_CryptoAuth);
            return wrapper->wrappedInterface->sendMessage(message, wrapper->wrappedInterface);
        }

        Crypto_randomize(&(String) { .bytes = (char*)header->handshake.nonce, .len = 24 });

        // Password auth
        uint8_t* passwordHash = NULL;
        if (wrapper->password != NULL) {
            struct Auth auth;
            hashPassword_sha256(&auth, wrapper->password);
            passwordHash = auth.secret;
            memcpy(header->handshake.auth.bytes,
                   &auth.challenge,
                   sizeof(union Headers_AuthChallenge));
        } else {
            // no auth.
            header->handshake.auth.challenge.type = 0;
        }

        uint8_t myTempPubKey[32];
        if (wrapper->nextNonce == 0 || wrapper->nextNonce == 2) {
            crypto_box_curve25519xsalsa20poly1305_keypair(myTempPubKey, wrapper->secret);
        } else if (wrapper->nextNonce == 3) {
            // If nextNonce is 1 then we have our pubkey stored in wrapper->tempKey,
            // otherwise we need to recalculate it each time.
            crypto_scalarmult_curve25519_base(myTempPubKey, wrapper->secret);
        }

//    permA permB [tempA]
//    permB tempA [tempB]

        uint8_t sharedSecret[32];
        if (wrapper->nextNonce < 2) {
            // Sending hello
            getSharedSecret(sharedSecret,
                            wrapper->context->privateKey,
                            wrapper->herPerminentPubKey,
                            passwordHash);
            // Look at that, I started it.
            wrapper->isInitiator = true;
            wrapper->nextNonce = 1;
            memcpy(wrapper->tempKey, myTempPubKey, 32);
        } else {
            // 2 or 3
            // sending key
            // Handshake2 wrapper->tempKey holds her public temp key.
            // it was put there by recieveMessage()
            getSharedSecret(sharedSecret,
                            wrapper->context->privateKey,
                            wrapper->tempKey,
                            passwordHash);
            wrapper->nextNonce = 3;
        }

        assert(encryptKey(header, sharedSecret, myTempPubKey) == 0);
        cryptHandshakeMessage(content, header->handshake.nonce, wrapper->secret);

        return wrapper->wrappedInterface->sendMessage(wrapper->wrappedInterface);

    } else {
        // send traffic
        cipher(header, &wrapper->externalMessage, wrapper->secret, wrapper->isInitiator);
        return wrapper->wrappedInterface->sendMessage(wrapper->wrappedInterface);
    }
}

/** Call the external interface and tell it that a message has been received. */
static void callReceivedMessage(struct Wrapper* wrapper)
{
    if (wrapper->externalInterface.receiveMessage != NULL) {
        wrapper->externalInterface.receiveMessage(&wrapper->externalMessage,
                                                  &wrapper->externalInterface);
    }
}
/*
0   hello>  0
1   hello>  2
3  <key     2
3  <key     3
*/

static void receiveMessage(struct Message* received, struct Interface* interface)
{
    struct Wrapper* wrapper = (struct Wrapper*) interface;
    union Headers_CryptoAuth* header = (union Headers_CryptoAuth*) received->bytes;

    if (header->nonce == *((uint32_t*)wrapper->context->publicKey) || wrapper->nextNonce < 4)
    {
        if (received->length < sizeof(union Headers_CryptoAuth)) {
            // runt
            return;
        }

        // handshake
        // nextNonce 0: recieving hello.
        // nextNonce 1: recieving key, we sent hello.
        // nextNonce 2: recieving first data packet or duplicate hello.
        // nextNonce 3: recieving first data packet.
        // nextNonce >3: handshake complete

        if (wrapper->nextNonce < 2 && header->nonce == 0) {
            // Reset without knowing key is allowed until state reaches 2.
            // this is because we don't know that the other end knows our key until we
            // have received a valid packet from them.
            // We can't allow the upper layer to see this message because it's not authenticated.
            if (!knowHerKey(wrapper)) {
                memcpy(wrapper->herPerminentPubKey, header->handshake.publicKey, 32);
            }
            received->length = sizeof(union Headers_CryptoAuth);
            sendMessage(received, &wrapper->externalInterface);
            return;
        }

        uint8_t* passwordHash = NULL;
        uint8_t passwordHashStore[32];
        tryAuth(header, passwordHashStore, wrapper);

        struct Message content = {
            .bytes = ((uint8_t*)header) + sizeof(union Headers_CryptoAuth),
            .length = wrapper->wrappedMessage->length - sizeof(union Headers_CryptoAuth)
        };

        // What the nextNonce will become if this packet is valid.
        uint32_t nextNonce;

        // This will become wrapper->secret if this packet is valid.
        uint8_t sharedSecret[32];

        if (wrapper->nextNonce == 0
            || header->nonce == *((uint32_t*)wrapper->context->publicKey)) {
            // They sent a hello (this might be a reset connection hello)
            // Decrypt message with perminent keys.
            uint8_t* herPermKey =
                knowHerKey(wrapper) ? wrapper->herPerminentPubKey : header->handshake.publicKey;
            getSharedSecret(sharedSecret,
                            wrapper->context->privateKey,
                            herPermKey,
                            passwordHash);
            nextNonce = 2;
        } else if (wrapper->nextNonce == 1) {
            // We sent the hello, this is a key
            getSharedSecret(sharedSecret,
                            wrapper->tempKey,
                            wrapper->herPerminentPubKey,
                            passwordHash);
            nextNonce = 3;
        }

        cryptHandshakeMessage(&content, header->handshake.nonce, wrapper->secret);

        // Decrypt her temp public key and store it in session->tempKey.
        if (decryptKey(header, wrapper->secret, wrapper->tempKey) != 0) {
            // invalid
            return;
        }

        wrapper->nextNonce = nextNonce;
        memcpy(wrapper->herPerminentPubKey, header->handshake.publicKey, 32);

        // If this is a handshake which was initiated in reverse because we
        // didn't know the other node's key, now send what we were going to send.
        if (wrapper->bufferedMessage.bytes != NULL && content.length == 0) {
            memcpy(wrapper->wrappedMessage->bytes,
                   wrapper->bufferedMessage.bytes,
                   wrapper->bufferedMessage.length);
            wrapper->wrappedMessage->length = wrapper->bufferedMessage.length;
            sendMessage(&wrapper->externalInterface);
            return;
        }

        setRequiredPadding(wrapper);
        callReceivedMessage(wrapper);

        return;




    } else {
        if (received->length < 4) {
            // RUNT!!!
            return;
        }
        setRequiredPadding(wrapper);

        struct Message content = {
            .bytes = received->bytes + 4,
            .length = received->length - 4
        };
        cipher(header, content, wrapper->secret, !wrapper->isInitiator);

        callReceivedMessage(wrapper);
    }
}

struct Interface* CryptoAuth_wrapInterface(struct Interface* toWrap,
                                           uint8_t herPublicKey[32],
                                           String* password,
                                           struct CryptoAuth* context)
{
    struct Wrapper* wrapper =
        toWrap->allocator->clone(sizeof(struct Wrapper), 1, toWrap->allocator &(struct Wrapper) {
            .user = NULL,
            .nextNonce = 0,
            .context = context,
            .wrappedInterface = toWrap,
            .
        });
    wrapper->user = NULL;
    wrapper->nextNonce = 0;
    wrapper->context = context;
    wrapper->wrappedInterface = toWrap;
    wrapper->wrappedMessage = toWrap->getEmptyMessage(toWrap);
    wrapper->password = (password != NULL)
        ? benc_newBinaryString(password->bytes, password->len, toWrap->allocator)
        : NULL;
    toWrap->receiverContext = wrapper;
    toWrap->receiveMessage = receiveMessage;

    struct Interface iface = {
        .senderContext = wrapper,
        .getEmptyMessage = getEmptyMessage,
        .receiveMessage = receiveMessage,
        .sendMessage = sendMessage,
        .allocator = toWrap->allocator
    };
    memcpy(&wrapper->externalInterface, &iface, sizeof(struct Interface));
    memcpy(wrapper->herPerminentPubKey, herPublicKey, 32);

    return &wrapper->externalInterface;
}
