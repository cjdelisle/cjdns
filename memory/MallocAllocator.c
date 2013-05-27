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

#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "memory/MallocAllocator_pvt.h"
#include "util/Bits.h"
#include "util/log/Log.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
    fprintf(stderr, "%s:%d  [%lu] bytes\n",
            ident,
            context->identLine,
            context->allocatedHere);

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
static void failure(struct MallocAllocator_pvt* context,
                    const char* message,
                    const char* identFile,
                    int identLine)
{
    // get the root allocator.
    struct MallocAllocator_pvt* rootAlloc = context;
    while (rootAlloc->lastSibling && rootAlloc->lastSibling != rootAlloc) {
        rootAlloc = rootAlloc->lastSibling;
    }
    // can't use this allocator because it failed.
    unroll(rootAlloc, NULL);

    Assert_failure("%s:%d Fatal error: [%s] spaceAvailable [%lu]",
                   identFile, identLine, message,
                   (unsigned long)context->rootAlloc->spaceAvailable);
}

static inline unsigned long getRealSize(unsigned long requestedSize)
{
    return ((requestedSize + (sizeof(char*) - 1)) & ~(sizeof(char*) - 1)) // align
        + sizeof(struct MallocAllocator_Allocation)
    #ifdef MallocAllocator_USE_CANARIES
        + sizeof(long)
    #endif
    ;
}

#define END_CANARY(alloc) ((long*) alloc)[ (alloc->size / sizeof(long)) - 1 ]

static inline void setCanaries(struct MallocAllocator_Allocation* alloc,
                               struct MallocAllocator_pvt* context)
{
    #ifdef MallocAllocator_USE_CANARIES
        alloc->beginCanary = context->canary;
        END_CANARY(alloc) = alloc->beginCanary;
    #endif
}

static inline void checkCanaries(struct MallocAllocator_Allocation* alloc,
                                 struct MallocAllocator_pvt* context)
{
    #ifdef MallocAllocator_USE_CANARIES
        Assert_always(alloc->beginCanary == context->canary
            && END_CANARY(alloc) == alloc->beginCanary);
    #endif
}

static inline void* newAllocation(struct MallocAllocator_pvt* context,
                                  unsigned long size,
                                  const char* identFile,
                                  int identLine)
{
    int64_t realSize = getRealSize(size);
    if (context->rootAlloc->spaceAvailable <= realSize) {
        failure(context, "Out of memory, limit exceeded.", identFile, identLine);
    }

    context->rootAlloc->spaceAvailable -= realSize;
    context->allocatedHere += realSize;

    struct MallocAllocator_Allocation* alloc = malloc(realSize);
    if (alloc == NULL) {
        failure(context, "Out of memory, malloc() returned NULL.", identFile, identLine);
    }
    alloc->next = context->allocations;
    alloc->size = realSize;
    context->allocations = alloc;
    setCanaries(alloc, context);

    return (void*) (alloc + 1);
}

static void disconnectAllocator(struct Allocator* allocator, const char* file, int line);

/**
 * Triggered when freeAllocator() is called and the allocator nolonger
 * has any remaining links to the allocator tree.
 */
