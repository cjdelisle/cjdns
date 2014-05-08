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
#include "benc/Object.h"
#include "dht/DHTMessage.h"
#include "dht/DHTModule.h"
#include "dht/DHTModuleRegistry.h"
#include "memory/Allocator.h"
#include "io/Reader.h"
#include "io/ArrayReader.h"
#include "io/Writer.h"
#include "io/ArrayWriter.h"
#include "benc/serialization/BencSerializer.h"
#include "benc/serialization/standard/StandardBencSerializer.h"
#include "util/Bits.h"
#include "util/log/Log.h"
#include "wire/Message.h"


#define SERIALIZER StandardBencSerializer_get()

struct SerializationModule_context {
    struct DHTModule module;
    struct Log* logger;
};

/*--------------------Prototypes--------------------*/
static int handleOutgoing(struct DHTMessage* message,
                          void* vcontext);
static int handleIncoming(struct DHTMessage* message,
                          void* vcontext);

/*--------------------Interface--------------------*/

void SerializationModule_register(struct DHTModuleRegistry* registry,
                                  struct Log* logger,
                                  struct Allocator* allocator)
{
    struct SerializationModule_context* context =
        Allocator_malloc(allocator, sizeof(struct SerializationModule_context));
    Bits_memcpyConst(context, (&(struct SerializationModule_context) {
        .module = {
            .name = "SerializationModule",
            .context = context,
            .handleIncoming = handleIncoming,
            .handleOutgoing = handleOutgoing
        },
        .logger = logger
    }), sizeof(struct SerializationModule_context));

    DHTModuleRegistry_register(&(context->module), registry);
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
    uint8_t buff[DHTMessage_MAX_SIZE];
    struct Writer* writer = ArrayWriter_new(buff, DHTMessage_MAX_SIZE, message->allocator);
    SERIALIZER->serializeDictionary(writer, message->asDict);
    while ((writer->bytesWritten + message->binMessage->length) % 8) {
        Message_push8(message->binMessage, 0, NULL);
    }
    message->binMessage->length = 0;
    Message_push(message->binMessage, buff, writer->bytesWritten, NULL);

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
    message->asDict = Dict_new(message->allocator);

    struct Reader* reader = ArrayReader_new(message->binMessage->bytes,
                                            message->binMessage->length,
                                            message->allocator);

    int ret = SERIALIZER->parseDictionary(reader, message->allocator, message->asDict);
    if (ret != 0) {
        #ifdef Log_INFO
            struct SerializationModule_context* context = vcontext;
            Log_info(context->logger, "Failed to parse message [%d]", ret);
        #endif
        return -2;
    }
    if (message->binMessage->length != (int)reader->bytesRead) {
        #ifdef Log_INFO
            struct SerializationModule_context* context = vcontext;
            Log_info(context->logger, "Message contains [%d] bytes of crap at the end",
                     (int)(message->binMessage->length - (int)reader->bytesRead));
        #endif
    }
    Message_reset(message->binMessage);

    return 0;
}
