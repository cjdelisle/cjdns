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
#include "rust/cjdns_sys/Rffi.h"
#include "util/events/EventBase.h"
#include "util/events/Pipe.h"
#include "util/events/Socket.h"
#include "util/events/Timeout.h"
#include "memory/Allocator.h"
#include "util/events/Process.h"
#include "util/log/Log.h"
#include "util/log/FileWriterLog.h"
#include "util/CString.h"
#include "util/Assert.h"
#include "wire/Message.h"
#include "wire/Error.h"
#include "interface/addressable/AddrIface.h"

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
    struct Allocator* rootAlloc;
    EventBase_t* base;
    struct Log* log;

    // Parent only
    int fd;

    // child only
    char* name;
    Identity
};

static void onConnectionParent(void* context, const struct Sockaddr* addr)
{
    printf("onConnectionParent\n");
    struct Context* c = Identity_check((struct Context*) context);
    struct Allocator* alloc = Allocator_child(c->alloc);
    Message_t* msg = Message_new(0, 256, alloc);
    Err_assert(Message_epush(msg, MESSAGE, CString_strlen(MESSAGE) + 1));
    Err_assert(AddrIface_pushAddr(msg, addr));
    if (!Defined(win32)) {
        Message_setAssociatedFd(msg, c->fd);
    }
    printf("Parent sending message [%s] len [%d]\n", MESSAGE, Message_getLength(msg));
    Iface_send(&c->iface, msg);
    Allocator_free(alloc);
}

static int g_childStopped = 0;
static struct Context* g_context = NULL;

static Iface_DEFUN receiveMessageParent(Message_t* msg, struct Iface* iface)
{
    struct Context* c = Identity_check((struct Context*) iface);
    Err_assert(AddrIface_popAddr(NULL, msg));
    printf("msg length is %d\n", Message_getLength(msg));
    Assert_true(Message_getLength(msg) == (int)CString_strlen(MESSAGEB)+1);
    Assert_true(!Bits_memcmp(Message_bytes(msg), MESSAGEB, CString_strlen(MESSAGEB)+1));
    g_context = c;
    if (g_childStopped) {
        printf("Parent stopped in receiveMessageParent\n");
        Allocator_free(c->rootAlloc);
    }
    return NULL;
}

static void timeout(void* vNULL)
{
    Assert_true(!"timed out.");
}

static void childDone(struct Allocator_OnFreeJob* ofj) {
    printf("Shutdown child\n");
    struct Context* c = Identity_check((struct Context*) ofj->userData);
    EventBase_endLoop(c->base);
    // This causes a segfault because the logger calles into freed memory
    // Allocator_free(c->rootAlloc);
}

static Iface_DEFUN receiveMessageChild(Message_t* msg, struct Iface* iface)
{
    struct Context* c = Identity_check((struct Context*) iface);
    Allocator_t* alloc = Allocator_child(c->alloc);
    Allocator_t* alloc1 = Allocator_child(alloc);
    Message_t* m = Message_clone(msg, alloc1);
    printf("Child received message\n");
    Assert_true(Message_getLength(m) == (int)CString_strlen(MESSAGE)+1);
    Assert_true(!Bits_memcmp(Message_bytes(m), MESSAGE, CString_strlen(MESSAGE)+1));

    if (!Defined(win32)) {
        int fd = Message_getAssociatedFd(msg);
        Assert_true(fd > -1);
        if (lseek(fd, 0, SEEK_SET) < 0) {
            printf("lseek(%d) failed: errno %s\n", fd, strerror(errno));
            Assert_failure("lseek()");
        }
        uint8_t* buf = Allocator_calloc(Message_getAlloc(msg), 2048, 1);
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

    Err_assert(Message_eshift(m, -((int)CString_strlen(MESSAGE))));
    Err_assert(Message_epush(m, MESSAGEB, CString_strlen(MESSAGEB)));

    Allocator_onFree(alloc1, childDone, c);

    Iface_send(&c->iface, m);

    Allocator_free(alloc);

//    exit(0);

    // shutdown
    // Allocator_free(c->rootAlloc);

    return NULL;
}

static void child(char* name, struct Context* ctx)
{
    Iface_t* iface = NULL;
    Err_assert(Socket_connect(&iface, name, ctx->alloc));
    ctx->iface.send = receiveMessageChild;
    ctx->name = name;
    Iface_plumb(&ctx->iface, iface);
    Timeout_setTimeout(timeout, NULL, 10000, ctx->base, ctx->alloc);
    EventBase_beginLoop(ctx->base);
}

static void onChildExit(int64_t exit_status, int term_signal)
{
    printf("\n\n\nChild process exit status = %d term_signal = %d\n\n\n",
        (int)exit_status, term_signal);
    Assert_true(exit_status == 0);
    g_childStopped = true;
    if (g_context) {
        printf("Parent stopped in onChildExit\n");
        EventBase_endLoop(g_context->base);
        // Allocator_free(g_context->rootAlloc);
    }
}

int main(int argc, char** argv)
{
    struct Allocator* allocator = Allocator_new(1<<20);
    EventBase_t* eb = EventBase_new(allocator);
    struct Allocator* alloc = Allocator_child(allocator);
    struct Log* log = FileWriterLog_new(stdout, alloc);
    Rffi_setLogger(log);
    struct Context* ctx = Allocator_calloc(alloc, sizeof(struct Context), 1);
    Identity_set(ctx);
    ctx->alloc = alloc;
    ctx->rootAlloc = allocator;
    ctx->base = eb;
    ctx->log = log;
    ctx->iface.send = receiveMessageParent;

    if (argc > 3 && !CString_strcmp("Process_test", argv[1]) && !CString_strcmp("child", argv[2])) {
        child(argv[3], ctx);
        return 0;
    }

    struct Random* rand = NULL;
    Err_assert(Random_new(&rand, alloc, log));
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

    Socket_Server_t* ss = NULL;
    Err_assert(Socket_server(&ss, name->bytes, alloc));
    Socket_serverOnConnect(ss, onConnectionParent, ctx);
    Iface_plumb(&ctx->iface, ss->iface);

    const char* path = Process_getPath(alloc);

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

    const char* args[] = { "Process_test", "child", name->bytes, NULL };

    Assert_true(!Process_spawn(path, args, alloc, onChildExit));

    Timeout_setTimeout(timeout, NULL, 10000, eb, alloc);

    EventBase_beginLoop(eb);

    unlink(name->bytes);
    return 0;
}
