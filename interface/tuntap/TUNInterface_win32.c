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
#include "interface/Iface.h"
#include "interface/tuntap/TUNInterface.h"
#include "util/CString.h"
#include "exception/WinEr.h"

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

typedef struct Ctx_s {
    HANDLE quitEvent;
    volatile BOOL shouldQuit;

    WINTUN_ADAPTER_HANDLE adapter;
    HANDLE workers[2];
    WINTUN_SESSION_HANDLE session;

    Identity
} Ctx_t;

static int onFree(struct Allocator_OnFreeJob* job)
{
    Ctx_t* ctx = Identity_check((Ctx_t*) job->userData);
    ctx->shouldQuit = TRUE;
    if (ctx->quitEvent) {
        SetEvent(ctx->quitEvent);
    }
    for (size_t i = 0; i < _countof(ctx->workers); i++) {
        if (!ctx->workers[i]) { continue; }
        WaitForSingleObject(ctx->workers[i], INFINITE);
        CloseHandle(ctx->workers[i]);
    }
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
}

static DWORD WINAPI receivePackets(_Inout_ DWORD_PTR vctx)
{
    Ctx_t* ctx = Identity_check((Ctx_t*) vctx);
    HANDLE WaitHandles[] = { WintunGetReadWaitEvent(ctx->session), ctx->quitEvent };

    while (!ctx->shouldQuit) {
        DWORD packetSize = 0;
        BYTE* packet = WintunReceivePacket(ctx->session, &packetSize);
        if (packet) {
            struct Allocator* alloc = Allocator_child(ctx->alloc);
            struct Message* msg = Message_new(packetSize, 512, alloc);
            Bits_memcpy(msg->bytes, packet, packetSize);
            PrintPacket(Packet, PacketSize);
            WintunReleaseReceivePacket(Session, Packet);
        } else {
            DWORD LastError = GetLastError();
            switch (LastError)
            {
            case ERROR_NO_MORE_ITEMS:
                if (WaitForMultipleObjects(_countof(WaitHandles), WaitHandles, FALSE, INFINITE) == WAIT_OBJECT_0)
                    continue;
                return ERROR_SUCCESS;
            default:
                LogError(L"Packet read failed", LastError);
                return LastError;
            }
        }
    }
    return ERROR_SUCCESS;
}

static Er_DEFUN(Ctx_t* createIface(const char* interfaceName,
                                   char assignedInterfaceName[TUNInterface_IFNAMSIZ],
                                   int isTapMode,
                                   struct EventBase* base,
                                   struct Log* logger,
                                   struct Allocator* alloc))
{
    Er(initialize(alloc));

    Ctx_t* ctx = Allocator_calloc(alloc, sizeof(Ctx_t), 1);
    Identity_set(ctx);
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
    Log_info("Using Wintun v%u.%u", (winTunVer >> 16) & 0xff, winTunVer & 0xff);

    if (!(ctx->session = WintunStartSession(ctx->adapter, 1<<22))) {
        Er_raise(alloc, "Unable open or create TUN adapter [%d]",
            WinEr_strerror(GetLastError()));
    }

    ctx->workers[0] =
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)receivePackets, (LPVOID)ctx, 0, NULL);
    ctx->workers[1] =
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)sendPackets, (LPVOID)ctx, 0, NULL);
    
    if (!ctx->workers[0] || !ctx->workers[1]) {
        Er_raise(alloc, "Failed to create threads [%d]",
            WinEr_strerror(GetLastError()));
    }
    

    
    WaitForMultipleObjectsEx(_countof(Workers), Workers, TRUE, INFINITE, TRUE);
    LastError = ERROR_SUCCESS;

    // struct TAPInterface* tap = Er(TAPInterface_new(interfaceName, logger, base, alloc));
    // CString_safeStrncpy(assignedInterfaceName, tap->assignedName, TUNInterface_IFNAMSIZ);
    // if (isTapMode) { Er_ret(&tap->generic); }
    // struct TAPWrapper* tapWrapper = TAPWrapper_new(&tap->generic, logger, alloc);
    // struct NDPServer* ndp =
    //     NDPServer_new(&tapWrapper->internal, logger, TAPWrapper_LOCAL_MAC, alloc);
    // struct ARPServer* arp =
    //     ARPServer_new(&ndp->internal, logger, TAPWrapper_LOCAL_MAC, alloc);
    // Er_ret(&arp->internal);
    Er_ret(NULL);
}

Er_DEFUN(Ctx_t* TUNInterface_new(const char* interfaceName,
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
    Er_ret(out);
}