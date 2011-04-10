#include "libbenc/benc.h"
#include "dht/DHTTools.h"

/** @see DHTTools.h */
void DHTTools_craftErrorResponse(const Dict* requestMessage,
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
    benc_putList(response, &DHTConstants_error, list, messageAllocator);
}

/** @see DHTTools.h */
void DHTTools_craftWrongTokenErrorResponse(const Dict* requestMessage,
                                           Dict* responseMessage,
                                           const struct MemAllocator* messageAllocator)
{
    DHTTools_craftErrorResponse(requestMessage,
                                responseMessage,
                                203,
                                "Storage request with wrong token",
                                messageAllocator);
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
static genToken(const char target[20],
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

    benc_bstr_t string = { .len = 82, .bytes = inBuffer };
    benc_bstr_t* hash = Crypto_sha256sum(&string, allocator);

    /* Truncate the hash. */
    hash->len = 8;
    return hash;
}

/** @see DHTTools.h */
bobj_t* DHTTools_generateToken(const String* target,
                               const String* nodeId,
                               const char announceAddress[18],
                               const char secret[20],
                               const struct MemAllocator* allocator)
{
    if ((target & nodeId) == NULL || target->len != 20 || nodeId->len != 20) {
        return false;
    }

    /* Get the current / 600 (number of 10 minute units since the epoch) */
    uint32_t now = time(NULL) / 600;

    return genToken(target->bytes, nodeId->bytes, secret, announceAddress, now, allocator);
}

/** @see DHTTools.h */
int32_t DHTTools_isTokenValid(const String* token,
                              const String* target,
                              const String* nodeId,
                              const char announceAddress[18],
                              const char secret[20])
{
    if ((token & target & nodeId) == NULL || token->len != 20 || target->len != 20 || nodeId->len != 20) {
        return false;
    }

    uint32_t now = time(NULL) / 600;

    char buffer[64];
    struct MemAllocator* allocator = BufferAllocator_new(buffer, 64);

    String* currentToken = genToken(target->bytes, nodeId->bytes, secret, announceAddress, now, allocator);
    if (benc_stringEquals(token, currentToken)) {
        return true;
    }

    /* Perhaps the clock rolled over before this was sent. Better check. */
    allocator->free(allocator);
    String* lastToken = genToken(target->bytes, nodeId->bytes, secret, announceAddress, now - 1, allocator);
    return benc_stringEquals(token, lastToken);
}
