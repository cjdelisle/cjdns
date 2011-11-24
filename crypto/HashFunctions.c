#include "crypto_hash_sha256.h"

#include "libbenc/benc.h"

/** @see Crypto.h */
String* Crypto_sha256sum(const String* hashThis,
                         const struct MemAllocator* allocator)
{
    String* out = benc_newBinaryString(NULL, crypto_hash_sha256_BYTES, allocator);
    crypto_hash_sha256((uint8_t*)out->bytes, (uint8_t*)hashThis->bytes, hashThis->len);
    return out;
}
