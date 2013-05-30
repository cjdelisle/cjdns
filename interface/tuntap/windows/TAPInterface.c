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

/*
 * Portions of this code are copied from QEMU project which is licensed
 * under GPLv2 or greater, further contributions are licensed under GPLv3
 * or greater.
 */

/*
 *  TAP-Win32 -- A kernel driver to provide virtual tap device functionality
 *               on Windows.  Originally derived from the CIPE-Win32
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
        Except_raise(eh, -1, "DeviceIoControl(TAP_IOCTL_GET_VERSION) out size [%d] expected [%d]",
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
#include "util/Hex.h"
static DWORD WINAPI piper(LPVOID param)
{
    HANDLE* pipes = param;
    union {
        struct {
            uint32_t length_be;
            uint8_t data[2044];
        } components;
        uint8_t bytes[2048];
    } buff;
    for (;;) {
        DWORD bytes;
        if (ReadFile(pipes[1], buff.components.data, 2044, &bytes, NULL)) {
            buff.components.length_be = Endian_hostToBigEndian32(((uint32_t)bytes));
            bytes += 4;
            DWORD total = 0;
            DWORD written;
            do {
                WriteFile(pipes[0], &buff.bytes[total], bytes - total, &written, NULL);
                total += written;
            } while (total < bytes);
        } else {

FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,
               NULL,
               GetLastError(),
               MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
               (char*)buff.bytes,
               2048,
               NULL);
printf("Error [%s] [%lu]\n", buff.bytes, (unsigned long) pipes[0]);
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

    flushAddresses(dev->name, eh);

    Log_debug(logger, "Opening TAP-Windows device [%s] at location [%s]", dev->name, dev->path);

    HANDLE tap = CreateFile(dev->path,
                            GENERIC_READ | GENERIC_WRITE,
                            0,
                            0,
                            OPEN_EXISTING,
                            FILE_ATTRIBUTE_SYSTEM,
                            0);

    if (tap == INVALID_HANDLE_VALUE) {
        WinFail_fail(eh, "CreateFile(tapDevice)", GetLastError());
    }

    struct TAPInterface_Version_pvt ver = { .major = 0 };
    getVersion(tap, &ver, eh);

    setEnabled(tap, 1, eh);

    HANDLE pipe = CreateNamedPipeA("\\\\.\\pipe\\cjdns_pipe_abcdefg",
                                   PIPE_ACCESS_DUPLEX | FILE_FLAG_FIRST_PIPE_INSTANCE,
                                   PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
                                   PIPE_UNLIMITED_INSTANCES, 65536, 65536, 0, NULL);

    struct Pipe* p = Pipe_named("abcdefg", base, eh, alloc);
    p->logger = logger;

    HANDLE* fh = Allocator_malloc(alloc, sizeof(HANDLE) * 3);
    fh[0] = tap;
    fh[1] = pipe;
    fh[2] = tap;

    CreateThread(NULL, 0, piper, (LPVOID)&fh[1], 0, NULL);
    //CreateThread(NULL, 0, piper, (LPVOID)&fh, 0, NULL);

    Log_info(logger, "Opened TAP-Windows device [%s] version [%lu.%lu.%lu] at location [%s]",
             dev->name, ver.major, ver.minor, ver.debug, dev->path);

    return FramingInterface_new(2048, &p->iface, alloc);
}
