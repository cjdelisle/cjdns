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
#include "util/Bits.h"
#include "util/AddrTools.h"
#include "util/Assert.h"

int main()
{
    uint8_t out[16];
    Assert_true(!AddrTools_parseIp(out, (uint8_t*)"[::1]"));
    Assert_true(!Bits_memcmp(out, (uint8_t*)"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1", 16));
    return 0;
}
