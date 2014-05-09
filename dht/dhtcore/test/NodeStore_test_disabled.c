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
#include "memory/MallocAllocator.h"
#include "crypto/random/Random.h"
#include "crypto/AddressCalc.h"
#include "dht/Address.h"
#include "dht/dhtcore/Node.h"
#include "dht/dhtcore/NodeList.h"
#include "dht/dhtcore/NodeStore.h"
#include "io/FileWriter.h"
#include "switch/LabelSplicer.h"
#include "util/Assert.h"
#include "util/Endian.h"
#include "util/log/WriterLog.h"
#include "util/version/Version.h"

#include "crypto_scalarmult_curve25519.h"

#include <stddef.h>
#include <stdio.h>

static struct Allocator* alloc = NULL;
static struct Random* rand = NULL;
static struct Log* logger = NULL;
static bool genKeys = 0;
static int nextKey = 0;

static char* KEYS[] = {
    #include "dht/dhtcore/test/TestKeys.data"
};

static struct NodeStore* setUp(struct Address* myAddress, uint32_t capacity)
{
    return NodeStore_new(myAddress, capacity, alloc, logger, rand);
}

static void missingKey()
{
    printf("mismatching key, run this:\n"
           "head ./dht/dhtcore/NodeStore.c -n 14 > ./TestKeys.data && "
           "./build_linux/testcjdroute NodeStore_test --genkeys | sed -ne "
           "'s/^created new key: \\[\\(.\\{32\\}\\)\\(.\\{32\\}\\)\\]$/\"\\1\"\\n\"\\2\",/p' "
           "| sed 's/\\([0-9a-f]\\{2\\}\\)/\\\\x\\1/g' >> TestKeys.data && "
           "mv ./TestKeys.data ./dht/dhtcore/test/\n");
    Assert_true(false);
}

static uint64_t getPath(int* hops)
{
    int i;
    uint64_t out = 0;
    for (i = 0; hops[i] != 1; i++) ;
    for (; i >= 0; i--) {
        int bits = NumberCompress_bitsUsedForNumber(hops[i]);
        out <<= bits;
        out |= NumberCompress_getCompressed(hops[i], bits);
    }
    return out;
}

static struct Address* createAddress(int mostSignificantAddressSpaceByte, int* hops)
{
    uint64_t path = getPath(hops);
    struct Address address = { .path = 0 };

    if (!genKeys) {
        if ((int)(sizeof(KEYS) / sizeof(*KEYS)) < (nextKey + 1)) {
            missingKey();
        }
        Bits_memcpyConst(address.key, KEYS[nextKey], 32);
        if (AddressCalc_addressForPublicKey(address.ip6.bytes, address.key)
            && (mostSignificantAddressSpaceByte == -1
                || address.ip6.bytes[8] == mostSignificantAddressSpaceByte))
        {
            nextKey++;
            uint8_t publicKeyHex[65];
            Hex_encode(publicKeyHex, 65, address.key, 32);
            printf("created new key: [%s]\n", publicKeyHex);

            address.path = path;
            return Allocator_clone(alloc, &address);
        } else {
            uint8_t publicKeyHex[65];
            Hex_encode(publicKeyHex, 65, address.key, 32);
            printf("bad key: [%s]\n", publicKeyHex);
            if (address.ip6.ints.three) {
                uint8_t printedAddr[40];
                AddrTools_printIp(printedAddr, address.ip6.bytes);
                printf("addr: [%s]\n", printedAddr);
            }
                missingKey();
        }
    }

    for (;;) {
        Random_bytes(rand, address.key, 32);
        // Brute force for keys until one matches FC00:/8
        if (AddressCalc_addressForPublicKey(address.ip6.bytes, address.key)
            && (mostSignificantAddressSpaceByte == -1
                || address.ip6.bytes[8] == mostSignificantAddressSpaceByte))
        {

            uint8_t publicKeyHex[65];
            Hex_encode(publicKeyHex, 65, address.key, 32);
            printf("created new key: [%s]\n", publicKeyHex);

            address.path = path;
            return Allocator_clone(alloc, &address);
        }
    }
}

