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

#include "util/Identity.h"

/**
 * A handle which is provided in response to calls to Allocator_onFree().
 * This handle is sutable for use with Allocator_notOnFree() to cancel a job.
 */
struct Allocator_OnFreeJob;
typedef int (* Allocator_OnFreeCallback)(struct Allocator_OnFreeJob* job);
struct Allocator_OnFreeJob {
    /** Set by allocator. */
    const Allocator_OnFreeCallback cancel;

    /** Complete the job, used only if the callback returns Allocator_ONFREE_ASYNC. */
    const Allocator_OnFreeCallback complete;

    /** Set by caller. */
    Allocator_OnFreeCallback callback;
    void* userData;

    /** Set by allocator. */
    Identity
};

/**
 * If an onFree job needs to complete asynchronously, it should return this,
 * then when it is complete it must call job->complete(job) on the OnFreeJob
 * which was passed to it.
 */
#define Allocator_ONFREE_ASYNC 10000

/**
 * Allocator for structured memory management.
 * The objective of the allocator structure is to make manual memory management easier, specifically
 * to make making a mistake difficult.
 *
 * Every function which allocates memory, either to return a structure or to do processing which
 * cannot be done on the stack takes an allocator as a parameter.
 *
 * In traditional C, each call to malloc() must be traced to a corrisponding free() call, a
 * laborious process which can be partially automated but inevitably leaves some memory leak
 * investigative work to the developer. Allocator attempts to move the memory freeing operations
 * close to the memory allocations thus making bugs easy to spot without searching over large
 * amounts of code.
 *
 * With Allocator, you might do the following:
 *
 * struct Allocator* child = Allocator_child(myAlloc); <-- myAlloc is the one provided to you
 * potentiallyLeakyFunction(child);
 * Allocator_free(child);
 *
 * Given this simple pattern, as long as potentiallyLeakyFunction() did not bypass the allocator
 * system using malloc() directly, we can prove that it is not the source of a memory leak.
 * As the real code is far more complex than this contrived example, there are a few rules which
 * have proven useful in preventing both memory leaks and dangling pointers.
 *
 * #1 Do not create new root allocators, create child allocators instead.
 * When you call MallocAllocator_new() or equivilant, you are creating a parentless allocator and
 * you must take responsability for it's freeing when you are finished with it. In cjdns there is
 * only one call to a main allocator and all other allocators are spawned from it using
 * Allocator_child().
 * Exception: In certain code which interfaces with libuv, an alternate root allocator is necessary
 *    because libuv teardown process is asynchronous and memory used by libuv must not be freed
 *    until this is complete.
 *
 * #2 Free your allocators and not anyone else's.
 * With precious few exceptions, an allocator is always freed in the same .c file where it was
 * created. It is obviously rude to destroy something of someone else's just as it is rude to leave
 * things lying around expecting someone else to clean up after you. Sometimes you want to "take
 * ownership" of some memory which somebody else allocated and they are passing to you. Rather
 * than slowly allocate your own memory and copy the data over, you can use Allocator_adopt() to
 * hold that memory in existance until you and the creator both are finished with it.
 *
 * #3 Assume that any allocator may be freed at any time.
 * A typical example is the ping message. When a ping is sent, a structure is allocated to hold
 * information about the ping so that when the response comes back it will be recognized. That
 * structure is inserted into a table of outstanding pings. If that allocator were freed while the
 * ping was outstanding, the response would come back and the table lookup would access freed
 * memory. To prevent this, every place where temporary memory is placed into a more permanent
 * structure (the table), Allocator_onFree() is used to hook the freeing of that memory and add a
 * function to remove the entry from the table.
 * Cjdns is notably lacking in "deregister" or "cancel" type functions as the accepted method of
 * deregistering a peer or cancelling an operation is by freeing the associated allocator, both
 * simplifying the code and avoiding bug prone "cold" codepaths.
 *
 * The function pointers in the allocator structure are best called through the associated macros.
 */
struct Allocator;
struct Allocator
{
    void* (* const malloc)(unsigned long numberOfBytes,
                           struct Allocator* thisAlloc,
                           const char* identFile,
                           int identLine);

    void* (* const calloc)(unsigned long numberOfBytes,
                           unsigned long multiplier,
                           struct Allocator* thisAlloc,
                           const char* identFile,
                           int identLine);
    void* (* const realloc)(const void* originalAllocation,
                            unsigned long numberOfBytes,
                            struct Allocator* thisAllocator,
                            const char* identFile,
                            int identLine);

    void* (* const clone)(unsigned long numberOfBytes,
                          struct Allocator* thisAllocator,
                          const void* toClone,
                          const char* identFile,
                          int identLine);


    void (*  free)(struct Allocator* alloc, const char* identFile, int identLine);

    struct Allocator_OnFreeJob* (* const onFree)(struct Allocator* alloc,
                                                 const char* file,
                                                 int line);


    struct Allocator* (* const child)(struct Allocator* thisAlloc,
                                      const char* identFile,
                                      int identLine);

    void (* const adopt)(struct Allocator* parentAlloc,
                         struct Allocator* alloc,
                         const char* identFile,
                         int identLine);
};



