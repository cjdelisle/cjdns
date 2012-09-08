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

#define _POSIX_SOURCE 1 // fdopen()

#include "admin/angel/Angel.h"
#include "admin/angel/AngelChan.h"
#include "admin/angel/Waiter.h"
#include "benc/Dict.h"
#include "benc/String.h"
#include "benc/serialization/standard/StandardBencSerializer.h"
#include "benc/serialization/BencSerializer.h"
#include "io/ArrayReader.h"
#include "io/FileReader.h"
#include "io/FileWriter.h"
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "exception/Except.h"
#include "exception/AbortHandler.h"
#include "util/Bits.h"
#include "util/Assert.h"
#include "util/Security.h"

#include <unistd.h>
#include <event2/event.h>
#include <stdint.h>
#include <errno.h>

/**
 * Spawn a new process.
 *
 * @param binaryPath the path to the file to execute.
 * @param args a list of strings representing the arguments to the command followed by NULL.
 * @return 0 if all went well, -1 if forking fails.
 */
static int spawnProcess(char* binaryPath, char** args, struct Allocator* alloc)
{
    int pid = fork();
    if (pid < 0) {
        return -1;
    } else if (pid == 0) {
        char** argv;
        {
            int argCount;
            for (argCount = 0; args[argCount]; argCount++);
            argv = alloc->malloc((argCount + 1) * sizeof(char*), alloc);
            argv[argCount + 1] = NULL;
        }
        for (int i = 1; args[i - 1]; i++) {
            argv[i] = args[i - 1];
        }
        argv[0] = binaryPath;
        // Goodbye :)
        execvp(binaryPath, argv);
        _exit(72);
    }
    return 0;
}

/**
 * Initialize the core.
 *
 * @param coreBinaryPath the path to the core binary.
 * @param toCore a pointer to an int which will be set to the
 *               file descriptor for writing to the core.
 * @param fromCore a pointer to an int which will be set to the
 *                 file descriptor for reading from the core.
 * @param alloc an allocator.
 * @param eh an exception handler in case something goes wrong.
 */
static void initCore(char* coreBinaryPath,
                     int* toCore,
                     int* fromCore,
                     struct Allocator* alloc,
                     struct Except* eh)
{
    int pipes[2][2];
    if (pipe(pipes[0]) || pipe(pipes[1])) {
        Except_raise(eh, -1, "Failed to create pipes [%s]", strerror(errno));
    }

    // Pipes used in the core process.
    #define TO_ANGEL (pipes[1][1])
    #define FROM_ANGEL (pipes[0][0])

    // Pipes used in the angel process (here).
    #define TO_CORE (pipes[0][1])
    #define FROM_CORE (pipes[1][0])

    char toAngel[32];
    char fromAngel[32];
    snprintf(toAngel, 32, "%u", TO_ANGEL);
    snprintf(fromAngel, 32, "%u", FROM_ANGEL);
    char* args[] = { toAngel, fromAngel, "20000", NULL };

    FILE* file;
    if ((file = fopen(coreBinaryPath, "r")) != NULL) {
        fclose(file);
    } else {
        Except_raise(eh, -1, "Can't open core executable [%s] for reading.", coreBinaryPath);
    }

    if (spawnProcess(coreBinaryPath, args, alloc)) {
        Except_raise(eh, -1, "Failed to spawn core process.");
    }

    *toCore = TO_CORE;
    *fromCore = FROM_CORE;
}

static void sendConfToCore(int toCore, struct Allocator* alloc, Dict* config, struct Except* eh)
{
    int toCore2 = dup(toCore);
    if (toCore2 == -1) {
        Except_raise(eh, -1, "Failed to duplicate file descriptor [%s]", strerror(errno));
    }
    FILE* ftoCore = fdopen(toCore2, "w");
    if (!ftoCore) {
        int err = errno;
        close(toCore2);
        Except_raise(eh, -1, "Failed to open file descriptor [%s]", strerror(err));
    }

    struct Writer* writer = FileWriter_new(ftoCore, alloc);
    if (StandardBencSerializer_get()->serializeDictionary(writer, config)) {
        fclose(ftoCore);
        Except_raise(eh, -1, "Failed to send configuration to core.");
    }
    fclose(ftoCore);
}

/**
 * @param bindAddr a string representation of the address to bind to, if 0.0.0.0:0,
 *                 an address will be chosen automatically.
 * @param alloc an allocator to build the output.
 * @param eh an exception handler which will be triggered if anything goes wrong.
 * @param sock a pointer which will be set to the bound socket.
 * @return a dictionary containing: "addr" which is a string representation of the ip address
 *         which was bound and "port" which is the bound port as an integer.
 */
