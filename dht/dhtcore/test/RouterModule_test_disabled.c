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
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <event2/event.h>

#include "dht/DHTModules.h"
#include "dht/SerializationModule.h"
#include "dht/ReplyModule.h"
#include "dht/dhtcore/Node.h"
#include "dht/dhtcore/NodeStore.h"
#include "dht/dhtcore/RouterModule.h"
#include "dht/dhtcore/RouterModule_struct.h"
#include "dht/dhtcore/test/TestFramework.h"
#include "memory/MemAllocator.h"
#include "memory/BufferAllocator.h"

#define MY_ADDRESS "hgzyxwvutsrqponmlkji           3"
// This key converts to address: fc39:c3ba:c711:00aa:666d:90b0:1ab6:e8c3
//                 this is the most significant byte ^^
// Which puts it in the center of our set of addresses.

void testQuery(struct DHTMessage** outMessagePtr,
               struct DHTModuleRegistry* registry,
               struct MemAllocator* allocator)
{
    *outMessagePtr = NULL;
    // on the wire  xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    //              d1:q2:fn 3:tar16: < ipv6  address > 4:txid2: aae
    const char* requestMessage =
        "d"
          "1:q"    "2:fn"
          "3:tar" "16:" "\xfc\x01\x01\x01\x01\x01\x01\x01\x21\x01\x01\x01\x01\x01\x01\x01"
          "4:txid" "2:aa"
        "e";

    struct Address addr;
    memset(&addr, 0, sizeof(struct Address));
    memcpy(&addr.key, "ponmlkjihgzyxwvutsrq           \0", 32);
    memcpy(&addr.networkAddress_be, " 00011  ", 8);
    struct DHTMessage message =
    {
        .length = strlen(requestMessage),
        .address = &addr,
        .allocator = allocator,
    };
    memcpy(message.bytes, requestMessage, message.length);

    DHTModules_handleIncoming(&message, registry);

    struct DHTMessage* outMessage = *outMessagePtr;
    assert(outMessage != NULL);
    assert(outMessage->replyTo != NULL);

    // Another hack because the output contains nulls, see: #define ADD_NODE()
    for (int i = 0; i < outMessage->length; i++) {
        if (outMessage->bytes[i] == '\0') {
            outMessage->bytes[i] = ' ';
        }
    }
    outMessage->bytes[outMessage->length] = '\0';

    printf("\n\n%s\n%d\n", outMessage->bytes, (int) outMessage->length);

    // on the wire  xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx
    //              d1:n280: < content - multiple  of forty  bytes long > 3:txid2: aae
    const char* expectedResponse =
        "d"
          "1:n" "280:"
            "cdefghijklmnopqrstuv             00009  "
            "qponmlkjihgzyxwvutsr             00001  "
            "fghijklmnopqrstuvwxy             00006  "
            "lkjihgzyxwvutsrqponm             00005  "
            "onmlkjihgzyxwvutsrqp             00003  "
            "bcdefghijklmnopqrstu             00002  "
            "mlkjihgzyxwvutsrqpon             00004  "
          "4:txid" "2:aa"
        "e";

    assert(outMessage->length == strlen(expectedResponse));
    assert(memcmp(outMessage->bytes, expectedResponse, strlen(expectedResponse)) == 0);
}

bool testSearch_callback(void* context, struct DHTMessage* message)
{
    *((struct DHTMessage**) context) = message;
    return true;
}

void testSearch(struct DHTMessage** outMessagePtr,
                struct RouterModule* routerModule,
                struct DHTModuleRegistry* registry,
                struct MemAllocator* allocator)
{
    *outMessagePtr = NULL;

    #define REQUEST_HASH "\xfc\x01\x01\x01\x01\x01\x01\x01\x21\x01\x01\x01\x01\x01\x01\x01"

    struct DHTMessage* callbackMessage = NULL;

    RouterModule_beginSearch((uint8_t*) REQUEST_HASH,
                             testSearch_callback,
                             &callbackMessage,
                             routerModule);

    struct DHTMessage* outMessage = *outMessagePtr;
    assert(outMessage != NULL);

    // Need to be able to work around the fact that the string contains nulls.
    #define EXPECTED_OUTPUT(tid) \
        "d"                                     \
          "1:q" "2:fn"                          \
          "3:tar" "16:" REQUEST_HASH            \
          "4:txid" "2:" tid                     \
        "e"

    for (uint32_t i = 0; i < (uint32_t) outMessage->length; i++) {
      //printf("%.2X", (unsigned int) outMessage->bytes[i] & 0xFF);
    }
    //printf("\n%s\n", outMessage->bytes);
    //printf("\n%s\n", outMessage->peerAddress);

    assert(outMessage->length == strlen(EXPECTED_OUTPUT("xx")));
    assert(memcmp(outMessage->bytes, EXPECTED_OUTPUT("8\x00"), outMessage->length) == 0);
    //assert(strcmp(outMessage->address->networkAddress, " 00014  ") == 0);
    // In a normal DHT, 00014 is the closest node, however, 00011 has sent us a message in
    // testQuery() and thus his reach is 1 and he beats all other nodes which are 0-reach.
    // Search queries are allowed to select nodes which are further from the target than us.
    assert(strcmp((char*) &outMessage->address->networkAddress_be, " 00011  ") == 0);

    #undef EXPECTED_OUTPUT

    #define CRAFTED_REPLY(tid) \
        "d"                                                    \
          "1:n" "200:"                                         \
            "97bkjs8qpd5hc1mubj3qbfyqzmzxp3rg" " 00017  "      \
            "by1szn122nqk1vncjtm612444rlh6ztr" " 00018  "      \
            "u42wbyr0wznhkbqr1r7u627dwsvb8853" " 00019  "      \
            "97bkjs8qpd5hc1mubj3qbfyqzmzxp3rg" " 00020  "      \
            "2lr8w01hhrxqng8mm8nf3nlwh5nyxzyl" " 00021  "      \
          "4:txid" "2:" tid                                    \
        "e"