static struct Address* randomIp(int* hops)
{
    return createAddress(-1, hops);
}

// This creates a random address which is a peer
static struct Address* randomAddress()
{
    return randomIp((int[]){0,1}/*0x13*/);
}

static void test_addNode()
{
    struct Address* myAddr = randomAddress();
    struct NodeStore* store = setUp(myAddr, 2);
    // adding ourselves should be null
    Assert_true(NodeStore_addNode(store, myAddr, 1, Version_CURRENT_PROTOCOL) == NULL);

    // adding a random node with no peer should be null
    Assert_true(
        !NodeStore_addNode(store, randomIp((int[]){2,2,1}/*0x155*/), 1, Version_CURRENT_PROTOCOL)
    );

    // adding a peer should be non-null
    Assert_true(
        NodeStore_addNode(store, randomIp((int[]){2,1}/*0x15*/), 1, Version_CURRENT_PROTOCOL)
    );

    // adding a node behind our peer should also be non-null
    Assert_true(
        NodeStore_addNode(store, randomIp((int[]){2,2,1}/*0x155*/), 1, Version_CURRENT_PROTOCOL)
    );

    // test at capacity and verify worst path is replaced
    NodeStore_addNode(store, randomIp((int[]){3,2,1}/*0x157*/), 1, Version_CURRENT_PROTOCOL);
    struct Node* node =
        NodeStore_addNode(store, randomIp((int[]){0,1}/*0x13*/), 1, Version_CURRENT_PROTOCOL);
    Assert_true( Address_isSameIp(&node->address, &NodeStore_dumpTable(store,1)->address) );
}

static void test_getNodesByAddr()
{
    struct Address* myAddr = randomAddress();
    struct NodeStore* store = setUp(myAddr, 8);
    struct Address* otherAddr = randomIp((int[]){0,1}/*0x13*/);

    // make sure we can add an addr and get it back
    NodeStore_addNode(store, otherAddr, 1, Version_CURRENT_PROTOCOL);
    struct NodeList* list = NodeStore_getNodesByAddr(otherAddr, 1, alloc, store);
    Assert_true(list->size == 1);
    Assert_true(Address_isSameIp(&list->nodes[0]->address, otherAddr));

    // try for two
    otherAddr->path = getPath((int[]){2,1}) /*0x15*/;
    NodeStore_addNode(store, otherAddr, 1, Version_CURRENT_PROTOCOL);
    list = NodeStore_getNodesByAddr(otherAddr, 2, alloc, store);
    Assert_true(list->size == 2);
    Assert_true(Address_isSameIp(&list->nodes[0]->address, otherAddr));
    Assert_true(Address_isSameIp(&list->nodes[1]->address, otherAddr));

    // make sure 1 still works
    list = NodeStore_getNodesByAddr(otherAddr, 1, alloc, store);
    Assert_true(list->size == 1);
}

