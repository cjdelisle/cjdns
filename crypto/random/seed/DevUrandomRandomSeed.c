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
#include "crypto/random/seed/DevUrandomRandomSeed.h"
#include "util/Identity.h"
#include "util/Bits.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

/** Number of times to try each operation. */
#define MAX_TRIES 10

static int get(struct RandomSeed* randomSeed, uint64_t output[8])
{
    Bits_memset(output, 0, 64);
    int fd = -1;
    int tries = 0;
    while ((fd = open("/dev/urandom", O_RDONLY, 0)) < 0) {
        if (++tries > MAX_TRIES || errno == ENOENT) {
            return -1;
        }
        sleep(1);
    }
    tries = 0;
    uint8_t* buff = (uint8_t*) output;
    int count = 64;
    while (count > 0) {
        int r = read(fd, buff, count);
        if (r < 1) {
            if (++tries > MAX_TRIES) {
                break;
            }
            sleep(1);
            continue;
        }
        buff += r;
        count -= r;
    }
    close(fd);
    if (count == 0 && !Bits_isZero(output, 64)) {
        return 0;
    }
    return -1;
}

struct RandomSeed* DevUrandomRandomSeed_new(struct Allocator* alloc)
{
    return Allocator_clone(alloc, (&(struct RandomSeed) {
        .get = get,
        .name = "/dev/urandom"
    }));
}
