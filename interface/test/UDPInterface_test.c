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
#include "admin/testframework/AdminTestFramework.h"
#include "admin/Admin.h"
#include "admin/AdminClient.h"
#include "benc/Dict.h"
#include "benc/String.h"
#include "benc/Int.h"
#include "interface/UDPInterface_admin.h"
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "net/InterfaceController.h"
#include "io/FileWriter.h"
#include "io/Writer.h"
#include "util/Log.h"

#include <event2/event.h>
#include "util/Assert.h"

uint8_t key[InterfaceController_KEY_SIZE];
typedef struct sockname_s {
    char* node;
    char* service;
    struct sockaddr_in* cached;
    uint8_t state;
    struct UDPInterface* udpif;
} testsockname;

static int insertEndpointCalls = 0;
static int insertEndpoint(int (*recalc)(uint8_t key[InterfaceController_KEY_SIZE],void*),
                          void* arg,
                          uint8_t herPublicKey[32],
                          String* password,
                          struct Interface* externalInterface,
                          struct InterfaceController* ic)
{
    insertEndpointCalls++;
    return recalc(key,arg);
}

static int registerInterfaceCalls = 0;
static void registerInterface(struct Interface* externalInterface,
                              struct InterfaceController* ic)
{
    registerInterfaceCalls++;
}

int main()
{
    struct AdminTestFramework* fw = AdminTestFramework_setUp();

    // mock interface controller.
    struct InterfaceController ifController = {
        .insertEndpoint = insertEndpoint,
        .registerInterface = registerInterface
    };

    UDPInterface_admin_register(fw->eventBase,
                                fw->alloc,
                                fw->logger,
                                fw->admin,
                                &ifController);

    Dict* dict = Dict_new(fw->alloc);
    struct AdminClient_Result* res = AdminClient_rpcCall(String_CONST("UDPInterface_new"),
                                                         dict,
                                                         fw->client,
                                                         fw->alloc);
    Assert_always(!res->err);
    //printf("result content: >>%s<<", res->messageBytes);
    Assert_always(!strcmp("d5:error4:none15:interfaceNumberi0ee", (char*) res->messageBytes));
    Assert_always(registerInterfaceCalls == 1);

    // bad key
    dict = Dict_new(fw->alloc);
    Dict_putString(dict, String_CONST("publicKey"), String_CONST("notAValidKey"), fw->alloc);
    Dict_putString(dict, String_CONST("address"), String_CONST("127.0.0.1:12345"), fw->alloc);
    res = AdminClient_rpcCall(
        String_CONST("UDPInterface_beginConnection"), dict, fw->client, fw->alloc);
    Assert_always(!strcmp("d5:error37:publicKey must be 52 characters long.e",
                          (char*) res->messageBytes));

    //printf("result content: >>%s<<", res->messageBytes);

    dict = Dict_new(fw->alloc);
    Dict_putString(dict,
                   String_CONST("publicKey"),
                   String_CONST("c86pf0ngj3wlb569juqm10yzv29n9t4w5tmsyhx6xd3fbqjlcu50.k"),
                   fw->alloc);
    Dict_putString(dict, String_CONST("address"), String_CONST("127.0.0.1:12345"), fw->alloc);
    res = AdminClient_rpcCall(
        String_CONST("UDPInterface_beginConnection"), dict, fw->client, fw->alloc);
    Assert_always(!strcmp("d5:error4:nonee", (char*) res->messageBytes));

    dict = Dict_new(fw->alloc);
    Dict_putString(dict,
                   String_CONST("publicKey"),
                   String_CONST("c86pf0ngj3wlb569juqm10yzv29n9t4w5tmsyhx6xd3fbqjlcu50.k"),
                   fw->alloc);
    Dict_putString(dict, String_CONST("address"),
                   String_CONST("hostname.is.invalid:20"), fw->alloc);
    res = AdminClient_rpcCall(
        String_CONST("UDPInterface_beginConnection"), dict,
        fw->client, fw->alloc);
    Assert_always(!strcmp("d5:error13:unknown errore",(char*)res->messageBytes));

    dict = Dict_new(fw->alloc);
    Dict_putString(dict,
                   String_CONST("publicKey"),
                   String_CONST("c86pf0ngj3wlb569juqm10yzv29n9t4w5tmsyhx6xd3fbqjlcu50.k"),
                   fw->alloc);
    Dict_putString(dict, String_CONST("address"), String_CONST("github.com:20"), fw->alloc);
    res = AdminClient_rpcCall(
        String_CONST("UDPInterface_beginConnection"), dict,
        fw->client, fw->alloc);
    Assert_always(!strcmp("d5:error4:nonee",(char*)res->messageBytes));

}
