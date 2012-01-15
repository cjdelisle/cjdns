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

#include <benc/Object.h>
#include <memory/Allocator.h>
#include <memory/BufferAllocator.h>
#include <io/Reader.h>
#include <io/ArrayReader.h>
#include <io/Writer.h>
#include <io/ArrayWriter.h>
#include "benc/serialization/BencSerializer.h"
#include "benc/serialization/standard/StandardBencSerializer.h"

#include <string.h>

#define SERIALIZER List_getStandardBencSerializer()

struct SerializationModule_context {
    struct DHTModule module;
};

/*--------------------Prototypes--------------------*/
static int handleOutgoing(struct DHTMessage* message,
                          void* vcontext);
static int handleIncoming(struct DHTMessage* message,
                          void* vcontext);

/*--------------------Interface--------------------*/

void SerializationModule_register(struct DHTModuleRegistry* registry, const struct Allocator* allocator)
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

    DHTModules_register(&(context->module), registry);
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

    SERIALIZER->serializeDictionary(writer, message->asDict);
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
    message->asDict = message->allocator->malloc(sizeof(Dict), message->allocator);

    vcontext = vcontext;
    struct Reader* reader = ArrayReader_new(message->bytes, MAX_MESSAGE_SIZE, message->allocator);

    if (SERIALIZER->parseDictionary(reader, message->allocator, message->asDict) != 0) {
        return -2;
    }

    return 0;
}