static void realFreeAllocator(struct MallocAllocator_pvt* context, const char* file, int line)
{
    Assert_true(!context->lastSibling);
    Assert_true(!context->adoptions || !context->adoptions->parents);

    // Do the onFree jobs.
    struct MallocAllocator_OnFreeJob* job = context->onFree;
    while (job != NULL) {
        job->callback(job->callbackContext);
        job = job->next;
    }

    // Disconnect all of the child allocators.
    struct MallocAllocator_pvt* child = context->firstChild;
    while (child) {
        struct MallocAllocator_pvt* nextChild = child->nextSibling;
        disconnectAllocator(&child->pub, file, line);
        child = nextChild;
    }

    // Disconnect (and free if necessary) the adopted children.

    struct MallocAllocator_List* childL = context->adoptions ? context->adoptions->children : NULL;
    while (childL) {
        struct MallocAllocator_List** cpp = &childL->alloc->adoptions->parents;
        struct MallocAllocator_List* cp = *cpp;
        int found = 0;
        while ((cp = *cpp)) {
            if (cp->alloc == context) {
                *cpp = cp->next;
                found = 1;
                break;
            }
            cpp = &cp->next;
        }
        Assert_true(found);

        if (!childL->alloc->adoptions->parents && !childL->alloc->lastSibling) {
            // This child now has no remaining links to the tree.
            realFreeAllocator(childL->alloc, file, line);
        }
        childL = childL->next;
    }

    // Free all of the allocations including the one which holds the allocator.
    struct MallocAllocator_Allocation* loc = context->allocations;
    while (loc != NULL) {
        context->rootAlloc->spaceAvailable += loc->size;
        struct MallocAllocator_Allocation* nextLoc = loc->next;

        checkCanaries(loc, context);

        // TODO: make this optional.
        Bits_memset(loc, 0xee, loc->size);
        free(loc);
        loc = nextLoc;
    }
}

/**
 * Disconnect an allocator from it's parent and free it if there are no more links to the tree.
 */
static void disconnectAllocator(struct Allocator* allocator, const char* file, int line)
{
    struct MallocAllocator_pvt* context = Identity_cast((struct MallocAllocator_pvt*) allocator);

    // Remove this allocator from the sibling list.
    if (context->lastSibling != NULL && context->lastSibling->nextSibling == context) {
        context->lastSibling->nextSibling = context->nextSibling;

    } else if (context->lastSibling != NULL && context->lastSibling->firstChild == context) {
        context->lastSibling->firstChild = context->nextSibling;

    } else if (context->lastSibling == context) {
        // root alloc
        Assert_always(!context->nextSibling);

    } else if (context->lastSibling != NULL) {
        failure(context, "The last sibling of this allocator has no reference to it.", file, line);
    }
    if (context->nextSibling != NULL) {
        context->nextSibling->lastSibling = context->lastSibling;
    }
    context->lastSibling = NULL;

    if (context->adoptions && context->adoptions->parents) {
        return;
    }

    realFreeAllocator(context, file, line);
}

/** @see Allocator->malloc() */
static void* allocatorMalloc(unsigned long length,
                             struct Allocator* allocator,
                             const char* identFile,
                             int identLine)
{
    struct MallocAllocator_pvt* ctx = Identity_cast((struct MallocAllocator_pvt*) allocator);
    return newAllocation(ctx, length, identFile, identLine);
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
                              unsigned long size,
                              struct Allocator* allocator,
                              const char* identFile,
                              int identLine)
{
    if (original == NULL) {
        return allocatorMalloc(size, allocator, identFile, identLine);
    }

    struct MallocAllocator_pvt* context = Identity_cast((struct MallocAllocator_pvt*) allocator);
    struct MallocAllocator_Allocation** locPtr = &context->allocations;
    struct MallocAllocator_Allocation* origLoc =
        ((struct MallocAllocator_Allocation*) original) - 1;
    for (;;) {
        struct MallocAllocator_Allocation* loc = *locPtr;
        if (loc == NULL) {
            failure(context,
                    "Reallocation of memory which was not allocated using this allocator.",
                    identFile,
                    identLine);
        }
        checkCanaries(loc, context);
        if (loc == origLoc) {
            break;
        }
        locPtr = &loc->next;
    }

    struct MallocAllocator_Allocation* nextLoc = origLoc->next;

    size_t realSize = getRealSize(size);
    if (context->rootAlloc->spaceAvailable + origLoc->size < realSize) {
        failure(context, "Out of memory, limit exceeded.", identFile, identLine);
    }
    context->rootAlloc->spaceAvailable += origLoc->size;
    context->rootAlloc->spaceAvailable -= realSize;
    context->allocatedHere -= origLoc->size;
    context->allocatedHere += realSize;

    struct MallocAllocator_Allocation* alloc = realloc(origLoc, realSize);

    if (alloc == NULL) {
        failure(context, "Out of memory, realloc() returned NULL.", identFile, identLine);
    }
    alloc->next = nextLoc;
    alloc->size = realSize;
    *locPtr = alloc;

    setCanaries(alloc, context);

    return (void*) (alloc + 1);
}

