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

#define _POSIX_SOURCE // fdopen()

#include "admin/Admin.h"
#include "admin/angel/AngelChan.h"
#include "admin/angel/Waiter.h"
#include "admin/AuthorizedPasswords.h"
#include "benc/Int.h"
#include "benc/serialization/BencSerializer.h"
#include "benc/serialization/standard/StandardBencSerializer.h"
#include "crypto/Crypto.h"
#include "dht/ReplyModule.h"
#include "dht/SerializationModule.h"
#include "dht/dhtcore/RouterModule_admin.h"
#include "exception/AbortHandler.h"
#include "exception/WriteErrorHandler.h"
#include "interface/UDPInterface_admin.h"
#include "io/ArrayReader.h"
#include "io/FileWriter.h"
#include "io/Reader.h"
#include "io/Writer.h"
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "net/Ducttape.h"
#include "net/DefaultInterfaceController.h"
#include "net/SwitchPinger.h"
#include "net/SwitchPinger_admin.h"
#include "switch/SwitchCore.h"

#include <crypto_scalarmult_curve25519.h>

#include <stdlib.h>
#include <unistd.h>

// Failsafe: abort if more than 2^22 bytes are allocated (4MB)
#define ALLOCATOR_FAILSAFE (1<<22)

static void parsePrivateKey(uint8_t privateKey[32],
                            struct Address* addr,
                            struct Except* eh)
{
    crypto_scalarmult_curve25519_base(addr->key, privateKey);
    AddressCalc_addressForPublicKey(addr->ip6.bytes, addr->key);
    if (addr->ip6.bytes[0] != 0xFC) {
        Except_raise(eh, -1, "Ip address outside of the FC00/8 range, invalid private key.");
    }
}

static void adminPing(Dict* input, void* vadmin, String* txid)
{
    Dict d = Dict_CONST(String_CONST("q"), String_OBJ(String_CONST("pong")), NULL);
    Admin_sendMessage(&d, txid, (struct Admin*) vadmin);
}

struct MemoryContext
{
    struct Allocator* allocator;
    struct Admin* admin;
};

static void adminMemory(Dict* input, void* vcontext, String* txid)
{
    struct MemoryContext* context = vcontext;
    Dict d = Dict_CONST(
        String_CONST("bytes"), Int_OBJ(MallocAllocator_bytesAllocated(context->allocator)), NULL
    );
    Admin_sendMessage(&d, txid, context->admin);
}

static Dict* getInitialConfig(int fromAngel,
                              struct event_base* eventBase,
                              struct Allocator* alloc,
                              struct Except* eh)
{
    uint8_t buff[AngelChan_INITIAL_CONF_BUFF_SIZE] = {0};
    uint32_t amountRead =
        Waiter_getData(buff, AngelChan_INITIAL_CONF_BUFF_SIZE, fromAngel, eventBase, eh);
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

static void sendResponse(int toAngel, uint8_t syncMagic[8])
{
    char buff[64] =
        "d"
          "5:angel" "d"
            "9:syncMagic" "16:\1\1\2\3\4\5\6\7\1\1\2\3\4\5\6\7"
          "e"
        "e";

    uint32_t length = strlen(buff);
    char* location = strstr(buff, "\1\1\2\3\4\5\6\7\1\1\2\3\4\5\6\7");
    Assert_true(location > buff);
    Hex_encode((uint8_t*)location, 16, syncMagic, 8);
    write(toAngel, buff, length);
}

/*
 * This process is started with 2 parameters, they must all be numeric in base 10.
 * toAngel the pipe which is used to send data back to the angel process.
 * fromAngel the pipe which is used to read incoming data from the angel.
 *
 * Upon initialization, this process will wait for an initial configuration to be sent to
 * it and then it will send an initial response.
 */
int main(int argc, char** argv)
{
    uint8_t syncMagic[8];
    randombytes(syncMagic, 8);

    struct Except* eh = AbortHandler_INSTANCE;
    int toAngel;
    int fromAngel;
    if (argc != 3
        || !(toAngel = atoi(argv[1]))
        || !(fromAngel = atoi(argv[2])))
    {
        Except_raise(eh, -1, "This is internal to cjdns and shouldn't started manually.");
    }

    struct Allocator* alloc = MallocAllocator_new(ALLOCATOR_FAILSAFE);


    FILE* toAngelF = fdopen(toAngel, "w");
    Assert_always(toAngelF != NULL);
    struct Writer* toAngelWriter = FileWriter_new(toAngelF, alloc);
    eh = WriteErrorHandler_new(toAngelWriter, alloc);

    struct event_base* eventBase = event_base_new();

    struct Writer* logwriter = FileWriter_new(stdout, alloc);
    struct Log* logger = &(struct Log) { .writer = logwriter };

    Dict* config = getInitialConfig(fromAngel, eventBase, alloc, eh);
    String* privateKeyHex = Dict_getString(config, String_CONST("privateKey"));
    Dict* adminConf = Dict_getDict(config, String_CONST("admin"));
    String* pass = Dict_getString(adminConf, String_CONST("pass"));
    if (!pass || !privateKeyHex) {
        Except_raise(eh, -1, "Expected 'pass' and 'privateKey' in configuration.");
    }
    uint8_t privateKey[32];
    if (privateKeyHex->len != 64
        || Hex_decode(privateKey, 32, (uint8_t*) privateKeyHex->bytes, 64) != 32)
    {
        Except_raise(eh, -1, "privateKey must be 64 bytes of hex.");
    }

    sendResponse(toAngel, syncMagic);

    struct Admin* admin = Admin_new(fromAngel, toAngel, alloc, logger, eventBase, pass, syncMagic);


    // CryptoAuth
    struct Address addr;
    parsePrivateKey(privateKey, &addr, eh);
    struct CryptoAuth* cryptoAuth = CryptoAuth_new(alloc, privateKey, eventBase, logger);

    struct SwitchCore* switchCore = SwitchCore_new(logger, alloc);
    struct DHTModuleRegistry* registry = DHTModuleRegistry_new(alloc);
    ReplyModule_register(registry, alloc);

    // Router
    struct RouterModule* router = RouterModule_register(registry,
                                                        alloc,
                                                        addr.key,
                                                        eventBase,
                                                        logger,
                                                        admin);

    SerializationModule_register(registry, alloc);

    struct Ducttape* dt = Ducttape_register(privateKey,
                                            registry,
                                            router,
                                            switchCore,
                                            eventBase,
                                            alloc,
                                            logger,
                                            admin);

    struct SwitchPinger* sp =
        SwitchPinger_new(&dt->switchPingerIf, eventBase, logger, alloc);

    // Interfaces.
    struct InterfaceController* ifController =
        DefaultInterfaceController_new(cryptoAuth,
                                       switchCore,
                                       router,
                                       logger,
                                       eventBase,
                                       sp,
                                       alloc);

    // ------------------- Register RPC functions ----------------------- //
    SwitchPinger_admin_register(sp, admin, alloc);
    UDPInterface_admin_register(eventBase, alloc, logger, admin, ifController);
    RouterModule_admin_register(router, admin, alloc);
    AuthorizedPasswords_init(admin, cryptoAuth, alloc);
    Admin_registerFunction("ping", adminPing, admin, false, NULL, admin);

    struct MemoryContext* mc =
        alloc->clone(sizeof(struct MemoryContext), alloc,
            &(struct MemoryContext) {
                .allocator = alloc,
                .admin = admin
            });
    Admin_registerFunction("memory", adminMemory, mc, false, NULL, admin);


    event_base_dispatch(eventBase);
}
