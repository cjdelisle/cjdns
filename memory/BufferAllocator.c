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
struct BufferAllocator_pvt;
#define Allocator_Provider_CONTEXT_TYPE struct BufferAllocator_pvt
#include "exception/Except.h"
#include "memory/BufferAllocator.h"
#include "util/Bits.h"
#include "util/Identity.h"

#include <stdint.h>

/**
 * TODO(cjd): addOnFreeJob adds a job which is only run when the root allocator is freed
 *            and it needs to be run when the allocator which called it, or any of that allocator's
 *            ancestors is freed, not just the root.
 */

/* Define alignment as the size of a pointer which is usually 4 or 8 bytes. */
#define ALIGNMENT sizeof(char*)

/** Internal state for Allocator. */
struct BufferAllocator_pvt
{
    /** Pointer to the beginning of the buffer. */
    char* const basePointer;

    /** Pointer to the place in the buffer to allocate the next block of memory. */
    char* pointer;

    /** Pointer to the end of the buffer. */
    char* const endPointer;

    struct Except* onOOM;

    Identity
};



/**
 * Get a pointer which is aligned on memory boundries.
 *
 * @param pointer the location where the pointer should be.
 * @param alignedOn how big the word is that the boundry should be aligned on.
 */
#define getAligned(pointer, alignedOn) \
    ((char*) ((uintptr_t)( ((char*)(pointer)) + (alignedOn) - 1) & ~ ((alignedOn) - 1)))

/** @see Allocator_malloc() */
static void* allocatorMalloc(struct BufferAllocator_pvt* context, unsigned long length)
{
    Identity_check(context);
    char* pointer = getAligned(context->pointer, ALIGNMENT);
    char* endOfAlloc = pointer + length;

    if (endOfAlloc >= context->endPointer) {
        Except_throw(context->onOOM, "BufferAllocator ran out of memory.");
    }

    if (endOfAlloc < context->pointer) {
        Except_throw(context->onOOM, "BufferAllocator integer overflow.");
    }

    context->pointer = endOfAlloc;
    return (void*) pointer;
}

static void* provideMemory(struct BufferAllocator_pvt* context,
                           struct Allocator_Allocation* original,
                           unsigned long size,
                           struct Allocator* group)
{
    if (original == NULL) {
        return allocatorMalloc(context, size);
    }

    if (((char*)original) + original->size == context->pointer) {
        // This is reallocating the last allocation.
        // clear the allocation then let allocatorMalloc() recreate it.
        context->pointer = (char*)original;
    }

    if (size == 0) {
        return NULL;
    }

    void* newAlloc = allocatorMalloc(context, size);
    if (newAlloc != original) {
        Assert_true((char*)newAlloc > (char*)original + original->size);
        Bits_memcpy(newAlloc, original, original->size);
    }
    return newAlloc;
}

/** @see BufferAllocator.h */
struct Allocator* BufferAllocator__new(void* buffer,
                                       unsigned long length,
                                       char* file,
                                       int line)
{
    struct BufferAllocator_pvt stackAlloc = {
        .basePointer = buffer,
        .pointer = buffer,
        .endPointer = ((char*)buffer) + length
    };
    Identity_set(&stackAlloc);
    struct BufferAllocator_pvt* alloc =
        allocatorMalloc(&stackAlloc, sizeof(struct BufferAllocator_pvt));
    Bits_memcpy(alloc, &stackAlloc, sizeof(struct BufferAllocator_pvt));
    return Allocator_new(0, provideMemory, alloc, file, line);
}
