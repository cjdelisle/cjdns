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
#include "util/events/libuv/EventBase_pvt.h"
#include "interface/Iface.h"
#include "interface/tuntap/TUNInterface.h"
#include "util/CString.h"
#include "exception/WinEr.h"
#include "wire/Message.h"

#include <stdio.h>

// MSVCisms which are not present in mingw-w64 as of the time of this writing
#define _Out_cap_c_(x)
#define _Ret_bytecount_(x)

#include "interface/tuntap/wintun.h"

/*
        WinEr_check(alloc, (
            RegOpenKeyEx(HKEY_LOCAL_MACHINE, connection_string, 0, KEY_READ, &connKey)
        ));
        */

static WINTUN_CREATE_ADAPTER_FUNC WintunCreateAdapter;
static WINTUN_DELETE_ADAPTER_FUNC WintunDeleteAdapter;
static WINTUN_DELETE_POOL_DRIVER_FUNC WintunDeletePoolDriver;
static WINTUN_ENUM_ADAPTERS_FUNC WintunEnumAdapters;
static WINTUN_FREE_ADAPTER_FUNC WintunFreeAdapter;
static WINTUN_OPEN_ADAPTER_FUNC WintunOpenAdapter;
static WINTUN_GET_ADAPTER_LUID_FUNC WintunGetAdapterLUID;
static WINTUN_GET_ADAPTER_NAME_FUNC WintunGetAdapterName;
static WINTUN_SET_ADAPTER_NAME_FUNC WintunSetAdapterName;
static WINTUN_GET_RUNNING_DRIVER_VERSION_FUNC WintunGetRunningDriverVersion;
static WINTUN_SET_LOGGER_FUNC WintunSetLogger;
static WINTUN_START_SESSION_FUNC WintunStartSession;
static WINTUN_END_SESSION_FUNC WintunEndSession;
static WINTUN_GET_READ_WAIT_EVENT_FUNC WintunGetReadWaitEvent;
static WINTUN_RECEIVE_PACKET_FUNC WintunReceivePacket;
static WINTUN_RELEASE_RECEIVE_PACKET_FUNC WintunReleaseReceivePacket;
static WINTUN_ALLOCATE_SEND_PACKET_FUNC WintunAllocateSendPacket;
static WINTUN_SEND_PACKET_FUNC WintunSendPacket;

