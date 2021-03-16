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

// TODO(cjd): this is nasty, we need a wrapper.
#include "util/events/libuv/UvWrapper.h"
#include "util/events/libuv/EventBase_pvt.h"

#include "exception/Er.h"
#include "exception/WinEr.h"
#include "memory/Allocator.h"
#include "interface/tuntap/windows/TAPInterface.h"
#include "interface/tuntap/windows/TAPDevice.h"
#include "util/events/EventBase.h"
#include "util/platform/netdev/NetDev.h"
#include "wire/Error.h"
#include "wire/Message.h"

#include <stdio.h>
#include <windows.h>
#include <io.h>

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

static Er_DEFUN(void getVersion(
    HANDLE tap,
    struct TAPInterface_Version_pvt* version,
    struct Allocator* alloc))
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
        WinEr_fail(alloc, "DeviceIoControl(TAP_IOCTL_GET_VERSION)", err);
    }
    if (version_len != sizeof(struct TAPInterface_Version_pvt)) {
        CloseHandle(tap);
        Er_raise(alloc, "DeviceIoControl(TAP_IOCTL_GET_VERSION) out size [%d] expected [%d]",
                     (int)version_len, (int)sizeof(struct TAPInterface_Version_pvt));
    }
    Er_ret();
}

static Er_DEFUN(void setEnabled(HANDLE tap, int status, struct Allocator* alloc))
{
    unsigned long len = 0;

    BOOL bret = DeviceIoControl(tap, TAP_IOCTL_SET_MEDIA_STATUS,
                                &status, sizeof (status),
                                &status, sizeof (status), &len, NULL);
    if (!bret) {
        DWORD err = GetLastError();
        CloseHandle(tap);
        WinEr_fail(alloc, "DeviceIoControl(TAP_IOCTL_SET_MEDIA_STATUS)", err);
    }
    Er_ret();
}

#define WRITE_MESSAGE_SLOTS 20
struct TAPInterface_pvt
{
    struct TAPInterface pub;

    uv_iocp_t readIocp;
    struct Message* readMsg;

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
    // Choose odd numbers so that the message will be aligned despite the weird header size.
    struct Message* msg = tap->readMsg = Message_new(1534, 514, alloc);
    OVERLAPPED* readol = (OVERLAPPED*) tap->readIocp.overlapped;
    if (!ReadFile(tap->handle, msg->msgbytes, 1534, NULL, readol)) {
        switch (GetLastError()) {
            case ERROR_IO_PENDING:
            case ERROR_IO_INCOMPLETE: break;
            default: Assert_failure("ReadFile(tap): %s\n", WinEr_strerror(GetLastError()));
        }
    } else {
        // It doesn't matter if it returns immediately, it will also return async.
        //Log_debug(tap->log, "Read returned immediately");
    }
    Log_debug(tap->log, "Posted read");
}

static void readCallbackB(struct TAPInterface_pvt* tap)
{
    struct Message* msg = tap->readMsg;
    tap->readMsg = NULL;
    DWORD bytesRead;
    OVERLAPPED* readol = (OVERLAPPED*) tap->readIocp.overlapped;
    if (!GetOverlappedResult(tap->handle, readol, &bytesRead, FALSE)) {
        Assert_failure("GetOverlappedResult(read, tap): %s\n", WinEr_strerror(GetLastError()));
    }
    Er_assert(Message_truncate(msg, bytesRead));
    Log_debug(tap->log, "Read [%d] bytes", Message_getLength(msg));
    Iface_send(&tap->pub.generic, msg);
    Allocator_free(Message_getAlloc(msg));
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
    tap->isPendingWrite = 1;
    struct Message* msg = tap->writeMsgs[0];
    OVERLAPPED* writeol = (OVERLAPPED*) tap->writeIocp.overlapped;
    if (!WriteFile(tap->handle, msg->msgbytes, Message_getLength(msg), NULL, writeol)) {
        switch (GetLastError()) {
            case ERROR_IO_PENDING:
            case ERROR_IO_INCOMPLETE: break;
            default: Assert_failure("WriteFile(tap): %s\n", WinEr_strerror(GetLastError()));
        }
    } else {
        // It doesn't matter if it returns immediately, it will also return async.
        //Log_debug(tap->log, "Write returned immediately");
    }
    Log_debug(tap->log, "Posted write [%d] bytes", Message_getLength(msg));
}

