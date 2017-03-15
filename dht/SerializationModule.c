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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "benc/Object.h"
#include "benc/String.h"
#include "dht/DHTMessage.h"
#include "dht/DHTModule.h"
#include "dht/DHTModuleRegistry.h"
#include "memory/Allocator.h"
#include "benc/serialization/standard/BencMessageReader.h"
#include "benc/serialization/standard/BencMessageWriter.h"
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
    Bits_memcpy(context, (&(struct SerializationModule_context) {
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
   // This is always at the end of the message.
    Assert_true(!message->binMessage->length);
    Assert_true(!((uintptr_t)message->binMessage->bytes % 4) || !"alignment fault0");

    String* q = Dict_getStringC(message->asDict, "q");
    if (q) {
        String* txid = Dict_getStringC(message->asDict, "txid");
        Assert_true(txid);
        String* newTxid = String_newBinary(NULL, txid->len + 1, message->allocator);
        newTxid->bytes[0] = '0';
        Bits_memcpy(&newTxid->bytes[1], txid->bytes, txid->len);
        Dict_putStringC(message->asDict, "txid", newTxid, message->allocator);
    }

    BencMessageWriter_write(message->asDict, message->binMessage, NULL);

    Assert_true(!((uintptr_t)message->binMessage->bytes % 4) || !"alignment fault");

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
    struct SerializationModule_context* context = vcontext;
    char* err =
        BencMessageReader_readNoExcept(message->binMessage, message->allocator, &message->asDict);
    if (err) {
        Log_info(context->logger, "Failed to parse message [%s]", err);
        return -2;
    }
    if (message->binMessage->length) {
        Log_info(context->logger, "Message contains [%d] bytes of crap at the end",
                 (int)message->binMessage->length);
    }

    String* q = Dict_getStringC(message->asDict, "q");
    String* txid = Dict_getStringC(message->asDict, "txid");
    if (!txid) {
        Log_info(context->logger, "query with no txid");
        return -2;
    }
    if (q) {
        if (txid->bytes[0] == '1') {
            Log_debug(context->logger, "query txid which appears to be meant for subnode");
            return -2;
        }
    } else {
        if (txid->bytes[0] != '0') {
            Log_debug(context->logger, "reply txid which is not from old pathfinder");
            return -2;
        }
        String* newTxid = String_newBinary(&txid->bytes[1], txid->len - 1, message->allocator);
        Dict_putStringC(message->asDict, "txid", newTxid, message->allocator);
    }

    return 0;
}
