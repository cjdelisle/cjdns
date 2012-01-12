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
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "memory/MemAllocator.h"
#include "memory/MallocAllocator.h"

struct OnFreeJob;
struct OnFreeJob {
    void (* callback)(void* callbackContext);
    void* callbackContext;
    struct OnFreeJob* next;
};

struct Allocation;
struct Allocation {
    struct Allocation* next;
    size_t size;
};

/** Internal state for MemAllocator. */
struct Context;
struct Context
{
    /**
     * A linked list of the allocations made with this allocator.
     * These are all freed when the allocator is freed.
     */
    struct Allocation* allocations;

    /** A linked list of jobs which must be done when this allocator is freed. */
    struct OnFreeJob* onFree;

    /**
     * When this allocator is freed, lastSibling->nextSibling will be set to nextSibling
     * removing this allocator from the linked list.
     * GOTCHYA: if this is the first sibling, lastSibling will point to the parent and
     *          in that case, lastSibling->firstChild becomes nextSibling.
     */
    struct Context* lastSibling;

    /** A pointer to the next allocator which is a child of the same parent. */
    struct Context* nextSibling;

    /** The first child allocator, this will be freed when this allocator is freed. */
    struct Context* firstChild;

    /** The number of bytes which can be allocated by this allocator and all of it's family. */
    size_t* spaceAvailable;

    /** This allocator. */
    struct MemAllocator allocator;
};

/** The first ("genesis") allocator, not a child of any other allocator. */
struct FirstContext
{
    /** The context for the first allocator. */
    struct Context context;

    /** The actual storage location for the size limit. */
    size_t spaceAvailable;
};

static void failure(const char* message)
{
    fprintf(stderr, "Fatal error: %s\n", message);
    abort();
}

static inline void* newAllocation(struct Context* context, size_t size)
{
    size_t realSize = sizeof(struct Allocation) + size;
    if (*(context->spaceAvailable) <= realSize) {
        failure("Out of memory, limit exceeded.");
    }
    *(context->spaceAvailable) -= realSize;
    struct Allocation* alloc = malloc(realSize);
    if (alloc == NULL) {
        failure("Out of memory, malloc() returned NULL.");
    }
    alloc->next = context->allocations;
    alloc->size = realSize;
    context->allocations = alloc;
    return (void*) (alloc + 1);
}

/** @see MemAllocator->free() */
static void freeAllocator(const struct MemAllocator* allocator)
{
    struct Context* context = (struct Context*) allocator->context;

    // Free all of the child allocators.
    struct Context* child = context->firstChild;
    while (child != NULL) {
        struct Context* nextChild = child->nextSibling;
        freeAllocator(&child->allocator);
        child = nextChild;
    }

    // Do the onFree jobs.
    struct OnFreeJob* job = context->onFree;
    while (job != NULL) {
        job->callback(job->callbackContext);
        job = job->next;
    }

    // Remove this allocator from the sibling list.
    if (context->lastSibling->nextSibling == context) {
        context->lastSibling->nextSibling = context->nextSibling;
    } else if (context->lastSibling->firstChild == context) {
        context->lastSibling->firstChild = context->nextSibling;
    } else {
        failure("The last sibling of this allocator has no reference to it.");
    }
    if (context->nextSibling != NULL) {
        context->nextSibling->lastSibling = context->lastSibling;
    }

    // Free all of the allocations including the one which holds the allocator.
    struct Allocation* loc = context->allocations;
    while (loc != NULL) {
        *(context->spaceAvailable) += loc->size;
        struct Allocation* nextLoc = loc->next;
        free(loc);
        loc = nextLoc;
    }
}

/** @see MemAllocator->malloc() */
static void* allocatorMalloc(size_t length, const struct MemAllocator* allocator)
{
    return newAllocation(allocator->context, length);
}

/** @see MemAllocator->calloc() */
static void* allocatorCalloc(size_t length, size_t count, const struct MemAllocator* allocator)
{
    void* pointer = allocator->malloc(length * count, allocator);
    memset(pointer, 0, length * count);
    return pointer;
}

