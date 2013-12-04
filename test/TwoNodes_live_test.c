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
#include "admin/angel/Core.h"
#include "memory/MallocAllocator.h"
#include "memory/Allocator.h"
#include "util/log/FileWriterLog.h"
#include "wire/Message.h"
#include "interface/Interface.h"
#include "util/events/EventBase.h"
#include "crypto/random/Random.h"
#include "crypto/random/libuv/LibuvEntropyProvider.h"
#include "exception/Except.h"
/*
static uint8_t messageToAngel(struct Message* msg, struct Interface* iface)
{
    return 0;
}


struct FirstMessageContext {

};

static void sendFirstMessageToCore(void* vcontext)
{
    struct FirstMessageContext* ctx = vcontext;
    Interface_receiveMessage(ctx->msg, ctx->angelIface);
}

static void startNode(struct Allocator* alloc,
                      struct Log* logger,
                      struct EventBase* base,
                      struct Random* rand,
                      struct Except* eh)
{
    struct Interface* angelIface = Allocator_clone(alloc, (&(struct Interface) {
        .sendMessage = messageToAngel
    }));

    struct Allocator* tempAlloc = Allocator_child(alloc);
    struct FirstMessageContext* firstMessage =
    Timeout_setTimeout(sendFirstMessageToCore, ctx, 0, base, tempAlloc);

    Core_init(alloc, logger, base, angelIface, rand, eh);
}
*/
int main()
{
    struct Except* eh = NULL;
    struct Allocator* alloc = MallocAllocator_new(1<<24);
    struct Log* logger = FileWriterLog_new(stdout, alloc);
    struct EventBase* base = EventBase_new(alloc);
    struct Random* rand = LibuvEntropyProvider_newDefaultRandom(base, logger, eh, alloc);
    Allocator_setCanary(alloc, (unsigned long)Random_uint64(rand));

    //startNode(alloc, logger, base, rand, eh);

    Allocator_free(alloc);
    return 0;
}
