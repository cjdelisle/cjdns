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
#include "crypto/random/Random.h"
#include "util/events/EventBase.h"
#include "util/events/PipeServer.h"
#include "util/events/Pipe.h"
#include "util/events/Timeout.h"
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "util/events/Process.h"
#include "util/log/Log.h"
#include "util/log/FileWriterLog.h"
#include "util/CString.h"
#include "util/Assert.h"
#include "wire/Message.h"
#include "wire/Error.h"

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MESSAGE  "IT WORKS!"
#define MESSAGEB "INDEED"

struct Context {
    struct Iface iface;
    struct Allocator* alloc;
    struct EventBase* base;
    struct Log* log;

    // Parent only
    int fd;

    // child only
    char* name;
    Identity
};

static void onConnectionParent(struct PipeServer* p, struct Sockaddr* addr)
{
    struct Context* c = Identity_check((struct Context*) p->userData);
    struct Allocator* alloc = Allocator_child(c->alloc);
    struct Message* msg = Message_new(0, 256, alloc);
    Er_assert(Message_epush(msg, MESSAGE, CString_strlen(MESSAGE) + 1));
    Er_assert(AddrIface_pushAddr(msg, addr));
    if (!Defined(win32)) {
        Message_setAssociatedFd(msg, c->fd);
    }
    printf("Parent sending message [%s] len [%d]\n", MESSAGE, msg->length);
    Iface_send(&c->iface, msg);
    Allocator_free(alloc);
}

static Iface_DEFUN receiveMessageParent(struct Message* msg, struct Iface* iface)
{
    struct Context* c = Identity_check((struct Context*) iface);
    Er_assert(AddrIface_popAddr(msg));
    Assert_true(msg->length == (int)CString_strlen(MESSAGEB)+1);
    Assert_true(!Bits_memcmp(msg->bytes, MESSAGEB, CString_strlen(MESSAGEB)+1));
    Allocator_free(c->alloc);
    return Error(NONE);
}

static void timeout(void* vNULL)
{
    Assert_true(!"timed out.");
}

static void onConnectionChild(struct Pipe* p, int status)
{
    Assert_true(!status);
    printf("Child connected\n");
}

static Iface_DEFUN receiveMessageChild(struct Message* msg, struct Iface* iface)
{
    struct Context* c = Identity_check((struct Context*) iface);
    struct Message* m = Message_clone(msg, c->alloc);
    printf("Child received message\n");
    Assert_true(m->length == (int)CString_strlen(MESSAGE)+1);
    Assert_true(!Bits_memcmp(m->bytes, MESSAGE, CString_strlen(MESSAGE)+1));

    if (!Defined(win32)) {
        int fd = Message_getAssociatedFd(msg);
        if (lseek(fd, 0, SEEK_SET) < 0) {
            printf("lseek(%d) failed: errno %s\n", fd, strerror(errno));
            Assert_failure("lseek()");
        }
        uint8_t* buf = Allocator_calloc(msg->alloc, 2048, 1);
        if (read(fd, buf, 1024) < 0) {
            printf("read(%d) failed: errno %s\n", fd, strerror(errno));
            Assert_failure("read()");
        }
        if (CString_strncmp(buf, c->name, 1024)) {
            printf("want: %s\n"
                   "got:  %s", c->name, buf);
            Assert_failure("file content is wrong");
        }
    }

    Er_assert(Message_eshift(m, -((int)CString_strlen(MESSAGE))));
    Er_assert(Message_epush(m, MESSAGEB, CString_strlen(MESSAGEB)));

    Iface_send(&c->iface, m);

    // shutdown
    Allocator_free(c->alloc);

    return Error(NONE);
}

static void child(char* name, struct Context* ctx)
{
    struct Pipe* pipe = Er_assert(Pipe_named(name, ctx->base, ctx->log, ctx->alloc));
    pipe->onConnection = onConnectionChild;
    pipe->userData = ctx;
    ctx->iface.send = receiveMessageChild;
    ctx->name = name;
    Iface_plumb(&ctx->iface, &pipe->iface);
    Timeout_setTimeout(timeout, NULL, 2000, ctx->base, ctx->alloc);
    EventBase_beginLoop(ctx->base);
}

int main(int argc, char** argv)
{
    struct Allocator* alloc = MallocAllocator_new(1<<20);
    struct EventBase* eb = EventBase_new(alloc);
    struct Log* log = FileWriterLog_new(stdout, alloc);
    struct Context* ctx = Allocator_calloc(alloc, sizeof(struct Context), 1);
    Identity_set(ctx);
    ctx->alloc = alloc;
    ctx->base = eb;
    ctx->log = log;
    ctx->iface.send = receiveMessageParent;

    if (argc > 3 && !CString_strcmp("Process_test", argv[1]) && !CString_strcmp("child", argv[2])) {
        child(argv[3], ctx);
        return 0;
    }

    struct Random* rand = Random_new(alloc, log, NULL);
    char randName[32] = {0};
    Random_base32(rand, (uint8_t*)randName, 31);
    String* name = String_printf(alloc, "%s%scjdns-test-%s", Pipe_PATH, Pipe_PATH_SEP, randName);
    if (!Defined(win32)) {
        String* textName =
            String_printf(alloc, "%s%scjdns-test-%s.txt", Pipe_PATH, Pipe_PATH_SEP, randName);
        int fd = open(textName->bytes, O_CREAT | O_TRUNC | O_RDWR, 0600);
        Assert_true(fd >= 0);
        Assert_true(write(fd, name->bytes, name->len) == ((ssize_t)name->len));
        ctx->fd = fd;
        unlink(textName->bytes);
    }

    struct PipeServer* pipe = PipeServer_named(name->bytes, eb, NULL, log, alloc);
    pipe->userData = ctx;
    pipe->onConnection = onConnectionParent;
    Iface_plumb(&ctx->iface, &pipe->iface.iface);

    char* path = Process_getPath(alloc);

    Assert_true(path != NULL);
    #ifdef win32
        Assert_true(CString_strstr(path, ":\\") == path + 1); /* C:\ */
        Assert_true(CString_strstr(path, ".exe"));
    #elif openbsd
        Assert_true(path[0] == 'b'); // Process_getPath returns relative paths on openbsd
    #elif netbsd
        Assert_true(path[0] == 'b'); // Process_getPath returns relative paths on netbsd too
    #else
        Assert_true(path[0] == '/');
    #endif

    char* args[] = { "Process_test", "child", name->bytes, NULL };

    Assert_true(!Process_spawn(path, args, eb, alloc, NULL));

    Timeout_setTimeout(timeout, NULL, 2000, eb, alloc);

    EventBase_beginLoop(eb);

    unlink(name->bytes);
    return 0;
}
