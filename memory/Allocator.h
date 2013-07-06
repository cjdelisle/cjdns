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
#ifndef Allocator_H
#define Allocator_H

#include <stdlib.h>
#include <stdbool.h>

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
     * Free the heap allocations held by the allocator and any of its children.
     *
     * @param this the Allocator which is being called. Use: allocator->free(allocator);
     */
    void (* const free)(const struct Allocator* this, const char* identFile, int identLine);

    /**
     * Add a function to be called when the allocator is freed.
     * This helps keep track of memory allocated by third party applications
     * which demand to use malloc themselves.
     *
     * @param callback the function to call.
     * @param callbackContext the data to pass the function when calling it.
     * @param this the memory allocator.
     * @return a pointer which can be passed to notOnFree() to remove the job.
     */
    void* (* const onFree)(void (*callback)(void* callbackContext),
                           void* callbackContext,
                           const struct Allocator* this);

    /**
     * Remove a function which was registered with onFree().
     *
     * @param job the return value from calling onFree().
     * @param alloc the memory allocator.
     * @return true if the job was found and removed, false otherwise.
     */
    bool (* const notOnFree)(void* job, struct Allocator* alloc);

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
                           const struct Allocator* thisAlloc,
                           const char* identFile,
                           int identLine);

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
                           const struct Allocator* thisAlloc,
                           const char* identFile,
                           int identLine);

    /**
     * Allocate some memory and copy something into that memory space.
     * The allocation will be aligned on the size of a pointer, if you need further alignment then
     * you must handle it manually.
     *
     * @param numberOfBytes how much memory to allocate.
     * @param thisAllocator the memory allocator.
     * @param toClone a pointer to something which will be cloned into the newly allocated memory,
     *                if this is NULL or is not as large as numberOfBytes, undefined behavior will
     *                result.
     * @return a pointer to the newly allocated memory.
     */
    void* (* const clone)(size_t numberOfBytes,
                          const struct Allocator* thisAllocator,
                          const void* toClone,
                          const char* identFile,
                          int identLine);

    /**
     * Re-allocate memory so that an allocation can be expanded.
     * The allocation will be aligned on the size of a pointer, if you need further alignment then
     * you must handle it manually.
     *
     * @param originalAllocation a pointer to the original memory allocation which is to be
     *                           reallocated.
     * @param numberOfBytes how much memory to allocate.
     * @param thisAllocator the memory allocator.
     * @return a pointer to the newly allocated memory.
     */
    void* (* const realloc)(const void* originalAllocation,
                            size_t numberOfBytes,
                            const struct Allocator* thisAllocator,
                            const char* identFile,
                            int identLine);

    /**
     * Get a new child of this allocator.
     * When this allocator is freed all of its children will be freed as well.
     *
     * @param this the memory allocator, use Allocator_child(allocator) to get a child.
     * @param identFile the name of the file where this was called from to aid in debugging.
     * @param identLine the line number where this was called.
     * @return a child allocator.
     */
    struct Allocator* (* const child)(const struct Allocator* thisAlloc,
                                      const char* identFile,
                                      int identLine);
};

#define Allocator_clone(alloc, content) \
    alloc->clone(sizeof(*content), alloc, content, __FILE__, __LINE__)

#define Allocator_malloc(alloc, size) \
    alloc->malloc(size, alloc, __FILE__, __LINE__);

#define Allocator_calloc(alloc, size, count) \
    alloc->calloc(size, count, alloc, __FILE__, __LINE__)

#define Allocator_onFree(alloc, callback, context) \
    alloc->onFree(callback, context, alloc)

#define Allocator_notOnFree(alloc, job) \
    alloc->notOnFree(job, alloc)

#define Allocator_realloc(alloc, orig, size) \
    alloc->realloc(orig, size, alloc, __FILE__, __LINE__)

#define Allocator_child(alloc) \
    alloc->child(alloc, __FILE__, __LINE__)

#define Allocator_free(alloc) \
    alloc->free(alloc, __FILE__, __LINE__)

#endif /* MEMALLOCATOR_H */