/** @see Allocator_child() */
static struct Allocator* childAllocator(struct Allocator* allocator, const char* file, int line)
{
    struct MallocAllocator_pvt* parent = Identity_cast((struct MallocAllocator_pvt*) allocator);

    struct MallocAllocator_pvt* child =
        newAllocation(parent, sizeof(struct MallocAllocator_pvt), file, line);

    Bits_memset(child, 0, sizeof(struct MallocAllocator_pvt));
    Bits_memcpyConst(&child->pub, &parent->pub, sizeof(struct Allocator));
    Identity_set(child);

    // Add the child to it's own allocation list.
    child->allocations = parent->allocations;

    // Remove the child from the parent's allocation list.
    parent->allocations = parent->allocations->next;

    // Drop the rest of the linked list from the child's allocation
    child->allocations->next = NULL;

    // Link the child into the parent's allocator list
    child->lastSibling = parent;
    child->nextSibling = parent->firstChild;
    if (parent->firstChild != NULL) {
        parent->firstChild->lastSibling = child;
    }
    child->rootAlloc = parent->rootAlloc;
    child->identFile = file;
    child->identLine = line;
    child->nextCanary = child->canary = parent->nextCanary;
    parent->firstChild = child;

    child->rootAlloc = parent->rootAlloc;

    // Now set the child to use it's own canaries so it will free properly.
    setCanaries(child->allocations, child);

    return &child->pub;
}

static int removeOnFreeJob(struct Allocator_OnFreeJob* toRemove)
{
    struct MallocAllocator_OnFreeJob* job = (struct MallocAllocator_OnFreeJob*) toRemove;
    struct MallocAllocator_pvt* context = Identity_cast(job->alloc);
    struct MallocAllocator_OnFreeJob** jobPtr = &(context->onFree);
    while (*jobPtr != NULL) {
        if (*jobPtr == job) {
            *jobPtr = (*jobPtr)->next;
            return 0;
        }
        jobPtr = &(*jobPtr)->next;
    }
    return -1;
}

static struct MallocAllocator_pvt* getParent(struct MallocAllocator_pvt* child)
{
    struct MallocAllocator_pvt* ls = child->lastSibling;
    while (ls) {
        if (ls->firstChild == child) {
           return ls;
        }
        if (ls == ls->lastSibling) {
            // root alloc
            return NULL;
        }
        ls = ls->lastSibling;
    }
    Assert_true(0);
}

static bool isAncestorOf(struct MallocAllocator_pvt* maybeParent,
                         struct MallocAllocator_pvt* maybeChild)
{
    if (maybeParent == maybeChild) {
        return true;
    }
    if (maybeParent == NULL || maybeChild == NULL) {
        return false;
    }
    if (isAncestorOf(maybeParent, getParent(maybeChild))) {
        return true;
    }
    if (maybeChild->adoptions) {
        struct MallocAllocator_List* al = maybeChild->adoptions->parents;
        while (al) {
            if (isAncestorOf(maybeParent, al->alloc)) {
                return true;
            }
        }
    }
    return false;
}

