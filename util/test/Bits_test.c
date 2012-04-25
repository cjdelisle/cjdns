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
#include "crypto/Crypto.h"
#include "util/Bits.h"

#include <stdio.h>
#include <assert.h>

int main()
{
    uint64_t x;
    randombytes((uint8_t*) &x, 8);

    assert(Bits_bitReverse64(Bits_bitReverse64(x)) == x);
    assert(Bits_bitReverse64(1) == ((uint64_t)1)<<63);
    assert(Bits_bitReverse64(0) == 0);
}
