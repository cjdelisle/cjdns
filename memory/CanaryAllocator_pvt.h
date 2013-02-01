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

struct CanaryAllocator_pvt
{
    struct Allocator pub;
    struct Allocator* alloc;
    uint32_t** canaries;

    /** Store the first and last canary pointers to make sure the list hasn't been corrupted. */
    uint32_t* firstCanary;
    uint32_t* lastCanary;

    int canaryCount;
    uint32_t canaryValue;
    struct Random* rand;
    Identity
};

int CanaryAllocator_isOverflow(struct CanaryAllocator_pvt* ctx);

#endif
