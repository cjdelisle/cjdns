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
#include "crypto/CryptoAuth_pvt.h"
#include "memory/Allocator.h"
#include "util/Hex.h"
#include "util/Bits.h"

#include <stdio.h>

#define HELLOWORLDLOWER "hello world"
#define HELLOWORLDLEN 12

static void encryptRndNonceTest()
{
    uint8_t nonce[24];
    Bits_memset(nonce, 0, 24);

    uint8_t secret[32];
    Bits_memset(secret, 0, 32);

    Allocator_t* alloc = Allocator_new(1<<14);
    Message_t* m = Message_new(0, 512, alloc);
    Er_assert(Message_epush(m, NULL, 44));
    Er_assert(Message_epop(m, NULL, 44));
    Er_assert(Message_epush(m, HELLOWORLDLOWER, CString_strlen(HELLOWORLDLOWER)+1));

    CryptoAuth_encryptRndNonce(nonce, m, secret);

    uint8_t* expected = (uint8_t*) "1391ac5d03ba9f7099bffbb6e6c69d67ae5bd79391a5b94399b293dc";
    uint8_t output[57];
    Hex_encode(output, 57, Message_bytes(m), Message_getLength(m));

    printf("\n%s\n%s\n", (char*) expected, (char*) output);
    Assert_true(!Bits_memcmp(expected, output, 56));

    Assert_true(!CryptoAuth_decryptRndNonce(nonce, m, secret));
    Assert_true(Message_getLength(m) == HELLOWORLDLEN
        && !Bits_memcmp(Message_bytes(m), HELLOWORLDLOWER, Message_getLength(m)));
    
    Allocator_free(alloc);
}

int main()
{
    encryptRndNonceTest();
    return 0;
}