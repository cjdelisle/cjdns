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
#include "admin/testframework/AdminTestFramework.h"
#include "benc/String.h"
#include "benc/serialization/BencSerializer.h"
#include "benc/serialization/standard/StandardBencSerializer.h"
#include "exception/AbortHandler.h"
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "memory/BufferAllocator.h"
#include "io/ArrayReader.h"
#include "io/FileWriter.h"
#include "io/Writer.h"
#include "util/Assert.h"
#include "util/log/Log.h"
#include "util/log/WriterLog.h"
#include "util/Process.h"

#include <event2/event.h>
#include <string.h>
#include <unistd.h>

#define SYNC_MAGIC "0001020304050607"
#define SYNC_MAGIC_BYTES "\x00\x01\x02\x03\x04\x05\x06\x07"

static void spawnAngel(int* fromAngelOut, int* toAngelOut)
{
    int pipeToAngel[2];
    int pipeFromAngel[2];
    Assert_true(!pipe(pipeToAngel) && !pipe(pipeFromAngel));

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

    int fromAngel = pipeFromAngel[0];
    int toAngel = pipeFromAngel[1];
    *fromAngelOut = fromAngel;
    *toAngelOut = toAngel;
}

/** @return a string representing the address and port to connect to. */
static String* initAngel(int fromAngel,
                         int toAngel,
                         struct event_base* eventBase,
                         struct Allocator* alloc)
{
    // Client sends this to the angel.
    const char* angelInitialData =
        "d"
          "5:admin" "d"
            "4:bind" "11:127.0.0.1:0"
            "4:pass" "4:abcd"
          "e"
        "e";

    write(toAngel, angelInitialData, strlen(angelInitialData));

    #define BUFFER_SZ 1023
    uint8_t buff[BUFFER_SZ + 1] = {0};

    // This is angel->core data, we can throw this away.
    Waiter_getData(buff, BUFFER_SZ, fromAngel, eventBase, AbortHandler_INSTANCE);

    const char* coreToAngelData =
        "d"
          "5:angel" "d"
            "9:syncMagic" "16:" SYNC_MAGIC
          "e"
        "e";

    write(toAngel, coreToAngelData, strlen(coreToAngelData));

    // This is angel->client data, it will tell us which port was bound.
    Waiter_getData(buff, BUFFER_SZ, fromAngel, eventBase, AbortHandler_INSTANCE);

    uint8_t allocBuff[256];
    struct Allocator* tempAlloc = BufferAllocator_new(allocBuff, 256);
    struct Reader* reader = ArrayReader_new(buff, BUFFER_SZ, tempAlloc);
    Dict configStore;
    Dict* config = &configStore;
    Assert_true(!StandardBencSerializer_get()->parseDictionary(reader, tempAlloc, config));

    Dict* admin = Dict_getDict(config, String_CONST("admin"));
    String* bind = Dict_getString(admin, String_CONST("bind"));
    Assert_true(bind);

    return String_clone(bind, alloc);
}

/**
 * This spawns itself as the Angel process which spawns itself again as the core process.
 * The "core process" pipes all of it's inputs back to the originating process
 */

struct AdminTestFramework* AdminTestFramework_setUp(int argc, char** argv)
{
    if (argc == 4 && !strcmp("angel", argv[1])) {
        exit(AngelInit_main(argc, argv));
    }

    struct Allocator* alloc = MallocAllocator_new(1<<20);

    struct Writer* logwriter = FileWriter_new(stdout, alloc);
    Assert_always(logwriter);
    struct Log* logger = WriterLog_new(logwriter, alloc);

    struct event_base* eventBase = event_base_new();

    int fromAngel;
    int toAngel;
    spawnAngel(&fromAngel, &toAngel);

    String* addrStr = initAngel(fromAngel, toAngel, eventBase, alloc);

    String* password = String_new("abcd", alloc);
    uint8_t syncMagic[8] = SYNC_MAGIC_BYTES;
    struct Admin* admin =
        Admin_new(fromAngel, toAngel, alloc, logger, eventBase, password, syncMagic);


    // Now setup the client.

    struct sockaddr_storage addr;
    int addrLen = sizeof(struct sockaddr_storage);
    memset(&addr, 0, sizeof(struct sockaddr_storage));
    Assert_true(!evutil_parse_sockaddr_port(addrStr->bytes, (struct sockaddr*) &addr, &addrLen));

    struct AdminClient* client =
        AdminClient_new(&addr, addrLen, password, eventBase, logger, alloc);

    Assert_always(client);

    return alloc->clone(sizeof(struct AdminTestFramework), alloc, &(struct AdminTestFramework) {
        .admin = admin,
        .client = client,
        .alloc = alloc,
        .eventBase = eventBase,
        .logger = logger
    });
}
