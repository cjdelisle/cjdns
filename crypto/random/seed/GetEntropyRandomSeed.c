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
#define _GNU_SOURCE
#include "crypto/random/seed/GetEntropyRandomSeed.h"
#include "util/Identity.h"
#include "util/Bits.h"

#include <unistd.h>
#include <errno.h>
#include <sys/syscall.h>

#ifndef __OPENBSD__
static int getentropy(void *buf, size_t buflen)
{
    int ret;

    if (buflen > 256) {
        goto failure;
    }

    ret = syscall(SYS_getrandom, buf, buflen, 0);

    if (ret < 0) {
        return ret;
    }

    if ((size_t)ret == buflen) {
        return 0;
    }

    failure:
        errno = EIO;
        return -1;
}
#endif

static int get(struct RandomSeed* randomSeed, uint64_t output[8])
{
    if (getentropy(output, 64) < 0) {
        return -1;
    } else {
        return 0;
    }
}

struct RandomSeed* GetEntropyRandomSeed_new(struct Allocator* alloc)
{
    return Allocator_clone(alloc, (&(struct RandomSeed) {
        .get = get,
        .name = "getentropy(2)"
    }));
}
