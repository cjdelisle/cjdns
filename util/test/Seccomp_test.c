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
#include "util/log/FileWriterLog.h"
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "util/Seccomp.h"
#include "util/events/EventBase.h"
#include "util/events/Process.h"
#include "util/events/Pipe.h"
#include "util/events/Timeout.h"
#include "util/CString.h"
#include "crypto/random/Random.h"

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

static void onConnectionChild(struct Pipe* p, int status)
{
    // hax
    struct Allocator* alloc = (struct Allocator*) p->userData;
    struct Log* logger = p->logger;

    Seccomp_dropPermissions(alloc, logger, NULL);
    Assert_true(Seccomp_isWorking());

    struct Message* ok = Message_new(0, 512, alloc);
    Message_push(ok, "OK", 3, NULL);

    struct Iface iface = { .send = NULL };
    Iface_plumb(&p->iface, &iface);
    Iface_send(&iface, ok);

    // just set a timeout long enough that we're pretty sure the parent will get the message
    // before we quit.
    Timeout_setInterval(childComplete, p->base, 10, p->base, alloc);
}

static void timeout(void* vNULL)
{
    Assert_true(!"timed out");
}

static int child(char* pipeName, struct Allocator* alloc, struct Log* logger)
{
    struct EventBase* eb = EventBase_new(alloc);
    struct Pipe* pipe = Pipe_named(Pipe_PATH, pipeName, eb, NULL, alloc);
    pipe->onConnection = onConnectionChild;
    pipe->logger = logger;
    pipe->userData = alloc;

    Timeout_setTimeout(timeout, eb, 2000, eb, alloc);
    EventBase_beginLoop(eb);

    return 0;
}

static Iface_DEFUN receiveMessageParent(struct Message* msg, struct Iface* iface)
{
    struct Context* ctx = Identity_check((struct Context*) iface);
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
    char name[32] = {0};
    Random_base32(rand, (uint8_t*)name, 31);

    struct Context* ctx = Allocator_calloc(alloc, sizeof(struct Context), 1);
    Identity_set(ctx);
    ctx->alloc = alloc;
    ctx->iface.send = receiveMessageParent;
    ctx->eventBase = eb;

    struct Pipe* pipe = Pipe_named(Pipe_PATH, name, eb, NULL, alloc);
    pipe->logger = logger;
    Iface_plumb(&ctx->iface, &pipe->iface);

    char* path = Process_getPath(alloc);
    char* args[] = { "Seccomp_test", "child", name, NULL };

    Assert_true(!Process_spawn(path, args, eb, alloc, NULL));

    Timeout_setTimeout(timeout, NULL, 2000, eb, alloc);

    EventBase_beginLoop(eb);
    return 0;
}
