module.exports = (builder, js) => {

    var files = [
        'include/uv.h',
        'include/tree.h',
        'include/uv-errno.h',
        'src/fs-poll.c',
        'src/inet.c',
        'src/queue.h',
        'src/uv-common.c',
        'src/uv-common.h',
        'src/version.c'
    ];

    if (builder.config.systemName === 'win32') {
        files.push(
            'include/uv-win.h',
            'src/win/async.c',
            'src/win/atomicops-inl.h',
            'src/win/core.c',
            'src/win/dl.c',
            'src/win/error.c',
            'src/win/fs.c',
            'src/win/fs-event.c',
            'src/win/getaddrinfo.c',
            'src/win/handle.c',
            'src/win/handle-inl.h',
            'src/win/internal.h',
            'src/win/iocp.c',
            'src/win/loop-watcher.c',
            'src/win/pipe.c',
            'src/win/thread.c',
            'src/win/poll.c',
            'src/win/process.c',
            'src/win/process-stdio.c',
            'src/win/req.c',
            'src/win/req-inl.h',
            'src/win/signal.c',
            'src/win/stream.c',
            'src/win/stream-inl.h',
            'src/win/tcp.c',
            'src/win/tty.c',
            'src/win/threadpool.c',
            'src/win/timer.c',
            'src/win/udp.c',
            'src/win/util.c',
            'src/win/winapi.c',
            'src/win/winapi.h',
            'src/win/winsock.c',
            'src/win/winsock.h',
        );
    } else {
        files.push(
            'include/uv-unix.h',
            'include/uv-linux.h',
            'include/uv-sunos.h',
            'include/uv-darwin.h',
            'include/uv-bsd.h',
            'src/unix/async.c',
            'src/unix/atomic-ops.h',
            'src/unix/core.c',
            'src/unix/dl.c',
            'src/unix/fs.c',
            'src/unix/getaddrinfo.c',
            'src/unix/internal.h',
            'src/unix/loop.c',
            'src/unix/loop-watcher.c',
            'src/unix/pipe.c',
            'src/unix/poll.c',
            'src/unix/process.c',
            'src/unix/signal.c',
            'src/unix/spinlock.h',
            'src/unix/stream.c',
            'src/unix/tcp.c',
            'src/unix/thread.c',
            'src/unix/threadpool.c',
            'src/unix/timer.c',
            'src/unix/tty.c',
            'src/unix/udp.c',
        )
    }
    var android = /android/i.test(builder.config.gcc);
    if (['linux','darwin'].indexOf(builder.config.systemName)) {
        files.push('src/unix/proctitle.c');
    }
    if (builder.config.systemName === 'darwin') {
        files.push(
            'src/unix/darwin.c',
            'src/unix/fsevents.c',
            'src/unix/darwin-proctitle.c',
        );
    }
    if (builder.config.systemName === 'linux') {
        files.push(
            'src/unix/linux-core.c',
            'src/unix/linux-inotify.c',
            'src/unix/linux-syscalls.c',
            'src/unix/linux-syscalls.h',
        );
        if (android) {
            files.push('src/unix/pthread-fixes.c');
        }
    }
    switch (builder.config.systemName) {
        case 'sunos': files.push('src/unix/sunos.c'); break;
        case 'dragonflybsd':
        case 'freebsd': files.push('src/unix/freebsd.c'); break;
        case 'openbsd': files.push('src/unix/openbsd.c'); break;
        case 'netbsd': files.push('src/unix/netbsd.c'); break;
    }
    if (['darwin', 'freebsd', 'dragonflybsd', 'openbsd', 'netbsd'].indexOf(builder.config.systemName) > -1) {
        files.push('src/unix/kqueue.c');
    }

    for (f of files) {
        if (f.endsWith(".c")) {
            console.log("DEPENDENCY " + f);
            js.linkerDependency("node_build/dependencies/libuv/" + f);
        }
    }
}