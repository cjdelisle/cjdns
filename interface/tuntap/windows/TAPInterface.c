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

// TODO(cjd): this is nasty, we need a wrapper.
#include "util/events/libuv/UvWrapper.h"
#include "util/events/libuv/EventBase_pvt.h"

#include "exception/Except.h"
#include "exception/WinFail.h"
#include "memory/Allocator.h"
#include "interface/tuntap/windows/TAPDevice.h"
#include "interface/tuntap/windows/TAPInterface.h"
//#include "interface/FramingInterface.h"
#include "util/events/EventBase.h"
//#include "util/events/Pipe.h"
#include "util/platform/netdev/NetDev.h"
#include "wire/Error.h"

/*
 * Portions of this code are copied from QEMU project which is licensed
 * under GPLv2 or greater, further contributions are licensed under GPLv3
 * or greater.
 */

/*
 *  TAP-Win32 -- A kernel driver to provide virtual tap device functionality
 *               on win32.  Originally derived from the CIPE-Win32
 *               project by Damion K. Wilson, with extensive modifications by
 *               James Yonan.
 *
 *  All source code which derives from the CIPE-Win32 project is
 *  Copyright (C) Damion K. Wilson, 2003, and is released under the
 *  GPL version 2 (see below).
 *
 *  All other source code is Copyright (C) James Yonan, 2003-2004,
 *  and is released under the GPL version 2 (see below).
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program (see the file COPYING included with this
 *  distribution); if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdio.h>
#include <windows.h>
#include <io.h>

//=============
// TAP IOCTLs
//=============

#define TAP_CONTROL_CODE(request,method) \
  CTL_CODE (FILE_DEVICE_UNKNOWN, request, method, FILE_ANY_ACCESS)

#define TAP_IOCTL_GET_MAC               TAP_CONTROL_CODE (1, METHOD_BUFFERED)
#define TAP_IOCTL_GET_VERSION           TAP_CONTROL_CODE (2, METHOD_BUFFERED)
#define TAP_IOCTL_GET_MTU               TAP_CONTROL_CODE (3, METHOD_BUFFERED)
#define TAP_IOCTL_GET_INFO              TAP_CONTROL_CODE (4, METHOD_BUFFERED)
#define TAP_IOCTL_CONFIG_POINT_TO_POINT TAP_CONTROL_CODE (5, METHOD_BUFFERED)
#define TAP_IOCTL_SET_MEDIA_STATUS      TAP_CONTROL_CODE (6, METHOD_BUFFERED)
#define TAP_IOCTL_CONFIG_DHCP_MASQ      TAP_CONTROL_CODE (7, METHOD_BUFFERED)
#define TAP_IOCTL_GET_LOG_LINE          TAP_CONTROL_CODE (8, METHOD_BUFFERED)
#define TAP_IOCTL_CONFIG_DHCP_SET_OPT   TAP_CONTROL_CODE (9, METHOD_BUFFERED)


struct TAPInterface_Version_pvt {
    unsigned long major;
    unsigned long minor;
    unsigned long debug;
};

static void getVersion(HANDLE tap, struct TAPInterface_Version_pvt* version, struct Except* eh)
{
    ULONG version_len;
    BOOL bret = DeviceIoControl(tap,
                                TAP_IOCTL_GET_VERSION,
                                version,
                                sizeof(struct TAPInterface_Version_pvt),
                                version,
                                sizeof(struct TAPInterface_Version_pvt),
                                &version_len,
                                NULL);
    if (!bret) {
        DWORD err = GetLastError();
        CloseHandle(tap);
        WinFail_fail(eh, "DeviceIoControl(TAP_IOCTL_GET_VERSION)", err);
    }
    if (version_len != sizeof(struct TAPInterface_Version_pvt)) {
        CloseHandle(tap);
        Except_throw(eh, "DeviceIoControl(TAP_IOCTL_GET_VERSION) out size [%d] expected [%d]",
                     (int)version_len, (int)sizeof(struct TAPInterface_Version_pvt));
    }
}

static void setEnabled(HANDLE tap, int status, struct Except* eh)
{
    unsigned long len = 0;

    BOOL bret = DeviceIoControl(tap, TAP_IOCTL_SET_MEDIA_STATUS,
                                &status, sizeof (status),
                                &status, sizeof (status), &len, NULL);
    if (!bret) {
        DWORD err = GetLastError();
        CloseHandle(tap);
        WinFail_fail(eh, "DeviceIoControl(TAP_IOCTL_SET_MEDIA_STATUS)", err);
    }
}
/*
union TAPInterface_buffer {
    struct {
        uint32_t length_be;
        uint8_t data[2044];
    } components;
    uint8_t bytes[2048];
};

#define TAPInterface_FdAndOl_state_AWAITING_READ  1
#define TAPInterface_FdAndOl_state_AWAITING_WRITE 2

struct TAPInterface_FdAndOl
{
    HANDLE fd;
    OVERLAPPED ol;
    union TAPInterface_buffer buff;
    int state;
    DWORD bytes;
    DWORD offset;
    char* name;
};

struct TAPInterface_ThreadContext
{
    struct TAPInterface_FdAndOl tap;
    struct TAPInterface_FdAndOl pipe;
};

#include "util/Hex.h"

/ **
 * Copy data from one file handle to another.
 * @return the handle which has blocked.
 * /
#define thread_copy_ADD_FRAMING    1
#define thread_copy_REMOVE_FRAMING 2
static HANDLE thread_copy(struct TAPInterface_FdAndOl* from,
                          struct TAPInterface_FdAndOl* to,
                          int framing)
{
    for (;;) {
        // Read from one socket into this buffer
        uint8_t* readTo;

        DWORD bytesToRead = 2044;

        if (framing == thread_copy_ADD_FRAMING) {
            readTo = from->buff.components.data;
        } else {
            Assert_true(framing == thread_copy_REMOVE_FRAMING);
            readTo = from->buff.bytes;

            if (!from->bytes) {
                // If we're reading from the pipe, we want to read only 4 bytes
                // and use that to determine how many bytes we actually want to read.
                bytesToRead = 4;
            } else {
                bytesToRead = from->bytes;
            }
        }

        DWORD bytesRead = 0;
        if (from->state == TAPInterface_FdAndOl_state_AWAITING_READ) {
            if (!GetOverlappedResult(from->fd, &from->ol, &bytesRead, FALSE)) {
                switch (GetLastError()) {
                    case ERROR_IO_PENDING:
                    case ERROR_IO_INCOMPLETE: return from->fd;
                    default:;
                }
                printf("GetOverlappedResult(read, %s): %s\n",
                       from->name, WinFail_strerror(GetLastError()));
                Assert_true(0);
            }
            printf("read completed from %s with %d bytes\n", from->name, (int)bytesRead);
            from->state = 0;

        } else if (from->state == TAPInterface_FdAndOl_state_AWAITING_WRITE) {
            DWORD bytesWritten;
            if (!GetOverlappedResult(to->fd, &from->ol, &bytesWritten, FALSE)) {
                switch (GetLastError()) {
                    case ERROR_IO_PENDING:
                    case ERROR_IO_INCOMPLETE: return to->fd;
                    default:;
                }
                printf("GetOverlappedResult(write, %s): %s\n",
                       to->name, WinFail_strerror(GetLastError()));
                Assert_true(0);
            }
            from->state = 0;

            // TODO(cjd): Assuming windows will fill-or-kill a write if it's async.
            Assert_true(bytesWritten == from->bytes);
            from->bytes = 0;
            from->offset = 0;
            printf("write to %s with %d bytes completed\n", to->name, (int)bytesWritten);
            // successfully finished a write, loop back and try again.
            continue;

        } else if (!ReadFile(from->fd, &readTo[from->offset], bytesToRead, &bytesRead, &from->ol)) {
            switch (GetLastError()) {
                case ERROR_IO_PENDING:
                case ERROR_IO_INCOMPLETE: {
                    from->state = TAPInterface_FdAndOl_state_AWAITING_READ;
                    printf("read pending from %s\n", from->name);
                    return from->fd;
                }

                default: {
                    printf("ReadFile(%s): %s\n", from->name, WinFail_strerror(GetLastError()));
                    Assert_true(0);
                }
            }

        } else {
            printf("read completed immedietly from %s with %d bytes\n",
                   from->name, (int)bytesRead);
        }

        // Write from this buffer into the other socket
        uint8_t* writeFrom;

        if (framing == thread_copy_REMOVE_FRAMING) {
            writeFrom = from->buff.components.data;
            if (!from->bytes) {
                Assert_true(bytesRead == 4);
                from->bytes = Endian_bigEndianToHost32(from->buff.components.length_be) + 4;
                Assert_true(from->bytes <= 2044);
            }

            if (bytesRead < from->bytes) {
                from->offset += bytesRead;
                from->bytes -= bytesRead;
                continue;
            }

        } else {
            writeFrom = from->buff.bytes;
            from->buff.components.length_be = Endian_hostToBigEndian32(bytesRead);
            from->bytes = bytesRead + 4;
        }

        from->offset = 0;
        DWORD bytes;
        if (!WriteFile(to->fd,
                       / *&* /writeFrom, / *[from->offset],* /
                       from->bytes, / * - (writeFrom - from->buff.bytes), * /
                       &bytes,
                       &from->ol))
        {
            switch (GetLastError()) {
                case ERROR_IO_PENDING:
                case ERROR_IO_INCOMPLETE: {
                    from->state = TAPInterface_FdAndOl_state_AWAITING_WRITE;
                    return to->fd;
                }
                default:;
            }
            printf("WriteFile(%s): %s\n", to->name, WinFail_strerror(GetLastError()));
            Assert_true(0);
        } else {
            // TODO(cjd): Writing this in the hopes that windows has the common decency to
            //            fill-or-kill a write request if it's async...
            Assert_true(bytes == from->bytes);
            printf("write to %s with %d bytes completed immedietly\n", to->name, (int)bytes);
            from->bytes = 0;
            from->offset = 0;
        }
    }
}

static DWORD WINAPI thread_main(LPVOID param)
{
    struct TAPInterface_ThreadContext* tc = (struct TAPInterface_ThreadContext*)param;

    for (;;) {
        HANDLE handles[2];
        handles[0] = thread_copy(&tc->tap, &tc->pipe, thread_copy_ADD_FRAMING);
        handles[1] = thread_copy(&tc->pipe, &tc->tap, thread_copy_REMOVE_FRAMING);
        if (WaitForMultipleObjects(2, handles, FALSE, 3000) == WAIT_FAILED) {
            printf("WaitForMultipleObjects(): %s\n", WinFail_strerror(GetLastError()));
        }
        FlushFileBuffers(tc->pipe.fd);
    }
    return 0;
}
*/
#define WRITE_MESSAGE_SLOTS 20
struct TAPInterface_pvt
{
    struct TAPInterface pub;

