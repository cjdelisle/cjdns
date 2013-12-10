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
#include "benc/serialization/standard/StandardBencSerializer.h"
#include "benc/serialization/BencSerializer.h"
#include "io/ArrayReader.h"
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
#include "util/events/Timeout.h"

struct NodeContext {
    struct Interface angelIface;
    struct Sockaddr* boundAddr;
    struct Allocator* alloc;
    struct EventBase* base;
    uint8_t privateKeyHex[64];

    Identity
};

static uint8_t messageToAngel(struct Message* msg, struct Interface* iface)
{
    struct NodeContext* ctx = Identity_cast((struct NodeContext*) iface);
    if (ctx->boundAddr) { return 0; }
    struct Allocator* alloc = Allocator_child(ctx->alloc);
    struct Reader* reader = ArrayReader_new(msg->bytes, msg->length, alloc);
    Dict* config = Dict_new(alloc);
    Assert_always(!StandardBencSerializer_get()->parseDictionary(reader, alloc, config));
    Dict* admin = Dict_getDict(config, String_CONST("admin"));
    String* bind = Dict_getString(admin, String_CONST("bind"));
    struct Sockaddr_storage ss;
    Assert_always(!Sockaddr_parse(bind->bytes, &ss));
    ctx->boundAddr = Sockaddr_clone(&ss.addr, ctx->alloc);
    Allocator_free(alloc);
    EventBase_endLoop(ctx->base);
    return 0;
}

static void sendFirstMessageToCore(void* vcontext)
{
    struct NodeContext* ctx = Identity_cast((struct NodeContext*) vcontext);
    struct Allocator* alloc = Allocator_child(ctx->alloc);
    struct Message* msg = Message_new(0, 512, alloc);
    String* messageStr = String_new(
        "d"
            "10:privateKey" "64:xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
             "5:admin" "d"
                 "4:pass" "1:x"
                 "4:bind" "11:127.0.0.1:0"
             "e"
             "7:logging" "d"
                 "5:logTo" "6:stdout"
             "e"
        "e", alloc);
    char* privateKeyHex = Bits_memmem(messageStr->bytes, "xxxx", messageStr->len, 4);
    Bits_memcpyConst(privateKeyHex, ctx->privateKeyHex, 64);
    Message_push(msg, messageStr->bytes, messageStr->len, NULL);
    Interface_receiveMessage(&ctx->angelIface, msg);
    Allocator_free(alloc);
}

static struct NodeContext* startNode(char* privateKeyHex,
                                     struct Allocator* alloc,
                                     struct Log* logger,
                                     struct EventBase* base,
                                     struct Random* rand,
                                     struct Except* eh)
{
    struct NodeContext* ctx = Allocator_clone(alloc, (&(struct NodeContext) {
        .angelIface = {
            .sendMessage = messageToAngel
        },
        .alloc = alloc,
        .base = base
    }));
    Identity_set(ctx);

    Bits_memcpyConst(ctx->privateKeyHex, privateKeyHex, 64);

    Timeout_setTimeout(sendFirstMessageToCore, ctx, 0, base, alloc);

    Core_init(alloc, logger, base, &ctx->angelIface, rand, eh);

    // sendFirstMessageToCore causes the core to react causing messageToAngel which ends the loop
    EventBase_beginLoop(base);

    return ctx;
}

static void linkNodes(struct NodeContext* client, struct NodeContext* server)
{
}

int main()
{
    struct Except* eh = NULL;
    struct Allocator* alloc = MallocAllocator_new(1<<24);
    struct Log* logger = FileWriterLog_new(stdout, alloc);
    struct EventBase* base = EventBase_new(alloc);
    struct Random* rand = LibuvEntropyProvider_newDefaultRandom(base, logger, eh, alloc);
    Allocator_setCanary(alloc, (unsigned long)Random_uint64(rand));

    char* privateKeyA = "5e2295679394e5e1db67c238abbc10292ad9b127904394c52cc5fff39383e920";
    struct NodeContext* nodeA = startNode(privateKeyA, alloc, logger, base, rand, eh);

    char* privateKeyB = "6569bf3f0d168faa6dfb2912f8ee5ee9b938319e97618fdf06caed73b1aad1cc";
    struct NodeContext* nodeB = startNode(privateKeyB, alloc, logger, base, rand, eh);

    linkNodes(nodeA, nodeB);

    //EventBase_beginLoop(base);

    Allocator_free(alloc);
    return 0;
}
