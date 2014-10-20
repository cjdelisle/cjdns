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
#include "util/AddrTools.h"
#include "util/CString.h"
#include <stdio.h>

static int usage(char* appName)
{
    printf("Usage: %s <public key>\n"
           "\n"
           "Get a cjdns IPv6 address from a public key.\n"
           "The key should be in Base32 and end in '.k'.\n",
           appName);
    return 0;
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        return usage(argv[0]);
    }

    uint8_t keyBytes[32];
    uint8_t ip6Bytes[16];
    String key = { .bytes = argv[1], .len = CString_strlen(argv[1]) };

    int ret = Key_parse(&key, keyBytes, ip6Bytes);
    switch (ret) {
        case Key_parse_TOO_SHORT: fprintf(stderr, "ERROR: Key_parse_TOO_SHORT\n"); return 1;
        case Key_parse_MALFORMED: fprintf(stderr, "ERROR: Key_parse_MALFORMED\n"); return 1;
        case Key_parse_DECODE_FAILED: fprintf(stderr, "ERROR: Key_parse_DECODE_FAILED\n"); return 1;
        case Key_parse_INVALID: fprintf(stderr, "ERROR: Key_parse_INVALID\n"); return 1;
        case 0: break;
        default: fprintf(stderr, "ERROR: unknown error [%d]\n", ret); return 1;
    }

    uint8_t output[40] = {0};
    AddrTools_printShortIp(output, ip6Bytes);
    printf("%s\n", output);
    return 0;
}

