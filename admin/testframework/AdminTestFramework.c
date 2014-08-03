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
#include "admin/angel/AngelInit.h"
#include "admin/angel/InterfaceWaiter.h"
#include "admin/testframework/AdminTestFramework.h"
#include "benc/String.h"
#include "benc/Int.h"
#include "benc/serialization/standard/BencMessageWriter.h"
#include "crypto/random/Random.h"
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "interface/FramingInterface.h"
#include "interface/addressable/UDPAddrInterface.h"
#include "io/FileWriter.h"
#include "io/Writer.h"
#include "util/events/EventBase.h"
#include "util/events/Pipe.h"
#include "util/events/Process.h"
#include "util/Assert.h"
#include "util/log/Log.h"
#include "util/log/WriterLog.h"

#include <unistd.h>
#include <stdlib.h>

static void spawnAngel(char* testName,
                       char* asClientPipeName,
                       struct EventBase* base,
                       struct Allocator* alloc)
{
    char* args[] = { testName, "angel", asClientPipeName, NULL };

    struct Allocator* tempAlloc = Allocator_child(alloc);
    char* path = Process_getPath(tempAlloc);
    Assert_true(path);
    Process_spawn(path, args, base, alloc);
    Allocator_free(tempAlloc);
}

/** @return a string representing the address and port to connect to. */
static void initAngel(struct Pipe* asClientPipe,
                      struct Interface* asCoreIface,
                      char* asCorePipeName,
                      struct EventBase* eventBase,
                      struct Log* logger,
                      struct Allocator* alloc,
                      struct Random* rand)
{
    Dict admin = Dict_CONST(
        String_CONST("bind"), String_OBJ(String_CONST("127.0.0.1")), Dict_CONST(
        String_CONST("corePipeName"), String_OBJ(String_CONST(asCorePipeName)), Dict_CONST(
        String_CONST("pass"), String_OBJ(String_CONST("abcd")), NULL
    )));
    Dict message = Dict_CONST(
        String_CONST("admin"), Dict_OBJ(&admin), NULL
    );

    struct Allocator* tempAlloc = Allocator_child(alloc);

    struct Message* toAngel = Message_new(0, 1024, tempAlloc);
    BencMessageWriter_write(&message, toAngel, NULL);

    Log_info(logger, "Writing intial configuration to angel on [%s]", asClientPipe->name);
    Interface_sendMessage(&asClientPipe->iface, toAngel);

    // This is client->angel->core data, we can throw this away.
    //struct Message* angelToCore =
    InterfaceWaiter_waitForData(asCoreIface, eventBase, tempAlloc, NULL);

    // unterminated string
    //Log_info(logger, "Init message from angel to core: [%s]", angelToCore->bytes);

    // Send response on behalf of core.
    Dict* coreToAngelResp = Dict_new(tempAlloc);
    Dict_putString(coreToAngelResp, String_CONST("error"), String_CONST("none"), tempAlloc);
    struct Message* coreToAngelMsg = Message_new(0, 256, tempAlloc);
    BencMessageWriter_write(coreToAngelResp, coreToAngelMsg, NULL);
    Interface_sendMessage(asCoreIface, coreToAngelMsg);

    // This is angel->client data, it will tell us which port was bound.
    struct Message* angelToClient =
        InterfaceWaiter_waitForData(&asClientPipe->iface, eventBase, tempAlloc, NULL);

    uint8_t lastByte = angelToClient->bytes[angelToClient->length-1];
    angelToClient->bytes[angelToClient->length-1] = '\0';
    printf("Response from angel to client: [%s%c]\n", angelToClient->bytes, (char)lastByte);

    Allocator_free(tempAlloc);

    return;
}

/**
 * This spawns itself as the Angel process which spawns itself again as the core process.
 * The "core process" pipes all of its inputs back to the originating process
 */

struct AdminTestFramework* AdminTestFramework_setUp(int argc, char** argv, char* testName)
{
    if (argc > 2 && !CString_strcmp(testName, argv[1]) && !CString_strcmp("angel", argv[2])) {
        exit(AngelInit_main(argc-1, &argv[1]));
    }

    struct Allocator* alloc = MallocAllocator_new(1<<20);

    struct Writer* logwriter = FileWriter_new(stdout, alloc);
    Assert_true(logwriter);
    struct Log* logger = WriterLog_new(logwriter, alloc);

    struct EventBase* eventBase = EventBase_new(alloc);
    struct Random* rand = Random_new(alloc, logger, NULL);


    char asClientPipeName[32] = {0};
    Random_base32(rand, (uint8_t*)asClientPipeName, 31);
    struct Pipe* asClientPipe = Pipe_named(asClientPipeName, eventBase, NULL, alloc);
    asClientPipe->logger = logger;

    char asCorePipeName[32] = {0};
    Random_base32(rand, (uint8_t*)asCorePipeName, 31);
    struct Pipe* asCorePipe = Pipe_named(asCorePipeName, eventBase, NULL, alloc);
    asCorePipe->logger = logger;
    struct Interface* asCoreIface = FramingInterface_new(65535, &asCorePipe->iface, alloc);

    spawnAngel(testName, asClientPipeName, eventBase, alloc);

    Log_info(logger, "Initializing Angel");
    initAngel(asClientPipe, asCoreIface, (char*)asCorePipe->name, eventBase, logger, alloc, rand);

    struct Sockaddr_storage addr;
    Assert_true(!Sockaddr_parse("127.0.0.1", &addr));

    Log_info(logger, "Binding UDP admin socket");
    struct AddrInterface* udpAdmin =
        UDPAddrInterface_new(eventBase, &addr.addr, alloc, NULL, logger);

    String* password = String_new("abcd", alloc);
    struct Admin* admin = Admin_new(udpAdmin, alloc, logger, eventBase, password);

    // Now setup the client.

    struct AdminClient* client =
        AdminClient_new(udpAdmin->addr, password, eventBase, logger, alloc);

    Assert_true(client);

    return Allocator_clone(alloc, (&(struct AdminTestFramework) {
        .admin = admin,
        .client = client,
        .alloc = alloc,
        .eventBase = eventBase,
        .logger = logger,
        .addr = Sockaddr_clone(udpAdmin->addr, alloc),
        .angelInterface = asCoreIface
    }));
}

void AdminTestFramework_tearDown(struct AdminTestFramework* framework)
{
    Dict* exitCmd = Dict_new(framework->alloc);
    Dict_putString(exitCmd, String_CONST("q"), String_CONST("Angel_exit"), framework->alloc);
    struct Message* msg = Message_new(0, 256, framework->alloc);
    BencMessageWriter_write(exitCmd, msg, NULL);
    Interface_sendMessage(framework->angelInterface, msg);
    Allocator_free(framework->alloc);
}
