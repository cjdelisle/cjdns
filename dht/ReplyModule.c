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

#include "dht/CJDHTConstants.h"
#include "dht/DHTMessage.h"
#include "dht/DHTModule.h"
#include "dht/DHTModuleRegistry.h"
#include "benc/Object.h"
#include "wire/Message.h"

/**
 * The reply module replies to all incoming queries.
 * It also modifies outgoing replies to make sure that a reply packet has the
 * correct transaction id and is labeled as a reply. It adds the "y":"r" and
 * the "t":"aa" to the packet.
 * It is the core of the cjdns dht engine.
 */

/*--------------------Prototypes--------------------*/
static int handleIncoming(struct DHTMessage* message, void* vcontext);
static int handleOutgoing(struct DHTMessage* message, void* vcontext);

/*--------------------Interface--------------------*/

/**
 * Register a new ReplyModule.
 *
 * @param registry the DHT module registry for signal handling.
 * @param allocator a means to allocate memory.
 */
void ReplyModule_register(struct DHTModuleRegistry* registry, struct Allocator* allocator)
{
    struct DHTModule* dm = Allocator_clone(allocator, (&(struct DHTModule) {
        .name = "ReplyModule",
        // We use the registry itself as the context
        .context = registry,
        .handleIncoming = handleIncoming,
        .handleOutgoing = handleOutgoing
    }));
    DHTModuleRegistry_register(dm, registry);
}

static int handleIncoming(struct DHTMessage* message, void* vcontext)
{
    if (Dict_getString(message->asDict, CJDHTConstants_QUERY) == NULL) {
        return 0;
    }

    struct DHTModuleRegistry* registry = (struct DHTModuleRegistry*) vcontext;

    Message_reset(message->binMessage);
    Assert_true(!((uintptr_t)message->binMessage->bytes % 4) || !"alignment fault0");

    struct DHTMessage* reply = Allocator_clone(message->allocator, (&(struct DHTMessage) {
        .replyTo = message,
        .address = message->address,
        .allocator = message->allocator,
        .binMessage = message->binMessage
    }));

    DHTModuleRegistry_handleOutgoing(reply, registry);

    return 0;
}

static int handleOutgoing(struct DHTMessage* message, void* vcontext)
{
    if (message->replyTo != NULL) {
        if (message->asDict == NULL) {
            message->asDict = Dict_new(message->allocator);
        }

        // Put the transaction ID
        String* tid = Dict_getString(message->replyTo->asDict, CJDHTConstants_TXID);
        if (tid != NULL) {
            Dict_putString(message->asDict, CJDHTConstants_TXID, tid, message->allocator);
        }
    }
    return 0;
}