    uv_iocp_t readIocp;
    struct Message* readMsg;
    long readLength;

    uv_iocp_t writeIocp;
    struct Message* writeMsgs[WRITE_MESSAGE_SLOTS];
    /** This allocator holds messages pending write in memory until they are complete. */
    struct Allocator* pendingWritesAlloc;
    int writeMessageCount;

    int isPendingWrite;

    HANDLE handle;

    struct Log* log;
    struct Allocator* alloc;

    struct EventBase* base;
    Identity
};


static void readCallbackB(struct TAPInterface_pvt* tap);

static void postRead(struct TAPInterface_pvt* tap)
{
    struct Allocator* alloc = Allocator_child(tap->alloc);
    struct Message* msg = tap->readMsg = Message_new(1536, 512, alloc);
    OVERLAPPED* readol = (OVERLAPPED*) tap->readIocp.overlapped;
    tap->readLength = 0;
    if (!ReadFile(tap->handle, msg->bytes, 1536, &tap->readLength, readol)) {
        switch (GetLastError()) {
            case ERROR_IO_PENDING:
            case ERROR_IO_INCOMPLETE: break;
            default: Assert_failure("ReadFile(tap): %s\n", WinFail_strerror(GetLastError()));
        }
        Log_debug(tap->log, "Posted read");

    } else {
        Log_debug(tap->log, "Read returned immediately");
        readCallbackB(tap);
    }
}

