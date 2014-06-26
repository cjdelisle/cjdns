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
#include "benc/serialization/json/JsonBencSerializer.h"
#include "benc/serialization/BencSerializer.h"
#include "io/ArrayReader.h"
#include "admin/angel/Core.h"
#include "admin/AdminClient.h"
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
#include "crypto/Key.h"
#include "util/log/Log_impl.h"
#include "io/FileReader.h"
#include "io/ArrayWriter.h"
#include "util/Hex.h"

#include "crypto_scalarmult_curve25519.h"

#include <unistd.h> // isatty()

struct NodeContext {
    struct Interface angelIface;
    struct Sockaddr* boundAddr;
    struct Allocator* alloc;
    struct EventBase* base;
    uint8_t privateKeyHex[64];
    String* publicKey;
    struct AdminClient* adminClient;

    char* nodeName;

    /** Admin socket to bind */
    String* bind;

    /** Admin password */
    String* pass;

    /** UDPInterface */
    int ifNum;
    struct Sockaddr* udpAddr;

    struct Log nodeLog;
    struct Log* parentLogger;

    Identity
};

static uint8_t messageToAngel(struct Message* msg, struct Interface* iface)
{
    struct NodeContext* ctx = Identity_check((struct NodeContext*) iface);
    if (ctx->boundAddr) { return 0; }
    struct Allocator* alloc = Allocator_child(ctx->alloc);
    struct Reader* reader = ArrayReader_new(msg->bytes, msg->length, alloc);
    Dict* config = Dict_new(alloc);
    Assert_true(!StandardBencSerializer_get()->parseDictionary(reader, alloc, config));
    Dict* admin = Dict_getDict(config, String_CONST("admin"));
    String* bind = Dict_getString(admin, String_CONST("bind"));
    struct Sockaddr_storage ss;
    Assert_true(!Sockaddr_parse(bind->bytes, &ss));
    ctx->boundAddr = Sockaddr_clone(&ss.addr, ctx->alloc);
    Allocator_free(alloc);
    EventBase_endLoop(ctx->base);
    return 0;
}

static void sendFirstMessageToCore(void* vcontext)
{
    struct NodeContext* ctx = Identity_check((struct NodeContext*) vcontext);
    struct Allocator* alloc = Allocator_child(ctx->alloc);
    struct Message* msg = Message_new(0, 512, alloc);

    Dict* d = Dict_new(alloc);
    Dict_putString(d, String_CONST("privateKey"), String_new(ctx->privateKeyHex, alloc), alloc);

    Dict* logging = Dict_new(alloc);
    {
        Dict_putString(logging, String_CONST("logTo"), String_CONST("stdout"), alloc);
    }
    Dict_putDict(d, String_CONST("logging"), logging, alloc);

    Dict* admin = Dict_new(alloc);
    {
        Dict_putString(admin, String_CONST("bind"), ctx->bind, alloc);
        Dict_putString(admin, String_CONST("pass"), ctx->pass, alloc);
    }
    Dict_putDict(d, String_CONST("admin"), admin, alloc);

    uint8_t buff[512];
    struct Writer* writer = ArrayWriter_new(buff, 512, alloc);
    Assert_true(!StandardBencSerializer_get()->serializeDictionary(writer, d));
    Message_push(msg, buff, Writer_bytesWritten(writer), NULL);

    Interface_receiveMessage(&ctx->angelIface, msg);
    Allocator_free(alloc);
}

struct RPCCall;

typedef void (* RPCCallback)(struct RPCCall* call, struct AdminClient_Result* res);

struct RPCCall
{
    String* func;
    Dict* args;
    struct NodeContext* node;
    RPCCallback callback;
};

struct Context
{
    struct RPCCall* rpcCalls;
    int rpcCallCount;
    int nextCall;
    struct Allocator* rpcAlloc;

    struct Random* rand;
    struct EventBase* base;
    struct Log* logger;
    struct Allocator* alloc;