/** @see MemAllocator->clone() */
static void* allocatorClone(size_t length, const struct MemAllocator* allocator, const void* toClone)
{
    void* pointer = allocator->malloc(length, allocator);
    memcpy(pointer, toClone, length);
    return pointer;
}

/** @see MemAllocator->realloc() */
static void* allocatorRealloc(const void* original,
                              size_t size,
                              const struct MemAllocator* allocator)
{
    if (original == NULL) {
        return allocatorMalloc(size, allocator);
    }

    struct Context* context = allocator->context;
    struct Allocation** locPtr = &context->allocations;
    struct Allocation* origLoc = ((struct Allocation*) original) - 1;
    for (;;) {
        struct Allocation* loc = *locPtr;
        if (loc == NULL) {
            failure("Reallocation of memory which was not allocated using this allocator.");
        }
        if (loc == origLoc) {
            break;
        }
        locPtr = &loc->next;
    }

    struct Allocation* nextLoc = origLoc->next;

    size_t realSize = sizeof(struct Allocation) + size;
    if (*(context->spaceAvailable) + origLoc->size < realSize) {
        failure("Out of memory, limit exceeded.");
    }
    *(context->spaceAvailable) += origLoc->size;
    *(context->spaceAvailable) -= realSize;

    struct Allocation* alloc = realloc(origLoc, realSize);

    if (alloc == NULL) {
        failure("Out of memory, realloc() returned NULL.");
    }
    alloc->next = nextLoc;
    alloc->size = realSize;
    *locPtr = alloc;
    return (void*) (alloc + 1);
}

/** @see MemAllocator->child() */
static struct MemAllocator* childAllocator(const struct MemAllocator* allocator)
{
    struct Context* context = allocator->context;

    struct MemAllocator* childAlloc = MallocAllocator_new(0);
    struct Context* child = (struct Context*) childAlloc->context;
    child->lastSibling = context;
    child->nextSibling = context->firstChild;
    if (context->firstChild != NULL) {
        context->firstChild->lastSibling = child;
    }
    child->spaceAvailable = context->spaceAvailable;
    context->firstChild = child;

    return childAlloc;
}

/** @see MemAllocator->onFree() */
static void addOnFreeJob(void (* callback)(void* callbackContext),
                         void* callbackContext,
                         const struct MemAllocator* this)
{
    struct Context* context =
        (struct Context*) this->context;

    struct OnFreeJob* newJob =
        this->calloc(sizeof(struct OnFreeJob), 1, this);
    newJob->callback = callback;
    newJob->callbackContext = callbackContext;

    struct OnFreeJob* job = context->onFree;
    if (job == NULL) {
        context->onFree = newJob;
        return;
    }
    while (job->next != NULL) {
        job = job->next;
    }
    job->next = newJob;
}

/** @see MallocAllocator.h */
struct MemAllocator* MallocAllocator_new(size_t sizeLimit)
{
    struct FirstContext stackContext = {
        .spaceAvailable = (sizeLimit == 0) ? SIZE_MAX : sizeLimit
    };
    stackContext.context.spaceAvailable = &stackContext.spaceAvailable;

    struct FirstContext* firstContext =
        newAllocation(&stackContext.context, sizeof(struct FirstContext));
    memcpy(firstContext, &stackContext, sizeof(struct FirstContext));
    firstContext->context.spaceAvailable = &firstContext->spaceAvailable;
    struct Context* context = &firstContext->context;

    struct MemAllocator allocator = {
        .context = context,
        .free = freeAllocator,
        .malloc = allocatorMalloc,
        .calloc = allocatorCalloc,
        .clone = allocatorClone,
        .realloc = allocatorRealloc,
        .child = childAllocator,
        .onFree = addOnFreeJob
    };

    memcpy(&context->allocator, &allocator, sizeof(struct MemAllocator));
    return &context->allocator;
}
