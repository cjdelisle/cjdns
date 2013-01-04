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
#define string_strrchr
#include "util/platform/libc/string.h"
#include <inttypes.h>
#include <stdio.h>

#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "memory/MallocAllocator_pvt.h"
#include "util/Bits.h"
#include "util/log/Log.h"

/** This provides the padding for each line based on the depth in the stack. */
struct Unroller;
struct Unroller
{
    const char* const content;
    const struct Unroller* const last;
};
static void writeUnroller(const struct Unroller* unroller)
{
    if (unroller) {
        writeUnroller(unroller->last);
        fprintf(stderr, "%s", unroller->content);
    }
}
static void unroll(struct MallocAllocator_pvt* context, struct Unroller* unroller)
{
    writeUnroller(unroller);
    const char* ident = (context->identFile) ? strrchr(context->identFile, '/') : " UNKNOWN";
    ident = ident ? ident + 1 : context->identFile;
    fprintf(stderr, "%s:%d  [%" PRIu64 "] bytes\n",
            ident,
            context->identLine,
            (uint64_t)context->allocatedHere);

    if (context->firstChild) {
        unroll(context->firstChild, &(struct Unroller) {
            .content = ((context->nextSibling) ? "| " : "  "),
            .last = unroller
        });
    }
    if (context->nextSibling) {
        unroll(context->nextSibling, unroller);
    }
}

Gcc_NORETURN
static void failure(struct MallocAllocator_pvt* context, const char* message)
{
    // get the root allocator.
    struct MallocAllocator_pvt* rootAlloc = context;
    while (rootAlloc->lastSibling) {
        rootAlloc = rootAlloc->lastSibling;
    }
    // can't use this allocator because it failed.
    unroll(rootAlloc, NULL);

    fprintf(stderr, "Fatal error: [%s] spaceAvailable [%" PRIu64 "]\n",
            message,
            *context->spaceAvailable);
    exit(0);
}

static inline void* newAllocation(struct MallocAllocator_pvt* context, size_t size)
{
    int64_t realSize = sizeof(struct MallocAllocator_Allocation) + size;
    if (*(context->spaceAvailable) <= realSize) {
        failure(context, "Out of memory, limit exceeded.");
    }

    *(context->spaceAvailable) -= realSize;
    context->allocatedHere += realSize;

    struct MallocAllocator_Allocation* alloc = malloc(realSize);
    if (alloc == NULL) {
        failure(context, "Out of memory, malloc() returned NULL.");
    }
    alloc->next = context->allocations;
    alloc->size = realSize;
    context->allocations = alloc;
    return (void*) (alloc + 1);
}

/** @see Allocator->free() */
static void freeAllocator(const struct Allocator* allocator)
{
    struct MallocAllocator_pvt* context = Identity_cast((struct MallocAllocator_pvt*) allocator);

    // Do the onFree jobs.
    struct MallocAllocator_OnFreeJob* job = context->onFree;
    while (job != NULL) {
        job->callback(job->callbackContext);
        job = job->next;
    }

    // Free all of the child allocators.
    struct MallocAllocator_pvt* child = context->firstChild;
    while (child != NULL) {
        struct MallocAllocator_pvt* nextChild = child->nextSibling;
        freeAllocator(&child->pub);
        child = nextChild;
    }

    // Remove this allocator from the sibling list.
    if (context->lastSibling != NULL &&
        context->lastSibling->nextSibling == context) {
        context->lastSibling->nextSibling = context->nextSibling;
    } else if (context->lastSibling != NULL &&
               context->lastSibling->firstChild == context) {
        context->lastSibling->firstChild = context->nextSibling;
    } else if (context->lastSibling != NULL) {
        failure(context, "The last sibling of this allocator has no reference to it.");
    }
    if (context->nextSibling != NULL) {
        context->nextSibling->lastSibling = context->lastSibling;
    }

    // Free all of the allocations including the one which holds the allocator.
    struct MallocAllocator_Allocation* loc = context->allocations;
    while (loc != NULL) {
        *(context->spaceAvailable) += loc->size;
        struct MallocAllocator_Allocation* nextLoc = loc->next;

        // TODO: make this optional.
        Bits_memset(loc, 0xff, loc->size);

        free(loc);
        loc = nextLoc;
    }
}

/** @see Allocator->malloc() */
static void* allocatorMalloc(size_t length, const struct Allocator* allocator)
{
    struct MallocAllocator_pvt* ctx = Identity_cast((struct MallocAllocator_pvt*) allocator);
    return newAllocation(ctx, length);
}

/** @see Allocator->calloc() */
static void* allocatorCalloc(size_t length, size_t count, const struct Allocator* allocator)
{
    void* pointer = allocatorMalloc(length * count, allocator);
    Bits_memset(pointer, 0, length * count);
    return pointer;
}

/** @see Allocator->clone() */
static void* allocatorClone(size_t length, const struct Allocator* allocator, const void* toClone)
{
    void* pointer = allocatorMalloc(length, allocator);
    Bits_memcpy(pointer, toClone, length);
    return pointer;
}

