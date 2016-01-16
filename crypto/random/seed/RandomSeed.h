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
#ifndef RandomSeed_H
#define RandomSeed_H

#include "memory/Allocator.h"
#include "util/log/Log.h"
#include "util/Linker.h"
Linker_require("crypto/random/seed/RandomSeed.c");

#include <stdint.h>

struct RandomSeed;

typedef struct RandomSeed* (* RandomSeed_Provider)(struct Allocator* alloc);
typedef int (* RandomSeed_Get)(struct RandomSeed* context, uint64_t buff[8]);

struct RandomSeed
{
    /**
     * Get some random numbers.
     *
     * @param context the seed context.
     * @param buff an array of uint32_t.
     * @param count the number of 32 bit elements in the array.
     */
    RandomSeed_Get get;

    /** A human friendly name for the random generator seed provider. */
    const char* name;
};

struct RandomSeed* RandomSeed_new(RandomSeed_Provider* providers,
                                  int providerCount,
                                  struct Log* logger,
                                  struct Allocator* alloc);

int RandomSeed_get(struct RandomSeed* rs, uint64_t buffer[8]);

#endif
