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
#ifndef CanaryAllocator_pvt_H
#define CanaryAllocator_pvt_H

#include "crypto/random/Random.h"
#include "memory/Allocator.h"
#include "memory/CanaryAllocator.h"
#include "util/Identity.h"

#include <stdint.h>

struct CanaryAllocator_Allocation {
    const char* file;
    int line;
    uint32_t* beginCanary;
    uint32_t* endCanary;
    uint32_t canaryValue;
};

struct CanaryAllocator_pvt
{
    struct Allocator pub;
    struct Allocator* alloc;

    struct CanaryAllocator_Allocation* allocations;

    /** Store the first file to make sure the list hasn't been corrupted. */
    const char* firstFile;

    int allocCount;

    uint32_t canaryValue;
    struct Random* rand;
    Identity
};

int CanaryAllocator_isOverflow(struct CanaryAllocator_pvt* ctx);

#endif
