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
#define _GNU_SOURCE
#include "crypto/random/seed/LinuxRandomUuidSysctlRandomSeed.h"
#include "util/Identity.h"
#include "util/Bits.h"
#include "util/Hex.h"

#include <unistd.h>
#include <fcntl.h>

static int getUUID(uint64_t output[2])
{
    uint8_t uuid[40];
    Bits_memset(output, 0, 16);
    int fd = open("/proc/sys/kernel/random/uuid",O_RDONLY);
    if (fd < 0)
    {
        return -1;
    }
    int rc = read(fd,uuid,36);
    close(fd);
    if (rc < 32)
    {
        return -1;
    }
    uuid[rc] = 0;
    /* strip '-' */
    int i,j = 0;
    for (i = 0; uuid[i] != 0; ++i) {
        if (uuid[i] != '-')
        {
            uuid[j++] = uuid[i];
        }
    }
    uuid[j] = 0;
    if (Hex_decode((uint8_t*) output, 16, uuid, j) != 16
        || Bits_isZero(output, 16))
    {
        return -1;
    }

    return 0;
}

static int get(struct RandomSeed* randomSeed, uint64_t output[8])
{

#ifdef SYS_getrandom
    // Try using getrandom instead sysctl as with systems with getrandom
    // sysctl is probably alrady deprecated and possibly disabled.
    Bits_memset(output, 0, 64); // Just make sure that it is zero all along.

    long ret = syscall(SYS_getrandom, output, 64, 0);
    if (ret == 64 && !Bits_isZero(output, 64)) {
        return 0;
    }
#endif
    if (getUUID(output) || getUUID(output+2) || getUUID(output+4) || getUUID(output+6)) {
        return -1;
    }
    return 0;
}

struct RandomSeed* LinuxRandomUuidSysctlRandomSeed_new(struct Allocator* alloc)
{
    return Allocator_clone(alloc, (&(struct RandomSeed) {
        .get = get,
        .name = "sysctl(RANDOM_UUID) (Linux)"
    }));
}
