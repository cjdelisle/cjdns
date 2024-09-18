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
#include "dht/SerializationModule.h"
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
    Assert_true(!Message_getLength(message->binMessage));
    Assert_true(!((uintptr_t)Message_bytes(message->binMessage) % 4) || !"alignment fault0");

    if (Dict_getStringC(message->asDict, "q")) {
        String* txid = Dict_getStringC(message->asDict, "txid");
        Assert_true(txid);
        String* newTxid = String_newBinary(NULL, txid->len + 2, message->allocator);
        newTxid->bytes[0] = '0';
        newTxid->bytes[1] = '0';
        Bits_memcpy(&newTxid->bytes[2], txid->bytes, txid->len);
        Dict_putStringC(message->asDict, "txid", newTxid, message->allocator);
    }

    Err_assert(BencMessageWriter_write(message->asDict, message->binMessage));

    Assert_true(!((uintptr_t)Message_bytes(message->binMessage) % 4) || !"alignment fault");

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
    const char* err =
        BencMessageReader_readNoExcept(message->binMessage, message->allocator, &message->asDict);
    if (err) {
        Log_info(context->logger, "Failed to parse message [%s]", err);
        return -2;
    }
    if (Message_getLength(message->binMessage)) {
        Log_info(context->logger, "Message contains [%d] bytes of crap at the end",
                 (int)Message_getLength(message->binMessage));
    }

    String* q = Dict_getStringC(message->asDict, "q");
    String* txid = Dict_getStringC(message->asDict, "txid");
    if (!txid) {
        Log_info(context->logger, "query with no txid");
        return -2;
    }
    if (!q) {
        if (txid->len < 2 || txid->bytes[0] != '0' || txid->bytes[1] != '0') {
            // spammy
            //Log_debug(context->logger, "reply txid which is not from old pathfinder");
            return -2;
        }
        String* newTxid = String_newBinary(&txid->bytes[2], txid->len - 2, message->allocator);
        Dict_putStringC(message->asDict, "txid", newTxid, message->allocator);
    }

    return 0;
}
