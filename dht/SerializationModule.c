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

struct SerializationModule_context {
    struct DHTModule module;
};

/*--------------------Prototypes--------------------*/
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
#include<assert.h>
struct DHTModule* SerializationModule_new(struct MemAllocator* allocator)
{
    struct SerializationModule_context* context =
        allocator->malloc(sizeof(struct SerializationModule_context), allocator);
    memcpy(context, &(struct SerializationModule_context) {
        .module = {
            .name = "SerializationModule",
            .context = context,
            .handleIncoming = handleIncoming,
            .handleOutgoing = handleOutgoing
        }
    }, sizeof(struct SerializationModule_context));

    assert(context == context->module.context);

    return &context->module;
}

/*--------------------Internals--------------------*/

/**
 * Take an outgoing message and serialize the bencoded message.
 *
 * @see DHTModule->handleOutgoing in DHTModules.h
 */
static int handleOutgoing(struct DHTMessage* message,
                          void* vcontext)
{
    vcontext = vcontext;
    struct Writer* writer = ArrayWriter_new(message->bytes, MAX_MESSAGE_SIZE, message->allocator);
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
    vcontext = vcontext;
    struct Reader* reader = ArrayReader_new(message->bytes, MAX_MESSAGE_SIZE, message->allocator);

    bobj_parse(reader, message->allocator, &message->bencoded);

    /* sanity check. */
    if (message->bencoded == NULL || message->bencoded->type != BENC_DICT) {
        return -1;
    }
    message->asDict = &(message->bencoded->as.dict);

    return 0;
}
