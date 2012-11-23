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
#include "memory/Allocator.h"
#include "memory/BufferAllocator.h"
#include "memory/CanaryAllocator_pvt.h"
#include "util/Assert.h"

void tryOverflowLow(struct Allocator* alloc, int bytes)
{
    uint8_t* buff = Allocator_malloc(alloc, bytes);
    buff[-1] = 0;
    Assert_always(CanaryAllocator_isOverflow((struct CanaryAllocator_pvt*) alloc));
}

void tryOverflowHigh(struct Allocator* alloc, int bytes)
{
    uint8_t* buff = Allocator_malloc(alloc, bytes);
    buff[bytes + 3] = 0;
    Assert_always(CanaryAllocator_isOverflow((struct CanaryAllocator_pvt*) alloc));
}

void tryOverflow(struct Allocator* alloc, int bytes)
{
    // can't free these child allocators or they will explode!
    struct Allocator* child = Allocator_child(alloc);
    tryOverflowLow(child, bytes);
    child = Allocator_child(alloc);
    tryOverflowHigh(alloc, bytes);
}

void tryCalloc(struct Allocator* alloc, int bytes)
{
    Allocator_calloc(alloc, bytes, 1);
    CanaryAllocator_check(alloc);
}

int main()
{
    struct Allocator* bAlloc;
    BufferAllocator_STACK(bAlloc, 20000);

    struct Allocator* alloc = CanaryAllocator_new(bAlloc, NULL);

    tryCalloc(alloc, 0);
    tryCalloc(alloc, 1);
    tryCalloc(alloc, 2);
    tryCalloc(alloc, 3);
    tryCalloc(alloc, 4);
    tryCalloc(alloc, 5);
    tryCalloc(alloc, 6);
    tryCalloc(alloc, 7);
    tryCalloc(alloc, 8);
    tryCalloc(alloc, 9);
    tryCalloc(alloc, 10);
    tryCalloc(alloc, 11);
    tryCalloc(alloc, 12);

    void* buff = Allocator_calloc(alloc, 1, 1);
    CanaryAllocator_check(alloc);
    Allocator_realloc(alloc, buff, 3);
    CanaryAllocator_check(alloc);

    tryOverflow(alloc, 0);
    tryOverflow(alloc, 1);
    tryOverflow(alloc, 2);
    tryOverflow(alloc, 3);
    tryOverflow(alloc, 4);
    tryOverflow(alloc, 5);
    tryOverflow(alloc, 6);
    tryOverflow(alloc, 7);
    tryOverflow(alloc, 8);
    tryOverflow(alloc, 9);
    tryOverflow(alloc, 10);
    tryOverflow(alloc, 11);
    tryOverflow(alloc, 12);
}
