/* vim: set expandtab ts=4 sw=4: */
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "crypto_hash_sha512.h"
#include "util/Bits.h"
#include "util/Endian.h"
#include "crypto/AddressCalc.h"

#include <stdint.h>
#include <stdbool.h>

/* These two constants are in big endian so they can be compared
 * immediately with a uint8_t[] containing an address.
 */
#define ADDRESS_PREFIX_U64 (Endian_hostToBigEndian64( \
            ((uint64_t) AddressCalc_ADDRESS_PREFIX) << (64 - AddressCalc_ADDRESS_PREFIX_BITS)))
#define ADDRESS_PREFIX_MASK (Endian_hostToBigEndian64( \
            UINT64_MAX << (64 - AddressCalc_ADDRESS_PREFIX_BITS)))

#include <stdio.h>

bool AddressCalc_validAddress(const uint8_t address[16])
{
    uint64_t significant_bits;
    Bits_memcpy (&significant_bits, address, sizeof(uint64_t));
    return (significant_bits & ADDRESS_PREFIX_MASK) == ADDRESS_PREFIX_U64;
}

void AddressCalc_makeValidAddress(uint8_t address[16])
{
    uint64_t significant_bits;
    Bits_memcpy (&significant_bits, address, sizeof(uint64_t));
    significant_bits &= ~ADDRESS_PREFIX_MASK; // zero out the prefix
    significant_bits |= ADDRESS_PREFIX_U64; // put the new prefix
    Bits_memcpy (address, &significant_bits, sizeof(uint64_t));
}

bool AddressCalc_addressForPublicKey(uint8_t addressOut[16], const uint8_t key[32])
{
    uint8_t hash[crypto_hash_sha512_BYTES];
    crypto_hash_sha512(hash, key, 32);
    crypto_hash_sha512(hash, hash, crypto_hash_sha512_BYTES);
    if (addressOut) {
        Bits_memcpy(addressOut, hash, 16);
    }
    return AddressCalc_validAddress(addressOut);
}

