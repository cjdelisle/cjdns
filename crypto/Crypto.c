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
#include "crypto/Crypto.h"
#include "memory/MemAllocator.h"
#include "memory/BufferAllocator.h"
#include "libbenc/benc.h"

#include "crypto_hash_sha256.h"

#include <assert.h>
#include <stdio.h>
#include <event2/util.h>


/** @see Crypto.h */
void Crypto_init()
{
    if (evutil_secure_rng_init() != 0) {
        fprintf(stderr, "Unable to initialize secure random number generator, bailing out.");
        abort();
    }
}

/** @see Crypto.h */
String* Crypto_sha256sum(const String* hashThis,
                         const struct MemAllocator* allocator)
{
    String* out = benc_newBinaryString(NULL, crypto_hash_sha256_BYTES, allocator);
    crypto_hash_sha256((uint8_t*)out->bytes, (uint8_t*)hashThis->bytes, hashThis->len);
    return out;
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