static void adopt(struct Allocator* adoptedParent,
                  struct Allocator* childToAdopt,
                  const char* file,
                  int line)
{
    struct MallocAllocator_pvt* parent = Identity_cast((struct MallocAllocator_pvt*) adoptedParent);
    struct MallocAllocator_pvt* child = Identity_cast((struct MallocAllocator_pvt*) childToAdopt);

    if (isAncestorOf(child, parent)) {
        failure(parent, "Adoption would create cyclical reference.", file, line);
    }

    if (!parent->adoptions) {
        parent->adoptions =
            allocatorCalloc(sizeof(struct MallocAllocator_Adoptions), 1, adoptedParent, file, line);
    }
    if (!child->adoptions) {
        child->adoptions =
            allocatorCalloc(sizeof(struct MallocAllocator_Adoptions), 1, childToAdopt, file, line);
    }

    struct MallocAllocator_List* pl =
        allocatorCalloc(sizeof(struct MallocAllocator_List), 1, adoptedParent, file, line);
    pl->alloc = child;
    pl->next = parent->adoptions->children;
    parent->adoptions->children = pl;

    struct MallocAllocator_List* cl =
        allocatorCalloc(sizeof(struct MallocAllocator_List), 1, childToAdopt, file, line);
    cl->alloc = parent;
    cl->next = child->adoptions->parents;
    child->adoptions->parents = cl;
}

/** @see Allocator->onFree() */
static struct Allocator_OnFreeJob* addOnFreeJob(void (* callback)(void* callbackContext),
                                                void* callbackContext,
                                                struct Allocator* allocator)
{
    struct MallocAllocator_pvt* context = Identity_cast((struct MallocAllocator_pvt*) allocator);

    struct MallocAllocator_OnFreeJob* newJob =
        Allocator_clone(allocator, (&(struct MallocAllocator_OnFreeJob) {
            .generic = {
                .cancel = removeOnFreeJob
            },
            .callback = callback,
            .callbackContext = callbackContext,
            .alloc = context
        }));

    struct MallocAllocator_OnFreeJob* job = context->onFree;
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

/** @see MallocAllocator.h */
struct Allocator* MallocAllocator_newWithIdentity(unsigned long sizeLimit,
                                                  const char* identFile,
                                                  int identLine)
{
    // Add in the size of the allocator so that a very small sizeLimit is sane.
    sizeLimit += getRealSize(sizeof(struct MallocAllocator_FirstCtx));

    struct MallocAllocator_FirstCtx stackContext = {
        .spaceAvailable = (sizeLimit == 0) ? SIZE_MAX : sizeLimit,
        .context = {
            .identFile = identFile,
            .identLine = identLine,
            .canary = (long) 0x09F911029D74E35Bll,
            .nextCanary = (long) 0xD84156C5635688C0ll,
        }
    };
    stackContext.maxSpace = stackContext.spaceAvailable;
    stackContext.context.rootAlloc = &stackContext;
    Identity_set(&stackContext.context);

    struct MallocAllocator_FirstCtx* firstContext =
        allocatorClone(sizeof(struct MallocAllocator_FirstCtx),
                       &stackContext.context.pub,
                       &stackContext,
                       identFile,
                       identLine);

    struct MallocAllocator_pvt* context = &firstContext->context;
    context->rootAlloc = firstContext;

    struct Allocator allocator = {
        .free = disconnectAllocator,
        .malloc = allocatorMalloc,
        .calloc = allocatorCalloc,
        .clone = allocatorClone,
        .realloc = allocatorRealloc,
        .child = childAllocator,
        .onFree = addOnFreeJob,
        .adopt = adopt
    };

    // Remove the size of the allocator so it doesn't appear to be wrong.
    firstContext->maxSpace -= getRealSize(sizeof(struct MallocAllocator_FirstCtx));

    Bits_memcpyConst(&context->pub, &allocator, sizeof(struct Allocator));

    context->lastSibling = context;

    Identity_set(context);
    return &context->pub;
}

unsigned long MallocAllocator_bytesAllocated(struct Allocator* allocator)
{
    struct MallocAllocator_pvt* context = Identity_cast((struct MallocAllocator_pvt*) allocator);
    return context->rootAlloc->maxSpace - context->rootAlloc->spaceAvailable;
}

void MallocAllocator_setCanary(struct Allocator* alloc, long value)
{
    #ifdef MallocAllocator_USE_CANARIES
        struct MallocAllocator_pvt* context = Identity_cast((struct MallocAllocator_pvt*) alloc);
        context->nextCanary ^= value;
    #endif
}
