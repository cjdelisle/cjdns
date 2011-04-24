#include <string.h>
#include <time.h>

#include "crypto/Crypto.h"
#include "dht/dhtstore/DHTStoreTools.h"
#include "dht/DHTConstants.h"
#include "libbenc/benc.h"
#include "memory/MemAllocator.h"
#include "memory/BufferAllocator.h"

/** @see DHTStoreTools.h */
void DHTStoreTools_craftErrorReply(const Dict* requestMessage,
                                   Dict* responseMessage,
                                   int64_t errorCode,
                                   const char* errorMessage,
                                   const struct MemAllocator* messageAllocator)
{
    String* transactionId = benc_lookupString(requestMessage, &DHTConstants_transactionId);
    if (transactionId != NULL) {
        benc_putString(responseMessage, &DHTConstants_transactionId, transactionId, messageAllocator);
    }
    List* list = benc_addInteger(NULL, errorCode, messageAllocator);
    benc_addString(list, benc_newString(errorMessage, messageAllocator), messageAllocator);
    benc_putList(responseMessage, &DHTConstants_error, list, messageAllocator);
}

/**
 * Generate a token.
 * Outputs will be 8 character bencoded strings but more than 32 bytes will be allocated
 * since it is truncated SHA-256.
 *
 * @param target an input which defines the token output.
 * @param nodeId an input which defines the token output.
 * @param announceAddress an input which defines the token output.
 * @param secret an input which defines the token output.
 * @param counter a number which can be used to make tokens expire after a time limit or other means.
 * @param allocator the means to acquire memory for storing the token.
 * @return a token which will always be the same for the same inputs.
 */
static String* genToken(const char target[20],
                        const char nodeId[20],
                        const char secret[20],
                        const char announceAddress[18],
                        uint32_t counter,
                        const struct MemAllocator* allocator)
{
    char inBuffer[82];
    char* buffPtr = inBuffer;

    memcpy(buffPtr, target, 20);
    buffPtr += 20;

    memcpy(buffPtr, nodeId, 20);
    buffPtr += 20;

    memcpy(buffPtr, secret, 20);
    buffPtr += 20;

    memcpy(buffPtr, &counter, 4);
    buffPtr += 4;

    if (announceAddress != NULL) {
        memcpy(buffPtr, announceAddress, 18);
    } else {
        memset(buffPtr, 0, 18);
    }
    buffPtr += 18;

    String string = { .len = 82, .bytes = inBuffer };
    String* hash = Crypto_sha256sum(&string, allocator);

    /* Truncate the hash. */
    hash->len = 8;
    return hash;
}

/** @see DHTStoreTools.h */
String* DHTStoreTools_generateToken(const String* target,
                                    const String* nodeId,
                                    const char announceAddress[18],
                                    const char secret[20],
                                    const struct MemAllocator* allocator)
{
    if (target == NULL || nodeId == NULL || target->len != 20 || nodeId->len != 20) {
        return NULL;
    }

    // Get the current / 600 (number of 10 minute units since the epoch)
    uint32_t now = time(NULL) / 600;

    return genToken(target->bytes, nodeId->bytes, secret, announceAddress, now, allocator);
}

/** @see DHTStoreTools.h */
int32_t DHTStoreTools_isTokenValid(const String* token,
                                   const String* target,
                                   const String* nodeId,
                                   const char announceAddress[18],
                                   const char secret[20])
{
    if (token == NULL
        || target == NULL
        || nodeId == NULL
        || token->len != 8
        || target->len != 20
        || nodeId->len != 20)
    {
        return 0;
    }

    uint32_t now = time(NULL) / 600;

    char buffer[256];
    struct MemAllocator* allocator = BufferAllocator_new(buffer, 256);

    String* currentToken = genToken(target->bytes, nodeId->bytes, secret, announceAddress, now, allocator);
    if (benc_stringEquals(token, currentToken)) {
        return 1;
    }

    // Perhaps the clock rolled over before this was sent. Better check.
    allocator->free(allocator);
    String* lastToken = genToken(target->bytes, nodeId->bytes, secret, announceAddress, now - 1, allocator);
    return benc_stringEquals(token, lastToken);
}
