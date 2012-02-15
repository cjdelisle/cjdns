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
#ifndef Allocator_H
#define Allocator_H

#include <stdlib.h>

struct Allocator;

/**
 * Writer interface which writes data to memory and
 * provides pointers to the memory locations where it wrote.
 */
struct Allocator
{
    /** The internal state of the Allocator. */
    void* const context;

    /**
     * Free the heap allocations held by the allocator and any of it's children.
     *
     * @param this the Allocator which is being called. Use: allocator->free(allocator);
     */
    void (* const free)(const struct Allocator* this);

    /**
     * Add a function to be called when the allocator is freed.
     * This helps keep track of memory allocated by third party applications
     * which demand to use malloc themselves.
     *
     * @param callback the function to call.
     * @param callbackContext the data to pass the function when calling it.
     * @param this the memory allocator.
     */
    void (* const onFree)(void (*callback)(void* callbackContext),
                          void* callbackContext,
                          const struct Allocator* this);

    /**
     * Allocate some memory from this memory allocator.
     * The allocation will be aligned on the size of a pointer, if you need further alignment then
     * you must handle it manually.
     *
     * @param numberOfBytes how much memory to allocate.
     * @param this the memory allocator, use allocator->malloc(10, allocator) to allocate 10 bytes.
     * @return a pointer to the newly allocated memory.
     * @see malloc()
     */
    void* (* const malloc)(size_t numberOfBytes,
                           const struct Allocator* this);

    /**
     * Allocate some memory from this memory allocator.
     * The allocation will be aligned on the size of a pointer, if you need further alignment then
     * you must handle it manually.
     * Memory location will be initialized to 0.
     *
     * @param numberOfBytes how much memory to allocate.
     * @param multiplier how many times the number of bytes to allocate.
     * @param this the memory allocator.
     * @return a pointer to the newly allocated memory.
     * @see calloc()
     */
    void* (* const calloc)(size_t numberOfBytes,
                           size_t multiplier,
                           const struct Allocator* this);

    /**
     * Allocate some memory and copy something into that memory space.
     * The allocation will be aligned on the size of a pointer, if you need further alignment then
     * you must handle it manually.
     *
     * @param numberOfBytes how much memory to allocate.
     * @param thisAllocator the memory allocator.
     * @param toClone a pointer to something which will be cloned into the newly allocated memory,
     *                if this is NULL or is not as large as numberOfBytes, undefined behavior will result.
     * @return a pointer to the newly allocated memory.
     */
    void* (* const clone)(size_t numberOfBytes,
                          const struct Allocator* thisAllocator,
                          const void* toClone);

    /**
     * Re-allocate memory so that an allocation can be expanded.
     * The allocation will be aligned on the size of a pointer, if you need further alignment then
     * you must handle it manually.
     *
     * @param originalAllocation a pointer to the original memory allocation which is to be reallocated.
     * @param numberOfBytes how much memory to allocate.
     * @param thisAllocator the memory allocator.
     * @return a pointer to the newly allocated memory.
     */
    void* (* const realloc)(const void* originalAllocation,
                            size_t numberOfBytes,
                            const struct Allocator* thisAllocator);

    /**
     * Get a new child of this allocator.
     * When this allocator is freed all of it's children will be freed as well.
     *
     * @param this the memory allocator, use allocator->child(allocator) to get a child.
     * @return a child allocator.
     */
    struct Allocator* (* const child)(const struct Allocator* this);
};

#endif /* MEMALLOCATOR_H */
