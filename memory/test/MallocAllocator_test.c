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
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "memory/Allocator.h"
#include "memory/MallocAllocator.c"

#define ALLOCATION_SIZE sizeof(struct Allocation)
#define ALLOCATOR_SIZE sizeof(struct FirstContext)

int main()
{
    struct Allocator* alloc = MallocAllocator_new(2048);
    size_t bytesUsed;

    assert((bytesUsed = MallocAllocator_bytesAllocated(alloc)) == 0);
    alloc->malloc(25, alloc);
    bytesUsed += 25 + ALLOCATION_SIZE;
    assert(MallocAllocator_bytesAllocated(alloc) == bytesUsed);

    struct Allocator* child = alloc->child(alloc);
    bytesUsed += ALLOCATION_SIZE + ALLOCATOR_SIZE;
    assert(MallocAllocator_bytesAllocated(alloc) == bytesUsed);

    child->malloc(30, child);
    bytesUsed += 30 + ALLOCATION_SIZE;
    assert(MallocAllocator_bytesAllocated(alloc) == bytesUsed);

    child->free(child);
    bytesUsed -= 30 + ALLOCATION_SIZE;
    bytesUsed -= ALLOCATION_SIZE + ALLOCATOR_SIZE;
    assert(MallocAllocator_bytesAllocated(alloc) == bytesUsed);
}
