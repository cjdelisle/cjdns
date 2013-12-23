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
#include "dht/DHTMessage.h"
#include "dht/DHTModule.h"
#include "dht/DHTModuleRegistry.h"
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"

#include <stdio.h>

struct Context
{
    struct DHTMessage* theMessage;
    int ret;
};

static int handleOutgoing(struct DHTMessage* message, void* vcontext)
{
    struct Context* context = (struct Context*) vcontext;
    if (message == context->theMessage) {
        context->ret = 0;
    } else {
        context->ret = -2;
    }
    return 0;
}

static int testOutputHandler()
{
    struct DHTMessage theMessage;

    struct Context context =
    {
        .theMessage = &theMessage,
        .ret = -1
    };

    struct DHTModule module = {
        .name = "TestModule",
        .context = &context,
        .handleOutgoing = handleOutgoing
    };

    struct Allocator* allocator = MallocAllocator_new(2048);

    struct DHTModuleRegistry* reg = DHTModuleRegistry_new(allocator);
    DHTModuleRegistry_register(&module, reg);

    DHTModuleRegistry_handleOutgoing(&theMessage, reg);

    /* These should be ignored. */
    DHTModuleRegistry_handleIncoming(&theMessage, reg);

    if (context.ret == -1) {
        printf("message not received");
    } else if (context.ret == -2) {
        printf("wrong message received");
    } else {
        Allocator_free(allocator);
        return 0;
    }
    return -1;
}

int main()
{
    return testOutputHandler();
}
