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

#include "exception/Except.h"
#include "exception/WinFail.h"
#include "memory/Allocator.h"
#include "interface/tuntap/windows/TAPDevice.h"
#include "interface/tuntap/windows/TAPInterface.h"
#include "interface/FramingInterface.h"
#include "util/events/EventBase.h"
#include "util/events/Pipe.h"
#include "util/platform/netdev/NetDev.h"

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

union TAPInterface_buffer {
    struct {
        uint32_t length_be;
        // account for ethernet misallignment
        uint16_t pad;
        uint8_t data[2042];
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

/**
 * Copy data from one file handle to another.
 * @return the handle which has blocked.
 */
#define thread_copy_ADD_FRAMING    1
#define thread_copy_REMOVE_FRAMING 2
static HANDLE thread_copy(struct TAPInterface_FdAndOl* from,
                          struct TAPInterface_FdAndOl* to,
                          int framing)
{
    for (;;) {
        uint8_t* readTo = (framing == thread_copy_ADD_FRAMING)
            ? from->buff.components.data : from->buff.bytes;
        uint8_t* writeFrom = (framing == thread_copy_REMOVE_FRAMING)
            ? from->buff.components.data : from->buff.bytes;

        DWORD bytesToRead = 2042;
        if (framing == thread_copy_REMOVE_FRAMING) {
            if (!from->bytes) {
                bytesToRead = 4;
            } else {
                bytesToRead = from->bytes;
            }
        }

        DWORD bytesRead = 0;
        if (from->state == TAPInterface_FdAndOl_state_AWAITING_READ) {
            if (!GetOverlappedResult(from->fd, &from->ol, &bytesRead, FALSE)) {
                switch (GetLastError()) {
                    case ERROR_IO_PENDING: return from->fd;
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
                    case ERROR_IO_PENDING: return to->fd;
                    default:;
                }
                printf("GetOverlappedResult(write, %s): %s\n",
                       to->name, WinFail_strerror(GetLastError()));
                Assert_true(0);
            }
            from->state = 0;
            if (bytesWritten < from->bytes) {
                from->bytes -= bytesWritten;
                from->offset += bytesWritten;
                goto writeMore;
            } else {
                Assert_true(bytesWritten == from->bytes);
                from->bytes = 0;
                from->offset = 0;
                printf("write to %s with %d bytes completed\n", to->name, (int)bytesWritten);
                // successfully finished a write, loop back and try again.
                continue;
            }

        } else if (!ReadFile(from->fd, &readTo[from->offset], bytesToRead, &bytesRead, &from->ol)) {
            switch (GetLastError()) {
                case ERROR_IO_PENDING: {
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

        if (framing == thread_copy_REMOVE_FRAMING) {
            if (!from->bytes) {
                if (bytesRead < 4) {
                    from->offset += bytesRead;
                    continue;
                }
                from->bytes = Endian_bigEndianToHost32(from->buff.components.length_be) + 4;
                Assert_true(from->bytes <= 2042);
            }

            if (bytesRead < from->bytes) {
                from->offset += bytesRead;
                from->bytes -= bytesRead;
                continue;
            }

        } else {
            from->bytes = bytesRead;
            from->bytes += 2;
            from->buff.components.length_be = Endian_hostToBigEndian32(((uint32_t)from->bytes));
            from->bytes += 4;
        }

        from->offset = 0;
        writeMore:
        for (;;) {
            DWORD bytes;
            if (!WriteFile(to->fd,
                           &writeFrom[from->offset],
                           from->bytes - (writeFrom - from->buff.bytes),
                           &bytes,
                           &from->ol))
            {
                switch (GetLastError()) {
                    case ERROR_IO_PENDING: {
                        from->state = TAPInterface_FdAndOl_state_AWAITING_WRITE;
                        return to->fd;
                    }
                    default:;
                }
                printf("WriteFile(%s): %s\n", to->name, WinFail_strerror(GetLastError()));
                Assert_true(0);
            } else {
                if (bytes < from->bytes) {
                    from->bytes += bytes;
                    from->offset += bytes;
                    continue;
                }
                Assert_true(bytes == from->bytes);
                printf("write to %s with %d bytes completed immedietly\n", to->name, (int)bytes);
                from->bytes = 0;
                from->offset = 0;
                break;
            }
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
    }
    return 0;
}

struct Interface* TAPInterface_new(const char* preferredName,
                                   char** assignedName,
                                   struct Except* eh,
                                   struct Log* logger,
                                   struct EventBase* base,
                                   struct Allocator* alloc)
{
    Log_debug(logger, "Getting TAP-Windows device name");

    struct TAPDevice* dev = TAPDevice_find(preferredName, eh, alloc);
    *assignedName = dev->name;

    NetDev_flushAddresses(dev->name, eh);

    Log_debug(logger, "Opening TAP-Windows device [%s] at location [%s]", dev->name, dev->path);

    struct TAPInterface_ThreadContext* tc =
        Allocator_calloc(alloc, sizeof(struct TAPInterface_ThreadContext), 1);

    WinFail_assert(eh, (tc->tap.ol.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)) != NULL);
    WinFail_assert(eh, (tc->pipe.ol.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)) != NULL);

    tc->tap.name = "tap";
    tc->pipe.name = "pipe";

    tc->tap.fd = CreateFile(dev->path,
                            GENERIC_READ | GENERIC_WRITE,
                            0,
                            0,
                            OPEN_EXISTING,
                            FILE_ATTRIBUTE_SYSTEM | FILE_FLAG_OVERLAPPED,
                            0);

    if (tc->tap.fd == INVALID_HANDLE_VALUE) {
        WinFail_fail(eh, "CreateFile(tapDevice)", GetLastError());
    }

    struct TAPInterface_Version_pvt ver = { .major = 0 };
    getVersion(tc->tap.fd, &ver, eh);

    setEnabled(tc->tap.fd, 1, eh);

    tc->pipe.fd =
        CreateNamedPipeA("\\\\.\\pipe\\cjdns_pipe_abcdefg",
                         PIPE_ACCESS_DUPLEX | FILE_FLAG_FIRST_PIPE_INSTANCE | FILE_FLAG_OVERLAPPED,
                         PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
                         PIPE_UNLIMITED_INSTANCES,
                         65536,
                         65536,
                         0,
                         NULL);

    struct Pipe* p = Pipe_named("abcdefg", base, eh, alloc);
    p->logger = logger;

//    CreateThread(NULL, 0, piper, (LPVOID)&fh[1], 0, NULL);
    CreateThread(NULL, 0, thread_main, (LPVOID)tc, 0, NULL);

    Log_info(logger, "Opened TAP-Windows device [%s] version [%lu.%lu.%lu] at location [%s]",
             dev->name, ver.major, ver.minor, ver.debug, dev->path);

    return FramingInterface_new(2048, &p->iface, alloc);
}
