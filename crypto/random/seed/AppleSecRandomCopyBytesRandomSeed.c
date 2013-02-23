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

#include "crypto/random/seed/AppleSecRandomCopyBytesRandomSeed.h"

#ifdef __APPLE__
#include <Availability.h>
#endif

#if __OSX_AVAILABLE_STARTING(__MAC_10_7,__IPHONE_2_0)
#include <Security/SecRandom.h>
#include <inttypes.h>

static int get(struct RandomSeed* randomSeed, uint64_t output[8])
{
    if (SecRandomCopyBytes(NULL, 64, (uint8_t *)output) != 0) {
        return -1;
    }
    return 0;
}
#else
static int get(struct RandomSeed* randomSeed, uint64_t output[8])
{
    return -1;
}
#endif

struct RandomSeed* AppleSecRandomCopyBytesRandomSeed_new(struct Allocator* alloc)
{
    return Allocator_clone(alloc, (&(struct RandomSeed) {
        .get = get,
        .name = "SecRandomCopyBytes(Rnd, Count, Bytes) (OS X)"
    }));
}

