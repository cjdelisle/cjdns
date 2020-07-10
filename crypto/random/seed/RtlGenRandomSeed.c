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
#include "crypto/random/seed/RtlGenRandomSeed.h"
#include "util/Bits.h"

#include <stdint.h>
#include <windows.h>
#include <ntsecapi.h>

static int get(struct RandomSeed* rand, uint64_t buff[8])
{
    Bits_memset(buff, 0, 64);
    int ret = RtlGenRandom(buff, 64);
    if (!ret || Bits_isZero(buff, 64)) {
        return -1;
    }
    return 0;
}

struct RandomSeed* RtlGenRandomSeed_new(struct Allocator* alloc)
{
    return Allocator_clone(alloc, (&(struct RandomSeed) {
        .get = get,
        .name = "RtlGenRandom() (Windows)"
    }));
}
