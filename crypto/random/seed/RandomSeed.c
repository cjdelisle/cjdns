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
#include "crypto/random/seed/RandomSeed.h"
#include "util/Identity.h"
#include "util/log/Log.h"
#include "util/Bits.h"

#include <crypto_hash_sha512.h>

#include "crypto/random/seed/RtlGenRandomSeed.h"
#include "crypto/random/seed/BsdKernArndSysctlRandomSeed.h"
#include "crypto/random/seed/DevUrandomRandomSeed.h"
#include "crypto/random/seed/LinuxRandomUuidSysctlRandomSeed.h"
#include "crypto/random/seed/ProcSysKernelRandomUuidRandomSeed.h"

static RandomSeed_Provider PROVIDERS[] = {
    /** windows */
    RtlGenRandomSeed_new,

    /** bsd syscall(KERN_ARND) */
    BsdKernArndSysctlRandomSeed_new,

    /** /dev/urandom */
    DevUrandomRandomSeed_new,

    /** linux syscall(RANDOM_UUID) */
    LinuxRandomUuidSysctlRandomSeed_new,

    /** linux /proc/sys/kernel/random/uuid */
    ProcSysKernelRandomUuidRandomSeed_new
};

struct RandomSeed_pvt
{
    struct RandomSeed pub;
    struct RandomSeed** rsList;
    int rsCount;
    struct Log* logger;
    Identity
};

static int get(struct RandomSeed* rs, uint64_t buffer[8])
{
    struct RandomSeed_pvt* ctx = Identity_cast((struct RandomSeed_pvt*) rs);
    Log_info(ctx->logger, "Attempting to seed random number generator");

    // each provider overwrites input and output is a rolling hash.
    struct {
        uint64_t output[8];
        uint64_t input[8];
    } buff = { .output = {0} };

    int successCount = 0;
    for (int i = 0; i < ctx->rsCount; i++) {
        if (!ctx->rsList[i]->get(ctx->rsList[i], buff.input)) {
            Log_info(ctx->logger, "Trying random seed [%s] Success", ctx->rsList[i]->name);
            crypto_hash_sha512((uint8_t*)buff.output, (uint8_t*)&buff, sizeof(buff));
            successCount++;
        } else {
            Log_info(ctx->logger, "Trying random seed [%s] Failed", ctx->rsList[i]->name);
        }
    }
    Bits_memcpyConst(buffer, buff.output, sizeof(buff.output));

    if (successCount > 0) {
        Log_info(ctx->logger, "Seeding random number generator succeeded with [%d] sources",
                 successCount);
        return 0;
    } else {
        Log_error(ctx->logger, "Seeding random number generator failed");
        return -1;
    }
}

int RandomSeed_get(struct RandomSeed* rs, uint64_t buffer[8])
{
    return rs->get(rs, buffer);
}

struct RandomSeed* RandomSeed_new(RandomSeed_Provider* providers,
                                  struct Log* logger,
                                  struct Allocator* alloc)
{
    int providerCount = sizeof(PROVIDERS) / sizeof(RandomSeed_Provider);
    if (providers) {
        for (int i = 0; providers[i]; i++) {
            providerCount++;
        }
    }

    struct RandomSeed** rsList = Allocator_calloc(alloc, sizeof(struct RandomSeed), providerCount);
    int i = 0;
    if (providers) {
        for (int j = 0; providers[j]; j++) {
            struct RandomSeed* rs = providers[j](alloc);
            if (rs) {
                rsList[i++] = rs;
            }
        }
    }
    for (int j = 0; j < (int)(sizeof(PROVIDERS) / sizeof(RandomSeed_Provider)); j++) {
        struct RandomSeed* rs = PROVIDERS[j](alloc);
        if (rs) {
            rsList[i++] = rs;
        }
    }

    struct RandomSeed_pvt* out = Allocator_malloc(alloc, sizeof(struct RandomSeed_pvt));

    out->rsList = rsList;
    out->rsCount = i;
    out->logger = logger;
    out->pub.get = get;
    out->pub.name = "RandomSeed conglomeration of random seed providers";
    Identity_set(out);

    return &out->pub;
}