    struct NodeContext** nodes;
    Dict* confNodes;
    String** names;

    Identity
};

static String* getPublicKey(char* privateKeyHex, struct Allocator* alloc)
{
    uint8_t privateKey[32];
    uint8_t publicKey[32];
    Hex_decode(privateKey, 32, privateKeyHex, 65);
    crypto_scalarmult_curve25519_base(publicKey, privateKey);
    return Key_stringify(publicKey, alloc);
}

static void printLog(struct Log* log,
                     enum Log_Level logLevel,
                     const char* file,
                     int line,
                     const char* format,
                     va_list args)
{
    struct NodeContext* ctx = Identity_check(
        (struct NodeContext*) (((char*)log) - offsetof(struct NodeContext, nodeLog))
    );
    struct Allocator* alloc = Allocator_child(ctx->alloc);
    String* str = String_printf(alloc, "[%s] %s", ctx->nodeName, file);
    ctx->parentLogger->print(ctx->parentLogger, logLevel, str->bytes, line, format, args);
    Allocator_free(alloc);
}

static struct RPCCall* pushCall(struct Context* ctx)
{
    ctx->rpcCalls = Allocator_realloc(ctx->rpcAlloc,
                                      ctx->rpcCalls,
                                      sizeof(struct RPCCall) * (ctx->rpcCallCount+1));
    Bits_memset(&ctx->rpcCalls[ctx->rpcCallCount], 0, sizeof(struct RPCCall));
    return &ctx->rpcCalls[ctx->rpcCallCount++];
}

static void bindUDPCallback(struct RPCCall* call, struct AdminClient_Result* res)
{
    Assert_true(!res->err);
    Log_debug(&call->node->nodeLog, "UDPInterface_new() -> [%s]", res->messageBytes);
    String* addr = Dict_getString(res->responseDict, String_CONST("bindAddress"));
    int64_t* ifNum = Dict_getInt(res->responseDict, String_CONST("interfaceNumber"));
    struct Sockaddr_storage ss;
    Assert_true(!Sockaddr_parse(addr->bytes, &ss));
    call->node->ifNum = *ifNum;
    call->node->udpAddr = Sockaddr_clone(&ss.addr, call->node->alloc);
}

static void bindUDP(struct Context* ctx, struct NodeContext* node)
{
    struct RPCCall* call = pushCall(ctx);
    call->func = String_new("UDPInterface_new", ctx->rpcAlloc);
    call->args = Dict_new(ctx->rpcAlloc);
    call->node = node;
    call->callback = bindUDPCallback;
}

static struct NodeContext* startNode(char* nodeName,
                                     char* privateKeyHex,
                                     Dict* admin,
                                     struct Context* ctx,
                                     struct Except* eh)
{
    struct NodeContext* node = Allocator_clone(ctx->alloc, (&(struct NodeContext) {
        .angelIface = {
            .sendMessage = messageToAngel
        },
        .alloc = ctx->alloc,
        .base = ctx->base,
        .nodeLog = {
            .print = printLog
        },
        .parentLogger = ctx->logger,
        .nodeName = nodeName
    }));
    Identity_set(node);

    node->bind = Dict_getString(admin, String_CONST("bind"));
    if (!node->bind) {
        node->bind = String_new("127.0.0.1:0", ctx->alloc);
    }
    node->pass = Dict_getString(admin, String_CONST("password"));
    if (!node->pass) {
        node->pass = String_new("x", ctx->alloc);
    }

    Bits_memcpyConst(node->privateKeyHex, privateKeyHex, 64);

    Timeout_setTimeout(sendFirstMessageToCore, node, 0, ctx->base, node->alloc);

    Core_init(node->alloc, &node->nodeLog, ctx->base, &node->angelIface, ctx->rand, eh);

    // sendFirstMessageToCore causes the core to react causing messageToAngel which ends the loop
    EventBase_beginLoop(ctx->base);

    node->adminClient = AdminClient_new(node->boundAddr,
                                        node->pass,
                                        ctx->base,
                                        &node->nodeLog,
                                        node->alloc);

