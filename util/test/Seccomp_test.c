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
#include "benc/String.h"
#include "util/log/FileWriterLog.h"
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "util/Seccomp.h"
#include "util/events/EventBase.h"
#include "util/events/Process.h"
#include "util/events/PipeServer.h"
#include "util/events/Pipe.h"
#include "util/events/Timeout.h"
#include "util/CString.h"
#include "crypto/random/Random.h"

#include <unistd.h>

struct Context
{
    struct Iface iface;
    struct Allocator* alloc;
    struct EventBase* eventBase;
    Identity
};

static void childComplete(void* vEventBase)
{
    EventBase_endLoop((struct EventBase*)vEventBase);
}

struct ChildCtx
{
    struct EventBase* base;
    struct Log* log;
    struct Pipe* pipe;
    struct Allocator* alloc;
    Identity
};

static void onConnectionChild(struct Pipe* pipe, int status)
{
    struct ChildCtx* child = Identity_check((struct ChildCtx*) pipe->userData);

    Er_assert(Seccomp_dropPermissions(child->alloc, child->log));
    Assert_true(Seccomp_isWorking());

    struct Message* ok = Message_new(0, 512, child->alloc);
    Message_push(ok, "OK", 3, NULL);

    struct Iface iface = { .send = NULL };
    Iface_plumb(&pipe->iface, &iface);
    Iface_send(&iface, ok);

    // just set a timeout long enough that we're pretty sure the parent will get the message
    // before we quit.
    Timeout_setInterval(childComplete, child->base, 10, child->base, child->alloc);
}

static void timeout(void* vNULL)
{
    Assert_true(!"timed out");
}

static void timeout2(void* vNULL)
{
    Assert_true(!"time out 2");
}

static int child(char* pipeName, struct Allocator* alloc, struct Log* logger)
{
    struct ChildCtx* ctx = Allocator_calloc(alloc, sizeof(struct ChildCtx), 1);
    ctx->base = EventBase_new(alloc);
    ctx->alloc = alloc;
    ctx->log = logger;
    ctx->pipe = Er_assert(Pipe_named(pipeName, ctx->base, logger, alloc));
    ctx->pipe->onConnection = onConnectionChild;
    ctx->pipe->userData = ctx;
    Identity_set(ctx);
    Timeout_setTimeout(timeout, ctx->base, 2000, ctx->base, alloc);
    EventBase_beginLoop(ctx->base);

    return 0;
}

static Iface_DEFUN receiveMessageParent(struct Message* msg, struct Iface* iface)
{
    struct Context* ctx = Identity_check((struct Context*) iface);
    // PipeServer pushes a uint32 identifier of the client who sent the message
    AddrIface_popAddr(msg, NULL);
    Assert_true(msg->length == 3);
    Assert_true(!Bits_memcmp(msg->bytes, "OK", 3));
    EventBase_endLoop(ctx->eventBase);
    return 0;
}

int main(int argc, char** argv)
{
    struct Allocator* alloc = MallocAllocator_new(20000);
    struct Log* logger = FileWriterLog_new(stdout, alloc);

    if (!Seccomp_exists()) {
        Log_debug(logger, "Seccomp not supported on this system");
        return 0;
    }
    if (argc > 3 && !CString_strcmp("Seccomp_test", argv[1]) && !CString_strcmp("child", argv[2])) {
        child(argv[3], alloc, logger);
        Allocator_free(alloc);
        return 0;
    }

    struct EventBase* eb = EventBase_new(alloc);
    struct Random* rand = Random_new(alloc, logger, NULL);
    char randName[32] = {0};
    Random_base32(rand, (uint8_t*)randName, 31);
    String* name = String_printf(alloc, "%s%scjdns-test-%s", Pipe_PATH, Pipe_PATH_SEP, randName);

    struct Context* ctx = Allocator_calloc(alloc, sizeof(struct Context), 1);
    Identity_set(ctx);
    ctx->alloc = alloc;
    ctx->iface.send = receiveMessageParent;
    ctx->eventBase = eb;

    struct PipeServer* pipe = PipeServer_named(name->bytes, eb, NULL, logger, alloc);
    Iface_plumb(&ctx->iface, &pipe->iface.iface);

    char* path = Process_getPath(alloc);
    char* args[] = { "Seccomp_test", "child", name->bytes, NULL };

    Assert_true(!Process_spawn(path, args, eb, alloc, NULL));

    Timeout_setTimeout(timeout2, NULL, 2000, eb, alloc);

    EventBase_beginLoop(eb);
    unlink(name->bytes);
    return 0;
}
