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

#include "crypto/Key.h"
#include "util/Base32.h"
#include "crypto/AddressCalc.h"

#include <stddef.h>

char* Key_parse_strerror(int error)
{
    switch (error) {
        case 0:                       return "none";
        case Key_parse_TOO_SHORT:     return "key must be 52 characters long";
        case Key_parse_MALFORMED:     return "key must end in .k";
        case Key_parse_DECODE_FAILED: return "failed to base-32 decode key";
        case Key_parse_INVALID:       return "not a valid cjdns public key";
        default:                      return "unknown error";
    }
}

#define Key_parse_TOO_SHORT -1
#define Key_parse_MALFORMED -2
#define Key_parse_DECODE_FAILED -3
#define Key_parse_INVALID -4


int Key_parse(String* key, uint8_t keyBytesOut[32], uint8_t ip6Out[16])
{
    if (!key || key->len < 52) {
        return Key_parse_TOO_SHORT;
    }
    if (key->bytes[52] != '.' || key->bytes[53] != 'k') {
        return Key_parse_MALFORMED;
    }
    if (Base32_decode(keyBytesOut, 32, (uint8_t*)key->bytes, 52) != 32) {
        return Key_parse_DECODE_FAILED;
    }
    if (ip6Out) {
        AddressCalc_addressForPublicKey(ip6Out, keyBytesOut);
        if (!AddressCalc_validAddress(ip6Out)) {
            return Key_parse_INVALID;
        }
    }
    return 0;
}

String* Key_stringify(uint8_t key[32], struct Allocator* alloc)
{
    String* out = String_newBinary(NULL, 54, alloc);
    Base32_encode((uint8_t*)out->bytes, 53, key, 32);
    out->bytes[52] = '.';
    out->bytes[53] = 'k';
    return out;
}
