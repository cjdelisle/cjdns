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
#include "crypto/random/seed/ProcSysKernelRandomUuidRandomSeed.h"
#include "util/Identity.h"
#include "util/Bits.h"
#include "util/Hex.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

/** Number of times to try each operation. */
#define MAX_TRIES 10

static int getUUID(uint64_t output[2])
{
    uint8_t buffer[40] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
    {
        int fd = -1;
        int tries = 0;
        while ((fd = open("/proc/sys/kernel/random/uuid", O_RDONLY, 0)) < 0) {
            if (++tries > MAX_TRIES || errno == ENOENT) {
                return -1;
            }
            sleep(1);
        }
        tries = 0;
        uint8_t* buff = (uint8_t*) buffer;
        int count = 37;
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
        if (count != 0) {
            return -1;
        }
    }

    // If it isn't in perfect form, fail.
    if (!(buffer[8] == '-'
        && buffer[13] == '-'
        && buffer[18] == '-'
        && buffer[23] == '-'
        && buffer[36] == '\n'))
    {
        return -1;
    }

    // fold back the last 4 characters into the locations of the dashes.
    buffer[8] = buffer[35];
    buffer[13] = buffer[34];
    buffer[18] = buffer[33];
    buffer[23] = buffer[32];
    buffer[32] = '\0';

    if (Hex_decode((uint8_t*)output, 16, buffer, 32) != 16) {
        return -1;
    }
    return 0;
}

static int get(struct RandomSeed* randomSeed, uint64_t output[8])
{
    if (getUUID(output) || getUUID(output+2) || getUUID(output+4) || getUUID(output+6)) {
        return -1;
    }
    return 0;
}

struct RandomSeed* ProcSysKernelRandomUuidRandomSeed_new(struct Allocator* alloc)
{
    return Allocator_clone(alloc, (&(struct RandomSeed) {
        .get = get,
        .name = "/proc/sys/kernel/random/uuid (Linux)"
    }));
}
