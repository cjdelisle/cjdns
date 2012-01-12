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
#include "crypto/AddressCalc.h"
#include "crypto/Crypto.h"
#include "dht/Address.h"
#include "util/Base32.h"
#include "util/Time.h"
#include "crypto_scalarmult_curve25519.h"

#include <stdint.h>
#include <stdio.h>

int main()
{
    uint8_t key[32];
    struct Address addr;
    randombytes(key, 32);
    uint32_t smallest = UINT32_MAX;
    for (uint32_t i = 0;; i++) {
        crypto_scalarmult_curve25519_base(key, key);
        AddressCalc_addressForPublicKey(addr.ip6.bytes, key);
        if ((addr.ip6.ints.three & 0xFF) == 0xFC) {
            uint32_t bits = Endian_bigEndianToHost32(addr.ip6.ints.four);
            if (bits < smallest) {
                smallest = bits;
                for (int it = 0; it < 32; it++) {
                    printf("%02x", key[it]);
                }
                uint8_t ipv6[40];
                Address_printIp(ipv6, &addr);
                printf("    %s\n", ipv6);
            }
        }
    }
}
