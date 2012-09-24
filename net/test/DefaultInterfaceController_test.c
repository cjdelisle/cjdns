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
#include "crypto/CryptoAuth.h"
#include "dht/ReplyModule.h"
#include "dht/dhtcore/RouterModule.h"
#include "dht/SerializationModule.h"
#include "net/DefaultInterfaceController.h"
#include "io/Writer.h"
#include "io/FileWriter.h"
#include "util/Log.h"
#include "util/WriterLog.h"
#include "memory/MallocAllocator.h"
#include "memory/Allocator.h"
#include "switch/NumberCompress.h"
#include "switch/SwitchCore.h"
#include "wire/Headers.h"

static uint8_t messageFromInterface(struct Message* message, struct Interface* thisIf)
{
    *((struct Message**) thisIf->senderContext) = message;
    return 0;
}

static int reconnectionNewEndpointTest(struct InterfaceController* ifController,
                                       uint8_t* pk,
                                       struct Message** fromSwitchPtr,
                                       struct Allocator* alloc,
                                       struct event_base* eventBase,
                                       struct Log* logger,
                                       struct Interface* routerIf)
{
    struct Message* message;
    struct Interface iface = {
        .sendMessage = messageFromInterface,
        .senderContext = &message,
        .allocator = alloc
    };

    uint8_t* buffer = alloc->malloc(512, alloc);

    struct Message* outgoing =
        &(struct Message) { .length = 0, .padding = 512, .bytes = buffer + 512 };

    struct CryptoAuth* externalCa = CryptoAuth_new(alloc, NULL, eventBase, logger);
    struct Interface* wrapped = CryptoAuth_wrapInterface(&iface, pk, false, false, externalCa);
    CryptoAuth_setAuth(String_CONST("passwd"), 1, wrapped);

    struct Interface icIface = {
        .allocator = alloc,
        .sendMessage = messageFromInterface,
        .senderContext = &message
    };
    ifController->registerInterface(&icIface, ifController);

    char* majic = "\xDE\xAD\xBE\xEF\xCA\xFE\xBA\xBE";
    uint8_t hexBuffer[1025];

    for (int i = 0; i < 4; i++) {

        outgoing->length = 0;
        outgoing->padding = 512;
        outgoing->bytes = buffer + 512;
        Message_shift(outgoing, 12);
        Bits_memcpyConst(outgoing->bytes, "hello world", 12);

        Message_shift(outgoing, Headers_SwitchHeader_SIZE);
        Bits_memcpyConst(outgoing->bytes, (&(struct Headers_SwitchHeader) {
            .label_be = Endian_hostToBigEndian64(1),
            .lowBits_be = 0
        }), Headers_SwitchHeader_SIZE);

        wrapped->sendMessage(outgoing, wrapped);

        // add the id tag
        Message_shift(outgoing, InterfaceController_KEY_SIZE);
        Bits_memcpyConst(outgoing->bytes, majic, InterfaceController_KEY_SIZE);

        *fromSwitchPtr = NULL;

        icIface.receiveMessage(outgoing, &icIface);

        message = *fromSwitchPtr;
        Assert_always(message);
        Assert_always(message->length == 24);

        Hex_encode(hexBuffer, 1025, message->bytes, message->length);
        printf("%s\n", hexBuffer);

        // Need to bounce the packets back when connecting after the first try.
        // This is needed to establish the CryptoAuth session and make the InterfaceController
        // merge the endpoints.
        if (i > 0) {
            // Reverse the bits to reverse the path:
            uint64_t path;
            Bits_memcpyConst(&path, message->bytes, 8);
            path = Bits_bitReverse64(path);
            Bits_memcpyConst(message->bytes, &path, 8);

            printf("sending back response.\n");
            routerIf->receiveMessage(message, routerIf);
            printf("forwarding response to external cryptoAuth.\n");
            Message_shift(message, -InterfaceController_KEY_SIZE);
            iface.receiveMessage(message, &iface);
            printf("forwarded.\n");
        } else {
            printf("not responding because we don't want to establish a connection yet.\n");
        }

        majic = "\xC0\xFF\xEE\x00\x11\x22\x33\x44";
    }

    // check everything except the label
    Assert_always(!strcmp((char*)hexBuffer+16, "0000000068656c6c6f20776f726c6400"));
    // check label: make sure the interface has been switched back into position 0.
    uint64_t label_be;
    Hex_decode((uint8_t*) &label_be, 8, hexBuffer, 16);
    uint64_t rev_label = Bits_bitReverse64(Endian_bigEndianToHost64(label_be));
    // check label is decoded to 0
    Assert_always(0 == NumberCompress_getDecompressed(rev_label,
                                                      NumberCompress_bitsUsedForLabel(rev_label)));
    // check no other bits are set
    uint64_t out = NumberCompress_getCompressed(0, NumberCompress_bitsUsedForLabel(rev_label));
    Assert_always(rev_label == out);
    return 0;
}


int main()
{
    struct Allocator* alloc = MallocAllocator_new(1<<20);

    struct Writer* logwriter = FileWriter_new(stdout, alloc);
    struct Log* logger = WriterLog_new(logwriter, alloc);

    struct event_base* eventBase = event_base_new();

    struct CryptoAuth* ca = CryptoAuth_new(alloc, NULL, eventBase, logger);
    uint8_t publicKey[32];
    Bits_memcpyConst(publicKey, ca->publicKey, 32);
    CryptoAuth_addUser(String_CONST("passwd"), 1, (void*)0x01, ca);

    struct SwitchCore* switchCore = SwitchCore_new(logger, alloc);
    struct Message* message;
    struct Interface iface = {
        .sendMessage = messageFromInterface,
        .senderContext = &message,
        .allocator = alloc
    };
    SwitchCore_setRouterInterface(&iface, switchCore);

    // These are unused.
    struct DHTModuleRegistry* registry = DHTModuleRegistry_new(alloc);
    struct RouterModule* rm =
        RouterModule_register(registry, alloc, publicKey, eventBase, logger, NULL);

    struct InterfaceController* ifController =
        DefaultInterfaceController_new(ca, switchCore, rm, logger, eventBase, NULL, alloc);

    ////////////////////////

    return reconnectionNewEndpointTest(ifController,
                                       publicKey,
                                       &message,
                                       alloc,
                                       eventBase,
                                       logger,
                                       &iface);
}