static void test_getPeers()
{
    // mucking around with switch labels...
    // see switch/NumberCompress.h
    struct Address* myAddr = createAddress(0x99, (int[]){1}/*0x01*/);   // 0001
    struct NodeStore* store = setUp(myAddr, 8);
    uint64_t target = getPath((int[]){7,1})/*0x1f*/;
    struct Address* oneHop1 = createAddress(0x02, (int[]){3,1}/*0x17*/);
    struct Address* oneHop2 = createAddress(0x01, (int[]){2,1}/*0x15*/);
    struct Address* oneHop3 = createAddress(0x04, (int[]){6,1}/*0x1d*/);
    struct Address* twoHop = createAddress(0x03, (int[]){2,2,1}/*0x155*/);

    // using variable scheme, 4 bits (suffix 1).
    /* specific to v3x5x8
        Assert_true(NumberCompress_bitsUsedForLabel(target) == 4);
        Assert_true(NumberCompress_bitsUsedForLabel(oneHop1->path) == 4);
        Assert_true(NumberCompress_bitsUsedForLabel(oneHop2->path) == 4);
        Assert_true(NumberCompress_bitsUsedForLabel(oneHop3->path) == 4);
        Assert_true(NumberCompress_bitsUsedForLabel(twoHop->path) == 4);
    */

    // verify we setup our test data correctly.
    Assert_true(LabelSplicer_isOneHop(oneHop1->path));
    Assert_true(LabelSplicer_isOneHop(oneHop2->path));
    Assert_true(LabelSplicer_isOneHop(oneHop3->path));
    Assert_true(!LabelSplicer_isOneHop(twoHop->path));

    // verify empty case, should always return a NodeList
    struct NodeList* list = NodeStore_getPeers(target, 1, alloc, store);
    Assert_true(list != NULL);
    Assert_true(list->size == 0);

    // add test nodes
    NodeStore_addNode(store, oneHop1, 1, Version_CURRENT_PROTOCOL);
    NodeStore_addNode(store, oneHop2, 1, Version_CURRENT_PROTOCOL);
    NodeStore_addNode(store, oneHop3, 1, Version_CURRENT_PROTOCOL);
    NodeStore_addNode(store, twoHop, 1, Version_CURRENT_PROTOCOL);

    // verify we only get nodes with 1 hop labels
    list = NodeStore_getPeers(target, 10, alloc, store);
    Assert_true(list->size == 3);
    for (uint32_t i=0; i < list->size; i++) {
        Assert_true(LabelSplicer_isOneHop(list->nodes[i]->address.path));
    }

    // verify max parameter works
    list = NodeStore_getPeers(target, 1, alloc, store);
    Assert_true(list->size == 1);
}

static void test_getBest()
{
    struct Address* myAddr = createAddress(0x99, (int[]){1}/*0x1*/);
    struct NodeStore* store = setUp(myAddr, 8);
    struct Address* target = createAddress(0x01, (int[]){2,1}/*0x15*/);
    struct Address* a = createAddress(0x05, (int[]){3,1}/*0x17*/);  // 1 hop
    struct Address* b = createAddress(0xff, (int[]){3,3,1}/*0x177*/); // 2 hops (behind 0x17)
    struct Node* best = NULL;

    // exact address match should be best
    NodeStore_addNode(store, a, 1, Version_CURRENT_PROTOCOL);
    NodeStore_addNode(store, b, 1, Version_CURRENT_PROTOCOL);
    best = NodeStore_getBest(b, store);
    Assert_true(best != NULL);
    Assert_true(Address_isSameIp(&best->address, b));

    // shortest label should be used if no exact match
    best = NodeStore_getBest(target, store);
    Assert_true(best != NULL);
    Assert_true(Address_isSameIp(&best->address, a));
}

static void test_getClosestNodes()
{
    // gets nodes that are "close" as in log2(path) aka # hops away
    struct Address* myAddr = createAddress(0x99, (int[]){1} /*0x1*/);
    struct NodeStore* store = setUp(myAddr, 8);
    struct Address* target = createAddress(0x06, (int[]){0,1}/*0x13*/);
    struct Address* oneHop = createAddress(0x05, (int[]){2,1}/*0x15*/);
    struct Address* twoHop = createAddress(0x08, (int[]){2,2,1}/*0x155*/);
    struct NodeList* closest = NULL;

    NodeStore_addNode(store, oneHop, 1, Version_CURRENT_PROTOCOL);
    NodeStore_addNode(store, twoHop, 1, Version_CURRENT_PROTOCOL);
    NodeStore_addNode(
        store, randomIp((int[]){3,3,3,3,1}/*0x17777*/), 1, Version_CURRENT_PROTOCOL);
    NodeStore_addNode(
        store, randomIp((int[]){2,3,2,3,2,1}/*0x157575*/), 1, Version_CURRENT_PROTOCOL);

    // check basic case returns in reverse order (of hops)
    closest = NodeStore_getClosestNodes(store, target, NULL, 2,
                                        Version_CURRENT_PROTOCOL, alloc);
    Assert_true(closest != NULL);
    Assert_true(closest->size == 2);
    Assert_true(Address_isSameIp(&closest->nodes[0]->address, twoHop));
    Assert_true(Address_isSameIp(&closest->nodes[1]->address, oneHop));
}