static void readCallbackB(struct TAPInterface_pvt* tap)
{
    struct Message* msg = tap->readMsg;
    tap->readMsg = NULL;
    msg->length = tap->readLength;
    Interface_receiveMessage(&tap->pub.generic, msg);
    postRead(tap);
}

static void readCallback(uv_iocp_t* readIocp)
{
    struct TAPInterface_pvt* tap =
        Identity_check((struct TAPInterface_pvt*)
            (((char*)readIocp) - offsetof(struct TAPInterface_pvt, readIocp)));
    readCallbackB(tap);
}

static void writeCallbackB(struct TAPInterface_pvt* tap);

static void postWrite(struct TAPInterface_pvt* tap)
{
    Assert_true(!tap->isPendingWrite);
    struct Message* msg = tap->writeMsgs[0];
    OVERLAPPED* writeol = (OVERLAPPED*) tap->writeIocp.overlapped;
    if (!WriteFile(tap->handle, msg->bytes, msg->length, NULL, writeol)) {
        switch (GetLastError()) {
            case ERROR_IO_PENDING:
            case ERROR_IO_INCOMPLETE: break;
            default: Assert_failure("WriteFile(tap): %s\n", WinFail_strerror(GetLastError()));
        }
        Log_debug(tap->log, "Posted write");
        tap->isPendingWrite = 1;
    } else {
        Log_debug(tap->log, "Write returned immediately");
        writeCallbackB(tap);
    }
}

