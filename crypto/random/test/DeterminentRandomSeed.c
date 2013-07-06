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
#include "crypto/random/test/DeterminentRandomSeed.h"
#include "util/Bits.h"
/**
 * A random seed which always outputs the same number,
 * obviously you don't want to use this in the real world.
 */

static int get(struct RandomSeed* rand, uint64_t buff[8])
{
    // chosen by fair dice roll, guaranteed random.
    Bits_memset(buff, 4, 64);
    return 0;
}

struct RandomSeed* DeterminentRandomSeed_new(struct Allocator* alloc)
{
    return Allocator_clone(alloc, (&(struct RandomSeed) {
        .get = get,
        .name = "DeterminentRandomSeed TESTING ONLY, THIS SHOULD NEVER BE USED IN PRODUCTION!"
    }));
}
