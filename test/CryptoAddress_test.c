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
#include "crypto/AddressCalc.h"
#include "dht/Address.h"
#include "util/CString.h"
#include "util/Bits.h"
#include "util/Base32.h"
#include "util/Hex.h"
#include "util/Assert.h"

#include "crypto_scalarmult_curve25519.h"

#include <stdio.h>

// hex: 751d3db85b848deaf221e0ed2b6cc17f587b29057d74cdd4dc0bd18b7157288e
static const uint8_t privateKey[32] = {
    0x75, 0x1d, 0x3d, 0xb8, 0x5b, 0x84, 0x8d, 0xea,
    0xf2, 0x21, 0xe0, 0xed, 0x2b, 0x6c, 0xc1, 0x7f,
    0x58, 0x7b, 0x29, 0x05, 0x7d, 0x74, 0xcd, 0xd4,
    0xdc, 0x0b, 0xd1, 0x8b, 0x71, 0x57, 0x28, 0x8e
};

// hex: d7c0df45001a5be5e81c95e519be5199055237cb9116882cadcefe48ab735173
// base32: r6jzx210usqbgnm3pdtm1z6btd14pvdtkn5j8qnpgqzknpggkuw0
static const uint8_t publicKey[32] = {
    0xd7, 0xc0, 0xdf, 0x45, 0x00, 0x1a, 0x5b, 0xe5,
    0xe8, 0x1c, 0x95, 0xe5, 0x19, 0xbe, 0x51, 0x99,
    0x05, 0x52, 0x37, 0xcb, 0x91, 0x16, 0x88, 0x2c,
    0xad, 0xce, 0xfe, 0x48, 0xab, 0x73, 0x51, 0x73
};

static const char publicKeyBase32[] = "r6jzx210usqbgnm3pdtm1z6btd14pvdtkn5j8qnpgqzknpggkuw0";
static const char ipv6[] = "fc68:cb2c:60db:cb96:19ac:34a8:fd34:03fc";

int main()
{
    /* verify public key */
    struct Address address;
    crypto_scalarmult_curve25519_base(address.key, privateKey);

    AddressCalc_addressForPublicKey(address.ip6.bytes, address.key);

    uint8_t privateKeyHexOut[65];
    uint8_t publicKeyHexOut[65];
    uint8_t publicKeyBase32Out[53];

    Hex_encode(privateKeyHexOut, 65, privateKey, 32);

    Hex_encode(publicKeyHexOut, 65, publicKey, 32);

    printf("Private key %s (hex)\n\nExpect:\nPublic Key: %s (hex)\n"
           "Public Key: %s (base32)\nAddress: %s\n",
           privateKeyHexOut,
           publicKeyHexOut,
           publicKeyBase32,
           ipv6);

    uint8_t addressOut[40];

    Hex_encode(publicKeyHexOut, 65, address.key, 32);
    Base32_encode(publicKeyBase32Out, 53, address.key, 32);
    Address_printIp(addressOut, &address);

    printf("\nGot:\nPublic Key: %s (hex)\n"
           "Public Key: %s (base32)\nAddress: %s\n",
           publicKeyHexOut,
           publicKeyBase32Out,
           addressOut);

    Assert_true(0 == Bits_memcmp(address.key, publicKey, 32));
    Assert_true(0 == CString_strcmp(publicKeyBase32, (char*) publicKeyBase32Out));
    Assert_true(0 == CString_strcmp(ipv6, (char*) addressOut));
    return 0;
}
