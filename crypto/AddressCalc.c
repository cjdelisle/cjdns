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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "crypto_hash_sha512.h"
#include "util/Bits.h"

#include <stdint.h>

int AddressCalc_validAddress(const uint8_t address[16])
{
    return address[0] == 0xFC;
}

int AddressCalc_addressForPublicKey(uint8_t addressOut[16], const uint8_t key[32])
{
    uint8_t hash[crypto_hash_sha512_BYTES];
    crypto_hash_sha512(hash, key, 32);
    crypto_hash_sha512(hash, hash, crypto_hash_sha512_BYTES);
    if (addressOut) {
        Bits_memcpy(addressOut, hash, 16);
    }
    return AddressCalc_validAddress(addressOut);
}

