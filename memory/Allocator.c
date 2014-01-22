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
#include "memory/Allocator_pvt.h"
#include "util/Bits.h"

#include <stdio.h>

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
static void unroll(struct Allocator_pvt* context, struct Unroller* unroller)
{
    writeUnroller(unroller);
    const char* ident = (context->pub.fileName) ? strrchr(context->pub.fileName, '/') : "UNKNOWN";
    ident = ident ? ident : context->pub.fileName;

    fprintf(stderr, "%s:%d [%lu] bytes%s\n",
            ident,
            context->pub.lineNum,
            context->allocatedHere,
            (context->pub.isFreeing) ? " (freeing)" : "");

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
static void failure(struct Allocator_pvt* context,
                    const char* message,
                    const char* fileName,
                    int lineNum)
{
    // get the root allocator.
    struct Allocator_pvt* rootAlloc = context;
    while (rootAlloc->lastSibling && rootAlloc->lastSibling != rootAlloc) {
        rootAlloc = rootAlloc->lastSibling;
    }
    // can't use this allocator because it failed.
    unroll(rootAlloc, NULL);

    Assert_failure("%s:%d Fatal error: [%s] totalBytes [%ld] remaining [%ld]",
                   fileName, lineNum, message,
                   (long)context->rootAlloc->maxSpace,
                   (long)context->rootAlloc->spaceAvailable);
}

static inline unsigned long getRealSize(unsigned long requestedSize)
{
    return ((requestedSize + (sizeof(char*) - 1)) & ~(sizeof(char*) - 1)) // align
        + sizeof(struct Allocator_Allocation_pvt)
    #ifdef Allocator_USE_CANARIES
        + sizeof(unsigned long)
    #endif
    ;
}

#define END_CANARY(alloc) ((unsigned long*) alloc)[ (alloc->pub.size / sizeof(unsigned long)) - 1 ]

static inline void setCanaries(struct Allocator_Allocation_pvt* alloc,
                               struct Allocator_pvt* context)
{
    #ifdef Allocator_USE_CANARIES
        END_CANARY(alloc) = alloc->beginCanary = context->canary;
    #endif
}

static inline void checkCanaries(struct Allocator_Allocation_pvt* alloc,
                                 struct Allocator_pvt* context)
{
    #ifdef Allocator_USE_CANARIES
        char* canary;
        if (alloc->beginCanary != context->canary) {
            canary = "begin";
        } else if (END_CANARY(alloc) != alloc->beginCanary) {
            canary = "end";
        } else {
            return;
        }
        Assert_failure("%s:%d Fatal error: invalid [%s] canary\n",
                       context->pub.fileName, context->pub.lineNum, canary);
    #endif
}

static inline void* newAllocation(struct Allocator_pvt* context,
                                  unsigned long size,
                                  const char* fileName,
                                  int lineNum)
{
    int64_t realSize = getRealSize(size);
    if (context->rootAlloc->spaceAvailable <= realSize) {
        failure(context, "Out of memory, limit exceeded", fileName, lineNum);
    }

    context->rootAlloc->spaceAvailable -= realSize;
    context->allocatedHere += realSize;

    struct Allocator_Allocation_pvt* alloc =
        context->rootAlloc->provider(context->rootAlloc->providerContext,
                                     NULL,
                                     realSize,
                                     &context->pub);
    if (alloc == NULL) {
        failure(context, "Out of memory, malloc() returned NULL", fileName, lineNum);
    }
    alloc->next = context->allocations;
    alloc->pub.size = realSize;
    alloc->pub.fileName = fileName;
    alloc->pub.lineNum = lineNum;
    context->allocations = alloc;
    setCanaries(alloc, context);

    return (void*) (alloc + 1);
}

struct Allocator_Allocation* Allocator_getAllocation(struct Allocator* alloc, int allocNum)
{
    struct Allocator_pvt* ctx = Identity_check((struct Allocator_pvt*)alloc);
    if (allocNum < 0) {
        return NULL;
    }
    struct Allocator_Allocation_pvt* allocation = ctx->allocations;
    for (;allocation && allocNum > 0; allocNum--) {
        allocation = allocation->next;
    }
    return (allocation) ? &allocation->pub : NULL;
}

struct Allocator* Allocator_getChild(struct Allocator* alloc, int childNumber)
{
    struct Allocator_pvt* ctx = Identity_check((struct Allocator_pvt*)alloc);
    if (childNumber < 0) {
        return NULL;
    }
    struct Allocator_pvt* child = ctx->firstChild;
    for (;child && childNumber > 0; childNumber--) {
        child = child->nextSibling;
    }
    return (child) ? &child->pub : NULL;
}

static int removeJob(struct Allocator_OnFreeJob_pvt* job)
{
    struct Allocator_pvt* context = Identity_check(job->alloc);
    struct Allocator_OnFreeJob_pvt* j = context->onFree;
    struct Allocator_OnFreeJob_pvt** jP = &context->onFree;
    while (j && j != job) {
        jP = &j->next;
        j = j->next;
    }
    if (j == job) {
        *jP = j->next;
        return 0;
    } else {
        return -1;
        failure(context, "Allocator_onFreeComplete() called multiple times", job->file, job->line);
    }
}

static void releaseAllocation(struct Allocator_pvt* context,
                              struct Allocator_Allocation_pvt* allocation,
                              Allocator_Provider provider,
                              Allocator_Provider_CONTEXT_TYPE* providerCtx)
{
    checkCanaries(allocation, context);

    // TODO: make this optional.
    Bits_memset(&(&allocation->pub)[1],
                0xee,
                allocation->pub.size - sizeof(struct Allocator_Allocation));

    provider(providerCtx,
             &allocation->pub,
             0,
             ((char*)context != (char*)allocation) ? &context->pub : NULL);
}

static void releaseMemory(struct Allocator_pvt* context,
                          Allocator_Provider provider,
                          Allocator_Provider_CONTEXT_TYPE* providerCtx)
{
    // Free all of the allocations including the one which holds the allocator.
    #ifdef PARANOIA
        unsigned long allocatedHere = context->allocatedHere;
    #endif

    context->rootAlloc->spaceAvailable += context->allocatedHere;

    struct Allocator_Allocation_pvt* loc = context->allocations;
    while (loc != NULL) {
        #ifdef PARANOIA
            allocatedHere -= loc->pub.size;
        #endif

        struct Allocator_Allocation_pvt* nextLoc = loc->next;
        releaseAllocation(context, loc, provider, providerCtx);
        loc = nextLoc;
    }
    #ifdef PARANOIA
        Assert_always(allocatedHere == 0);
    #endif
}

// disconnect an allocator from it's parent.
static void disconnect(struct Allocator_pvt* context)
{
    // Remove this allocator from the sibling list.
    Assert_true(context->lastSibling);

    if (context->lastSibling->nextSibling == context) {
        context->lastSibling->nextSibling = context->nextSibling;

    } else if (context->lastSibling->firstChild == context) {
        context->lastSibling->firstChild = context->nextSibling;

    } else if (context->lastSibling == context) {
        // root alloc
        Assert_true(!context->nextSibling);
    }

    if (context->nextSibling) {
        context->nextSibling->lastSibling = context->lastSibling;
        context->nextSibling = NULL;
    }

    context->lastSibling = context;
}

// connect an allocator to a new parent.
static void connect(struct Allocator_pvt* parent,
                    struct Allocator_pvt* child,
                    const char* file,
                    int line)
{
    Assert_true(child->lastSibling == child);
    Assert_true(child->nextSibling == NULL);
    child->nextSibling = parent->firstChild;
    parent->firstChild = child;
    child->lastSibling = parent;
}

static struct Allocator_pvt* getParent(struct Allocator_pvt* child)
{
    struct Allocator_pvt* ls = child->lastSibling;
    while (ls) {
        if (ls->firstChild == child) {
           return ls;
        }
        if (ls == ls->lastSibling) {
            // root alloc
            return NULL;
        }
        child = ls;
        ls = ls->lastSibling;
    }
    Assert_always(0);
}

static void freeAllocator(struct Allocator_pvt* context, const char* file, int line);

static void childFreed(struct Allocator_pvt* child)
{
    struct Allocator_pvt* parent = getParent(child);
    // disconnect the child and if there are no children left then call freeAllocator()
    // on the parent a second time.
    disconnect(child);
    if (parent && !parent->firstChild && parent->pub.isFreeing) {
        freeAllocator(parent, child->pub.fileName, child->pub.lineNum);
    }
}

void Allocator_onFreeComplete(struct Allocator_OnFreeJob* onFreeJob)
{
    struct Allocator_OnFreeJob_pvt* job = (struct Allocator_OnFreeJob_pvt*) onFreeJob;
    struct Allocator_pvt* context = Identity_check(job->alloc);

    if (removeJob(job)) {
        failure(context, "OnFreeJob->complete() called multiple times", job->file, job->line);
    }

    if (!context->onFree) {
        // There are no more jobs, release the memory.
        freeAllocator(context, context->pub.fileName, context->pub.lineNum);
    }
}

static void disconnectAdopted(struct Allocator_pvt* parent, struct Allocator_pvt* child)
{
    Assert_true(parent->adoptions);
    Assert_true(parent->adoptions->children);
    struct Allocator_List** cpp = &parent->adoptions->children;
    struct Allocator_List* cp;
    int found = 0;
    while ((cp = *cpp)) {
        if (cp->alloc == child) {
            *cpp = cp->next;
            found = 1;
            break;
        }
        cpp = &cp->next;
    }
    Assert_always(found);

    Assert_true(child->adoptions);
    Assert_true(child->adoptions->parents);
    cpp = &child->adoptions->parents;
    found = 0;
    while ((cp = *cpp)) {
        if (cp->alloc == parent) {
            *cpp = cp->next;
            found = 1;
            break;
        }
        cpp = &cp->next;
    }
    Assert_always(found);
}

/**
 * Triggered when freeAllocator() is called and the allocator nolonger
 * has any remaining links to the allocator tree.
 */
static void freeAllocator(struct Allocator_pvt* context, const char* file, int line)
{
    if (context->adoptions && context->adoptions->parents) {
        disconnect(context);
        connect(context->adoptions->parents->alloc, context, file, line);
        disconnectAdopted(context->adoptions->parents->alloc, context);
        return;
    }

    // When the last child calls us back via childFreed() we will be called the last time and
    // if this is not set, the child will be disconnected from us and we will be left.
    context->pub.isFreeing = 1;

    // from now on, fileName/line will point to the place of freeing.
    // this allows childFreed() to tell the truth when calling us back.
    context->pub.fileName = file;
    context->pub.lineNum = line;

    // Disconnect adopted children.
    struct Allocator_List* childL = context->adoptions ? context->adoptions->children : NULL;
    while (childL) {
        disconnectAdopted(context, childL->alloc);
        childL = childL->next;
    }

    // Do the onFree jobs.
    struct Allocator_OnFreeJob_pvt** jobP = &context->onFree;
    while (*jobP != NULL) {
        struct Allocator_OnFreeJob_pvt* job = *jobP;
        if (!job->pub.callback) {
            // no callback, remove the job
            Assert_true(!removeJob(job));
            continue;
        } else if (!job->done) {
            if  (job->pub.callback(&job->pub) != Allocator_ONFREE_ASYNC) {
                Assert_true(!removeJob(job));
                continue;
            }
            // asynchronously completing, don't bother it again.
            job->done = 1;
        }
        jobP = &job->next;
    }

    if (context->onFree) {
        // onFreeComplete() will call us back.
        return;
    }

    // Free children
    struct Allocator_pvt* child = context->firstChild;
    if (child) {
        while (child) {
            struct Allocator_pvt* nextChild = child->nextSibling;
            freeAllocator(child, file, line);
            child = nextChild;
        }
        // childFreed() will call us back.
        return;
    }

    // Grab out the provider and provider context in case the root allocator is freed.
    Allocator_Provider provider = context->rootAlloc->provider;
    Allocator_Provider_CONTEXT_TYPE* providerCtx = context->rootAlloc->providerContext;

    childFreed(context);
    releaseMemory(context, provider, providerCtx);
}

void Allocator__free(struct Allocator* alloc, const char* file, int line)
{
    struct Allocator_pvt* context = Identity_check((struct Allocator_pvt*) alloc);
    freeAllocator(context, file, line);
}

void* Allocator__malloc(struct Allocator* allocator,
                        unsigned long length,
                        const char* fileName,
                        int lineNum)
{
    struct Allocator_pvt* ctx = Identity_check((struct Allocator_pvt*) allocator);
    return newAllocation(ctx, length, fileName, lineNum);
}

void* Allocator__calloc(struct Allocator* alloc,
                        unsigned long length,
                        unsigned long count,
                        const char* fileName,
                        int lineNum)
{
    void* pointer = Allocator__malloc(alloc, length * count, fileName, lineNum);
    Bits_memset(pointer, 0, length * count);
    return pointer;
}

void* Allocator__realloc(struct Allocator* allocator,
                         const void* original,
                         unsigned long size,
                         const char* fileName,
                         int lineNum)
{
    if (original == NULL) {
        return Allocator__malloc(allocator, size, fileName, lineNum);
    }

    struct Allocator_pvt* context = Identity_check((struct Allocator_pvt*) allocator);
    struct Allocator_Allocation_pvt** locPtr = &context->allocations;
    struct Allocator_Allocation_pvt* origLoc =
        ((struct Allocator_Allocation_pvt*) original) - 1;
    for (;;) {
        struct Allocator_Allocation_pvt* loc = *locPtr;
        if (loc == NULL) {
            failure(context,
                    "Reallocation of memory which was not allocated using this allocator.",
                    fileName,
                    lineNum);
        }
        checkCanaries(loc, context);
        if (loc == origLoc) {
            break;
        }
        locPtr = &loc->next;
    }

    struct Allocator_Allocation_pvt* nextLoc = origLoc->next;

    if (size == 0) {
        // realloc(0) means free()
        *locPtr = nextLoc;
        Assert_true(origLoc->pub.size <= context->allocatedHere);
        context->rootAlloc->spaceAvailable += origLoc->pub.size;
        context->allocatedHere -= origLoc->pub.size;
        releaseAllocation(context,
                          origLoc,
                          context->rootAlloc->provider,
                          context->rootAlloc->providerContext);
        return NULL;
    }

    size_t realSize = getRealSize(size);
    if (context->rootAlloc->spaceAvailable + origLoc->pub.size < realSize) {
        failure(context, "Out of memory, limit exceeded.", fileName, lineNum);
    }
    context->rootAlloc->spaceAvailable += origLoc->pub.size;
    context->rootAlloc->spaceAvailable -= realSize;
    context->allocatedHere -= origLoc->pub.size;
    context->allocatedHere += realSize;

    struct Allocator_Allocation_pvt* alloc =
        context->rootAlloc->provider(context->rootAlloc->providerContext,
                                     &origLoc->pub,
                                     realSize,
                                     allocator);

    if (alloc == NULL) {
        failure(context, "Out of memory, realloc() returned NULL.", fileName, lineNum);
    }
    alloc->next = nextLoc;
    alloc->pub.size = realSize;
    *locPtr = alloc;

    setCanaries(alloc, context);

    return (void*) (alloc + 1);
}

void* Allocator__clone(struct Allocator* allocator,
                       const void* toClone,
                       unsigned long length,
                       const char* fileName,
                       int lineNum)
{
    void* pointer = Allocator__malloc(allocator, length, fileName, lineNum);
    Bits_memcpy(pointer, toClone, length);
    return pointer;
}

struct Allocator* Allocator__child(struct Allocator* allocator, const char* file, int line)
{
    struct Allocator_pvt* parent = Identity_check((struct Allocator_pvt*) allocator);

    struct Allocator_pvt stackChild = {
        .pub = {
            .fileName = file,
            .lineNum = line,
        },
        .rootAlloc = parent->rootAlloc
    };
    Identity_set(&stackChild);
    #ifdef Allocator_USE_CANARIES
        stackChild.nextCanary = stackChild.canary = parent->nextCanary;
    #endif

    struct Allocator_pvt* child =
        newAllocation(&stackChild, sizeof(struct Allocator_pvt), file, line);
    Bits_memcpyConst(child, &stackChild, sizeof(struct Allocator_pvt));

    // Link the child into the parent's allocator list
    child->lastSibling = parent;
    child->nextSibling = parent->firstChild;
    if (parent->firstChild != NULL) {
        parent->firstChild->lastSibling = child;
    }
    parent->firstChild = child;

    return &child->pub;
}

int Allocator_cancelOnFree(struct Allocator_OnFreeJob* toRemove)
{
    struct Allocator_OnFreeJob_pvt* job = (struct Allocator_OnFreeJob_pvt*) toRemove;
    struct Allocator_pvt* context = Identity_check(job->alloc);
    struct Allocator_OnFreeJob_pvt** jobPtr = &(context->onFree);
    while (*jobPtr != NULL) {
        if (*jobPtr == job) {
            *jobPtr = (*jobPtr)->next;
            return 0;
        }
        jobPtr = &(*jobPtr)->next;
    }
    return -1;
}

/** return 1 if true, otherwise zero. */
static int isAncestorOf(struct Allocator_pvt* maybeParent,
                        struct Allocator_pvt* maybeChild)
{
    if (maybeParent == maybeChild) {
        return 1;
    }
    if (maybeParent == NULL || maybeChild == NULL) {
        return 0;
    }
    if (isAncestorOf(maybeParent, getParent(maybeChild))) {
        return 1;
    }
    if (maybeChild->adoptions) {
        struct Allocator_List* al = maybeChild->adoptions->parents;
        while (al) {
            if (isAncestorOf(maybeParent, al->alloc)) {
                return 1;
            }
        }
    }
    return 0;
}

void Allocator__adopt(struct Allocator* adoptedParent,
                      struct Allocator* childToAdopt,
                      const char* file,
                      int line)
{
    struct Allocator_pvt* parent = Identity_check((struct Allocator_pvt*) adoptedParent);
    struct Allocator_pvt* child = Identity_check((struct Allocator_pvt*) childToAdopt);

    if (isAncestorOf(child, parent)) {
        // The child is a parent of the parent, this means an adoption would be meaningless
        // because if the child is otherwise freed, it will take the parent along with it.
        return;
    }

    if (!parent->adoptions) {
        parent->adoptions =
            Allocator__calloc(adoptedParent, sizeof(struct Allocator_Adoptions), 1, file, line);
    }
    if (!child->adoptions) {
        child->adoptions =
            Allocator__calloc(childToAdopt, sizeof(struct Allocator_Adoptions), 1, file, line);
    }

    struct Allocator_List* pl =
        Allocator__calloc(adoptedParent, sizeof(struct Allocator_List), 1, file, line);
    pl->alloc = child;
    pl->next = parent->adoptions->children;
    parent->adoptions->children = pl;

    struct Allocator_List* cl =
        Allocator__calloc(childToAdopt, sizeof(struct Allocator_List), 1, file, line);
    cl->alloc = parent;
    cl->next = child->adoptions->parents;
    child->adoptions->parents = cl;
}

struct Allocator_OnFreeJob* Allocator__onFree(struct Allocator* alloc,
                                              Allocator_OnFreeCallback callback,
                                              void* callbackContext,
                                              const char* file,
                                              int line)
{
    struct Allocator_pvt* context = Identity_check((struct Allocator_pvt*) alloc);

    struct Allocator_OnFreeJob_pvt* newJob =
        Allocator_clone(alloc, (&(struct Allocator_OnFreeJob_pvt) {
            .pub = {
                .callback = callback,
                .userData = callbackContext
            },
            .alloc = context,
            .file = file,
            .line = line
        }));
    Identity_set(newJob);

    struct Allocator_OnFreeJob_pvt* job = context->onFree;
    if (job == NULL) {
        context->onFree = newJob;
    } else {
        while (job->next != NULL) {
            job = job->next;
        }
        job->next = newJob;
    }
    return &newJob->pub;
}

struct Allocator* Allocator_new(unsigned long sizeLimit,
                                Allocator_Provider provider,
                                void* providerContext,
                                const char* fileName,
                                int lineNum)
{
    if (sizeLimit == 0) {
        sizeLimit = INT64_MAX - getRealSize(sizeof(struct Allocator_FirstCtx));
    }
    // Add in the size of the allocator so that a very small sizeLimit is sane.
    sizeLimit += getRealSize(sizeof(struct Allocator_FirstCtx));

    struct Allocator_FirstCtx stackContext = {
        .spaceAvailable = sizeLimit,
        .provider = provider,
        .providerContext = providerContext,
        .context = {
            .pub = {
                .fileName = fileName,
                .lineNum = lineNum,
            },
            #ifdef Allocator_USE_CANARIES
            .canary = (unsigned long) CompileTimeRandom_uint64(),
            .nextCanary = (unsigned long) CompileTimeRandom_uint64(),
            #endif
        }
    };
    stackContext.maxSpace = stackContext.spaceAvailable;
    stackContext.context.rootAlloc = &stackContext;
    Identity_set(&stackContext.context);

    struct Allocator_FirstCtx* firstContext =
        Allocator__clone(&stackContext.context.pub,
                         &stackContext,
                         sizeof(struct Allocator_FirstCtx),
                         fileName,
                         lineNum);

    struct Allocator_pvt* context = &firstContext->context;
    context->rootAlloc = firstContext;
    context->lastSibling = context;

    Identity_set(context);
    return &context->pub;
}

static inline uint64_t bytesAllocated(struct Allocator_pvt* ctx)
{
    uint64_t bytes = ctx->allocatedHere;
    for (struct Allocator_pvt* child = ctx->firstChild; child; child = child->nextSibling) {
        bytes += bytesAllocated(child);
    }
    return bytes;
}

unsigned long Allocator_bytesAllocated(struct Allocator* allocator)
{
    struct Allocator_pvt* context = Identity_check((struct Allocator_pvt*) allocator);
    return bytesAllocated(context);
}

void Allocator_setCanary(struct Allocator* alloc, unsigned long value)
{
    #ifdef Allocator_USE_CANARIES
        struct Allocator_pvt* context = Identity_check((struct Allocator_pvt*) alloc);
        context->nextCanary ^= value;
    #endif
}