static void writeCallbackB(struct TAPInterface_pvt* tap)
{
    Assert_true(tap->isPendingWrite);
    tap->isPendingWrite = 0;
    Assert_true(tap->writeMessageCount--);
    if (tap->writeMessageCount) {
        for (int i = 0; i < tap->writeMessageCount; i++) {
            tap->writeMsgs[i] = tap->writeMsgs[i+1];
        }
        postWrite(tap);
    } else {
        Log_debug(tap->log, "All pending writes are complete");
        Allocator_free(tap->pendingWritesAlloc);
        tap->pendingWritesAlloc = NULL;
    }
}

static void writeCallback(uv_iocp_t* writeIocp)
{
    struct TAPInterface_pvt* tap =
        Identity_check((struct TAPInterface_pvt*)
            (((char*)writeIocp) - offsetof(struct TAPInterface_pvt, writeIocp)));
    writeCallbackB(tap);
}

static void initIocp(struct TAPInterface_pvt* tap, struct Except* eh)
{
    struct EventBase_pvt* ebp = EventBase_privatize(tap->base);
    int ret;
    if ((ret = uv_iocp_start(ebp->loop, &tap->readIocp, tap->handle, readCallback))) {
        Except_throw(eh, "uv_iocp_start(readIocp): %s", uv_strerror(ret));
    }
    if ((ret = uv_iocp_start(ebp->loop, &tap->writeIocp, tap->handle, writeCallback))) {
        Except_throw(eh, "uv_iocp_start(writeIocp): %s", uv_strerror(ret));
    }
}

