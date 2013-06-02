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
#include <stdint.h>

#include "exception/Except.h"
#include "memory/BufferAllocator.h"
#include "util/Bits.h"
#include "util/Identity.h"

/**
 * TODO: addOnFreeJob adds a job which is only run when the root allocator is freed
 *       and it needs to be run when the allocator which called it, or any of that allocator's
 *       ancestors is freed, not just the root.
 */

/* Define alignment as the size of a pointer which is usually 4 or 8 bytes. */
#define ALIGNMENT sizeof(char*)

struct Job {
    struct Allocator_OnFreeJob generic;
    struct Allocator* alloc;
    struct Job* next;
    Identity
};

/** Internal state for Allocator. */
struct BufferAllocator {
    struct Allocator generic;

    /** Pointer to the beginning of the buffer. */
    char* basePointer;

    /** Pointer to a pointer to the place in the buffer to allocate the next block of memory. */
    char** pPointer;

    /** Pointer to the end of the buffer. */
    char* const endPointer;

    struct Job* onFree;
    /** Number of onfree jobs which are not yet complete. */
    int outstandingJobs;
    struct Except* onOOM;
    const char* file;
    int line;
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
static void* allocatorMalloc(unsigned long length,
                             struct Allocator* allocator,
                             const char* identFile,
                             int identLine)
{
    struct BufferAllocator* context = Identity_cast((struct BufferAllocator*) allocator);

    char* pointer = getAligned((*context->pPointer), ALIGNMENT);
    char* endOfAlloc = pointer + length;

    if (endOfAlloc >= context->endPointer) {
        Except_raise(context->onOOM, -1, "BufferAllocator ran out of memory [%s:%d]",
                     identFile, identLine);
    }

    if (endOfAlloc < *(context->pPointer)) {
        Except_raise(context->onOOM, -2, "BufferAllocator integer overflow [%s:%d]",
                     identFile, identLine);
    }

    (*context->pPointer) = endOfAlloc;
    return (void*) pointer;
}

/** @see Allocator->calloc() */
static void* allocatorCalloc(unsigned long length,
                             unsigned long count,
                             struct Allocator* allocator,
                             const char* identFile,
                             int identLine)
{
    void* pointer = allocatorMalloc(length * count, allocator, identFile, identLine);
    Bits_memset(pointer, 0, length * count);
    return pointer;
}

/** @see Allocator->clone() */
static void* allocatorClone(unsigned long length,
                            struct Allocator* allocator,
                            const void* toClone,
                            const char* identFile,
                            int identLine)
{
    void* pointer = allocatorMalloc(length, allocator, identFile, identLine);
    Bits_memcpy(pointer, toClone, length);
    return pointer;
}

/** @see Allocator->realloc() */
static void* allocatorRealloc(const void* original,
                              unsigned long length,
                              struct Allocator* allocator,
                              const char* identFile,
                              int identLine)
{
    if (original == NULL) {
        return allocatorMalloc(length, allocator, identFile, identLine);
    }

    // Need to pointer to make sure we dont copy too much.
    struct BufferAllocator* context = Identity_cast((struct BufferAllocator*) allocator);
    char* pointer = *context->pPointer;
    uint32_t amountToClone = (length < (uint32_t)(pointer - (char*)original))
        ? length
        : (uint32_t)(pointer - (char*)original);

    // The likelyhood of nothing having been allocated since is
    // almost 0 so we will always create a new
    // allocation and copy into it.
    void* newAlloc = allocatorMalloc(length, allocator, identFile, identLine);
    Bits_memcpy(newAlloc, original, amountToClone);
    return newAlloc;
}

/** @see Allocator->free() */
static void freeAllocator(struct Allocator* allocator, const char* identFile, int identLine)
{
    struct BufferAllocator* context = Identity_cast((struct BufferAllocator*) allocator);
    struct Job* job = context->onFree;
    while (job != NULL) {
        if (job->generic.callback) {
            job->generic.callback(&job->generic);
            context->outstandingJobs++;
        }
        job = job->next;
    }
}

static int removeOnFreeJob(struct Allocator_OnFreeJob* toRemove)
{
    struct Job* j = Identity_cast((struct Job*) toRemove);
    struct BufferAllocator* context = Identity_cast((struct BufferAllocator*) j->alloc);
    struct Job** jobPtr = &(context->onFree);
    while (*jobPtr != NULL) {
        if (*jobPtr == j) {
            *jobPtr = (*jobPtr)->next;
            return 0;
        }
        jobPtr = &(*jobPtr)->next;
    }
    return -1;
}

static int onFreeComplete(struct Allocator_OnFreeJob* job)
{
    struct BufferAllocator* context =
        Identity_cast((struct BufferAllocator*) ((struct Job*)job)->alloc);

    if (!--context->outstandingJobs) {
        if ((uintptr_t) context > (uintptr_t) context->pPointer) {
            // pPointer points to a destination which is > context unless this is a child alloc.
            return 0;
        }
        // complete
        (*context->pPointer) = context->basePointer;
    }
    return 0;
}

/** @see Allocator->onFree() */
static struct Allocator_OnFreeJob* onFree(struct Allocator* alloc,
                                          const char* file,
                                          int line)
{
    struct BufferAllocator* context = Identity_cast((struct BufferAllocator*) alloc);

    struct Job* newJob = Allocator_clone(alloc, (&(struct Job) {
        .generic = {
            .cancel = removeOnFreeJob,
            .complete = onFreeComplete
        },
        .alloc = alloc,
    }));
    Identity_set(&newJob->generic);

    struct Job* job = context->onFree;
    if (job == NULL) {
        context->onFree = newJob;

    } else {
        while (job->next != NULL) {
            job = job->next;
        }
        job->next = newJob;
    }
    return &newJob->generic;
}

/** @see Allocator_child() */
static struct Allocator* childAllocator(struct Allocator* alloc,
                                        const char* file,
                                        int line)
{
    struct BufferAllocator* context = Identity_cast((struct BufferAllocator*) alloc);
    struct BufferAllocator* child =
        allocatorClone(sizeof(struct BufferAllocator), alloc, context, file, line);
    child->file = file;
    child->line = line;
    return &child->generic;
}

static void adopt(struct Allocator* alloc, struct Allocator* allocB, const char* file, int line)
{
    Assert_always(!"Unimplemented");
}

/** @see BufferAllocator.h */
struct Allocator* BufferAllocator_newWithIdentity(void* buffer,
                                                  unsigned long length,
                                                  char* file,
                                                  int line)
{
    struct FirstAlloc {
        struct BufferAllocator alloc;
        char* pointer;
    };

    struct FirstAlloc tempAlloc = {
        .alloc = {
            .generic = {
                .free = freeAllocator,
                .malloc = allocatorMalloc,
                .calloc = allocatorCalloc,
                .clone = allocatorClone,
                .realloc = allocatorRealloc,
                .child = childAllocator,
                .onFree = onFree,
                .adopt = adopt
            },

            // Align the pointer to do the first write manually.
            .pPointer = NULL,
            .basePointer = getAligned(buffer, sizeof(char*)),
            .endPointer = ((char*)buffer) + length,
            .file = file,
            .line = line
        },
        .pointer = getAligned(buffer, sizeof(char*))
    };
    tempAlloc.alloc.pPointer = &tempAlloc.pointer;
    Identity_set(&tempAlloc.alloc);

    if (tempAlloc.alloc.endPointer < (*tempAlloc.alloc.pPointer)) {
        // int64_t overflow.
        return NULL;
    }

    if (length + (char*) buffer < (*tempAlloc.alloc.pPointer) + sizeof(struct BufferAllocator)) {
        // Not enough space to allocate the context.
        return NULL;
    }

    struct FirstAlloc* alloc = (struct FirstAlloc*) (*tempAlloc.alloc.pPointer);
    Bits_memcpyConst(alloc, &tempAlloc, sizeof(struct FirstAlloc));
    alloc->pointer += sizeof(struct FirstAlloc);
    alloc->alloc.pPointer = &alloc->pointer;

    return &alloc->alloc.generic;
}

void BufferAllocator_onOOM(struct Allocator* alloc,
                           struct Except* exceptionHandler)
{
    struct BufferAllocator* context = Identity_cast((struct BufferAllocator*) alloc);
    context->onOOM = exceptionHandler;
}