/**
 * Allocate some memory from this memory allocator.
 * The allocation will be aligned on the size of a pointer, if you need further alignment then
 * you must handle it manually.
 *
 * @param alloc the memory allocator.
 * @param size the number of bytes to allocate.
 * @return a pointer to the newly allocated memory.
 * @see malloc()
 */
#define Allocator_malloc(alloc, size) \
    (alloc)->malloc((size), (alloc), __FILE__, __LINE__)

/**
 * Allocate some memory from this memory allocator.
 * The allocation will be aligned on the size of a pointer, if you need further alignment then
 * you must handle it manually.
 * Memory location will be filled with 0 bytes.
 *
 * @param alloc the memory allocator.
 * @param size the number of bytes per element.
 * @param count the number of elements in the allocation.
 * @return a pointer to the newly allocated memory.
 * @see calloc()
 */
#define Allocator_calloc(alloc, size, count) \
    (alloc)->calloc((size), (count), (alloc), __FILE__, __LINE__)

/**
 * Re-allocate memory so that an allocation can be expanded.
 * The allocation will be aligned on the size of a pointer, if you need further alignment then
 * you must handle it manually.
 * Caution: Use of this function is not advisable with memory which is shared with other parts
 *          of the system.
 *
 * @param alloc the allocator to allocate with, must be the same allocator which allocated orig.
 * @param orig a pointer to the original memory allocation which is to be reallocated.
 *             if NULL, this function will behave exactly as Allocator_malloc().
 * @param size how much memory to allocate. If 0, this function will free the specific memory
 *             without freeing the entire allocator.
 * @return a pointer to the newly allocated memory.
 */
#define Allocator_realloc(alloc, orig, size) \
    (alloc)->realloc((orig), (size), (alloc), __FILE__, __LINE__)

/**
 * Allocate some memory and copy something into that memory space.
 * The allocation will be aligned on the size of a pointer, if you need further alignment then
 * you must handle it manually.
 * Caution: if content is an expression, it will be evaluated twice.
 *
 * @param alloc the memory allocator.
 * @param content a pointer to something which will be cloned into the newly allocated memory.
 *                the size of the new allocation will be sizeof(*content).
 * @return a pointer to the newly allocated memory.
 */
#define Allocator_clone(alloc, content) \
    (alloc)->clone(sizeof(*(content)), (alloc), (content), __FILE__, __LINE__)

/**
 * Sever the link between an allocator and it's original parent.
 * If it has been adopted using Allocator_adopt() then the freeing of the allocator will be deferred
 * until the allocator returned by Allocator_adopt() has also been freed.
 * Any allocator which has no surviving parent allocator will be implicitly freed.
 *
 * @param alloc the allocator to disconnect from it's parent.
 */
#define Allocator_free(alloc) \
    (alloc)->free((alloc), __FILE__, __LINE__)

/**
 * Add a function to be called when the allocator is freed.
 *
 * @param alloc the memory allocator.
 * @param callback the function to call.
 * @return an Allocator_OnFreeJob which can be cancelled with Allocator_cancelOnFree().
 */
#define Allocator_onFree(alloc, callback, context) \
    Allocator__onFree((alloc), (callback), (context), __FILE__, __LINE__)

static inline struct Allocator_OnFreeJob* Allocator__onFree(struct Allocator* alloc,
                                                            Allocator_OnFreeCallback callback,
                                                            void* context,
                                                            const char* file,
                                                            int line)
{
    struct Allocator_OnFreeJob* j = alloc->onFree(alloc, file, line);
    j->callback = callback;
    j->userData = context;
    return j;
}

/**
 * Remove a function which was registered with Allocator_onFree().
 *
 * @param job the return value from calling Allocator_onFree().
 * @return 0 if the job was found and removed, -1 otherwise.
 */
#define Allocator_cancelOnFree(job) \
    (job)->cancel(job)

/**
 * Spawn a new child of this allocator.
 * When this allocator is freed all of its children which have no surviving parent will also be
 * freed.
 *
 * @param alloc the memory allocator.
 * @return a child allocator.
 */
#define Allocator_child(alloc) \
    (alloc)->child((alloc), __FILE__, __LINE__)

/**
 * Adopt an allocator.
 * This creates a child of parentAlloc which is an adopted parent of toAdopt.
 * When Allocator_free() is called on toAdopt or one of it's parents, it will not be freed until
 * Allocator_free() has also been called on the allocator newly returned by this function.
 * This function may be used multiple times.
 *
 * Caution: Do not free an allocator which you did not create, even after adopting it.
 *
 * Allocator_adopt(myAlloc, somebodyElsesAllocator);
 * asynchronousStuff();
 * .... some time later...
 * Allocator_free(somebodyElsesAllocator);  <-- WRONG: you freed an allocator that is not yours.
 *
 *
 * struct Allocator* adoptedParent = Allocator_child(myAlloc);
 * Allocator_adopt(adoptedParent, somebodyElsesAllocator);
 * asynchronousStuff();
 * .... some time later...
 * Allocator_free(adoptedParent);  <-- RIGHT
 *
 *
 * @param parentAlloc the allocator to create a child of.
 * @param toAdopt the allocator which should be adopted by the returned child allocator.
 * @return a new allocator which is an adopted parent of toAdopt.
 */
#define Allocator_adopt(parentAlloc, toAdopt) \
    (parentAlloc)->adopt((parentAlloc), (toAdopt), __FILE__, __LINE__)

#endif