    node->adminClient->millisecondsToWait = 120000;

    bindUDP(ctx, node);
    node->publicKey = getPublicKey(privateKeyHex, node->alloc);

    return node;
}

static void beginConnectionCallback(struct RPCCall* call, struct AdminClient_Result* res)
{
    Assert_true(!res->err);
    Log_debug(&call->node->nodeLog, "UDPInterface_beginConnection() -> [%s]", res->messageBytes);
}

static void linkNodes(struct Context* ctx, struct NodeContext* client, struct NodeContext* server)
{
    Dict* addPasswordArgs = Dict_new(ctx->rpcAlloc);
    String* clientStr = String_printf(ctx->rpcAlloc, "%ld", (long) (uintptr_t) client);
    Dict_putString(addPasswordArgs,
                   String_new("password", ctx->rpcAlloc),
                   clientStr,
                   ctx->rpcAlloc);
    Dict_putString(addPasswordArgs,
                   String_new("user", ctx->rpcAlloc),
                   clientStr,
                   ctx->rpcAlloc);
    struct RPCCall* addPasswordCall = pushCall(ctx);
    addPasswordCall->func = String_new("AuthorizedPasswords_add", ctx->rpcAlloc);
    addPasswordCall->args = addPasswordArgs;
    addPasswordCall->node = server;

    // client
    Dict* beginConnectionArgs = Dict_new(ctx->rpcAlloc);
    Dict_putInt(beginConnectionArgs,
                String_new("interfaceNumber", ctx->rpcAlloc),
                client->ifNum,
                ctx->rpcAlloc);
    Dict_putString(beginConnectionArgs,
                   String_new("password", ctx->rpcAlloc),
                   clientStr,
                   ctx->rpcAlloc);
    Dict_putString(beginConnectionArgs,
                   String_new("publicKey", ctx->rpcAlloc),
                   server->publicKey,
                   ctx->rpcAlloc);
    char* udpAddr = Sockaddr_print(server->udpAddr, ctx->rpcAlloc);
    Dict_putString(beginConnectionArgs,
                   String_new("address", ctx->rpcAlloc),
                   String_new(udpAddr, ctx->rpcAlloc),
                   ctx->rpcAlloc);

    Log_info(ctx->logger, "Linking [%s] with [%s/%s]",
             client->nodeName, server->nodeName, udpAddr);

    struct RPCCall* connectCall = pushCall(ctx);
    connectCall->func = String_new("UDPInterface_beginConnection", ctx->rpcAlloc);
    connectCall->args = beginConnectionArgs;
    connectCall->node = client;
    connectCall->callback = beginConnectionCallback;
}

static void linkAllNodes(struct Context* ctx)
{
    int i = 0;
    String* key = NULL;
    Dict_forEach(ctx->confNodes, key) {
        Dict* val = Dict_getDict(ctx->confNodes, key);
        List* connectTo = Dict_getList(val, String_CONST("peers"));
        for (int j = 0; j < List_size(connectTo); j++) {
            String* server = List_getString(connectTo, j);
            Assert_true(server);
            for (int k = 0; k < Dict_size(ctx->confNodes); k++) {
                if (String_equals(server, ctx->names[k])) {
                    linkNodes(ctx, ctx->nodes[i], ctx->nodes[k]);
                    break;
                }
            }
        }
        i++;
    }
    ctx->confNodes = NULL;
}

static void startRpc(void* vcontext);

static void rpcCallback(struct AdminClient_Promise* promise, struct AdminClient_Result* res)
{
    struct Context* ctx = promise->userData;
    Identity_check(ctx);

    struct RPCCall* thisCall = &ctx->rpcCalls[ctx->nextCall];
    if (thisCall->callback) {
        thisCall->callback(thisCall, res);
    }

    ctx->nextCall++;
    startRpc(ctx);
}

