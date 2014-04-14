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
#include "crypto/random/Random.h"
#include "crypto/CryptoAuth.h"
#include "dht/ReplyModule.h"
#include "dht/dhtcore/RouterModule.h"
#include "dht/SerializationModule.h"
#include "net/DefaultInterfaceController.h"
#include "io/Writer.h"
#include "io/FileWriter.h"
#include "util/log/Log.h"
#include "util/log/WriterLog.h"
#include "util/events/EventBase.h"
#include "util/CString.h"
#include "util/Hex.h"
#include "memory/MallocAllocator.h"
#include "memory/Allocator.h"
#include "switch/NumberCompress.h"
#include "switch/SwitchCore.h"
#include "wire/Headers.h"
#include "test/TestFramework.h"

static uint8_t messageFromInterface(struct Message* message, struct Interface* thisIf)
{
    *((struct Message**) thisIf->senderContext) = message;
    return 0;
}

static int reconnectionNewEndpointTest(struct InterfaceController* ifController,
                                       uint8_t* pk,
                                       struct Message** fromSwitchPtr,
                                       struct Allocator* alloc,
                                       struct EventBase* eventBase,
                                       struct Log* logger,
                                       struct Interface* routerIf,
                                       struct Random* rand)
{
    struct Message* message;
    struct Interface iface = {
        .sendMessage = messageFromInterface,
        .senderContext = &message,
        .allocator = alloc
    };

    uint8_t* buffer = Allocator_malloc(alloc, 512);

    struct Message* outgoing =
        &(struct Message) { .length = 0, .padding = 512, .bytes = buffer + 512 };

    struct CryptoAuth* externalCa = CryptoAuth_new(alloc, NULL, eventBase, logger, rand);
    struct Interface* wrapped = CryptoAuth_wrapInterface(&iface, pk, NULL, false, "", externalCa);
    CryptoAuth_setAuth(String_CONST("passwd"), 1, wrapped);

    struct Interface icIface = {
        .allocator = alloc,
        .sendMessage = messageFromInterface,
        .senderContext = &message
    };

    InterfaceController_registerPeer(ifController, NULL, NULL, true, false, &icIface);

    uint8_t hexBuffer[1025];

    for (int i = 0; i < 4; i++) {

        outgoing->length = 0;
        outgoing->padding = 512;
        outgoing->bytes = buffer + 512;
        Message_shift(outgoing, 12, NULL);
        Bits_memcpyConst(outgoing->bytes, "hello world", 12);

        Message_shift(outgoing, Headers_SwitchHeader_SIZE, NULL);
        Bits_memcpyConst(outgoing->bytes, (&(struct Headers_SwitchHeader) {
            .label_be = Endian_hostToBigEndian64(1),
            .lowBits_be = 0
        }), Headers_SwitchHeader_SIZE);

        wrapped->sendMessage(outgoing, wrapped);

        *fromSwitchPtr = NULL;

        icIface.receiveMessage(outgoing, &icIface);

        message = *fromSwitchPtr;
        Assert_true(message);
        Assert_true(message->length == 24);

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
            iface.receiveMessage(message, &iface);
            printf("forwarded.\n");
        } else {
            printf("not responding because we don't want to establish a connection yet.\n");
        }
    }

    // check everything except the label
    Assert_true(!CString_strcmp((char*)hexBuffer+16, "0000000068656c6c6f20776f726c6400"));
    // check label: make sure the interface has been switched back into position 0.
    uint64_t label_be;
    Hex_decode((uint8_t*) &label_be, 8, hexBuffer, 16);
    uint64_t rev_label = Bits_bitReverse64(Endian_bigEndianToHost64(label_be));
    // check label is decoded to 0
    Assert_true(0 == NumberCompress_getDecompressed(rev_label,
                                                      NumberCompress_bitsUsedForLabel(rev_label)));
    // check no other bits are set
    uint64_t out = NumberCompress_getCompressed(0, NumberCompress_bitsUsedForLabel(rev_label));
    Assert_true(rev_label == out);
    return 0;
}


int main()
{
    struct Allocator* alloc = MallocAllocator_new(1<<20);

    struct TestFramework* tf =
        TestFramework_setUp("\xad\x7e\xa3\x26\xaa\x01\x94\x0a\x25\xbc\x9e\x01\x26\x22\xdb\x69"
                            "\x4f\xd9\xb4\x17\x7c\xf3\xf8\x91\x16\xf3\xcf\xe8\x5c\x80\xe1\x4a",
                            alloc, NULL);

    CryptoAuth_addUser(String_CONST("passwd"), 1, String_CONST("TEST"), tf->cryptoAuth);

    struct Message* message;
    struct Interface iface = {
        .sendMessage = messageFromInterface,
        .senderContext = &message,
        .allocator = alloc
    };
    SwitchCore_setRouterInterface(&iface, tf->switchCore);


    ////////////////////////

    int ret = reconnectionNewEndpointTest(tf->ifController,
                                          tf->publicKey,
                                          &message,
                                          alloc,
                                          tf->eventBase,
                                          tf->logger,
                                          &iface,
                                          tf->rand);
    Allocator_free(alloc);
    return ret;
}