static Dict* bindListener(String* bindAddr,
                          struct Allocator* alloc,
                          struct Except* eh,
                          evutil_socket_t* sock)
{
    struct sockaddr_storage addr;
    int addrLen = sizeof(struct sockaddr_storage);
    memset(&addr, 0, sizeof(struct sockaddr_storage));
    if (evutil_parse_sockaddr_port(bindAddr->bytes, (struct sockaddr*) &addr, &addrLen)) {
        Except_raise(eh, -1, "Unable to parse [%s] as an ip address and port, "
                             "eg: 127.0.0.1:11234", bindAddr->bytes);
    }

    if (!addr.ss_family) {
        addr.ss_family = AF_INET;
        // Apple gets mad if the length is wrong.
        addrLen = sizeof(struct sockaddr_in);
    }

    evutil_socket_t listener = socket(addr.ss_family, SOCK_STREAM, 0);
    if (listener < 0) {
        Except_raise(eh, -1, "Failed to allocate socket() [%s]", strerror(errno));
    }

    evutil_make_listen_socket_reuseable(listener);

    if (bind(listener, (struct sockaddr*) &addr, addrLen) < 0) {
        int err = errno;
        EVUTIL_CLOSESOCKET(listener);
        Except_raise(eh, -1, "Failed to bind() socket [%s]", strerror(err));
    }

    if (getsockname(listener, (struct sockaddr*) &addr, (ev_socklen_t*) &addrLen)) {
        int err = errno;
        EVUTIL_CLOSESOCKET(listener);
        Except_raise(eh, -1, "Failed to get socket name [%s]", strerror(err));
    }

    if (listen(listener, 16) < 0) {
        int err = errno;
        EVUTIL_CLOSESOCKET(listener);
        Except_raise(eh, -1, "Failed to listen on socket [%s]", strerror(err));
    }

    char addrStr[40];
    uint16_t port;
    switch(addr.ss_family) {
        case AF_INET:
            evutil_inet_ntop(AF_INET, &(((struct sockaddr_in*)&addr)->sin_addr), addrStr, 40);
            port = ((struct sockaddr_in*)&addr)->sin_port;
            break;
        case AF_INET6:
            evutil_inet_ntop(AF_INET6, &(((struct sockaddr_in6*)&addr)->sin6_addr), addrStr, 40);
            port = ((struct sockaddr_in6*)&addr)->sin6_port;
            break;
        default:
            Assert_always(false);
    }

    Dict* out = Dict_new(alloc);
    Dict_putString(out, String_CONST("addr"), String_new(addrStr, alloc), alloc);
    Dict_putInt(out, String_CONST("port"), port, alloc);
    *sock = listener;
    return out;
}

static Dict* getInitialConfigResponse(int fromCore,
                                      struct event_base* eventBase,
                                      struct Allocator* alloc,
                                      struct Except* eh)
{
    uint8_t buff[AngelChan_INITIAL_CONF_BUFF_SIZE] = {0};
    uint32_t amountRead =
        Waiter_getData(buff, AngelChan_INITIAL_CONF_BUFF_SIZE, fromCore, eventBase, eh);
    if (amountRead == AngelChan_INITIAL_CONF_BUFF_SIZE) {
        Except_raise(eh, -1, "initial config exceeds INITIAL_CONF_BUFF_SIZE");
    }

    struct Reader* reader = ArrayReader_new(buff, AngelChan_INITIAL_CONF_BUFF_SIZE, alloc);
    Dict* config = Dict_new(alloc);
    if (StandardBencSerializer_get()->parseDictionary(reader, alloc, config)) {
        Except_raise(eh, -1, "Failed to parse initial configuration.");
    }

    return config;
}

/**
 * Input:
 * {
 *   "admin": {
 *     "core": "/path/to/core/binary",
 *     "bind": "127.0.0.1:12345",
 *     "pass": "12345adminsocketpassword",
 *     "user": "setUidToThisUser"
 *   }
 * }
 * for example:
 * d5:admind4:core30:./build/admin/angel/cjdns-core4:bind15:127.0.0.1:123454:pass4:abcdee
 *
/home/user/wrk/cjdns/build/admin/angel/cjdns-core
 * "user" is optional, if set the angel will setuid() that user's uid.
 */
int main(int argc, char** argv)
{
    struct Except* eh = AbortHandler_INSTANCE;

    if (isatty(STDIN_FILENO)) {
        Except_raise(eh, -1, "This is internal to cjdns, it should not be started manually.");
    }

    struct Allocator* alloc = MallocAllocator_new(1<<20);
    struct Reader* reader = FileReader_new(stdin, alloc);
    Dict config;
    if (StandardBencSerializer_get()->parseDictionary(reader, alloc, &config)) {
        Except_raise(eh, -1, "Failed to parse configuration.");
    }

    Dict* admin = Dict_getDict(&config, String_CONST("admin"));
    String* core = Dict_getString(admin, String_CONST("core"));
    String* bind = Dict_getString(admin, String_CONST("bind"));
    String* pass = Dict_getString(admin, String_CONST("pass"));
    String* user = Dict_getString(admin, String_CONST("user"));

    if (!core || !bind || !pass) {
        Except_raise(eh, -1, "missing configuration params.");
    }

    evutil_socket_t tcpSocket;
    //Dict* boundAddr =
bindListener(bind, alloc, eh, &tcpSocket);

    int toCore;
    int fromCore;
    initCore(core->bytes, &toCore, &fromCore, alloc, eh);

    sendConfToCore(toCore, alloc, &config, eh);

    struct event_base* eventBase = event_base_new();
    Dict* configResp = getInitialConfigResponse(fromCore, eventBase, alloc, eh);
    Dict* adminResp = Dict_getDict(configResp, String_CONST("admin"));
    String* syncMagic = Dict_getString(adminResp, String_CONST("syncMagic"));
    if (!syncMagic || syncMagic->len != 8) {
        Except_raise(eh, -1, "didn't get proper syncMagic from core.");
    }

    if (user) {
        Security_setUser(user->bytes, NULL, eh);
    }

    Angel_start(pass, (uint8_t*)syncMagic->bytes, tcpSocket, toCore, fromCore, eventBase, alloc);
}
