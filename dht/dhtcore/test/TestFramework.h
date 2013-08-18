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
#ifndef TestFramework_H
#define TestFramework_H

#include "memory/Allocator.h"
#include "dht/DHTModules.h"
#include "dht/CJDHTConstants.h"

static int catchOutgoing(struct DHTMessage* message, void* vcontext);
static int bounceMessage(struct DHTMessage* message, void* vcontext);

static void TestFramework_registerBouncerModule(struct DHTModuleRegistry* registry,
                                                struct Allocator* allocator)
__attribute__((unused));

static void TestFramework_registerBouncerModule(struct DHTModuleRegistry* registry,
                                                struct Allocator* allocator)
{
    struct DHTModule* module =
        Allocator_clone(allocator, (&(struct DHTModule) {
            .context = registry,
            .handleIncoming = bounceMessage
        });
    DHTModules_register(module, registry);
}

static void TestFramework_registerOutputCatcher(struct DHTMessage** messagePointer,
                                                struct DHTModuleRegistry* registry,
                                                struct Allocator* allocator)
{
    struct DHTModule* module =
        Allocator_clone(allocator, (&(struct DHTModule) {
            .context = messagePointer,
            .handleOutgoing = catchOutgoing
        });
    DHTModules_register(module, registry);
}

// A little module which just sends an empty reply to any incoming message.
// Used to get a response from the store.
static int bounceMessage(struct DHTMessage* message, void* vcontext)
{
    struct DHTModuleRegistry* registry = (struct DHTModuleRegistry*) vcontext;
    struct DHTMessage* reply =
        message->allocator->malloc(sizeof(struct DHTMessage), message->allocator);
    reply->replyTo = message;
    reply->allocator = message->allocator;
    reply->asDict = Dict_new(reply->allocator);

String* queryType = Dict_getString(message->asDict, CJDHTConstants_QUERY);
printf("bouncing message %s", queryType->bytes);

    DHTModules_handleOutgoing(reply, registry);
    return 0;
}

// A module to catch responses and make a pointer to the message available.
static int catchOutgoing(struct DHTMessage* message, void* vcontext)
{
    *((struct DHTMessage**)vcontext) = message;
    return 0;
}
