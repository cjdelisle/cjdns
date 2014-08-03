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
#include "dht/DHTModule.h"
#include "dht/DHTModuleRegistry.h"
#include "memory/Allocator.h"
#include "util/Assert.h"

#include <stddef.h> // NULL

#define DEBUG2(x, y)
/* #define DEBUG2(x, y) fprintf(stderr, x, y); fflush(stderr) */

struct DHTModuleRegistry* DHTModuleRegistry_new(struct Allocator* allocator)
{
    struct DHTModuleRegistry* reg =
        Allocator_calloc(allocator, sizeof(struct DHTModuleRegistry), 1);
    reg->allocator = allocator;
    reg->members = Allocator_calloc(allocator, sizeof(char*), 1);
    return reg;
}

int DHTModuleRegistry_register(struct DHTModule* module,
                               struct DHTModuleRegistry* registry)
{
    registry->members = Allocator_realloc(registry->allocator,
                                          registry->members,
                                          sizeof(char*) * (registry->memberCount + 2));

    registry->members[registry->memberCount] = module;
    registry->memberCount++;
    registry->members[registry->memberCount] = NULL;

    return 0;
}

void DHTModuleRegistry_handleIncoming(struct DHTMessage* message,
                                      const struct DHTModuleRegistry* registry)
{
    if (!(message && registry && registry->members && registry->memberCount)) {
        return;
    }

    struct DHTModule** firstModulePtr = registry->members;
    struct DHTModule** modulePtr = registry->members + registry->memberCount - 1;
    struct DHTModule* module;

    while (modulePtr >= firstModulePtr) {
        module = *modulePtr;
        if (module && module->handleIncoming) {
            DEBUG2("<< calling: %s->handleIncoming\n", module->name);
            if (module->handleIncoming(message, module->context) != 0) {
                // TODO(cjd): Call a debugger with all unhandlable messages?
                return;
            }
        } else {
            DEBUG2("<< skipping %s->handleIncoming\n", module->name);
        }
        modulePtr--;
    }
}

static int dhtModuleHandleOutgoing(struct DHTModule* module, struct DHTMessage* message)
{
    Assert_ifParanoid(module);
    if (module->handleOutgoing) {
        DEBUG2(">> calling: %s->handleOutgoing\n", module->name);
        return module->handleOutgoing(message, module->context);
    } else {
        DEBUG2(">> skipping: %s->handleOutgoing\n", module->name);
    }
    return 0;
}

/**
 * Do something to every module which is registered.
 * @param doThis the callback.
 * @param registry state.
 */
static void forEachModule(int (*doThis)(struct DHTModule* module, struct DHTMessage* message),
                          struct DHTMessage* message,
                          const struct DHTModuleRegistry* registry)
{
    struct DHTModule** modulePtr = registry->members;
    struct DHTModule* module = *modulePtr;
    while (module) {
        if (doThis(module, message) != 0) {
            return;
        }
        modulePtr++;
        module = *modulePtr;
    }
}

void DHTModuleRegistry_handleOutgoing(struct DHTMessage* message,
                                      const struct DHTModuleRegistry* registry)
{
    forEachModule(dhtModuleHandleOutgoing, message, registry);
}
