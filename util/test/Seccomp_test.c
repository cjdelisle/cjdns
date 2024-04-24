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
#include "util/Identity.h"
#include "util/log/FileWriterLog.h"
#include "memory/Allocator.h"
#include "util/Seccomp.h"
#include "util/events/EventBase.h"
#include "util/events/Process.h"
#include "util/events/Pipe.h"
#include "util/events/Socket.h"
#include "util/events/Timeout.h"
#include "util/CString.h"
#include "crypto/random/Random.h"
#include "interface/addressable/AddrIface.h"

#include <unistd.h>

struct Context
{
    struct Iface iface;
    struct Allocator* alloc;
    struct EventBase* eventBase;
    Identity
};

struct ChildCtx
{
    struct EventBase* base;
    struct Log* log;
    Iface_t* socket;
    struct Allocator* alloc;
    Identity
};

static int childMessageSent(struct Allocator_OnFreeJob* j) {
    struct ChildCtx* child = Identity_check((struct ChildCtx*) j->userData);
    printf("Child shutting down\n");
    EventBase_endLoop(child->base);
    return 0;
}

static void timeout(void* vNULL)
{
    Assert_true(!"timed out");
}

static void onConnectionChild(struct ChildCtx* child)
{
    Er_assert(Seccomp_dropPermissions(child->alloc, child->log));
    Assert_true(Seccomp_isWorking());

    Allocator_t* alloc = Allocator_child(child->alloc);
    Allocator_t* alloc1 = Allocator_child(alloc);

    struct Message* ok = Message_new(0, 512, alloc1);
    Er_assert(Message_epush(ok, "OK", 3));

    struct Iface iface = { .send = NULL };
    Iface_plumb(child->socket, &iface);
    Iface_send(&iface, ok);

    Allocator_onFree(alloc1, childMessageSent, child);

    Allocator_free(alloc);

    // just set a timeout long enough that we're pretty sure the parent will get the message
    // before we quit.
    // Timeout_setInterval(childComplete, child->base, 10, child->base, child->alloc);
    Timeout_setTimeout(timeout, child->base, 2000, child->base, alloc);
    EventBase_beginLoop(child->base);
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
    ctx->socket = Er_assert(Socket_connect(pipeName, alloc));
    Identity_set(ctx);
    onConnectionChild(ctx);
    return 0;
}

static Iface_DEFUN receiveMessageParent(struct Message* msg, struct Iface* iface)
{
    struct Context* ctx = Identity_check((struct Context*) iface);
    // PipeServer pushes a uint32 identifier of the client who sent the message
    Er_assert(AddrIface_popAddr(msg));
    Assert_true(Message_getLength(msg) == 3);
    Assert_true(!Bits_memcmp(msg->msgbytes, "OK", 3));
    printf("Parent got reply\n");
    EventBase_endLoop(ctx->eventBase);
    return NULL;
}

int main(int argc, char** argv)
{
    struct Allocator* alloc = Allocator_new(20000);
    struct Log* logger = FileWriterLog_new(stdout, alloc);

    if (!Seccomp_exists()) {
        Log_debug(logger, "Seccomp not supported on this system");
        return 0;
    }
    if (argc > 3 && !CString_strcmp("Seccomp_test", argv[1]) && !CString_strcmp("child", argv[2])) {
        child(argv[3], alloc, logger);
        // Allocator_free(alloc); 
        // TODO: Freeing the allocator causes a Identity_check() assertion crash in Pipe.c, replace after async allocator free is abolished.
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

    Socket_Server_t* ss = Er_assert(Socket_server(name->bytes, alloc));
    Iface_plumb(&ctx->iface, ss->iface);

    const char* path = Process_getPath(alloc);
    const char* args[] = { "Seccomp_test", "child", name->bytes, NULL };

    Assert_true(!Process_spawn(path, args, eb, alloc, NULL));

    Timeout_setTimeout(timeout2, NULL, 2000, eb, alloc);

    EventBase_beginLoop(eb);
    unlink(name->bytes);
    return 0;
}
