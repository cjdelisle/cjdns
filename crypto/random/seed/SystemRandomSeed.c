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

#ifdef win32
    #include "crypto/random/seed/RtlGenRandomSeed.h"
#else
    #include "crypto/random/seed/DevUrandomRandomSeed.h"
    #ifdef linux
        #include "crypto/random/seed/ProcSysKernelRandomUuidRandomSeed.h"
        #if !defined(__ILP32__) && !defined(__aarch64__) && defined(__GLIBC__)
            #include "crypto/random/seed/LinuxRandomUuidSysctlRandomSeed.h"
        #endif
    #else
        #ifdef freebsd
            #include "crypto/random/seed/BsdKernArndSysctlRandomSeed.h"
        #endif
    #endif
    #include <sys/syscall.h>
    #if defined(__OPENBSD__) || (defined(SYS_getrandom) && \
        (SYS_getrandom != __NR_getrandom || defined(__NR_getrandom)))
        #include "crypto/random/seed/GetEntropyRandomSeed.h"
    #endif
#endif

static RandomSeed_Provider RandomSeedProvider_PROVIDERS[] = {
    #ifdef RtlGenRandomSeed_EXISTS
        RtlGenRandomSeed_new,
    #endif
    #ifdef DevUrandomRandomSeed_EXISTS
        DevUrandomRandomSeed_new,
    #endif
    #ifdef ProcSysKernelRandomUuidRandomSeed_EXISTS
        ProcSysKernelRandomUuidRandomSeed_new,
    #endif
    #ifdef LinuxRandomUuidSysctlRandomSeed_EXISTS
        LinuxRandomUuidSysctlRandomSeed_new,
    #endif
    #ifdef BsdKernArndSysctlRandomSeed_EXISTS
        BsdKernArndSysctlRandomSeed_new,
    #endif
    #ifdef GetEntropyRandomSeed_EXISTS
        GetEntropyRandomSeed_new,
    #endif
};

#define PROVIDERS_COUNT ((int)(sizeof RandomSeedProvider_PROVIDERS / sizeof *RandomSeedProvider_PROVIDERS))

RandomSeed_t* SystemRandomSeed_new(RandomSeed_Provider* additionalProviders,
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
        allProviders[i++] = RandomSeedProvider_PROVIDERS[j];
    }
    return RandomSeed_new(allProviders, providerCount, logger, alloc);
}
