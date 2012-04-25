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
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "memory/AllocatorTools.h"
#include "memory/BufferAllocator.h"

/**
 * TODO: addOnFreeJob adds a job which is only run when the root allocator is freed
 *       and it needs to be run when the allocator which called it, or any of that allocator's
 *       ancestors is freed, not just the root.
 */

/* Define alignment as the size of a pointer which is usually 4 or 8 bytes. */
#define ALIGNMENT sizeof(char*)

struct Job;
struct Job {
    void (* callback)(void* callbackContext);
    void* callbackContext;
    struct Job* next;
};

/** Internal state for Allocator. */
struct BufferAllocator_context {
    char* basePointer;
    char* pointer;
    char* const endPointer;
    struct Job* onFree;
    struct Job onOOM;
};


static void failure(const char* message)
{
    fprintf(stderr, "Fatal error: %s\n", message);
    abort();
}

static void oom(struct BufferAllocator_context* context)
{
    if (context->onOOM.callback) {
        context->onOOM.callback(context->onOOM.callbackContext);
    }
}

/** @see Allocator->malloc() */
static void* allocatorMalloc(size_t length, const struct Allocator* allocator)
{
    struct BufferAllocator_context* context =
        (struct BufferAllocator_context*) allocator->context;

    char* pointer = AllocatorTools_getAligned(context->pointer, ALIGNMENT);
    char* endOfAlloc = pointer + length;

    if (endOfAlloc >= context->endPointer) {
        /* out of bounds. */
        oom(context);
        failure("BufferAllocator ran out of memory.");
    }

    if (endOfAlloc < context->pointer) {
        /* integer overflow. */
        failure("BufferAllocator ran off the top end of the memory range.");
    }

    context->pointer = endOfAlloc;
    return (void*) pointer;
}

/** @see Allocator->calloc() */
static void* allocatorCalloc(size_t length, size_t count, const struct Allocator* allocator)
{
    void* pointer = allocator->malloc(length * count, allocator);
    memset(pointer, 0, length * count);
    return pointer;
}

/** @see Allocator->clone() */
static void* allocatorClone(size_t length, const struct Allocator* allocator, const void* toClone)
{
    void* pointer = allocator->malloc(length, allocator);
    memcpy(pointer, toClone, length);
    return pointer;
}

/** @see Allocator->realloc() */
static void* allocatorRealloc(const void* original, size_t length, const struct Allocator* allocator)
{
    if (original == NULL) {
        return allocatorMalloc(length, allocator);
    }

    // Need to pointer to make sure we dont copy too much.
    struct BufferAllocator_context* context =
        (struct BufferAllocator_context*) allocator->context;
    char* pointer = context->pointer;
    size_t amountToClone =
        length < (size_t)(pointer - (char*)original) ? length : (size_t)(pointer - (char*)original);

    // The likelyhood of nothing having been allocated since is almost 0 so we will always create a new
    // allocation and copy into it.
    void* newAlloc = allocator->malloc(length, allocator);
    memcpy(newAlloc, original, amountToClone);
    return newAlloc;
}

/** @see Allocator->free() */
static void freeAllocator(const struct Allocator* allocator)
{
    struct BufferAllocator_context* context =
        (struct BufferAllocator_context*) allocator->context;

    struct Job* job = context->onFree;
    while (job != NULL) {
        job->callback(job->callbackContext);
        job = job->next;
    }

    context->pointer = context->basePointer;
}

/** Fake freeAllocator so that child allocators don't get freed. */
static void doNothing(const struct Allocator* allocator)
{
}

/** @see Allocator->onFree() */
static void* addOnFreeJob(void (* callback)(void* callbackContext),
                          void* callbackContext,
                          const struct Allocator* this)
{
    struct BufferAllocator_context* context =
        (struct BufferAllocator_context*) this->context;

    struct Job* newJob =
        this->calloc(sizeof(struct Job), 1, this);
    newJob->callback = callback;
    newJob->callbackContext = callbackContext;

    struct Job* job = context->onFree;
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
    struct BufferAllocator_context* context = alloc->context;
    struct Job** jobPtr = &(context->onFree);
    while (*jobPtr != NULL) {
        if (*jobPtr == toRemove) {
            *jobPtr = (*jobPtr)->next;
            return true;
        }
        jobPtr = &(*jobPtr)->next;
    }
    return false;
}

/** @see Allocator->child() */
static struct Allocator* childAllocator(const struct Allocator* allocator)
{
    return allocator->clone(sizeof(struct Allocator), allocator, &(struct Allocator) {
        .context = allocator->context,
        .free = doNothing,
        .malloc = allocatorMalloc,
        .calloc = allocatorCalloc,
        .clone = allocatorClone,
        .realloc = allocatorRealloc,
        .child = childAllocator,
        .onFree = addOnFreeJob,
        .notOnFree = removeOnFreeJob
    });
}

/** @see BufferAllocator.h */
struct Allocator* BufferAllocator_new(void* buffer, size_t length)
{
    /* Write itself into it's own buffer :) */
    struct BufferAllocator_context context = {
        /* Align the pointer to do the first write manually. */
        .pointer = AllocatorTools_getAligned(buffer, sizeof(char*)),
        .basePointer = AllocatorTools_getAligned(buffer, sizeof(char*)),
        .endPointer = ((char*)buffer) + length
    };

    if (context.endPointer < context.pointer) {
        /* int64_t overflow. */
        return NULL;
    }

    if (length + (char*) buffer < context.pointer + sizeof(struct BufferAllocator_context)) {
        /* Not enough space to allocate the context. */
        return NULL;
    }

    /* put the context into the buffer. */
    memcpy(context.pointer, &context, sizeof(struct BufferAllocator_context));

    struct Allocator allocator = {
        .context = context.pointer,
        .free = freeAllocator,
        .malloc = allocatorMalloc,
        .calloc = allocatorCalloc,
        .clone = allocatorClone,
        .realloc = allocatorRealloc,
        .child = childAllocator,
        .onFree = addOnFreeJob
    };

    /* Get the pointer to the context where it's written in the buffer. */
    struct BufferAllocator_context* contextPtr = (struct BufferAllocator_context*) allocator.context;

    /* have to advance the pointer so as not to clobber the context. */
    contextPtr->pointer += sizeof(struct BufferAllocator_context);

    /* Now that the context is in place we can begin using the allocater. */
    struct Allocator* allocatorPtr = allocator.malloc(sizeof(struct Allocator), &allocator);
    if (allocatorPtr == NULL) {
        return NULL;
    }

    /* Reset the base pointer so as not to clobber the context on free() */
    contextPtr->basePointer = contextPtr->pointer;

    memcpy(allocatorPtr, &allocator, sizeof(struct Allocator));
    return allocatorPtr;
}

void BufferAllocator_onOOM(const struct Allocator* bufferAllocator,
                           void (* electronicThumb)(void* towel),
                           void* towel)
{
    struct BufferAllocator_context* context =
        (struct BufferAllocator_context*) bufferAllocator->context;
    context->onOOM.callback = electronicThumb;
    context->onOOM.callbackContext = towel;
}
