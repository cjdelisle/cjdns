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
#include "util/Base32.h"
#include "util/Hex.h"

#include "crypto_scalarmult_curve25519.h"

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

static int usage(char* appName)
{
    printf("Usage: %s\n"
           "\n"
           "As private keys are very sensitive, %s reads them from stdin.\n"
           "If your shell, terminal, or other program keeps history,\n"
           "please avoid echo 'key' | %s, or similar constructs.\n"
           "A heredoc is suitable for use in scripts, and will avoid\n"
           "unwanted revealing of the key in process lists, like so:\n"
           "%s <<EOF\n"
           "key\n"
           "EOF\n"
           "\n"
           "Key should be in hex form, maximum 64 characters.\n"
           "Extra characters will be silently ignored.\n",
           appName, appName, appName, appName);
    return 0;
}

int main(int argc, char** argv)
{
    struct Address address;
    uint8_t addressOut[40];
    uint8_t privateKey[32];
    uint8_t publicKeyBase32Out[53];
    uint8_t privateKeyHexIn[65] = {0};
    if (argc > 1)
    {
        return usage(argv[0]);
    }
    if (read(0,privateKeyHexIn,64) < 0)
    {
        fprintf(stderr, "Reading private key failed: %s\n", strerror(errno));
        return 1;
    }
    for (uint8_t* n = privateKeyHexIn;n < privateKeyHexIn + 65;n++)
    {
        if ('\n' == *n)
        {
            *n = '\0';
            break;
        }
    }

    Hex_decode(privateKey, 32, privateKeyHexIn, 65);
    crypto_scalarmult_curve25519_base(address.key, privateKey);
    AddressCalc_addressForPublicKey(address.ip6.bytes, address.key);
    if (address.ip6.bytes[0] == 0xFC) {
        Base32_encode(publicKeyBase32Out, 53, address.key, 32);
        Address_printShortIp(addressOut, &address);
        printf(    "Input privkey: %s\n"
            "Matching pubkey: %s.k\n"
            "Resulting address: %s\n"
            ,privateKeyHexIn,publicKeyBase32Out,addressOut);
        return 0;
    } else {
        fprintf(stderr, "Not a valid cjdns address\n");
        return 1;
    }

}