static void test_updateReach()
{
    struct NodeStore* store = setUp(randomAddress(), 8);
    struct Address* a = randomIp((int[]){3,1} /*0x17*/);  // 00010111      iface #3,#1
    struct Address* b = randomIp((int[]){2,1} /*0x15*/);  // 00010101      iface #2,#1
    struct Address* c = randomIp((int[]){2,2,1} /*0x155*/); // 000111011101  iface #2,#2,#1

    // verify c routes through b
    Assert_true(LabelSplicer_routesThrough(c->path, b->path));

    NodeStore_addNode(store, a, 1, Version_CURRENT_PROTOCOL);
    struct Node* node = NodeStore_addNode(store, b, 15, Version_CURRENT_PROTOCOL);
    NodeStore_addNode(store, c, 20, Version_CURRENT_PROTOCOL);

    // update reach of b, and verify reach of c is changed as well
    node->reach = 10;
    NodeStore_updateReach(node, store);
    Assert_true(NodeStore_dumpTable(store, 2)->reach == 10);
}

static void test_nonZeroNodes()
{
    struct NodeStore* store = setUp(randomAddress(), 8);
    struct Node* node =
       NodeStore_addNode(store, randomIp((int[]){0,1}), 1, Version_CURRENT_PROTOCOL);
    NodeStore_addNode(store, randomIp((int[]){2,1}), 1, Version_CURRENT_PROTOCOL);
    NodeStore_addNode(store, randomIp((int[]){3,1}), 1, Version_CURRENT_PROTOCOL);
    NodeStore_addNode(store, randomIp((int[]){4,1}), 1, Version_CURRENT_PROTOCOL);
    Assert_true(store->size == 4);
    Assert_true(NodeStore_nonZeroNodes(store)==4);

    // zero a node and make sure we get one less.
    node->reach = 0;
    NodeStore_updateReach(node, store);
    Assert_true(NodeStore_nonZeroNodes(store)==3);
}

static void test_size()
{
    struct NodeStore* store = setUp(randomAddress(), 8);
    Assert_true(NodeStore_size(store) == 0);
    NodeStore_addNode(store, randomAddress(), 0, Version_CURRENT_PROTOCOL);
    Assert_true(NodeStore_size(store) == 1);
}

static void test_getNodeByNetworkAddr()
{
    struct NodeStore* store = setUp(randomAddress(), 8);
    // empty case should be null
    Assert_true(NodeStore_getNodeByNetworkAddr(getPath((int[]){2,1}),store)==NULL);

    // happy case
    NodeStore_addNode(store, randomIp((int[]){0,1}), 1, Version_CURRENT_PROTOCOL);
    NodeStore_addNode(store, randomIp((int[]){3,1}), 1, Version_CURRENT_PROTOCOL);
    Assert_true(
        NodeStore_getNodeByNetworkAddr(getPath((int[]){3,1}),store)->address.path ==
            getPath((int[]){3,1})
    );
}

static void test_brokenPath()
{
    struct Address* myAddr = randomIp((int[]){1});
    struct NodeStore* store = setUp(myAddr, 8);
    struct Address* a = randomIp((int[]){3,1});
    struct Address* b = randomIp((int[]){2,1});
    struct Address* c = randomIp((int[]){2,2,1});

    // verify c routes through b
    Assert_true(LabelSplicer_routesThrough(c->path, b->path));

    // verify removing b should invalidate c
    NodeStore_addNode(store, a, 1, Version_CURRENT_PROTOCOL);
    NodeStore_addNode(store, b, 1, Version_CURRENT_PROTOCOL);
    NodeStore_addNode(store, c, 1, Version_CURRENT_PROTOCOL);

    // calling brokenPath on B directly should remove it
    Assert_true(NodeStore_brokenPath(b->path, store)==2);

    // should only have 1 valid route now...
    Assert_true(NodeStore_nonZeroNodes(store)==1);
}

