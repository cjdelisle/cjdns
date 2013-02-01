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
#include "admin/Admin.h"
#include "admin/AdminClient.h"
#include "admin/angel/Waiter.h"
#include "admin/angel/AngelInit.h"
#include "admin/angel/InterfaceWaiter.h"
#include "admin/testframework/AdminTestFramework.h"
#include "benc/String.h"
#include "benc/Int.h"
#include "benc/serialization/BencSerializer.h"
#include "benc/serialization/standard/StandardBencSerializer.h"
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "memory/BufferAllocator.h"
#include "memory/CanaryAllocator.h"
#include "interface/PipeInterface.h"
#include "interface/addressable/UDPAddrInterface.h"
#include "io/ArrayReader.h"
#include "io/ArrayWriter.h"
#include "io/FileWriter.h"
#include "io/Writer.h"
#include "util/events/EventBase.h"
#include "util/Assert.h"
#include "util/Errno.h"
#include "util/log/Log.h"
#include "util/log/WriterLog.h"
#include "util/Pipe.h"
#include "util/Process.h"
#define string_strlen
#define string_strchr
#define string_strcmp
#include "util/platform/libc/string.h"
#include <unistd.h>

static void spawnAngel(int* fromAngelOut, int* toAngelOut)
{
    int pipeToAngel[2];
    int pipeFromAngel[2];
    Assert_true(!Pipe_createUniPipe(pipeToAngel) && !Pipe_createUniPipe(pipeFromAngel));

    char pipeToAngelStr[8];
    snprintf(pipeToAngelStr, 8, "%d", pipeToAngel[0]);
    char pipeFromAngelStr[8];
    snprintf(pipeFromAngelStr, 8, "%d", pipeFromAngel[1]);
    char* args[] = { "angel", pipeToAngelStr, pipeFromAngelStr, NULL };

    uint8_t allocBuff[256];
    struct Allocator* tempAlloc = BufferAllocator_new(allocBuff, 256);
    char* path = Process_getPath(tempAlloc);
    Assert_true(path);
    Process_spawn(path, args);

    *fromAngelOut = pipeFromAngel[0];
    *toAngelOut = pipeToAngel[1];
}

/** @return a string representing the address and port to connect to. */
static void initAngel(int fromAngel,
                      int toAngel,
                      int corePipes[2][2],
                      struct PipeInterface** piOut,
                      struct EventBase* eventBase,
                      struct Log* logger,
                      struct Allocator* alloc,
                      struct Random* rand)
{
    #define TO_CORE (corePipes[0][1])
    #define FROM_CORE (corePipes[1][0])
    #define TO_ANGEL_AS_CORE (corePipes[1][1])
    #define FROM_ANGEL_AS_CORE (corePipes[0][0])

    Dict core = Dict_CONST(
        String_CONST("fromCore"), Int_OBJ(FROM_CORE), Dict_CONST(
        String_CONST("toCore"), Int_OBJ(TO_CORE), NULL
    ));
    Dict admin = Dict_CONST(
        String_CONST("bind"), String_OBJ(String_CONST("127.0.0.1")), Dict_CONST(
        String_CONST("core"), Dict_OBJ(&core), Dict_CONST(
        String_CONST("pass"), String_OBJ(String_CONST("abcd")), NULL
    )));
    Dict message = Dict_CONST(
        String_CONST("admin"), Dict_OBJ(&admin), NULL
    );

    struct Allocator* tempAlloc;
    BufferAllocator_STACK(tempAlloc, 1024);

    #define BUFFER_SZ 1023
    uint8_t buff[BUFFER_SZ + 1] = {0};
    struct Writer* w = ArrayWriter_new(buff, BUFFER_SZ, tempAlloc);
    StandardBencSerializer_get()->serializeDictionary(w, &message);

    Log_info(logger, "Writing intial configuration to angel on [%d] config: [%s]", toAngel, buff);
    write(toAngel, buff, w->bytesWritten(w));

