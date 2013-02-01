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
#include "admin/angel/Angel.h"
#include "admin/angel/Waiter.h"
#include "admin/angel/InterfaceWaiter.h"
#include "benc/Dict.h"
#include "benc/String.h"
#include "benc/serialization/standard/StandardBencSerializer.h"
#include "benc/serialization/BencSerializer.h"
#include "interface/Interface.h"
#include "interface/PipeInterface.h"
#include "io/ArrayReader.h"
#include "io/ArrayWriter.h"
#include "io/FileReader.h"
#include "io/FileWriter.h"
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "memory/CanaryAllocator.h"
#include "exception/Jmp.h"
#include "exception/Except.h"
#include "util/events/EventBase.h"
#include "util/platform/libc/strlen.h"
#include "util/Bits.h"
#include "util/Assert.h"
#include "util/Errno.h"
#include "util/Hex.h"
#include "util/log/WriterLog.h"
#include "util/Pipe.h"
#include "util/Process.h"
#include "util/Security.h"
#include "wire/Message.h"

#include <unistd.h>
#include <stdint.h>

/**
 * Initialize the core.
 *
 * @param coreBinaryPath the path to the core binary.
 * @param toCore a pointer to an int which will be set to the
 *               file descriptor for writing to the core.
 * @param fromCore a pointer to an int which will be set to the
 *                 file descriptor for reading from the core.
 * @param eh an exception handler in case something goes wrong.
 */
static void initCore(char* coreBinaryPath,
                     int* toCore,
                     int* fromCore,
                     struct Except* eh)
{
    int pipes[2][2];
    if (Pipe_createUniPipe(pipes[0]) || Pipe_createUniPipe(pipes[1])) {
        Except_raise(eh, -1, "Failed to create pipes [%s]", Errno_getString());
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
    char* args[] = { "core", toAngel, fromAngel, NULL };

    FILE* file;
    if ((file = fopen(coreBinaryPath, "r")) != NULL) {
        fclose(file);
    } else {
        Except_raise(eh, -1, "Can't open core executable [%s] for reading.", coreBinaryPath);
    }

    if (Process_spawn(coreBinaryPath, args)) {
        Except_raise(eh, -1, "Failed to spawn core process.");
    }

    *toCore = TO_CORE;
    *fromCore = FROM_CORE;
}

static void sendConfToCore(struct Interface* toCoreInterface,
                           struct Allocator* alloc,
                           Dict* config,
                           struct Except* eh,
                           struct Log* logger)
{
    #define CONFIG_BUFF_SIZE 1024
    uint8_t buff[CONFIG_BUFF_SIZE + 32] = {0};
    uint8_t* start = buff + 32;

