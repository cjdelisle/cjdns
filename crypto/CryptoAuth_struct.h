#ifndef CRYPTO_AUTH_STRUCT_H
#define CRYPTO_AUTH_STRUCT_H

#include "crypto/ReplayProtector.h"
#include "interface/Interface.h"
#include "libbenc/benc.h"
#include "memory/MemAllocator.h"
#include "wire/Headers.h"
#include "wire/Message.h"

#include <stdint.h>

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
    /** The public key of the other node. */
    uint8_t herPerminentPubKey[32];

    /**
     * If an object was associated with a password and the remote host authed
     * with the password this will be the object, otherwise it will be null.
     */
    void* user;

    /** The shared secret. */
    uint8_t secret[32];

    /** Used during handshake to hold her public key and my private key at different times. */
    uint8_t tempKey[32];

    /** An outgoing message which is buffered in the event that a reverse handshake is required. */
    struct Message* bufferedMessage;

    /** A password to use for authing with the other party. */
    String* password;

    /** Used for preventing replay attacks. */
    struct ReplayProtector replayProtector;

    /** The next nonce to use. */
    uint32_t nextNonce;

    /** The method to use for trying to auth with the server. */
    uint8_t authType;

    /** True if this node began the conversation. */
    bool isInitiator : 1;

    /** If true then the packets sent through this interface must be authenticated. */
    bool authenticatePackets : 1;

    /** If true and the other end is connecting, do not respond until a valid password is sent. */
    bool requireAuth : 1;

    /** A pointer back to the main cryptoauth context. */
    struct CryptoAuth* const context;

    /** The internal interface which we are wrapping. */
    struct Interface* const wrappedInterface;

    /** The interface which this wrapper provides. */
    struct Interface externalInterface;
};

#endif
