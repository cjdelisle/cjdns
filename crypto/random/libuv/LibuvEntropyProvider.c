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
#include "util/events/libuv/UvWrapper.h"
#include "crypto/random/libuv/LibuvEntropyProvider.h"
#include "crypto/random/Random.h"
#include "memory/Allocator.h"
#include "util/events/EventBase.h"
#include "util/events/Timeout.h"
#include "util/Identity.h"
#include "util/log/Log.h"

#include <inttypes.h>

/**
 * Number of milliseconds between samples.
 * Set to 1 second so random generator will be refreshed every 256 seconds.
 * (4 minutes 16 seconds)
 */
#define SAMPLE_MILLISECONDS 1000

struct LibuvEntropyProvider
{
    struct Allocator* alloc;
    struct Random* rand;
    struct Timeout* timeout;
    struct Log* logger;
    Identity
};

static void takeSample(void* vLibuvEntropyProvider)
{
    struct LibuvEntropyProvider* lep =
        Identity_check((struct LibuvEntropyProvider*) vLibuvEntropyProvider);
    uint64_t nanotime = uv_hrtime();
    Log_keys(lep->logger, "Adding high-res time [%" PRIu64 "] to random generator", nanotime);
    uint32_t input = (uint32_t) (nanotime ^ (nanotime >> 32));
    Random_addRandom(lep->rand, input);
}

void LibuvEntropyProvider_start(struct Random* provideTo,
                                struct EventBase* base,
                                struct Log* logger,
                                struct Allocator* alloc)
{
    struct LibuvEntropyProvider* lep =
        Allocator_calloc(alloc, sizeof(struct LibuvEntropyProvider), 1);
    Log_info(logger, "Taking clock samples every [%d]ms for random generator", SAMPLE_MILLISECONDS);
    lep->alloc = alloc;
    lep->rand = provideTo;
    lep->timeout = Timeout_setInterval(takeSample, lep, SAMPLE_MILLISECONDS, base, alloc);
    lep->logger = logger;
    Identity_set(lep);
}

struct Random* LibuvEntropyProvider_newDefaultRandom(struct EventBase* base,
                                                     struct Log* logger,
                                                     struct Except* eh,
                                                     struct Allocator* alloc)
{
    struct Random* rand = Random_new(alloc, logger, eh);
    LibuvEntropyProvider_start(rand, base, logger, alloc);
    return rand;
}