static void writeCallbackB(struct TAPInterface_pvt* tap)
{
    DWORD bytesWritten;
    OVERLAPPED* writeol = (OVERLAPPED*) tap->writeIocp.overlapped;
    if (!GetOverlappedResult(tap->handle, writeol, &bytesWritten, FALSE)) {
        Assert_failure("GetOverlappedResult(write, tap): %s\n", WinEr_strerror(GetLastError()));
    }

    Assert_true(tap->isPendingWrite);
    tap->isPendingWrite = 0;
    Assert_true(tap->writeMessageCount--);

    struct Message* msg = tap->writeMsgs[0];
    if (Message_getLength(msg) != (int)bytesWritten) {
        Log_info(tap->log, "Message of length [%d] truncated to [%d]",
                 Message_getLength(msg), (int)bytesWritten);
        Assert_true(Message_getLength(msg) > (int)bytesWritten);
    }

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

static Iface_DEFUN sendMessage(struct Message* msg, struct Iface* iface)
{
    struct TAPInterface_pvt* tap = Identity_check((struct TAPInterface_pvt*) iface);
    if (tap->writeMessageCount >= WRITE_MESSAGE_SLOTS) {
        Log_info(tap->log, "DROP message because the tap is lagging");
        return Error(OVERFLOW);
    }
    if (!tap->pendingWritesAlloc) {
        tap->pendingWritesAlloc = Allocator_child(tap->alloc);
    }
    tap->writeMsgs[tap->writeMessageCount++] = msg;
    Allocator_adopt(tap->pendingWritesAlloc, Message_getAlloc(msg));
    if (tap->writeMessageCount == 1) {
        postWrite(tap);
    }
    return Error(NONE);
}

Er_DEFUN(struct TAPInterface* TAPInterface_new(const char* preferredName,
                                      struct Log* logger,
                                      struct EventBase* base,
                                      struct Allocator* alloc))
{
    Log_debug(logger, "Getting TAP-Windows device name");

    struct TAPDevice* dev = Er(TAPDevice_find(preferredName, alloc));

    Er(NetDev_flushAddresses(dev->name, alloc));

    Log_debug(logger, "Opening TAP-Windows device [%s] at location [%s]", dev->name, dev->path);

    struct TAPInterface_pvt* tap = Allocator_calloc(alloc, sizeof(struct TAPInterface_pvt), 1);
    Identity_set(tap);
    tap->base = base;
    tap->alloc = alloc;
    tap->log = logger;
    tap->pub.assignedName = dev->name;
    tap->pub.generic.send = sendMessage;

    tap->handle = CreateFile(dev->path,
                             GENERIC_READ | GENERIC_WRITE,
                             0,
                             0,
                             OPEN_EXISTING,
                             FILE_ATTRIBUTE_SYSTEM | FILE_FLAG_OVERLAPPED,
                             0);

    if (tap->handle == INVALID_HANDLE_VALUE) {
        WinEr_fail(alloc, "CreateFile(tapDevice)", GetLastError());
    }

    struct EventBase_pvt* ebp = EventBase_privatize(tap->base);
    int ret;
    if ((ret = uv_iocp_start(ebp->loop, &tap->readIocp, tap->handle, readCallback))) {
        Er_raise(alloc, "uv_iocp_start(readIocp): %s", uv_strerror(ret));
    }
    if ((ret = uv_iocp_start(ebp->loop, &tap->writeIocp, tap->handle, writeCallback))) {
        Er_raise(alloc, "uv_iocp_start(writeIocp): %s", uv_strerror(ret));
    }

    struct TAPInterface_Version_pvt ver = { .major = 0 };
    Er(getVersion(tap->handle, &ver, alloc));

    Er(setEnabled(tap->handle, 1, alloc));

    Log_info(logger, "Opened TAP-Windows device [%s] version [%lu.%lu.%lu] at location [%s]",
             dev->name, ver.major, ver.minor, ver.debug, dev->path);

    // begin listening.
    postRead(tap);

    Er_ret(&tap->pub);
}
