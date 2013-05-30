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
#include "util/Assert.h"
#include "util/platform/libc/string.h"
#include <stdint.h>
#include <stdio.h>

#include "memory/Allocator.h"
#include "memory/MallocAllocator_pvt.h"

#ifdef MallocAllocator_USE_CANARIES
    #define ALLOCATION_SIZE sizeof(struct MallocAllocator_Allocation) + sizeof(long)
#else
    #define ALLOCATION_SIZE sizeof(struct MallocAllocator_Allocation)
#endif
#define ALLOCATOR_SIZE sizeof(struct MallocAllocator_pvt)

int main()
{
    struct Allocator* alloc = MallocAllocator_new(2048);
    size_t bytesUsed;

    bytesUsed = MallocAllocator_bytesAllocated(alloc);
    Assert_always(bytesUsed == ALLOCATION_SIZE + sizeof(struct MallocAllocator_FirstCtx));
    Allocator_malloc(alloc, 25);
    bytesUsed += 32 + ALLOCATION_SIZE;
    Assert_always(MallocAllocator_bytesAllocated(alloc) == bytesUsed);

    struct Allocator* child = Allocator_child(alloc);
    bytesUsed += ALLOCATION_SIZE + ALLOCATOR_SIZE;
    Assert_always(MallocAllocator_bytesAllocated(alloc) == bytesUsed);

    Allocator_malloc(child, 30);
    bytesUsed += 32 + ALLOCATION_SIZE;
    Assert_always(MallocAllocator_bytesAllocated(alloc) == bytesUsed);

    Allocator_free(child);
    bytesUsed -= 32 + ALLOCATION_SIZE;
    bytesUsed -= ALLOCATION_SIZE + ALLOCATOR_SIZE;
    Assert_always(MallocAllocator_bytesAllocated(alloc) == bytesUsed);

    Allocator_free(alloc);
}
