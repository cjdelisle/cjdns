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
#include "util/Gcc.h"
#include "util/Linker.h"
Linker_require("memory/Allocator.c");

/**
 * A handle which is provided in response to calls to Allocator_onFree().
 * This handle is sutable for use with Allocator_notOnFree() to cancel a job.
 */
struct Allocator_OnFreeJob;
typedef int (* Allocator_OnFreeCallback)(struct Allocator_OnFreeJob* job);
struct Allocator_OnFreeJob
{
    /** Set by caller. */
    Allocator_OnFreeCallback callback;
    void* userData;
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
 * In traditional C, each call to malloc() must be traced to a corresponding free() call, a
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
 * When you call MallocAllocator_new() or equivalent, you are creating a parentless allocator and
 * you must take responsibility for it's freeing when you are finished with it. In cjdns there is
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
struct Allocator
{
    /** The name of the file where this allocator was created. */
    const char* fileName;

    /** The number of the line where this allocator was created. */
    int lineNum;

    /** Non-zero if allocator is currently freeing. */
    int isFreeing;
};

struct Allocator_Allocation
{
    const char* fileName;

    int lineNum;

    unsigned long size;
};

/**
 * Get a child of a given allocator.
 *
 * @param alloc the parent
 * @param childNumber
 * @return a child allocator or NULL if childNumber is out of range.
 */
struct Allocator* Allocator_getChild(struct Allocator* alloc, int childNumber);

/**
 * Get one of the allocations held by this allocator.
 *
 * @param alloc the allocator.
 * @param allocNum the number of the allocation.
 * @return an allocation or NULL if allocNum is out of range.
 */
struct Allocator_Allocation* Allocator_getAllocation(struct Allocator* alloc, int allocNum);

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
Gcc_ALLOC_SIZE(2)
void* Allocator__malloc(struct Allocator* allocator,
                        unsigned long length,
                        const char* fileName,
                        int lineNum);
#define Allocator_malloc(a, b) Allocator__malloc((a),(b),Gcc_SHORT_FILE,Gcc_LINE)

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
Gcc_ALLOC_SIZE(2,3)
void* Allocator__calloc(struct Allocator* alloc,
                        unsigned long length,
                        unsigned long count,
                        const char* fileName,
                        int lineNum);
#define Allocator_calloc(a, b, c) Allocator__calloc((a),(b),(c),Gcc_SHORT_FILE,Gcc_LINE)

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
Gcc_ALLOC_SIZE(3)
void* Allocator__realloc(struct Allocator* allocator,
                         const void* original,
                         unsigned long size,
                         const char* fileName,
                         int lineNum);
#define Allocator_realloc(a, b, c) Allocator__realloc((a),(b),(c),Gcc_SHORT_FILE,Gcc_LINE)

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
Gcc_ALLOC_SIZE(3)
void* Allocator__clone(struct Allocator* allocator,
                       const void* toClone,
                       unsigned long length,
                       const char* fileName,
                       int lineNum);
#define Allocator_clone(a, b) Allocator__clone((a),(b),sizeof(*(b)),Gcc_SHORT_FILE,Gcc_LINE)

/**
 * Spawn a new child of this allocator.
 * When this allocator is freed all of its children which have no surviving parent will also be
 * freed.
 *
 * @param alloc the memory allocator.
 * @return a child allocator.
 */
struct Allocator* Allocator__child(struct Allocator* alloc, const char* fileName, int lineNum);
#define Allocator_child(a) Allocator__child((a),Gcc_SHORT_FILE,Gcc_LINE)

/**
 * Sever the link between an allocator and it's original parent.
 * If it has been adopted using Allocator_adopt() then the freeing of the allocator will be deferred
 * until the allocator returned by Allocator_adopt() has also been freed.
 * Any allocator which has no surviving parent allocator will be implicitly freed.
 * NOTE: This does not do what it seems to do, it does not necessarily *free* the allocator, it
 *       only promises to cut the link to the allocator's normal parent, if the allocator has been
 *       adopter then the adopted parent becomes the normal parent and then the allocator is not
 *       freed even though you asked to free it!
 *
 * @param alloc the allocator to disconnect from it's parent.
 */
void Allocator__free(struct Allocator* alloc, const char* file, int line);
#define Allocator_free(a) Allocator__free((a),Gcc_SHORT_FILE,Gcc_LINE)

/**
 * Add a function to be called when the allocator is freed.
 * There is no guarantee of which order the onFree jobs will be executed.
 *
 * @param alloc the memory allocator.
 * @param callback the function to call.
 * @return an Allocator_OnFreeJob which can be cancelled with Allocator_cancelOnFree().
 */
struct Allocator_OnFreeJob* Allocator__onFree(struct Allocator* alloc,
                                              Allocator_OnFreeCallback callback,
                                              void* context,
                                              const char* file,
                                              int line);
#define Allocator_onFree(a, b, c) Allocator__onFree((a), (b), (c), Gcc_SHORT_FILE, Gcc_LINE)

/**
 * Remove a function which was registered with Allocator_onFree().
 *
 * @param job the return value from calling Allocator_onFree().
 * @return 0 if the job was found and removed, -1 otherwise.
 */
int Allocator_cancelOnFree(struct Allocator_OnFreeJob* toRemove);

/**
 * Tell the allocator that an asynchronous onFree() job has completed.
 *
 * @param job the return value from calling Allocator_onFree().
 */
void Allocator_onFreeComplete(struct Allocator_OnFreeJob* onFreeJob);

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
 */
void Allocator__adopt(struct Allocator* parentAlloc,
                      struct Allocator* alloc,
                      const char* fileName,
                      int lineNum);
#define Allocator_adopt(a, b) Allocator__adopt((a),(b),Gcc_SHORT_FILE,Gcc_LINE)

/**
 * Disown an allocator.
 *
 * Sever the link between an adopted parent allocator and the child which it has adopted.
 * If this causes the child allocator to disconnect from the tree entirely, it will be
 * freed.
 *
 * @param parentAlloc the parent which has adopted the child allocator.
 * @param childToDisown the child allocator which has been adopted.
 */
void Allocator__disown(struct Allocator* parentAlloc,
                       struct Allocator* allocToDisown,
                       const char* fileName,
                       int lineNum);
#define Allocator_disown(a, b) Allocator__disown((a),(b),Gcc_SHORT_FILE,Gcc_LINE)

/**
 * Set the heap protection canary for the next child allocator.
 * If heap protection canaries are enabled, they will be added at the beginning and end
 * of each memory allocation and checked during free and other operations. If one is corrupted
 * the program will be aborted to protect against security attacks and other faults.
 * By default the canaries are statically set but this allows the value to be changed so that
 * the value of the canaries is unpredictable in order to foil targetted attacks.
 */
void Allocator_setCanary(struct Allocator* alloc, unsigned long value);

/**
 * Get the number of bytes allocated by this allocator and all of it's children.
 */
unsigned long Allocator_bytesAllocated(struct Allocator* allocator);

/**
 * Dump a memory snapshot to stderr.
 *
 * @param alloc any allocator in the tree, the whole tree will be dumped.
 * @param includeAllocations if non-zero then the individual memory allocations will be printed.
 */
void Allocator_snapshot(struct Allocator* alloc, int includeAllocations);


/**
 * The underlying memory provider function which backs the allocator.
 * This function is roughly equivilant to realloc() API in that it is used for allocation,
 * reallocation and freeing but it also contains a context field which allows the provider
 * to store it's state in a non-global way and a group pointer.
 *
 * The group pointer is used to add memory to an allocation group. If the group pointer is set to
 * NULL, the provider is requested to begin a new group, if the group pointer is not null, it will
 * be set to an allocation which had previously been returned by the provider, in this case the
 * provider should internally group this allocation with the other as they will likely be freed
 * at the same time.
 *
 * @param ctx the context which was passed to Allocator_new() along with the provider.
 * @param original if this is NULL then the allocator is to provide a new allocation, otherwise it
 *                 should resize or free an existing allocation.
 * @param size if this is 0 then the allocator should free original and return NULL, if it is not
 *             zero then original should be resized or created.
 * @param group if this is not NULL then the provider is being informed that the current allocation
 *              and the allocation in group are likely to have the same life span and should be
 *              colocated if it is logical to do so.
 */
#ifndef Allocator_Provider_CONTEXT_TYPE
    #define Allocator_Provider_CONTEXT_TYPE void
#endif
Gcc_ALLOC_SIZE(3)
typedef void* (* Allocator_Provider)(Allocator_Provider_CONTEXT_TYPE* ctx,
                                     struct Allocator_Allocation* original,
                                     unsigned long size,
                                     struct Allocator* group);

struct Allocator* Allocator_new(unsigned long sizeLimit,
                                Allocator_Provider provider,
                                Allocator_Provider_CONTEXT_TYPE* providerContext,
                                const char* fileName,
                                int lineNum);

#endif