static void test_dumpTable()
{
    struct NodeStore* store = setUp(randomAddress(), 8);
    NodeStore_addNode(store, randomIp((int[]){0,1}), 1, Version_CURRENT_PROTOCOL);
    struct Node* orig =
        NodeStore_addNode(store, randomIp((int[]){5,1}), 1, Version_CURRENT_PROTOCOL);
    NodeStore_addNode(store, randomIp((int[]){3,1}), 1, Version_CURRENT_PROTOCOL);
    // verify happy case
    struct Node* test = NodeStore_dumpTable(store, 1);
    Assert_true(Address_isSameIp(&orig->address, &test->address));
    // verify out of bounds index gets NULL
    Assert_true(NodeStore_dumpTable(store, -1) == NULL);
    Assert_true(NodeStore_dumpTable(store, 3) == NULL);
}

static void test_pathfinderTwo_splitLink()
{
    #ifndef EXPERIMENTAL_PATHFINDER
        return;
    #endif
    struct NodeStore* store = setUp(randomAddress(), 8);
    struct EncodingScheme* scheme = NumberCompress_defineScheme(alloc);

    // always linked to self
    Assert_true(NodeStore_linkCount(store->selfNode) == 1);

    // fcfe:15a1:15f1:ba25:ec32:4507:8d78:efef  0000.0000.0000.0031 --> 0000.0000.0000.0031
    NodeStore_discoverNode(store, randomIp((int[]){8,1}), 0,
                           Version_CURRENT_PROTOCOL, scheme, 0);

    // should be just efef in the table.
    Assert_true(NodeStore_linkCount(store->selfNode) == 2);

    // fc98:0c83:e1be:0bdc:e177:49c6:2f96:39ee  0000.0000.0000.a629 --> 0000.0000.0014.c531
    NodeStore_discoverNode(store, randomIp((int[]){8,4,8,4,1}), 0,
                           Version_CURRENT_PROTOCOL, scheme, 0);

    // Now should be efef->39ee
    Assert_true(NodeStore_linkCount(store->selfNode) == 2);
    struct Node_Link* linkSelfEfef = NodeStore_getLink(store->selfNode, 0);
    Assert_true(NodeStore_linkCount(linkSelfEfef->child) == 1);
    struct Node_Link* linkEfEf39ee = NodeStore_getLink(linkSelfEfef->child, 0);
    struct Node_Two* node39ee = linkEfEf39ee->child;
    Assert_true(NodeStore_linkCount(linkEfEf39ee->child) == 0);

    // fc1e:7c83:c316:11e3:2b3b:0b25:e667:2765  0000.0000.0000.0029 --> 0000.0000.0000.0531
    NodeStore_discoverNode(store, randomIp((int[]){8,4,1}), 0,
                           Version_CURRENT_PROTOCOL, scheme, 0);

    // This split efef->39ee resulting in:  efef->2765->39ee
    Assert_true(NodeStore_linkCount(linkSelfEfef->child) == 1);
    struct Node_Link* linkEfef2765 = NodeStore_getLink(linkSelfEfef->child, 0);
    Assert_true(linkEfef2765->child != node39ee);
    Assert_true(NodeStore_linkCount(linkEfef2765->child) == 1);
    struct Node_Link* link276539ee = NodeStore_getLink(linkEfef2765->child, 0);
    Assert_true(link276539ee->child == node39ee);
    Assert_true(NodeStore_linkCount(node39ee) == 0);
}

int main(int argc, char** argv)
{
    if (argc > 1 && !CString_strcmp(argv[argc-1], "--genkeys")) {
        genKeys = 1;
    }

    alloc = MallocAllocator_new(1<<20);
    struct Writer* writer = FileWriter_new(stdout, alloc);
    logger = WriterLog_new(writer, alloc);
    rand = Random_new(alloc, NULL, NULL);

    test_addNode();
    test_getBest();
    test_getNodesByAddr();
    test_getPeers();
    test_getClosestNodes();
    test_updateReach();
    test_nonZeroNodes();
    test_size();
    test_getNodeByNetworkAddr();
    test_brokenPath();
    test_dumpTable();
    test_pathfinderTwo_splitLink();

    Allocator_free(alloc);
    return 0;
}
