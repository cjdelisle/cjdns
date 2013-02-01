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

#define SIZE_INTS(size) ((size + 11) / 4)
#define SIZE_BYTES(size) (SIZE_INTS(size) * 4)

int CanaryAllocator_isOverflow(struct CanaryAllocator_pvt* ctx)
{
    if (ctx->canaryCount > 0) {
        if (ctx->firstCanary != ctx->canaries[0]
            || ctx->lastCanary != ctx->canaries[ctx->canaryCount - 1])
        {
            return 1;
        }
    }
    for (int i = 0; i < ctx->canaryCount; i++) {
        if (*ctx->canaries[i] != ctx->canaryValue) {
            return 1;
        }
    }
    return 0;
}

/** @see Allocator->free() */
static void freeAllocator(const struct Allocator* allocator)
{
    struct CanaryAllocator_pvt* ctx = Identity_cast((struct CanaryAllocator_pvt*) allocator);
    Assert_always(!CanaryAllocator_isOverflow(ctx));
    ctx->alloc->free(ctx->alloc);
}

static inline void* newAllocation(struct CanaryAllocator_pvt* ctx,
                                  uint32_t* allocation,
                                  size_t sizeInts)
{
    //Assert_always(!CanaryAllocator_isOverflow(ctx));
    ctx->canaries =
        ctx->alloc->realloc(ctx->canaries, (ctx->canaryCount + 2) * sizeof(uint32_t*), ctx->alloc);
    allocation[0] = ctx->canaryValue;
    if (ctx->canaryCount == 0) {
        ctx->firstCanary = allocation;
    }
    ctx->canaries[ctx->canaryCount++] = allocation;
    allocation[sizeInts - 1] = ctx->canaryValue;
    ctx->canaries[ctx->canaryCount++] = &allocation[sizeInts - 1];
    ctx->lastCanary = &allocation[sizeInts - 1];
    return (void*) (&allocation[1]);
}

/** @see Allocator->malloc() */
static void* allocatorMalloc(size_t size, const struct Allocator* allocator)
{
    struct CanaryAllocator_pvt* ctx = Identity_cast((struct CanaryAllocator_pvt*) allocator);
    // get it on an even number of ints.
    uint32_t* out = ctx->alloc->malloc(SIZE_BYTES(size), ctx->alloc);
    return newAllocation(ctx, out, SIZE_INTS(size));
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

/** @see Allocator->onFree() */
static void* addOnFreeJob(void (* callback)(void* callbackContext),
                          void* callbackContext,
                          const struct Allocator* allocator)
{
    struct CanaryAllocator_pvt* ctx = Identity_cast((struct CanaryAllocator_pvt*) allocator);
    return ctx->alloc->onFree(callback, callbackContext, ctx->alloc);
}

static bool removeOnFreeJob(void* toRemove, struct Allocator* alloc)
{
    struct CanaryAllocator_pvt* ctx = Identity_cast((struct CanaryAllocator_pvt*) alloc);
    return ctx->alloc->notOnFree(toRemove, ctx->alloc);
}

/** @see Allocator->realloc() */
static void* allocatorRealloc(const void* original,
                              size_t size,
                              const struct Allocator* allocator)
{
    struct CanaryAllocator_pvt* ctx = Identity_cast((struct CanaryAllocator_pvt*) allocator);
    if (((uint8_t*) original) == NULL) {
        return allocatorMalloc(size, allocator);
    }
    uint32_t* beginCanary = ((uint32_t*) original) - 1;
    Assert_always(*beginCanary == ctx->canaryValue);
    for (int i = 0; i < ctx->canaryCount; i++) {
        if (ctx->canaries[i] == beginCanary) {
            Assert_always(ctx->canaryCount > i + 1 && *ctx->canaries[i + 1] == ctx->canaryValue);
            for (int j = i + 2; j < ctx->canaryCount; j++) {
                ctx->canaries[j - 2] = ctx->canaries[j];
            }
            ctx->canaryCount -= 2;
            break;
        }
    }
    uint32_t* out = ctx->alloc->realloc(((uint32_t*)original) - 1, SIZE_BYTES(size), ctx->alloc);
    return newAllocation(ctx, out, SIZE_INTS(size));
}

/** @see Allocator_child() */
static struct Allocator* childAllocator(const struct Allocator* allocator,
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
            .onFree = addOnFreeJob,
            .notOnFree = removeOnFreeJob,
            .context = alloc->context
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