static Er_DEFUN(void initialize(struct Allocator* alloc)) {
    if (WintunCreateAdapter) {
        Er_ret();
    }
    HMODULE lib = LoadLibraryExW(L"wintun.dll", NULL,
        LOAD_LIBRARY_SEARCH_APPLICATION_DIR | LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (!lib) {
        WinEr_fail(alloc, "Unable to load wintun.dll", GetLastError());
    }
    char* failedLoading = NULL;

    #define LOAD(Name, Type) \
        if (!failedLoading && ((Name = (Type)GetProcAddress(lib, #Name)) == NULL)) { \
            failedLoading = #Name; }
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wcast-function-type"
    LOAD(WintunCreateAdapter, WINTUN_CREATE_ADAPTER_FUNC)
    LOAD(WintunDeleteAdapter, WINTUN_DELETE_ADAPTER_FUNC)
    LOAD(WintunDeletePoolDriver, WINTUN_DELETE_POOL_DRIVER_FUNC)
    LOAD(WintunEnumAdapters, WINTUN_ENUM_ADAPTERS_FUNC)
    LOAD(WintunFreeAdapter, WINTUN_FREE_ADAPTER_FUNC)
    LOAD(WintunOpenAdapter, WINTUN_OPEN_ADAPTER_FUNC)
    LOAD(WintunGetAdapterLUID, WINTUN_GET_ADAPTER_LUID_FUNC)
    LOAD(WintunGetAdapterName, WINTUN_GET_ADAPTER_NAME_FUNC)
    LOAD(WintunSetAdapterName, WINTUN_SET_ADAPTER_NAME_FUNC)
    LOAD(WintunGetRunningDriverVersion, WINTUN_GET_RUNNING_DRIVER_VERSION_FUNC)
    LOAD(WintunSetLogger, WINTUN_SET_LOGGER_FUNC)
    LOAD(WintunStartSession, WINTUN_START_SESSION_FUNC)
    LOAD(WintunEndSession, WINTUN_END_SESSION_FUNC)
    LOAD(WintunGetReadWaitEvent, WINTUN_GET_READ_WAIT_EVENT_FUNC)
    LOAD(WintunReceivePacket, WINTUN_RECEIVE_PACKET_FUNC)
    LOAD(WintunReleaseReceivePacket, WINTUN_RELEASE_RECEIVE_PACKET_FUNC)
    LOAD(WintunAllocateSendPacket, WINTUN_ALLOCATE_SEND_PACKET_FUNC)
    LOAD(WintunSendPacket, WINTUN_SEND_PACKET_FUNC)
    #pragma GCC diagnostic pop
    #undef LOAD

    if (failedLoading) {
        DWORD err = GetLastError();
        FreeLibrary(lib);
        Er_raise(alloc, "Unable to load symbol %s from wintun.dll [%s]",
            failedLoading, WinEr_strerror(err));
    }
    Er_ret();
}

typedef struct MsgLL_s {
    struct Message* msg;
    struct MsgLL_s* next;
} MsgLL_t;

typedef struct Ctx_s {
    struct Iface pub;
    uv_async_t notifier;

    // lock guarded
    uv_mutex_t lock;
    MsgLL_t* msgs;
    // end lock guarded

    HANDLE quitEvent;
    volatile BOOL shouldQuit;

    WINTUN_ADAPTER_HANDLE adapter;
    HANDLE receiveThread;
    WINTUN_SESSION_HANDLE session;

    struct Allocator* alloc;
    Identity
} Ctx_t;

static void incomingMsgs(uv_async_t* handle, int status)
{
    Ctx_t* ctx = Identity_containerOf(handle, Ctx_t, notifier);
    uv_mutex_lock(&ctx->lock);
    MsgLL_t* msgs = ctx->msgs;
    ctx->msgs = NULL;
    uv_mutex_unlock(&ctx->lock);
    while (msgs) {
        Iface_send(&ctx->pub, msgs->msg);
        struct Allocator* toFree = msgs->msg->alloc;
        msgs = msgs->next;
        Allocator_free(toFree);
    }
}

static int onFree(struct Allocator_OnFreeJob* job)
{
    Ctx_t* ctx = Identity_check((Ctx_t*) job->userData);
    ctx->shouldQuit = TRUE;
    if (ctx->quitEvent) {
        SetEvent(ctx->quitEvent);
    }
    WaitForSingleObject(ctx->receiveThread, INFINITE);
    CloseHandle(ctx->receiveThread);
    if (ctx->session) {
        WintunEndSession(ctx->session);
    }
    if (ctx->adapter) {
        WintunDeleteAdapter(ctx->adapter, FALSE, NULL);
        WintunFreeAdapter(ctx->adapter);
    }
    if (ctx->quitEvent) {
        CloseHandle(ctx->quitEvent);
    }
    uv_mutex_destroy(&ctx->lock);
    return 0;
}

static DWORD WINAPI receivePackets(LPVOID vctx)
{
    Ctx_t* ctx = Identity_check((Ctx_t*) vctx);
    HANDLE handles[] = { WintunGetReadWaitEvent(ctx->session), ctx->quitEvent };

    while (!ctx->shouldQuit) {
        DWORD packetSize = 0;
        BYTE* packet = WintunReceivePacket(ctx->session, &packetSize);
        if (packet) {
            struct Allocator* alloc = Allocator_child(ctx->alloc);
            struct Message* msg = Message_new(packetSize, 512, alloc);
            Bits_memcpy(msg->bytes, packet, packetSize);
            WintunReleaseReceivePacket(ctx->session, packet);
            MsgLL_t* ll = Allocator_calloc(alloc, sizeof(MsgLL_t), 1);
            ll->msg = msg;
            uv_mutex_lock(&ctx->lock);
                ll->next = ctx->msgs;
                ctx->msgs = ll;
            uv_mutex_unlock(&ctx->lock);
        } else {
            DWORD le = GetLastError();
            if (le != ERROR_NO_MORE_ITEMS) {
                printf("Error getting message from TUN [%s]\n", WinEr_strerror(le));
                continue;
            }
            DWORD ret = WaitForMultipleObjects(_countof(handles), handles, FALSE, INFINITE);
            if (ret == WAIT_OBJECT_0) {
                continue;
            }
            break;
        }
    }
    return ERROR_SUCCESS;
}

static Iface_DEFUN sendMessage(struct Message* msg, struct Iface* iface)
{
    Ctx_t* ctx = Identity_containerOf(iface, Ctx_t, pub);
    BYTE* packet = WintunAllocateSendPacket(ctx->session, msg->length);
    Bits_memcpy(packet, msg->bytes, msg->length);
    WintunSendPacket(ctx->session, packet);
    Allocator_free(msg->alloc);
    return Error(NONE);
}

static Er_DEFUN(Ctx_t* createIface(const char* interfaceName,
                                   char assignedInterfaceName[TUNInterface_IFNAMSIZ],
                                   int isTapMode,
                                   struct EventBase* eventBase,
                                   struct Log* log,
                                   struct Allocator* alloc))
{
    Er(initialize(alloc));

    Ctx_t* ctx = Allocator_calloc(alloc, sizeof(Ctx_t), 1);
    Identity_set(ctx);
    ctx->pub.send = sendMessage;
    uv_async_init(EventBase_privatize(eventBase)->loop, &ctx->notifier, incomingMsgs);
    uv_mutex_init(&ctx->lock);
    ctx->alloc = alloc;
    Allocator_onFree(alloc, onFree, ctx);

    if (!(ctx->quitEvent = CreateEventW(NULL, TRUE, FALSE, NULL))) {
        Er_raise(alloc, "Unable create quitEvent [%d]",
            WinEr_strerror(GetLastError()));
    }

    // This identifies cjdns devices so the same one will be used and not another VPN device
    GUID guid;
    Bits_memcpy(&guid, "THE_NEW_INTERNET", 16);

    if (!(ctx->adapter = WintunOpenAdapter(L"cjdns", L"cjdns"))) {
        if (!(ctx->adapter = WintunCreateAdapter(L"cjdns", L"cjdns", &guid, NULL))) {
            Er_raise(alloc, "Unable open or create TUN adapter [%d]",
                WinEr_strerror(GetLastError()));
        }
    }

    DWORD winTunVer = WintunGetRunningDriverVersion();
    Log_info(log, "Using Wintun v%u.%u", (winTunVer >> 16) & 0xff, winTunVer & 0xff);

    if (!(ctx->session = WintunStartSession(ctx->adapter, 1<<22))) {
        Er_raise(alloc, "Unable open or create TUN adapter [%d]",
            WinEr_strerror(GetLastError()));
    }

    if (!(ctx->receiveThread = CreateThread(NULL, 0, receivePackets, (LPVOID)ctx, 0, NULL))) {
        Er_raise(alloc, "Failed to create receiver thread [%s]",
            WinEr_strerror(GetLastError()));
    }

    Er_ret(ctx);
}

Er_DEFUN(struct Iface* TUNInterface_new(const char* interfaceName,
                                   char assignedInterfaceName[TUNInterface_IFNAMSIZ],
                                   int isTapMode,
                                   struct EventBase* base,
                                   struct Log* logger,
                                   struct Allocator* allocator))
{
    // In case of error, we free this one in order to clean everything up
    struct Allocator* alloc = Allocator_child(allocator);

    struct Er_Ret* er = NULL;
    Ctx_t* out = Er_check(&er, createIface(
        interfaceName, assignedInterfaceName, isTapMode, base, logger, alloc));
    if (er) {
        Allocator_free(alloc);
        return er;
    }
    Er_ret(&out->pub);
}