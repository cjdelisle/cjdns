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
#include "dht/DHTModule.h"
#include "dht/DHTModuleRegistry.h"
#include "memory/Allocator.h"

#include <stdio.h>

struct Context
{
    struct DHTMessage* theMessage;
    int ret;
};

static int handleIncoming(struct DHTMessage* message, void* vcontext)
{
    struct Context* context = (struct Context*) vcontext;
    if (message == context->theMessage) {
        context->ret = 0;
    } else {
        context->ret = -2;
    }
    return 0;
}

static int testInputHandler()
{
    struct DHTMessage theMessage;

    struct Context context =
    {
        .theMessage = &theMessage,
        .ret = -1
    };

    struct Context context2 =
    {
        .theMessage = &theMessage,
        .ret = -1
    };

    struct DHTModule module = {
        .name = "TestModule",
        .context = &context,
        .handleIncoming = handleIncoming
    };
    struct DHTModule module2 = {
        .name = "TestModule2",
        .context = &context2,
        .handleIncoming = handleIncoming
    };

    struct Allocator* allocator = Allocator_new(2048);

    struct DHTModuleRegistry* reg = DHTModuleRegistry_new(allocator, NULL);
    DHTModuleRegistry_register(&module, reg);
    DHTModuleRegistry_register(&module2, reg);

    DHTModuleRegistry_handleIncoming(&theMessage, reg);

    /* This should be ignored. */
    DHTModuleRegistry_handleOutgoing(&theMessage, reg);

    if (context.ret == -1) {
        printf("message not received");
    } else if (context.ret == -2) {
        printf("wrong message received");
    } else if (context2.ret == -1) {
        printf("message not received by all handlers.");
    } else if (context2.ret == -2) {
        printf("wrong message received by second handler.");
    } else {
        Allocator_free(allocator);
        return 0;
    }
    return -1;
}

int main()
{
    return testInputHandler();
}
