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
#include "crypto/random/seed/RandomSeed.h"
#include "crypto/random/seed/SystemRandomSeed.h"
#include "util/log/Log.h"

<?js file.RandomSeedProvider_providers = []; ?>
#define RandomSeedProvider_register(name) <?js file.RandomSeedProvider_providers.push(#name) ?>
#define RandomSeedProvider_list() <?js return file.RandomSeedProvider_providers.join(','); ?>

#ifdef win32
    #include "crypto/random/seed/RtlGenRandomSeed.h"
    RandomSeedProvider_register(RtlGenRandomSeed_new)
#else
    #include "crypto/random/seed/DevUrandomRandomSeed.h"
    RandomSeedProvider_register(DevUrandomRandomSeed_new)
    #ifdef linux
        #include "crypto/random/seed/ProcSysKernelRandomUuidRandomSeed.h"
        RandomSeedProvider_register(ProcSysKernelRandomUuidRandomSeed_new)
        #if !defined(__ILP32__) && !defined(__aarch64__) && defined(__GLIBC__)
            #include "crypto/random/seed/LinuxRandomUuidSysctlRandomSeed.h"
            RandomSeedProvider_register(LinuxRandomUuidSysctlRandomSeed_new)
        #endif
    #else
        #ifdef freebsd
            #include "crypto/random/seed/BsdKernArndSysctlRandomSeed.h"
            RandomSeedProvider_register(BsdKernArndSysctlRandomSeed_new)
        #endif
    #endif
    #include <sys/syscall.h>
    #if defined(__OPENBSD__) || (defined(SYS_getrandom) && \
        (SYS_getrandom != __NR_getrandom || defined(__NR_getrandom)))
        #include "crypto/random/seed/GetEntropyRandomSeed.h"
        RandomSeedProvider_register(GetEntropyRandomSeed_new)
    #endif
#endif

static RandomSeed_Provider PROVIDERS[] = { RandomSeedProvider_list() };
#define PROVIDERS_COUNT ((int)(sizeof(PROVIDERS) / sizeof(RandomSeed_Provider)))

struct RandomSeed* SystemRandomSeed_new(RandomSeed_Provider* additionalProviders,
                                        int additionalProviderCount,
                                        struct Log* logger,
                                        struct Allocator* alloc)
{
    int providerCount = PROVIDERS_COUNT + additionalProviderCount;

    RandomSeed_Provider* allProviders =
        Allocator_calloc(alloc, sizeof(RandomSeed_Provider), providerCount+1);
    int i = 0;
    for (int j = 0; j < additionalProviderCount; j++) {
        allProviders[i++] = additionalProviders[j];
    }
    for (int j = 0; j < PROVIDERS_COUNT; j++) {
        allProviders[i++] = PROVIDERS[j];
    }
    return RandomSeed_new(allProviders, providerCount, logger, alloc);
}