    // This is angel->core data, we can throw this away.
    //Waiter_getData(buff, BUFFER_SZ, fromAngel, eventBase, NULL);
    //Log_info(logger, "Init message from angel to core: [%s]", buff);
    Bits_memset(buff, 0, BUFFER_SZ);

    struct PipeInterface* pi =
        PipeInterface_new(FROM_ANGEL_AS_CORE, TO_ANGEL_AS_CORE, eventBase, logger, alloc, rand);
    *piOut = pi;

    Log_info(logger, "PipeInterface [%p] is now ready.", (void*)pi);

    // Make sure the angel sends data to the core.
    InterfaceWaiter_waitForData(&pi->generic, eventBase, alloc, NULL);

    // Send response on behalf of core.
    char coreToAngelResponse[128] = "           PADDING              "
        "d"
          "5:error" "4:none"
        "e";

    char* start = strchr(coreToAngelResponse, 'd');
    struct Message m = {
        .bytes = (uint8_t*) start,
        .length = strlen(start),
        .padding = start - coreToAngelResponse
    };
    pi->generic.sendMessage(&m, &pi->generic);

    // This is angel->client data, it will tell us which port was bound.
    Waiter_getData(buff, BUFFER_SZ, fromAngel, eventBase, NULL);

    printf("Response from angel to client: [%s]\n", buff);

    return;
}

/**
 * This spawns itself as the Angel process which spawns itself again as the core process.
 * The "core process" pipes all of its inputs back to the originating process
 */

struct AdminTestFramework* AdminTestFramework_setUp(int argc, char** argv)
{
    if (argc > 1 && !strcmp("angel", argv[1])) {
        exit(AngelInit_main(argc, argv));
    }

    struct Allocator* alloc = CanaryAllocator_new(MallocAllocator_new(1<<20), NULL);

    struct Writer* logwriter = FileWriter_new(stdout, alloc);
    Assert_always(logwriter);
    struct Log* logger = WriterLog_new(logwriter, alloc);

    struct EventBase* eventBase = EventBase_new(alloc);
    struct Random* rand = Random_new(alloc, logger, NULL);

    int fromAngel;
    int toAngel;
    int corePipes[2][2];
    if (Pipe_createUniPipe(corePipes[0]) || Pipe_createUniPipe(corePipes[1])) {
        Except_raise(NULL, -1, "Failed to create pipes [%s]", Errno_getString());
    }
    spawnAngel(&fromAngel, &toAngel);

    Log_info(logger, "Initializing Angel");
    struct PipeInterface* pi;
    initAngel(fromAngel, toAngel, corePipes, &pi, eventBase, logger, alloc, rand);

    struct Sockaddr_storage addr;
    Assert_true(!Sockaddr_parse("0.0.0.0", &addr));

    Log_info(logger, "Binding UDP admin socket");
    struct AddrInterface* udpAdmin =
        UDPAddrInterface_new(eventBase, &addr.addr, alloc, NULL, logger);

    String* password = String_new("abcd", alloc);
    struct Admin* admin = Admin_new(udpAdmin, alloc, logger, eventBase, password);

    // Now setup the client.

    struct AdminClient* client =
        AdminClient_new(udpAdmin->addr, password, eventBase, logger, alloc);

    Assert_always(client);

    return alloc->clone(sizeof(struct AdminTestFramework), alloc, &(struct AdminTestFramework) {
        .admin = admin,
        .client = client,
        .alloc = alloc,
        .eventBase = eventBase,
        .logger = logger,
        .addr = Sockaddr_clone(udpAdmin->addr, alloc),
        .angelInterface = &pi->generic
    });
}

void AdminTestFramework_tearDown(struct AdminTestFramework* framework)
{
    char buff[128] = "           PADDING              "
        "\xff\xff\xff\xff"
        "d"
          "1:q" "10:Angel_exit"
        "e";

    char* start = strchr(buff, '\xff');
    struct Message m = {
        .bytes = (uint8_t*) start,
        .length = strlen(start),
        .padding = start - buff
    };
    framework->angelInterface->sendMessage(&m, framework->angelInterface);
}
