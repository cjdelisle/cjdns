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
#include "crypto/random/seed/BsdKernArndSysctlRandomSeed.h"
#include "util/Identity.h"
#include "util/Bits.h"

#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/sysctl.h>

/** Number of times to try each operation. */
#define MAX_TRIES 10

static int get(struct RandomSeed* randomSeed, uint64_t output[8])
{
    int mib[] = { CTL_KERN, KERN_ARND };
    Bits_memset(output, 0, 64);
    size_t len = 64;
    if (sysctl(mib, 2, output, &len, NULL, 0) == -1) {
        // TOR/Libevent retry this 4 bytes at a time if it fails initially.
        size_t four = 4;
        int tries = 0;
        union {
            uint64_t longs[8];
            uint32_t ints[16];
        } num;
        for (int i = 0; i < 16; i++) {
            if (sysctl(mib, 2, &num.ints[i], &four, NULL, 0) == -1) {
                return -1;
            }
            if (num.ints[i] == 0) {
                i--;
                if (++tries > MAX_TRIES) {
                    return -1;
                }
            }
        }
    }
    return Bits_isZero(output, 64) ? -1 : 0;
}

struct RandomSeed* BsdKernArndSysctlRandomSeed_new(struct Allocator* alloc)
{
    return Allocator_clone(alloc, (&(struct RandomSeed) {
        .get = get,
        .name = "sysctl(KERN_ARND) (BSD)"
    }));
}
