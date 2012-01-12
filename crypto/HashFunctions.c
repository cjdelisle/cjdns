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
#include <stdlib.h>
#include <stdint.h>

#include "crypto_hash_sha256.h"

#include "crypto/sha1/sha1.h"
#include "libbenc/benc.h"

/** @see Crypto.h */
String* Crypto_sha256sum(const String* hashThis,
                         const struct MemAllocator* allocator)
{
    String* out = benc_newBinaryString(NULL, crypto_hash_sha256_BYTES, allocator);
    crypto_hash_sha256((uint8_t*)out->bytes, (uint8_t*)hashThis->bytes, hashThis->len);
    return out;
}

/** @see Crypto.h */
String* Crypto_sha1sum(const String* hashThis,
                       const struct MemAllocator* allocator)
{
    String* out = benc_newBinaryString(NULL, SHA1_DIGEST_SIZE, allocator);
    SHA1_CTX ctx;
    SHA1_Init(&ctx);
    SHA1_Update(&ctx, (uint8_t*)hashThis->bytes, hashThis->len);
    SHA1_Final(&ctx, (uint8_t*)out->bytes);
    return out;
}
