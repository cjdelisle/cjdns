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
#include "memory/Allocator.h"
#include "memory/CanaryAllocator.h"
#include "memory/CanaryAllocator_pvt.h"
#include "util/Assert.h"
#include "util/Bits.h"
#include "util/Identity.h"

#include <stdint.h>
#include <stdio.h>

#define SIZE_INTS(size) ((size + 11) / 4)
#define SIZE_BYTES(size) (SIZE_INTS(size) * 4)

int CanaryAllocator_isOverflow(struct CanaryAllocator_pvt* ctx)
{
    if (ctx->allocCount <= 0) {
        return 0;
    }
    if (ctx->firstFile != ctx->allocations[0].file) {
        fprintf(stderr, "CanaryAllocator:%d Allocations list is corrupt\n", __LINE__);
        return 1;
    }
    for (int i = 0; i < ctx->allocCount; i++) {
        if (ctx->allocations[i].canaryValue != ctx->canaryValue) {
            fprintf(stderr, "CanaryAllocator:%d Allocations list is corrupt\n", __LINE__);
            return 1;
        }
        if (*ctx->allocations[i].beginCanary != ctx->canaryValue
            || *ctx->allocations[i].endCanary != ctx->canaryValue)
        {
            fprintf(stderr, "%s:%d Buffer overflow, %s canary corrupt\n",
                    ctx->allocations[i].file, ctx->allocations[i].line,
                    (*ctx->allocations[i].beginCanary != ctx->canaryValue) ? "begin" : "end");
            return 1;
        }
    }
    return 0;
}

/** @see Allocator->free() */
static void freeAllocator(struct Allocator* allocator, const char* identFile, int identLine)
{
    struct CanaryAllocator_pvt* ctx = Identity_cast((struct CanaryAllocator_pvt*) allocator);
    CanaryAllocator_check(allocator);
    ctx->alloc->free(ctx->alloc, identFile, identLine);
}

static inline void* newAllocation(struct CanaryAllocator_pvt* ctx,
                                  uint32_t* allocation,
                                  unsigned long sizeInts,
                                  const char* identFile,
                                  int identLine)
{
    Assert_always(!CanaryAllocator_isOverflow(ctx));
    ctx->allocCount++;
    ctx->allocations =
        Allocator_realloc(ctx->alloc,
                          ctx->allocations,
                          ctx->allocCount * sizeof(struct CanaryAllocator_Allocation));
    if (ctx->allocCount == 1) {
        // This is used to check the integrity of the first element in the list.
        ctx->firstFile = identFile;
    }
    struct CanaryAllocator_Allocation* a = &ctx->allocations[ctx->allocCount - 1];
    a->canaryValue = ctx->canaryValue;
    a->file = identFile;
    a->line = identLine;
    a->beginCanary = allocation;
    a->endCanary = &allocation[sizeInts - 1];
    allocation[0] = ctx->canaryValue;
    allocation[sizeInts - 1] = ctx->canaryValue;
    return (void*) (&allocation[1]);
}

/** @see Allocator->malloc() */
static void* allocatorMalloc(unsigned long size,
                             struct Allocator* allocator,
                             const char* identFile,
                             int identLine)
{
    struct CanaryAllocator_pvt* ctx = Identity_cast((struct CanaryAllocator_pvt*) allocator);
    // get it on an even number of ints.
    uint32_t* out = ctx->alloc->malloc(SIZE_BYTES(size), ctx->alloc, identFile, identLine);
    return newAllocation(ctx, out, SIZE_INTS(size), identFile, identLine);
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

/** @see Allocator->onFree() */
static struct Allocator_OnFreeJob* addOnFreeJob(void (* callback)(void* callbackContext),
                                                void* callbackContext,
                                                struct Allocator* allocator)
{
    struct CanaryAllocator_pvt* ctx = Identity_cast((struct CanaryAllocator_pvt*) allocator);
    return ctx->alloc->onFree(callback, callbackContext, ctx->alloc);
}

/** @see Allocator->realloc() */
static void* allocatorRealloc(const void* original,
                              unsigned long size,
                              struct Allocator* allocator,
                              const char* identFile,
                              int identLine)
{
    struct CanaryAllocator_pvt* ctx = Identity_cast((struct CanaryAllocator_pvt*) allocator);
    if (((uint8_t*) original) == NULL) {
        return allocatorMalloc(size, allocator, identFile, identLine);
    }
    uint32_t* beginCanary = ((uint32_t*) original) - 1;
    Assert_always(*beginCanary == ctx->canaryValue);
    for (int i = 0; i < ctx->allocCount; i++) {
        if (ctx->allocations[i].beginCanary == beginCanary) {
            Bits_memmove(&ctx->allocations[i],
                         &ctx->allocations[i + 1],
                         (ctx->allocCount - i - 1) * sizeof(struct CanaryAllocator_Allocation));
            ctx->allocCount--;
            uint32_t* out = ctx->alloc->realloc(
                ((uint32_t*)original) - 1, SIZE_BYTES(size), ctx->alloc, identFile, identLine);
            return newAllocation(ctx, out, SIZE_INTS(size), identFile, identLine);
        }
    }
    Assert_true(0);
}

/** @see Allocator_child() */
static struct Allocator* childAllocator(struct Allocator* allocator,
                                        const char* identFile,
                                        int identLine)
{
    struct CanaryAllocator_pvt* ctx = Identity_cast((struct CanaryAllocator_pvt*) allocator);
    return CanaryAllocator_new(ctx->alloc->child(ctx->alloc, identFile, identLine), ctx->rand);
}

/** @see MallocAllocator.h */
struct Allocator* CanaryAllocator_new(struct Allocator* alloc, struct Random* rand)
{
    struct CanaryAllocator_pvt* ctx = Allocator_clone(alloc, (&(struct CanaryAllocator_pvt) {
        .pub = {
            .free = freeAllocator,
            .malloc = allocatorMalloc,
            .calloc = allocatorCalloc,
            .clone = allocatorClone,
            .realloc = allocatorRealloc,
            .child = childAllocator,
            .onFree = addOnFreeJob
        },
        .alloc = alloc,
        .rand = rand,
        .canaryValue = 0xdc57b1a4
    }));
    Identity_set(ctx);
    if (rand) {
        ctx->canaryValue = Random_uint32(rand);
    }
    return &ctx->pub;
}

void CanaryAllocator_check(struct Allocator* allocator)
{
    struct CanaryAllocator_pvt* ctx = Identity_cast((struct CanaryAllocator_pvt*) allocator);
    Assert_always(!CanaryAllocator_isOverflow(ctx));
}
