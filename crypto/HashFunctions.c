#include "libbenc/benc.h"

#include <gcrypt.h>

/**
 * Generate a hash of a message.
 * This function will produce a benc_bstr_t object of the length of
 * the output of the function.
 * Output will be allocated in the provided allocator.
 *
 * @param hashThis a bencoded string to be hashed.
 * @param allocator a means of getting memory for storing the output.
 * @param function an integer representation of the gcrypt function to use EG: GCRY_MD_SHA1
 * @return a bencoded string containing the hash output in binary.
 */
static benc_bstr_t* genericGCryptHash(const benc_bstr_t* hashThis,
                                      const struct MemAllocator* allocator,
                                      const uint32_t function)
{
    const uint32_t length = gcry_md_get_algo_dlen(function);
    gcry_md_hd_t hash;
    gcry_md_open(&hash, function, 0);
    gcry_md_write(hash, hashThis->bytes, hashThis->len);
    const char* buffer = (char*) gcry_md_read(hash, 0);

    char* outbytes = allocator->calloc(length, 1, allocator);
    benc_bstr_t* out = allocator->calloc(sizeof(benc_bstr_t), 1, allocator);
    out->len = length;
    out->bytes = outbytes;
 
    memcpy(outbytes, buffer, length);
    gcry_md_close(hash);

    return out;
}

/** @see Crypto.h */
benc_bstr_t* Crypto_sha1sum(const benc_bstr_t* hashThis,
                             const struct MemAllocator* allocator)
{
    return genericGCryptHash(hashThis, allocator, GCRY_MD_SHA1);
}

/** @see Crypto.h */
benc_bstr_t* Crypto_sha256sum(const benc_bstr_t* hashThis,
                              const struct MemAllocator* allocator)
{
    return genericGCryptHash(hashThis, allocator, GCRY_MD_SHA256);
}