static void startRpc(void* vcontext)
{
    struct Context* ctx = vcontext;
    Identity_check(ctx);

    if (ctx->nextCall >= ctx->rpcCallCount) {
        if (ctx->confNodes) {
            linkAllNodes(ctx);
        }
    }
    if (ctx->nextCall >= ctx->rpcCallCount) {
        Log_info(ctx->logger, "\n\nCompleted setting up simulation\n\n");
        Allocator_free(ctx->rpcAlloc);
        ctx->rpcAlloc = NULL;
        ctx->rpcCalls = NULL;
        ctx->rpcCallCount = 0;
        return;
    }

    struct RPCCall* nextCall = &ctx->rpcCalls[ctx->nextCall];
    struct AdminClient_Promise* promise = AdminClient_rpcCall(nextCall->func,
                                                              nextCall->args,
                                                              nextCall->node->adminClient,
                                                              ctx->rpcAlloc);
    promise->callback = rpcCallback;
    promise->userData = ctx;
}

static void letErRip(Dict* config, struct Allocator* alloc)
{
    struct Except* eh = NULL;
    struct Log* logger = FileWriterLog_new(stdout, alloc);
    struct EventBase* base = EventBase_new(alloc);
    struct Random* rand = LibuvEntropyProvider_newDefaultRandom(base, logger, eh, alloc);
    Allocator_setCanary(alloc, (unsigned long)Random_uint64(rand));

    struct Context sctx = {
        .rpcAlloc = Allocator_child(alloc),

        .logger = logger,
        .base = base,
        .rand = rand,
        .alloc = alloc,
    };
    struct Context* ctx = &sctx;
    Identity_set(ctx);

    ctx->confNodes = Dict_getDict(config, String_CONST("nodes"));
    ctx->nodes = Allocator_calloc(alloc, sizeof(char*), Dict_size(ctx->confNodes));
    ctx->names = Allocator_calloc(alloc, sizeof(String*), Dict_size(ctx->confNodes));

    String* key = NULL;
    int i = 0;
    Dict_forEach(ctx->confNodes, key) {
        Dict* val = Dict_getDict(ctx->confNodes, key);
        String* privateKeyHex = Dict_getString(val, String_CONST("privateKey"));
        Dict* admin = Dict_getDict(val, String_CONST("admin"));
        ctx->names[i] = key;
        ctx->nodes[i] = startNode(key->bytes, privateKeyHex->bytes, admin, ctx, eh);
        i++;
    }



    // begin the chain of RPC calls which sets up the net
    Timeout_setTimeout(startRpc, ctx, 0, base, ctx->rpcAlloc);

    EventBase_beginLoop(base);

    Allocator_free(alloc);
}

static int usage(char* appName)
{
    printf("Example usage: %s < config.json\n"
           "Example config:\n"
           "{\n"
           "  \"nodes\": {\n"
           "    \"alice\": {\n"
           "      \"privateKey\": "
               "\"5e2295679394e5e1db67c238abbc10292ad9b127904394c52cc5fff39383e920\",\n"
           "      \"peers\": []\n"
           "    },\n"
           "    \"bob\": {\n"
           "      \"privateKey\": "
               "\"6569bf3f0d168faa6dfb2912f8ee5ee9b938319e97618fdf06caed73b1aad1cc\",\n"
           "      \"peers\": [\n"
           "        \"alice\"\n"
           "      ]\n"
           "    }\n"
           "  }\n"
           "}\n", appName);

    return 0;
}

int main(int argc, char** argv)
{
    Assert_true(argc > 0);
    if (isatty(STDIN_FILENO)) {
        return usage(argv[0]);
    }

    struct Allocator* alloc = MallocAllocator_new(1<<30);

    struct Reader* stdinReader = FileReader_new(stdin, alloc);
    Dict config;
    if (JsonBencSerializer_get()->parseDictionary(stdinReader, alloc, &config)) {
        fprintf(stderr, "Failed to parse configuration.\n");
        return -1;
    }

    letErRip(&config, alloc);
}
