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
#include "crypto/Crypto.h"
#include "util/Hex.h"

#include <stdio.h>
#include <assert.h>

int main()
{
    uint8_t bytes[32];
    randombytes(bytes, 32);

    uint8_t hex[64] = {0};

    assert(Hex_encode(hex, 65, bytes, 32) == 64);

    //printf("hex encoded: %s\n", hex);

    uint8_t bytes2[32];
    assert(Hex_decode(bytes2, 32, hex, 64) == 32);

    assert(memcmp(bytes, bytes2, 32) == 0);
}
