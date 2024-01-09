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
    Er_assert(Message_epush(ok, "OK", 3));

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
    Er_assert(AddrIface_popAddr(msg));
    Assert_true(Message_getLength(msg) == 3);
    Assert_true(!Bits_memcmp(msg->msgbytes, "OK", 3));
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
        /* Allocator_free(alloc); */ 
        /* above line leads to error while `gdb -ex 'set follow-fork-mode child' -ex 'run Seccomp_test' ./target/release/testcjdroute`:
            GNU gdb (Ubuntu 12.0.90-0ubuntu1) 12.0.90
            Copyright (C) 2022 Free Software Foundation, Inc.
            License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
            This is free software: you are free to change and redistribute it.
            There is NO WARRANTY, to the extent permitted by law.
            Type "show copying" and "show warranty" for details.
            This GDB was configured as "x86_64-linux-gnu".
            Type "show configuration" for configuration details.
            For bug reporting instructions, please see:
            <https://www.gnu.org/software/gdb/bugs/>.
            Find the GDB manual and other documentation resources online at:
                <http://www.gnu.org/software/gdb/documentation/>.

            For help, type "help".
            Type "apropos word" to search for commands related to "word"...
            Reading symbols from ./target/release/testcjdroute...
            warning: Missing auto-load script at offset 0 in section .debug_gdb_scripts
            of file /home/yb/pkt/cjdns-dev/target/release/testcjdroute.
            Use `info auto-load python-scripts [REGEXP]' to list them.
            Starting program: /home/yb/pkt/cjdns-dev/target/release/testcjdroute Seccomp_test
            [Thread debugging using libthread_db enabled]
            Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
            [New Thread 0x7ffff7bff640 (LWP 141592)]
            1704797851 INFO RandomSeed.c:42 Attempting to seed random number generator
            1704797851 INFO RandomSeed.c:50 Trying random seed [/dev/urandom] Success
            1704797851 INFO RandomSeed.c:50 Trying random seed [/proc/sys/kernel/random/uuid (Linux)] Success
            1704797851 INFO RandomSeed.c:50 Trying random seed [sysctl(RANDOM_UUID) (Linux)] Success
            1704797851 INFO RandomSeed.c:50 Trying random seed [getentropy(2)] Success
            1704797851 INFO RandomSeed.c:63 Seeding random number generator succeeded with [4] sources
            [Attaching after Thread 0x7ffff7e9ad00 (LWP 141589) vfork to child process 141593]
            [New inferior 2 (process 141593)]
            [Thread debugging using libthread_db enabled]
            Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
            [Detaching vfork parent process 141589 after child exec]
            [Inferior 1 (process 141589) detached]
            process 141593 is executing new program: /home/yb/pkt/cjdns-dev/target/release/testcjdroute
            warning: Missing auto-load script at offset 0 in section .debug_gdb_scripts
            of file /home/yb/pkt/cjdns-dev/target/release/testcjdroute.
            Use `info auto-load python-scripts [REGEXP]' to list them.
            [Thread debugging using libthread_db enabled]
            Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
            [New Thread 0x7ffff7bff640 (LWP 141594)]
            1704797851 DEBUG Pipe.c:259 Pipe [/tmp/cjdns-test-gthc2tcsht26v4vsmp6fns0637pr5t] established connection
            1704797851 DEBUG Pipe.c:259 Pipe [(null)] established connection
            1704797851 DEBUG Pipe.c:112 Sending message of length [3]
            1704797851 DEBUG Pipe.c:212 Pipe incoming message len [3]


            thread 'main' panicked at rust/cjdns_sys/src/rffi/util.rs:32:5:
            Assertion failure [Pipe.c:290] [(Identity_ptr->Identity_verifier == ((uintptr_t) 0x1d268b90ed35c9f1ull))]


            Thread 2.1 "testcjdroute" received signal SIGSYS, Bad system call.
            [Switching to Thread 0x7ffff7e9ad00 (LWP 141593)]
            __GI___getcwd (buf=0x5555557c3440 "\340\234\341\367\377\177", size=512) at ../sysdeps/unix/sysv/linux/getcwd.c:80
            80      ../sysdeps/unix/sysv/linux/getcwd.c: No such file or directory.
            INS (gdb) bt
#0  __GI___getcwd (buf=0x5555557c3440 "\340\234\341\367\377\177", size=512) at ../sysdeps/unix/sysv/linux/getcwd.c:80
#1  0x00005555556ec728 in std::sys::unix::os::getcwd () at library/std/src/sys/unix/os.rs:160
#2  std::env::current_dir () at library/std/src/env.rs:57
#3  0x00005555556f2cb5 in std::sys_common::backtrace::_print_fmt () at library/std/src/sys_common/backtrace.rs:53
#4  std::sys_common::backtrace::_print::{impl#0}::fmt () at library/std/src/sys_common/backtrace.rs:44
#5  0x000055555571920c in core::fmt::rt::Argument::fmt () at library/core/src/fmt/rt.rs:138
#6  core::fmt::write () at library/core/src/fmt/mod.rs:1114
#7  0x00005555556f022e in std::io::Write::write_fmt<std::sys::unix::stdio::Stderr> () at library/std/src/io/mod.rs:1763
#8  0x00005555556f2be4 in std::sys_common::backtrace::_print () at library/std/src/sys_common/backtrace.rs:47
#9  std::sys_common::backtrace::print () at library/std/src/sys_common/backtrace.rs:34
#10 0x00005555556f45c3 in std::panicking::default_hook::{closure#1} () at library/std/src/panicking.rs:272
#11 0x00005555556f42e4 in std::panicking::default_hook () at library/std/src/panicking.rs:292
#12 0x000055555558c3ee in alloc::boxed::{impl#49}::call<(&core::panic::panic_info::PanicInfo), (dyn core::ops::function::Fn<(&core::panic::panic_info::PanicInfo), Output=()> + core::marker::Send + core::marker::Sync), alloc::alloc::Global> (self=<optimized out>, args=...) at /rustc/a28077b28a02b92985b3a3faecf92813155f1ea1/library/alloc/src/boxed.rs:2021
#13 cjdns_sys::rust_main::{async_block#1}::{closure#0} (info=0x200) at rust/cjdns_sys/src/lib.rs:23
#14 0x00005555556f4cc1 in alloc::boxed::{impl#49}::call<(&core::panic::panic_info::PanicInfo), (dyn core::ops::function::Fn<(&core::panic::panic_info::PanicInfo), Output=()> + core::marker::Send + core::marker::Sync), alloc::alloc::Global> () at library/alloc/src/boxed.rs:2021
#15 std::panicking::rust_panic_with_hook () at library/std/src/panicking.rs:735
#16 0x00005555556f4a41 in std::panicking::begin_panic_handler::{closure#0} () at library/std/src/panicking.rs:609
#17 0x00005555556f3326 in std::sys_common::backtrace::__rust_end_short_backtrace<std::panicking::begin_panic_handler::{closure_env#0}, !> () at library/std/src/sys_common/backtrace.rs:170
#18 0x00005555556f4792 in std::panicking::begin_panic_handler () at library/std/src/panicking.rs:597
#19 0x000055555557a095 in core::panicking::panic_fmt () at library/core/src/panicking.rs:72
#20 0x000055555559ab36 in core::panicking::panic_display<alloc::borrow::Cow<str>> (x=<optimized out>) at /rustc/a28077b28a02b92985b3a3faecf92813155f1ea1/library/core/src/panicking.rs:168
#21 cjdns_sys::rffi::util::Rffi_panic (msg=msg@entry=0x7fffffffcd40) at /rustc/a28077b28a02b92985b3a3faecf92813155f1ea1/library/std/src/panic.rs:26
#22 0x00005555555a4481 in Assert_failure (format=format@entry=0x555555720f88 "Assertion failure [%s:%d] [%s]\n") at util/Assert.c:38
#23 0x00005555555e5b01 in onFree (job=<optimized out>) at util/events/libuv/Pipe.c:290
#24 0x000055555557dff8 in cjdns_sys::rffi::allocator::free_allocs (allocs=...) at rust/cjdns_sys/src/rffi/allocator.rs:109
#25 cjdns_sys::rffi::allocator::Allocator::free (self=<optimized out>, source=...) at rust/cjdns_sys/src/rffi/allocator.rs:358
#26 0x000055555557e70d in cjdns_sys::rffi::allocator::Rffi_allocator_free (a=a@entry=0x5555557bfbd0, file=file@entry=0x555555724e9e, line=line@entry=118)
                at rust/cjdns_sys/src/rffi/allocator.rs:378
#27 0x00005555555a0104 in Allocator__free (alloc=alloc@entry=0x5555557bfbd0, file=file@entry=0x555555724e9e "Seccomp_test.c", line=line@entry=118) at memory/Allocator.c:23
#28 0x00005555555aa31e in Seccomp_test_main (argc=<optimized out>, argv=<optimized out>) at util/test/Seccomp_test.c:118
#29 0x00005555555a0df2 in main2 (detRand=0x5555557bfa10, alloc=0x5555557bf540, argv=0x5555557b3ae0, argc=4) at test/testcjdroute.c:238
#30 testcjdroute_main (argc=4, argv=0x5555557b3ae0) at test/testcjdroute.c:277
#31 0x000055555558c34e in cjdns_sys::rust_main::{async_block#1} () at rust/cjdns_sys/src/lib.rs:26
#32 0x000055555558aad2 in tokio::runtime::park::{impl#4}::block_on::{closure#0}<cjdns_sys::rust_main::{async_block_env#1}> ()
                at /home/yb/.cargo/registry/src/index.crates.io-6f17d22bba15001f/tokio-1.34.0/src/runtime/park.rs:282
#33 tokio::runtime::coop::with_budget<core::task::poll::Poll<()>, tokio::runtime::park::{impl#4}::block_on::{closure_env#0}<cjdns_sys::rust_main::{async_block_env#1}>> (budget=..., f=...)
                at /home/yb/.cargo/registry/src/index.crates.io-6f17d22bba15001f/tokio-1.34.0/src/runtime/coop.rs:107
#34 tokio::runtime::coop::budget<core::task::poll::Poll<()>, tokio::runtime::park::{impl#4}::block_on::{closure_env#0}<cjdns_sys::rust_main::{async_block_env#1}>> (f=...)
                at /home/yb/.cargo/registry/src/index.crates.io-6f17d22bba15001f/tokio-1.34.0/src/runtime/coop.rs:73
#35 tokio::runtime::park::CachedParkThread::block_on<cjdns_sys::rust_main::{async_block_env#1}> (self=0x7fffffffd5ef, f=<error reading variable: Cannot access memory at address 0x10>)
                at /home/yb/.cargo/registry/src/index.crates.io-6f17d22bba15001f/tokio-1.34.0/src/runtime/park.rs:282
#36 tokio::runtime::context::blocking::BlockingRegionGuard::block_on<cjdns_sys::rust_main::{async_block_env#1}> (self=<optimized out>, f=...)
                at /home/yb/.cargo/registry/src/index.crates.io-6f17d22bba15001f/tokio-1.34.0/src/runtime/context/blocking.rs:66
#37 0x000055555558ea7f in tokio::runtime::scheduler::multi_thread::{impl#0}::block_on::{closure#0}<cjdns_sys::rust_main::{async_block_env#1}> (blocking=0x5555557c3440)
                at /home/yb/.cargo/registry/src/index.crates.io-6f17d22bba15001f/tokio-1.34.0/src/runtime/scheduler/multi_thread/mod.rs:87
#38 tokio::runtime::context::runtime::enter_runtime<tokio::runtime::scheduler::multi_thread::{impl#0}::block_on::{closure_env#0}<cjdns_sys::rust_main::{async_block_env#1}>, ()> (
                handle=0x7fffffffd7f0, allow_block_in_place=<optimized out>, f=...) at /home/yb/.cargo/registry/src/index.crates.io-6f17d22bba15001f/tokio-1.34.0/src/runtime/context/runtime.rs:65
#39 0x0000555555587452 in tokio::runtime::scheduler::multi_thread::MultiThread::block_on<cjdns_sys::rust_main::{async_block_env#1}> (self=<optimized out>, handle=<optimized out>,
                future=<error reading variable: Cannot access memory at address 0x10>)
                at /home/yb/.cargo/registry/src/index.crates.io-6f17d22bba15001f/tokio-1.34.0/src/runtime/scheduler/multi_thread/mod.rs:86
#40 tokio::runtime::runtime::Runtime::block_on<cjdns_sys::rust_main::{async_block_env#1}> (self=<optimized out>, future=...)
        */
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
    Iface_plumb(&ctx->iface, pipe->iface.iface);

    const char* path = Process_getPath(alloc);
    const char* args[] = { "Seccomp_test", "child", name->bytes, NULL };

    Assert_true(!Process_spawn(path, args, eb, alloc, NULL));

    Timeout_setTimeout(timeout2, NULL, 2000, eb, alloc);

    EventBase_beginLoop(eb);
    unlink(name->bytes);
    return 0;
}