/** @see Allocator->realloc() */
static void* allocatorRealloc(const void* original,
                              size_t size,
                              const struct Allocator* allocator)
{
    if (original == NULL) {
        return allocatorMalloc(size, allocator);
    }

    struct MallocAllocator_pvt* context = Identity_cast((struct MallocAllocator_pvt*) allocator);
    struct MallocAllocator_Allocation** locPtr = &context->allocations;
    struct MallocAllocator_Allocation* origLoc =
        ((struct MallocAllocator_Allocation*) original) - 1;
    for (;;) {
        struct MallocAllocator_Allocation* loc = *locPtr;
        if (loc == NULL) {
            failure(context,
                    "Reallocation of memory which was not allocated using this allocator.");
        }
        if (loc == origLoc) {
            break;
        }
        locPtr = &loc->next;
    }

    struct MallocAllocator_Allocation* nextLoc = origLoc->next;

    size_t realSize = sizeof(struct MallocAllocator_Allocation) + size;
    if (*(context->spaceAvailable) + origLoc->size < realSize) {
        failure(context, "Out of memory, limit exceeded.");
    }
    *(context->spaceAvailable) += origLoc->size;
    *(context->spaceAvailable) -= realSize;
    context->allocatedHere -= origLoc->size;
    context->allocatedHere += realSize;

    struct MallocAllocator_Allocation* alloc = realloc(origLoc, realSize);

    if (alloc == NULL) {
        failure(context, "Out of memory, realloc() returned NULL.");
    }
    alloc->next = nextLoc;
    alloc->size = realSize;
    *locPtr = alloc;
    return (void*) (alloc + 1);
}

/** @see Allocator_child() */
static struct Allocator* childAllocator(const struct Allocator* allocator,
                                        const char* identFile,
                                        int identLine)
{
    struct MallocAllocator_pvt* context = Identity_cast((struct MallocAllocator_pvt*) allocator);

    uint32_t allocSize =
        sizeof(struct MallocAllocator_FirstCtx) + sizeof(struct MallocAllocator_Allocation);
    if (*(context->spaceAvailable) <= allocSize) {
        failure(context, "Out of memory, limit exceeded.");
    }

    struct Allocator* childAlloc = MallocAllocator_new(allocSize);

    *(context->spaceAvailable) -= allocSize;

    struct MallocAllocator_pvt* child = (struct MallocAllocator_pvt*) childAlloc;
    child->maxSpace = context->maxSpace;
    child->lastSibling = context;
    child->nextSibling = context->firstChild;
    if (context->firstChild != NULL) {
        context->firstChild->lastSibling = child;
    }
    child->spaceAvailable = context->spaceAvailable;
    child->identFile = identFile;
    child->identLine = identLine;
    context->firstChild = child;

    return childAlloc;
}

/** @see Allocator->onFree() */
static void* addOnFreeJob(void (* callback)(void* callbackContext),
                          void* callbackContext,
                          const struct Allocator* allocator)
{
    struct MallocAllocator_pvt* context = Identity_cast((struct MallocAllocator_pvt*) allocator);

    struct MallocAllocator_OnFreeJob* newJob =
        allocatorCalloc(sizeof(struct MallocAllocator_OnFreeJob), 1, allocator);
    newJob->callback = callback;
    newJob->callbackContext = callbackContext;

    struct MallocAllocator_OnFreeJob* job = context->onFree;
    if (job == NULL) {
        context->onFree = newJob;
    } else {
        while (job->next != NULL) {
            job = job->next;
        }
        job->next = newJob;
    }
    return newJob;
}

static bool removeOnFreeJob(void* toRemove, struct Allocator* alloc)
{
    struct MallocAllocator_pvt* context = Identity_cast((struct MallocAllocator_pvt*) alloc);
    struct MallocAllocator_OnFreeJob** jobPtr = &(context->onFree);
    while (*jobPtr != NULL) {
        if (*jobPtr == toRemove) {
            *jobPtr = (*jobPtr)->next;
            return true;
        }
        jobPtr = &(*jobPtr)->next;
    }
    return false;
}

/** @see MallocAllocator.h */
struct Allocator* MallocAllocator_newWithIdentity(size_t sizeLimit,
                                                  const char* identFile,
                                                  int identLine)
{
    struct MallocAllocator_FirstCtx stackContext = {
        .spaceAvailable = (sizeLimit == 0) ? SIZE_MAX : sizeLimit +
            sizeof(struct MallocAllocator_FirstCtx) + sizeof(struct Allocator),
        .context = {
            .identFile = identFile,
            .identLine = identLine
        }
    };
    stackContext.context.spaceAvailable = &stackContext.spaceAvailable;

    struct MallocAllocator_FirstCtx* firstContext =
        newAllocation(&stackContext.context, sizeof(struct MallocAllocator_FirstCtx));
    Bits_memcpyConst(firstContext, &stackContext, sizeof(struct MallocAllocator_FirstCtx));
    struct MallocAllocator_pvt* context = &firstContext->context;
    context->spaceAvailable = &firstContext->spaceAvailable;
    context->maxSpace = firstContext->spaceAvailable;

    struct Allocator allocator = {
        .context = context,
        .free = freeAllocator,
        .malloc = allocatorMalloc,
        .calloc = allocatorCalloc,
        .clone = allocatorClone,
        .realloc = allocatorRealloc,
        .child = childAllocator,
        .onFree = addOnFreeJob,
        .notOnFree = removeOnFreeJob
    };

    Bits_memcpyConst(&context->pub, &allocator, sizeof(struct Allocator));
    Identity_set(context);
    return &context->pub;
}

size_t MallocAllocator_bytesAllocated(struct Allocator* allocator)
{
    struct MallocAllocator_pvt* context = Identity_cast((struct MallocAllocator_pvt*) allocator);
    return context->maxSpace - *context->spaceAvailable;
}