    struct Address address = {
        .key = "ponmlkjihgzyxwvutsrq           \0"
    };
    memcpy(&address.networkAddress_be, " 00011  ", 8);

    struct DHTMessage message =
    {
        .length = strlen(CRAFTED_REPLY("xx")),
        .allocator = allocator,
        .address = &address
    };
    memcpy(message.bytes, CRAFTED_REPLY("8\x00"), message.length);
   // memcpy(message.peerAddress, peerAddress, 18);

    *outMessagePtr = NULL;

    DHTModules_handleIncoming(&message, registry);

    // Make sure the callback was called.
    assert(callbackMessage != NULL);

    // Make sure the node was promoted for it's fine service :P
    struct Address addr;
    memset(&addr, 0, sizeof(struct Address));
    memcpy(&addr.key, "ponmlkjihgzyxwvutsrq           \0", 32);
    struct Node* node1 =
        NodeStore_getNode(routerModule->nodeStore, &addr);
    //printf("node reach = %d", node1->reach);
    assert(node1->reach == 1601894175);

 /*   outMessage = *outMessagePtr;
    assert(outMessage != NULL);

    assert(strcmp("000022", outMessage->peerAddress) == 0);*/
}

int main()
{
    char buffer[1<<20];
    struct MemAllocator* allocator = BufferAllocator_new(buffer, 1<<20);
    struct DHTModuleRegistry* registry = DHTModules_new(allocator);

    ReplyModule_register(registry, allocator);
    struct RouterModule* routerModule =
        RouterModule_register(registry, allocator, (uint8_t*) MY_ADDRESS, event_base_new(), NULL);

    SerializationModule_register(registry, allocator);

    struct DHTMessage* outMessage;
    // dummy "network module" which just catches outgoing messages and makes them available.
    TestFramework_registerOutputCatcher(&outMessage, registry, allocator);

    struct Address addr;

    // damn this \0, was a mistake but to fix it would break all of the hashes :(
    #define ADD_NODE(address, netAddr) \
        memset(&addr, 0, sizeof(struct Address));                             \
        memcpy(&addr.networkAddress_be, netAddr "  ", 8);                     \
        memcpy(&addr.key, (uint8_t*) address "           \0", 32);            \
        RouterModule_addNode(&addr, routerModule)

//                                             most significant byte --vv
    ADD_NODE("qponmlkjihgzyxwvutsr", " 00001"); // fce8:573b:d230:ca3b 1c4e:f9d6 0632:9445
    ADD_NODE("bcdefghijklmnopqrstu", " 00002"); // fc65:9f4c:c061:84f9 2018:6e31 de3d:3bcf
    ADD_NODE("onmlkjihgzyxwvutsrqp", " 00003"); // fcbe:26ce:5c7a:9a0f 205b:358c b8f8:08bb
//                               search target --> fc01:0101:0101:0101 2101:0101 0101:0101
    ADD_NODE("mlkjihgzyxwvutsrqpon", " 00004"); // fc08:d8e6:e000:c95c 2192:d676 94f9:63a7
    ADD_NODE("lkjihgzyxwvutsrqponm", " 00005"); // fc7c:6c8d:e5ee:cf99 2f16:06c7 95ca:0c0b
    ADD_NODE("fghijklmnopqrstuvwxy", " 00006"); // fcac:3963:cbd1:6390 3e83:be89 a23f:ce66
    ADD_NODE("jihgzyxwvutsrqponmlk", " 00007"); // fc2e:3a5e:5e47:9769 4964:8f7f 3894:8c07
    ADD_NODE("kjihgzyxwvutsrqponml", " 00008"); // fc37:10aa:39ed:12bf 4a70:1507 dbe9:a054
    ADD_NODE("cdefghijklmnopqrstuv", " 00009"); // fcaa:113e:88da:d432 65f0:1c14 38d9:b656
//                                   this node --> fc39:c3ba:c711:00aa 666d:90b0 1ab6:e8c3
    ADD_NODE("rqponmlkjihgzyxwvuts", " 00010"); // fc43:41e7:2adc:d13b 78ce:1959 e4cc:c76e
    ADD_NODE("ponmlkjihgzyxwvutsrq", " 00011"); // fc83:2ab3:b65b:9ad1 7e7b:f61f e0fa:cb40
    ADD_NODE("efghijklmnopqrstuvwx", " 00012"); // fc08:0ba6:a8e2:7731 9dae:f9fd e502:767c
    ADD_NODE("abcdefghijklmnopqrst", " 00013"); // fc81:cab3:eda0:61aa 9fff:bdde 0168:f0dd
    ADD_NODE("ihgzyxwvutsrqponmlkj", " 00014"); // fc49:8fc7:7e43:981a bac1:0b5d 77fb:8818
    ADD_NODE("nmlkjihgzyxwvutsrqpo", " 00015"); // fc69:61a1:2bec:1444 bb9e:47d1 f8b3:a6a0
    ADD_NODE("defghijklmnopqrstuvw", " 00016"); // fc40:1d18:89a6:9a7e c8af:20fd 5c9f:8140
    ADD_NODE("ghijklmnopqrstuvwxyz", " 00017"); // fc74:0f2e:d77a:e5e7 cf4e:8fe9 7791:98e1


    #undef ADD_NODE

    testQuery(&outMessage, registry, allocator);
    testSearch(&outMessage, routerModule, registry, allocator);

    return 0;
}
