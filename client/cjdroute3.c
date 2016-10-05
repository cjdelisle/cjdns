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
#include "client/AdminClient.h"
#include "admin/angel/Core.h"
#include "admin/angel/InterfaceWaiter.h"
#include "client/Configurator.h"
#include "benc/Dict.h"
#include "benc/Int.h"
#include "benc/List.h"
#include "benc/serialization/BencSerializer.h"
#include "benc/serialization/json/JsonBencSerializer.h"
#include "benc/serialization/standard/BencMessageReader.h"
#include "benc/serialization/standard/BencMessageWriter.h"
#include "crypto/AddressCalc.h"
#include "crypto/CryptoAuth.h"
#include "dht/Address.h"
#include "exception/Except.h"
#include "interface/Iface.h"
#include "io/FileReader.h"
#include "io/FileWriter.h"
#include "io/Reader.h"
#include "io/Writer.h"
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "util/ArchInfo.h"
#include "util/Assert.h"
#include "util/Base32.h"
#include "util/CString.h"
#include "util/Defined.h"
#include "util/events/UDPAddrIface.h"
#include "util/events/Time.h"
#include "util/events/EventBase.h"
#include "util/events/Pipe.h"
#include "util/events/Process.h"
#include "util/events/FakeNetwork.h"
#include "util/Hex.h"
#include "util/log/Log.h"
#include "util/log/FileWriterLog.h"
#include "util/SysInfo.h"
#include "util/version/Version.h"

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#define DEFAULT_TUN_DEV "tun0"

static int usage(struct Allocator* alloc, char* appName)
{
    char* sysInfo = SysInfo_describe(SysInfo_detect(), alloc);
    printf("Cjdns %s %s\n"
           "Usage:\n"
           "    cjdroute-init --help                Print this information.\n"
           "    cjdroute-init PIPE_DIR PIPE_NAME PRIVATE_KEY ADMIN_BIND ADMIN_PASSWORD\n"
           "                                        Start the cjdroute-init pipe.\n"
           "                                        This pipe is where cjdroute itself gets\n"
           "                                        its base configuration from.\n"
           "    cjdroute core PIPE_DIR PIPE_NAME    Start cjdroute.\n"
           "\n",
           ArchInfo_getArchStr(), sysInfo);

    return 0;
}

int main(int argc, char** argv)
{
    Assert_ifParanoid(argc > 0);

    struct Allocator* allocator = MallocAllocator_new(1<<23);

    if (argc != 6 || (argc == 2 &&
        (!(CString_strcmp(argv[1], "--help") == 0) || (CString_strcmp(argv[1], "-h") == 0)))) {
        return usage(allocator, argv[0]);
    }

    struct Except* eh = NULL;
    struct EventBase* eventBase = EventBase_new(allocator);
    struct Log* logger = FileWriterLog_new(stdout, allocator);

    String* privateKey = String_new(argv[3], allocator);
    String* adminBind = String_new(argv[4], allocator);
    String* adminPass = String_new(argv[5], allocator);
    String* logTo = String_new("stdout", allocator);

    // --------------------- Welcome to cjdns ---------------------- //
    char* sysInfo = SysInfo_describe(SysInfo_detect(), allocator);
    Log_info(logger, "Cjdns %s %s", ArchInfo_getArchStr(), sysInfo);

    // --------------------- Setup Pipes to Angel --------------------- //
    struct Allocator* corePipeAlloc = Allocator_child(allocator);
    String* corePipeDir = String_new(argv[1], allocator);
    String* corePipeName = String_new(argv[2], allocator);
    if (!Defined(win32) && access(corePipeDir->bytes, W_OK)) {
        Except_throw(eh, "Don't have write permission to [%s].", corePipeDir->bytes);
    }
    Assert_ifParanoid(EventBase_eventCount(eventBase) == 0);
    struct Pipe* corePipe = Pipe_named(corePipeDir->bytes, corePipeName->bytes,
                                       eventBase, eh, corePipeAlloc);
    Assert_ifParanoid(EventBase_eventCount(eventBase) == 2);
    corePipe->logger = logger;

    // --------------------- Pre-Configure Core ------------------------- //
    Dict* preConf = Dict_new(allocator);
    Dict* adminPreConf = Dict_new(allocator);
    Dict* logPreConf = Dict_new(allocator);
    Dict_putDict(preConf, String_CONST("admin"), adminPreConf, allocator);
    Dict_putDict(preConf, String_CONST("logging"), logPreConf, allocator);
    Dict_putString(preConf, String_CONST("privateKey"), privateKey, allocator);
    Dict_putString(adminPreConf, String_CONST("bind"), adminBind, allocator);
    Dict_putString(adminPreConf, String_CONST("pass"), adminPass, allocator);
    Dict_putString(logPreConf, String_CONST("logTo"), logTo, allocator);

    struct Message* toCoreMsg = Message_new(0, 1024, allocator);
    BencMessageWriter_write(preConf, toCoreMsg, eh);
    Iface_CALL(corePipe->iface.send, toCoreMsg, &corePipe->iface);

    Log_debug(logger, "Sent [%d] bytes to core.", toCoreMsg->length);

    // --------------------- Get Response from Core --------------------- //

    struct Message* fromCoreMsg =
        InterfaceWaiter_waitForData(&corePipe->iface, eventBase, allocator, eh);
    Dict* responseFromCore = BencMessageReader_read(fromCoreMsg, allocator, eh);

    // --------------------- Close the Core Pipe --------------------- //
    Allocator_free(corePipeAlloc);
    corePipe = NULL;

    // --------------------- Get Admin Addr/Port/Passwd --------------------- //
    Dict* responseFromCoreAdmin = Dict_getDict(responseFromCore, String_CONST("admin"));
    adminBind = Dict_getString(responseFromCoreAdmin, String_CONST("bind"));

    if (!adminBind) {
        Except_throw(eh, "Didn't get ADMIN_BIND back from cjdroute.");
    }
    struct Sockaddr_storage adminAddr;
    if (Sockaddr_parse(adminBind->bytes, &adminAddr)) {
        Except_throw(eh, "Unable to parse [%s] as an IP address:port.",
                     adminBind->bytes);
    }

    Assert_ifParanoid(EventBase_eventCount(eventBase) == 0);

    Log_info(logger, "Admin API ready at [%s].", adminBind->bytes);

    return 0;
}
