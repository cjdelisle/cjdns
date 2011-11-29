#include <assert.h>
#include <stdio.h>
#include <event2/util.h>

#include "crypto/Crypto.h"
#include "memory/MemAllocator.h"
#include "memory/BufferAllocator.h"
#include "libbenc/benc.h"


/** @see Crypto.h */
void Crypto_init()
{
    if (evutil_secure_rng_init() != 0) {
        fprintf(stderr, "Unable to initialize secure random number generator, bailing out.");
        abort();
    }
}

/** @see Crypto.h */
void Crypto_randomize(String* toRandomize)
{
    evutil_secure_rng_get_bytes(toRandomize->bytes, toRandomize->len);
}

/**
 * This is used by libnacl which requires us to provide it with a randombytes() function.
 */
void randombytes(unsigned char* buffer,unsigned long long size)
{
    evutil_secure_rng_get_bytes(buffer, size);
}
