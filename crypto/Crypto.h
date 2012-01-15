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
#ifndef CRYPTO_H
#define CRYPTO_H

#include <stdint.h>
#include "libbenc/benc.h"
#include "wire/Message.h"

/**
 * Initialize the random number generator.
 * This must be called before doing anything else.
 */
void Crypto_init();

/**
 * Generate a SHA-256 hash of a message.
 * This function will produce a String struct of length 32
 * allocated in the provided allocator.
 *
 * @param hashThis a bencoded string to be hashed.
 * @param allocator a means of getting memory for storing the output.
 * @return a bencoded string containing the hash output in binary.
 */
benc_bstr_t* Crypto_sha256sum(const benc_bstr_t* hashThis,
                              const struct MemAllocator* allocator);

/**
 * Overwrite a string with random bytes.
 * The size of the string will dictate the number of random bytes written.
 *
 * @param toRandomize the String to scramble.
 */
void Crypto_randomize(String* toRandomize);

void randombytes(unsigned char* buffer, unsigned long long size);

#endif