static uint8_t sendMessage(struct Message* msg, struct Interface* iface)
{
    struct TAPInterface_pvt* tap = Identity_check((struct TAPInterface_pvt*) iface);
    if (tap->writeMessageCount >= WRITE_MESSAGE_SLOTS) {
        Log_info(tap->log, "DROP message because the tap is lagging");
        return Error_UNDELIVERABLE;
    }
    if (!tap->pendingWritesAlloc) {
        tap->pendingWritesAlloc = Allocator_child(tap->alloc);
    }
    tap->writeMsgs[tap->writeMessageCount++] = msg;
    Allocator_adopt(tap->pendingWritesAlloc, msg->alloc);
    if (tap->writeMessageCount == 1) {
        postWrite(tap);
    }
    return 0;
}

struct TAPInterface* TAPInterface_new(const char* preferredName,
                                      struct Except* eh,
                                      struct Log* logger,
                                      struct EventBase* base,
                                      struct Allocator* alloc)
{
    Log_debug(logger, "Getting TAP-Windows device name");

    struct TAPDevice* dev = TAPDevice_find(preferredName, eh, alloc);

    NetDev_flushAddresses(dev->name, eh);

    Log_debug(logger, "Opening TAP-Windows device [%s] at location [%s]", dev->name, dev->path);
/*
    struct TAPInterface_ThreadContext* tc =
        Allocator_calloc(alloc, sizeof(struct TAPInterface_ThreadContext), 1);

    WinFail_assert(eh, (tc->tap.ol.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)) != NULL);
    WinFail_assert(eh, (tc->pipe.ol.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)) != NULL);

    tc->tap.name = "tap";
    tc->pipe.name = "pipe";
*/

    struct TAPInterface_pvt* tap = Allocator_calloc(alloc, sizeof(struct TAPInterface_pvt), 1);
    Identity_set(tap);
    tap->base = base;
    tap->alloc = alloc;
    tap->log = logger;
    tap->pub.assignedName = dev->name;

    tap->handle = CreateFile(dev->path,
                             GENERIC_READ | GENERIC_WRITE,
                             0,
                             0,
                             OPEN_EXISTING,
                             FILE_ATTRIBUTE_SYSTEM | FILE_FLAG_OVERLAPPED,
                             0);

    if (tap->handle == INVALID_HANDLE_VALUE) {
        WinFail_fail(eh, "CreateFile(tapDevice)", GetLastError());
    }

    initIocp(tap, eh);

    // hack hack hack assignment of const pointer...
    Bits_memcpyConst((void*)&tap->pub.generic.sendMessage, &sendMessage, sizeof(uintptr_t));

    struct TAPInterface_Version_pvt ver = { .major = 0 };
    getVersion(tap->handle, &ver, eh);

    setEnabled(tap->handle, 1, eh);
/*
    tc->pipe.fd = CreateNamedPipeA(
        "\\\\.\\pipe\\cjdns_pipe_abcdefg",
        PIPE_ACCESS_DUPLEX
            | FILE_FLAG_FIRST_PIPE_INSTANCE
            | FILE_FLAG_OVERLAPPED
            | FILE_FLAG_WRITE_THROUGH,

        PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
        PIPE_UNLIMITED_INSTANCES,
        65536,
        65536,
        0,
        NULL
    );

    struct Pipe* p = Pipe_named("abcdefg", base, eh, alloc);
    p->logger = logger;
*/
//    CreateThread(NULL, 0, piper, (LPVOID)&fh[1], 0, NULL);
//    CreateThread(NULL, 0, thread_main, (LPVOID)tc, 0, NULL);

    Log_info(logger, "Opened TAP-Windows device [%s] version [%lu.%lu.%lu] at location [%s]",
             dev->name, ver.major, ver.minor, ver.debug, dev->path);

    return &tap->pub;
    //return FramingInterface_new(2048, &p->iface, alloc);
}
