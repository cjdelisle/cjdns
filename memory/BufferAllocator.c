#include "BufferAllocator.h"
#include <string.h>
#include <stdint.h>

/** Internal state for MemAllocator. */
struct BufferAllocator_context {
    char* basePointer;
    char* pointer;
    char* const endPointer;
};

/* Internal Prototypes. */
static void* allocatorMalloc(size_t length, struct MemAllocator* allocator);
static void* allocatorCalloc(size_t length, size_t count, struct MemAllocator* allocator);
static void freeAllocator(struct MemAllocator* allocator);
static struct MemAllocator* childAllocator(struct MemAllocator* allocator);

/**
 * Get a pointer which is aligned on memory boundries.
 *
 * @param pointer the location where the pointer should be.
 * @param alignedOn how big the word is that the boundry should be aligned on.
 */
#define GET_ALIGNED(pointer, alignedOn) \
    (char*) (((uintptr_t)pointer + alignedOn - 1) & ~ (alignedOn - 1))

/** @see BufferAllocator.h */
struct MemAllocator* BufferAllocator_new(void* buffer, size_t length)
{
    /* Write itself into it's own buffer :) */
    struct BufferAllocator_context context = {
        /* Align the pointer to do the first write manually. */
        .pointer = GET_ALIGNED(buffer, sizeof(char*)),
        .basePointer = GET_ALIGNED(buffer, sizeof(char*)),
        .endPointer = ((char*)buffer) + length
    };

    if (context.endPointer < context.pointer) {
        /* Integer overflow. */
        return NULL;
    }

    if (length + (char*) buffer < context.pointer + sizeof(struct BufferAllocator_context)) {
        /* Not enough space to allocate the context. */
        return NULL;
    }

    /* put the context into the buffer. */
    memcpy(context.pointer, &context, sizeof(struct BufferAllocator_context));

    struct MemAllocator allocator = {
        .context = context.pointer,
        .free = freeAllocator,
        .malloc = allocatorMalloc,
        .calloc = allocatorCalloc,
        .child = childAllocator
    };

    /* Get the pointer to the context where it's written in the buffer. */
    struct BufferAllocator_context* contextPtr = (struct BufferAllocator_context*) allocator.context;

    /* have to advance the pointer so as not to clobber the context. */
    contextPtr->pointer += sizeof(struct BufferAllocator_context);

    /* Now that the context is in place we can begin using the allocater. */
    struct MemAllocator* allocatorPtr = allocator.malloc(sizeof(struct MemAllocator), &allocator);
    if (allocatorPtr == NULL) {
        return NULL;
    }

    /* Reset the base pointer so as not to clobber the context on free() */
    contextPtr->basePointer = contextPtr->pointer;

    memcpy(allocatorPtr, &allocator, sizeof(struct MemAllocator));
    return allocatorPtr;
}

/*--------------------Internal--------------------*/

/** @see MemAllocator->malloc() */
static void* allocatorMalloc(size_t length, struct MemAllocator* allocator)
{
    /* Define alignment as the size of a pointer which is usually 4 or 8 bytes. */
    #define ALIGNMENT sizeof(char*)

    struct BufferAllocator_context* context =
        (struct BufferAllocator_context*) allocator->context;

    char* pointer = GET_ALIGNED(context->pointer, ALIGNMENT);
    char* endOfAlloc = pointer + length;

    if (endOfAlloc >= context->endPointer) {
        /* out of bounds. */
        return NULL;
    }

    if (endOfAlloc < context->pointer) {
        /* integer overflow. */
        return NULL;
    }

    context->pointer = endOfAlloc;
    return (void*) pointer;
}

/** @see MemAllocator->calloc() */
static void* allocatorCalloc(size_t length, size_t count, struct MemAllocator* allocator)
{
    void* pointer = allocator->malloc(length * count, allocator);
    if (pointer != NULL) {
        memset(pointer, 0, length * count);
    }
    return pointer;
}

/** @see MemAllocator->free() */
static void freeAllocator(struct MemAllocator* allocator)
{
    struct BufferAllocator_context* context =
        (struct BufferAllocator_context*) allocator->context;
    context->pointer = context->basePointer;
}

/** @see MemAllocator->child() */
static struct MemAllocator* childAllocator(struct MemAllocator* allocator)
{
    allocator = allocator;
    return NULL;
}

#undef GET_ALIGNED
