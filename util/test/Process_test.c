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
#include "crypto/random/Random.h"
#include "util/events/EventBase.h"
#include "util/events/Pipe.h"
#include "util/events/Timeout.h"
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "memory/CanaryAllocator.h"
#include "util/events/Process.h"
#include "util/log/Log.h"
#include "util/log/FileWriterLog.h"
#include "util/platform/libc/strlen.h"
#include "util/Assert.h"
#include "wire/Message.h"
#include "wire/Error.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define MESSAGE  "IT WORKS!"
#define MESSAGEB "INDEED"

struct Context {
    struct Allocator* alloc;
    struct EventBase* base;
    struct Log* log;
};

static void onConnectionParent(struct Pipe* p, int status)
{
    Assert_always(!status);
    struct Context* c = p->iface.receiverContext;

    struct Allocator* alloc = Allocator_child(c->alloc);
    uint8_t* bytes = Allocator_calloc(alloc, strlen(MESSAGE) + 1, 1);
    Bits_memcpy(bytes, MESSAGE, strlen(MESSAGE));
    struct Message* m = Allocator_clone(alloc, (&(struct Message) {
        .length = strlen(MESSAGE),
        .padding = 0,
        .capacity = strlen(MESSAGE),
        .alloc = alloc,
        .bytes = bytes
    }));
    printf("Parent sending message [%s]\n", bytes);
    Interface_sendMessage(&p->iface, m);
    Allocator_free(alloc);
}

static uint8_t receiveMessageParent(struct Message* msg, struct Interface* iface)
{
    struct Context* c = iface->receiverContext;
    Assert_always(msg->length == strlen(MESSAGEB));
    Assert_always(!Bits_memcmp(msg->bytes, MESSAGEB, strlen(MESSAGEB)));
    Allocator_free(c->alloc);
    return Error_NONE;
}

static void timeout(void* vNULL)
{
    Assert_always(!"timed out.");
}

static void onConnectionChild(struct Pipe* p, int status)
{
    Assert_always(!status);
    printf("Child connected\n");
}

static uint8_t receiveMessageChild(struct Message* m, struct Interface* iface)
{
    printf("Child received message\n");
    Assert_always(m->length == strlen(MESSAGE));
    Assert_always(!Bits_memcmp(m->bytes, MESSAGE, strlen(MESSAGE)));

    Message_shift(m, -((int)strlen(MESSAGE)) );
    Message_push(m, MESSAGEB, strlen(MESSAGEB));

    Interface_sendMessage(iface, m);

    // shutdown
    struct Context* ctx = iface->receiverContext;
    Allocator_free(ctx->alloc);

    return Error_NONE;
}

static void child(char* name, struct Context* ctx)
{
    struct Pipe* pipe = Pipe_named(name, ctx->base, NULL, ctx->alloc);
    pipe->logger = ctx->log;
    pipe->iface.receiveMessage = receiveMessageChild;
    pipe->iface.receiverContext = ctx;
    pipe->onConnection = onConnectionChild;
    Timeout_setTimeout(timeout, NULL, 2000, ctx->base, ctx->alloc);
    EventBase_beginLoop(ctx->base);
}

int main(int argc, char** argv)
{
    struct Allocator* alloc = CanaryAllocator_new(MallocAllocator_new(1<<20), NULL);
    struct EventBase* eb = EventBase_new(alloc);
    struct Log* log = FileWriterLog_new(stdout, alloc);
    struct Context* ctx = Allocator_calloc(alloc, sizeof(struct Context), 1);
    ctx->alloc = alloc;
    ctx->base = eb;
    ctx->log = log;
printf("Hello world!\n");
    if (argc > 1) {
        child(argv[1], ctx);
        return 0;
    }

    struct Random* rand = Random_new(alloc, log, NULL);
    char name[32] = {0};
    Random_base32(rand, (uint8_t*)name, 31);

    struct Pipe* pipe = Pipe_named(name, eb, NULL, alloc);
    pipe->logger = log;
    pipe->iface.receiveMessage = receiveMessageParent;
    pipe->iface.receiverContext = ctx;
    pipe->onConnection = onConnectionParent;


    char* path = Process_getPath(alloc);
printf("%s>>\n", path);
    Assert_true(path != NULL);
    #ifdef WIN32
        Assert_true(strstr(path, ":\\") == path + 1); /* C:\ */
        Assert_true(strstr(path, ".exe"));
    #else
        Assert_true(path[0] == '/');
    #endif

    char* args[] = { name, NULL };

    Assert_true(!Process_spawn(path, args, eb, alloc));

    Timeout_setTimeout(timeout, NULL, 2000, eb, alloc);

    EventBase_beginLoop(eb);
}