    struct Writer* writer = ArrayWriter_new(start, CONFIG_BUFF_SIZE - 33, alloc);
    if (StandardBencSerializer_get()->serializeDictionary(writer, config)) {
        Except_raise(eh, -1, "Failed to serialize pre-configuration for core.");
    }
    struct Message m = {
        .bytes = start,
        .length = writer->bytesWritten(writer),
        .padding = 32
    };
    Log_keys(logger, "Sent [%d] bytes to core [%s].", m.length, m.bytes);
    toCoreInterface->sendMessage(&m, toCoreInterface);
}

static void setUser(char* user, struct Log* logger, struct Except* eh)
{
    struct Jmp jmp;
    Jmp_try(jmp) {
        Security_setUser(user, logger, &jmp.handler);
    } Jmp_catch {
        if (jmp.code == Security_setUser_PERMISSION) {
            return;
        }
        Except_raise(eh, jmp.code, "%s", jmp.message);
    }
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
 * Pre-existing core mode:
 * {
 *   "admin": {
 *     "core": {
 *       "fromCore": 12,
 *       "toCore": 14
 *     },
 *     "bind": "127.0.0.1:12345",
 *     "pass": "12345adminsocketpassword",
 *     "user": "setUidToThisUser"
 *   }
 * }
 *
 * If "core" is a dictionary, the angel will behave as though the core is already spawned and
 * it will read from the core on the file descriptor given by "fromCore" and write to the file
 * given by "toCore".
 *
 * "user" is optional, if set the angel will setuid() that user's uid.
 */
int AngelInit_main(int argc, char** argv)
{
    struct Except* eh = NULL;

    int inFromClientNo;
    int outToClientNo;
    if (argc < 3 || (inFromClientNo = atoi(argv[2])) == 0) {
        inFromClientNo = STDIN_FILENO;
    }
    if (argc < 4 || (outToClientNo = atoi(argv[3])) == 0) {
        outToClientNo = STDOUT_FILENO;
    }

    struct Allocator* alloc = MallocAllocator_new(1<<20);
    struct Writer* logWriter = FileWriter_new(stdout, alloc);
    struct Log* logger = WriterLog_new(logWriter, alloc);
    struct Random* rand = Random_new(alloc, logger, eh);
    alloc = CanaryAllocator_new(alloc, rand);
    struct Allocator* tempAlloc = Allocator_child(alloc);
    struct EventBase* eventBase = EventBase_new(alloc);


    Log_debug(logger, "Initializing angel with input [%d] and output [%d]",
              inFromClientNo, outToClientNo);
    Log_debug(logger, "Getting pre-configuration from client");

    #define CONFIG_BUFF_SIZE 1024
    uint8_t buff[CONFIG_BUFF_SIZE] = {0};
    Waiter_getData(buff, CONFIG_BUFF_SIZE, inFromClientNo, eventBase, eh);

    Log_debug(logger, "Finished getting pre-configuration from client");

    struct Reader* reader = ArrayReader_new(buff, CONFIG_BUFF_SIZE, tempAlloc);
    Dict config;
    if (StandardBencSerializer_get()->parseDictionary(reader, tempAlloc, &config)) {
        Except_raise(eh, -1, "Failed to parse configuration.");
    }

    Dict* admin = Dict_getDict(&config, String_CONST("admin"));
    String* core = Dict_getString(admin, String_CONST("core"));
    String* bind = Dict_getString(admin, String_CONST("bind"));
    String* pass = Dict_getString(admin, String_CONST("pass"));
    String* user = Dict_getString(admin, String_CONST("user"));

    int toCore = -1;
    int fromCore = -1;
    if (!core) {
        Dict* coreDict = Dict_getDict(admin, String_CONST("core"));
        int64_t* toCorePtr = Dict_getInt(coreDict, String_CONST("toCore"));
        int64_t* fromCorePtr = Dict_getInt(coreDict, String_CONST("fromCore"));
        toCore = (toCorePtr) ? *toCorePtr : -1;
        fromCore = (fromCorePtr) ? *fromCorePtr : -1;
    }

    if (!bind || !pass || (!core && (toCore == -1 || fromCore == -1))) {
        Except_raise(eh, -1, "missing configuration params in preconfig. [%s]", buff);
    }

    if (core) {
        Log_info(logger, "Initializing core [%s]", core->bytes);
        initCore(core->bytes, &toCore, &fromCore, eh);
    }

    Log_debug(logger, "Sending pre-configuration to core.");
    struct PipeInterface* pif =
        PipeInterface_new(fromCore, toCore, eventBase, logger, alloc, rand);
    struct Interface* coreIface = &pif->generic;
    PipeInterface_waitUntilReady(pif);

    sendConfToCore(coreIface, tempAlloc, &config, eh, logger);

    struct Message* coreResponse = InterfaceWaiter_waitForData(coreIface, eventBase, tempAlloc, eh);
    write(outToClientNo, coreResponse->bytes, coreResponse->length);

    #ifdef Log_KEYS
        uint8_t lastChar = coreResponse->bytes[coreResponse->length-1];
        coreResponse->bytes[coreResponse->length-1] = 0;
        Log_keys(logger, "Sent [%s%c] to client.", coreResponse->bytes, lastChar);
        coreResponse->bytes[coreResponse->length-1] = lastChar;
    #endif

    if (user) {
        setUser(user->bytes, logger, eh);
    }

    Allocator_free(tempAlloc);
    Angel_start(coreIface, eventBase, logger, alloc);
    return 0;
}
