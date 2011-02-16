/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307 USA
 */

#include "DHTModules.h"

#include <libbenc/benc.h>
#include <memory/MemAllocator.h>
#include <memory/BufferAllocator.h>
#include <io/Reader.h>
#include <io/ArrayReader.h>
#include <io/Writer.h>
#include <io/ArrayWriter.h>

#include <string.h>

/* The number of bytes for storing deserialized packets. */
#define WORKSPACE_SIZE 4096

struct SerializationModule_context {
    char workspace[WORKSPACE_SIZE];
    struct MemAllocator* allocator;
    struct DHTModule module;
};

/*--------------------Prototypes--------------------*/
static void freeModule(struct DHTModule* module);
static int handleOutgoing(struct DHTMessage* message,
                          void* vcontext);
static int handleIncoming(struct DHTMessage* message,
                          void* vcontext);

/*--------------------Interface--------------------*/

/**
 * Get a new SerializationModule.
 *
 * @return a new SerializationModule or NULL if there is no space to allocate required memory.
 */
struct DHTModule* SerializationModule_new()
{
    struct SerializationModule_context* context =
        malloc(sizeof(struct SerializationModule_context));

    if (context == NULL) {
        return NULL;
    }

    context->allocator = BufferAllocator_new(context->workspace, WORKSPACE_SIZE);

    if (context->allocator == NULL) {
        return NULL;
    }

    struct DHTModule module = {
        .name = "SerializationModule",
        .context = context,
        .free = freeModule,
        .serialize = NULL,
        .deserialize = NULL,
        .compareNodes = NULL,
        .handleIncoming = handleIncoming,
        .handleOutgoing = handleOutgoing
    };

    memcpy(&context->module, &module, sizeof(struct DHTModule));

    return &context->module;
}

/*--------------------Internals--------------------*/

/** @see DHTModule->freeContext in DHTModules.h */
static void freeModule(struct DHTModule* module)
{
    if (module == NULL) {
        return;
    }

    struct SerializationModule_context* context =
        (struct SerializationModule_context*) module->context;

    free(context);
}

/**
 * Take an outgoing message and serialize the bencoded message.
 *
 * @see DHTModule->handleOutgoing in DHTModules.h
 */
static int handleOutgoing(struct DHTMessage* message,
                          void* vcontext)
{
    struct MemAllocator* allocator =
        ((struct SerializationModule_context*) vcontext)->allocator;

    allocator->free(allocator);
    struct Writer* writer = ArrayWriter_new(message->bytes, MAX_MESSAGE_SIZE, allocator);
    if (writer == NULL) {
        return -1;
    }

    bobj_serialize(writer, message->bencoded);
    message->length = writer->bytesWritten(writer);

    return 0;
}

/**
 * Take an incoming message and deserialize the bencoded message.
 *
 * @see DHTModule->handleIncoming in DHTModules.h
 */
static int handleIncoming(struct DHTMessage* message,
                          void* vcontext)
{
    struct MemAllocator* allocator =
        ((struct SerializationModule_context*) vcontext)->allocator;

    allocator->free(allocator);
    struct Reader* reader = ArrayReader_new(message->bytes, MAX_MESSAGE_SIZE, allocator);
    if (reader == NULL) {
        return -1;
    }

    bobj_parse(reader, allocator, &message->bencoded);

    return 0;
}

#undef WORKSPACE_SIZE
