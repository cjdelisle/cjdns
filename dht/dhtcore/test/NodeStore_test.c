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
#define string_strcmp
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
#include "util/platform/libc/string.h"

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
           "./build/dht/dhtcore/test/NodeStore_test --genkeys | sed -ne "
           "'s/^created new key: \\[\\(.\\{32\\}\\)\\(.\\{32\\}\\)\\]$/\"\\1\"\\n\"\\2\",/p' "
           "| sed 's/\\([0-9a-f]\\{2\\}\\)/\\\\x\\1/g' > TestKeys.data && "
           "mv ./TestKeys.data ./dht/dhtcore/test/\n");
    Assert_true(false);
}


static struct Address* createAddress(int mostSignificantAddressSpaceByte, uint64_t path)
{
    struct Address address = { .path = 0 };

    if (!genKeys) {
        if ((int)(sizeof(KEYS) * sizeof(char*)) < (nextKey + 1)) {
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

static struct Address* randomIp(uint64_t path)
{
   return createAddress(-1, path);
}

// This creates a random address which is a peer
static struct Address* randomAddress()
{
    return randomIp(0x13);
}

static void test_addNode()
{
    struct Address* myAddr = randomAddress();
    struct NodeStore* store = setUp(myAddr, 2);
    // adding ourselves should be null
    Assert_always(NodeStore_addNode(store, myAddr, 1, Version_CURRENT_PROTOCOL) == NULL);

    // adding a random node with no peer should be null
    Assert_always(NodeStore_addNode(store, randomIp(0x155), 1, Version_CURRENT_PROTOCOL) == NULL);

    // adding a peer should be non-null
    Assert_always(NodeStore_addNode(store, randomIp(0x15), 1, Version_CURRENT_PROTOCOL) != NULL);

    // adding a node behind our peer should also be non-null
    Assert_always(NodeStore_addNode(store, randomIp(0x155), 1, Version_CURRENT_PROTOCOL) != NULL);

    // test at capacity and verify worst path is replaced
    NodeStore_addNode(store, randomIp(0x157), 1, Version_CURRENT_PROTOCOL);
    struct Node* node = NodeStore_addNode(store, randomIp(0x13), 1, Version_CURRENT_PROTOCOL);
    Assert_always( Address_isSameIp(&node->address, &NodeStore_dumpTable(store,1)->address) );
}

static void test_getNodesByAddr()
{
    struct Address* myAddr = randomAddress();
    struct NodeStore* store = setUp(myAddr, 8);
    struct Address* otherAddr = randomIp(0x13);

    // make sure we can add an addr and get it back
    NodeStore_addNode(store, otherAddr, 1, Version_CURRENT_PROTOCOL);
    struct NodeList* list = NodeStore_getNodesByAddr(otherAddr, 1, alloc, store);
    Assert_always(list->size == 1);
    Assert_always(Address_isSameIp(&list->nodes[0]->address, otherAddr));

    // try for two
    otherAddr->path = 0x15;
    NodeStore_addNode(store, otherAddr, 1, Version_CURRENT_PROTOCOL);
    list = NodeStore_getNodesByAddr(otherAddr, 2, alloc, store);
    Assert_always(list->size == 2);
    Assert_always(Address_isSameIp(&list->nodes[0]->address, otherAddr));
    Assert_always(Address_isSameIp(&list->nodes[1]->address, otherAddr));

    // make sure 1 still works
    list = NodeStore_getNodesByAddr(otherAddr, 1, alloc, store);
    Assert_always(list->size == 1);
}

static void test_getPeers()
{
    // mucking around with switch labels...
    // see switch/NumberCompress.h
    struct Address* myAddr = createAddress(0x99,0x01);   // 0001
    struct NodeStore* store = setUp(myAddr, 8);
    uint64_t target = 0x1f;                              // 00011111      iface #7,#1
    struct Address* oneHop1 = createAddress(0x02, 0x17); // 00010111      iface #3,#1
    struct Address* oneHop2 = createAddress(0x01, 0x15); // 00010101      iface #2,#1
    struct Address* oneHop3 = createAddress(0x04, 0x1d); // 00011101      iface #6,#1
    struct Address* twoHop = createAddress(0x03, 0x155); // 000101010101  iface #2,#2,#1

    // using variable scheme, 4 bits (suffix 1).
    Assert_always(NumberCompress_bitsUsedForLabel(target) == 4);
    Assert_always(NumberCompress_bitsUsedForLabel(oneHop1->path) == 4);
    Assert_always(NumberCompress_bitsUsedForLabel(oneHop2->path) == 4);
    Assert_always(NumberCompress_bitsUsedForLabel(oneHop3->path) == 4);
    Assert_always(NumberCompress_bitsUsedForLabel(twoHop->path) == 4);

    // verify we setup our test data correctly.
    Assert_always(LabelSplicer_isOneHop(oneHop1->path));
    Assert_always(LabelSplicer_isOneHop(oneHop2->path));
    Assert_always(LabelSplicer_isOneHop(oneHop3->path));
    Assert_always(!LabelSplicer_isOneHop(twoHop->path));

    // verify empty case, should always return a NodeList
    struct NodeList* list = NodeStore_getPeers(target, 1, alloc, store);
    Assert_always(list != NULL);
    Assert_always(list->size == 0);

    // add test nodes
    NodeStore_addNode(store, oneHop1, 1, Version_CURRENT_PROTOCOL);
    NodeStore_addNode(store, oneHop2, 1, Version_CURRENT_PROTOCOL);
    NodeStore_addNode(store, oneHop3, 1, Version_CURRENT_PROTOCOL);
    NodeStore_addNode(store, twoHop, 1, Version_CURRENT_PROTOCOL);

    // verify we only get nodes with 1 hop labels
    list = NodeStore_getPeers(target, 10, alloc, store);
    Assert_always(list->size == 3);
    for (uint32_t i=0; i < list->size; i++) {
        Assert_always(LabelSplicer_isOneHop(list->nodes[i]->address.path));
    }

    // verify max parameter works
    list = NodeStore_getPeers(target, 1, alloc, store);
    Assert_always(list->size == 1);
}

static void test_getBest()
{
    struct Address* myAddr = createAddress(0x99,0x1);
    struct NodeStore* store = setUp(myAddr, 8);
    struct Address* target = createAddress(0x01, 0x15);
    struct Address* a = createAddress(0x05, 0x17);  // 1 hop
    struct Address* b = createAddress(0xff, 0x177); // 2 hops (behind 0x17)
    struct Node* best = NULL;

    // exact address match should be best
    NodeStore_addNode(store, a, 1, Version_CURRENT_PROTOCOL);
    NodeStore_addNode(store, b, 1, Version_CURRENT_PROTOCOL);
    best = NodeStore_getBest(b, store);
    Assert_always(best != NULL);
    Assert_always(Address_isSameIp(&best->address, b));

    // shortest label should be used if no exact match
    best = NodeStore_getBest(target, store);
    Assert_always(best != NULL);
    Assert_always(Address_isSameIp(&best->address, a));
}

static void test_getClosestNodes()
{
    // gets nodes that are "close" as in log2(path) aka # hops away
    struct Address* myAddr = createAddress(0x99,0x1);
    struct NodeStore* store = setUp(myAddr, 8);
    struct Address* target = createAddress(0x06, 0x13);
    struct Address* oneHop = createAddress(0x05, 0x15);
    struct Address* twoHop = createAddress(0x08, 0x155);
    struct NodeList* closest = NULL;

    NodeStore_addNode(store, oneHop, 1, Version_CURRENT_PROTOCOL);
    NodeStore_addNode(store, twoHop, 1, Version_CURRENT_PROTOCOL);
    NodeStore_addNode(store, randomIp(0x17777), 1, Version_CURRENT_PROTOCOL);
    NodeStore_addNode(store, randomIp(0x157575), 1, Version_CURRENT_PROTOCOL);

    // check basic case returns in reverse order (of hops)
    closest = NodeStore_getClosestNodes(store, target, NULL, 2,
                                        Version_CURRENT_PROTOCOL, alloc);
    Assert_always(closest != NULL);
    Assert_always(closest->size == 2);
    Assert_always(Address_isSameIp(&closest->nodes[0]->address, twoHop));
    Assert_always(Address_isSameIp(&closest->nodes[1]->address, oneHop));
}

static void test_updateReach()
{
    struct NodeStore* store = setUp(randomAddress(), 8);
    struct Address* a = randomIp(0x17);  // 00010111      iface #3,#1
    struct Address* b = randomIp(0x15);  // 00010101      iface #2,#1
    struct Address* c = randomIp(0x155); // 000111011101  iface #2,#2,#1

    // verify c routes through b
    Assert_always(LabelSplicer_routesThrough(c->path, b->path));

    NodeStore_addNode(store, a, 1, Version_CURRENT_PROTOCOL);
    struct Node* node = NodeStore_addNode(store, b, 15, Version_CURRENT_PROTOCOL);
    NodeStore_addNode(store, c, 20, Version_CURRENT_PROTOCOL);

    // update reach of b, and verify reach of c is changed as well
    node->reach = 10;
    NodeStore_updateReach(node, store);
    Assert_always(NodeStore_dumpTable(store, 2)->reach == 10);
}

static void test_nonZeroNodes()
{
    struct NodeStore* store = setUp(randomAddress(), 8);
    struct Node* node = NodeStore_addNode(store, randomIp(0x13), 1, Version_CURRENT_PROTOCOL);
    NodeStore_addNode(store, randomIp(0x15), 1, Version_CURRENT_PROTOCOL);
    NodeStore_addNode(store, randomIp(0x17), 1, Version_CURRENT_PROTOCOL);
    NodeStore_addNode(store, randomIp(0x19), 1, Version_CURRENT_PROTOCOL);
    Assert_always(store->size == 4);
    Assert_always(NodeStore_nonZeroNodes(store)==4);

    // zero a node and make sure we get one less.
    node->reach = 0;
    NodeStore_updateReach(node, store);
    Assert_always(NodeStore_nonZeroNodes(store)==3);
}

static void test_size()
{
    struct NodeStore* store = setUp(randomAddress(), 8);
    Assert_always(NodeStore_size(store) == 0);
    NodeStore_addNode(store, randomAddress(), 0, Version_CURRENT_PROTOCOL);
    Assert_always(NodeStore_size(store) == 1);
}

static void test_getNodeByNetworkAddr()
{
    struct NodeStore* store = setUp(randomAddress(), 8);
    // empty case should be null
    Assert_always(NodeStore_getNodeByNetworkAddr(0x15,store)==NULL);

    // happy case
    NodeStore_addNode(store, randomIp(0x13), 1, Version_CURRENT_PROTOCOL);
    NodeStore_addNode(store, randomIp(0x15), 1, Version_CURRENT_PROTOCOL);
    Assert_always(NodeStore_getNodeByNetworkAddr(0x15,store)->address.path == 0x15);
}

static void test_brokenPath()
{
    struct Address* myAddr = randomIp(0x01);
    struct NodeStore* store = setUp(myAddr, 8);
    struct Address* a = randomIp(0x17);  // 00010111      iface #3,#1
    struct Address* b = randomIp(0x15);  // 00010101      iface #2,#1
    struct Address* c = randomIp(0x155); // 000111011101  iface #2,#2,#1

    // verify c routes through b
    Assert_always(LabelSplicer_routesThrough(c->path, b->path));

    // verify removing b should invalidate c
    NodeStore_addNode(store, a, 1, Version_CURRENT_PROTOCOL);
    NodeStore_addNode(store, b, 1, Version_CURRENT_PROTOCOL);
    NodeStore_addNode(store, c, 1, Version_CURRENT_PROTOCOL);

    // calling brokenPath on B directly should remove it
    Assert_always(NodeStore_brokenPath(b->path, store)==2);

    // should only have 1 valid route now...
    Assert_always(NodeStore_nonZeroNodes(store)==1);
}

static void test_dumpTable()
{
    struct NodeStore* store = setUp(randomAddress(), 8);
    NodeStore_addNode(store, randomIp(0x13), 1, Version_CURRENT_PROTOCOL);
    struct Node* orig = NodeStore_addNode(store, randomIp(0x17), 1, Version_CURRENT_PROTOCOL);
    NodeStore_addNode(store, randomIp(0x15), 1, Version_CURRENT_PROTOCOL);
    // verify happy case
    struct Node* test = NodeStore_dumpTable(store, 1);
    Assert_always(Address_isSameIp(&orig->address, &test->address));
    // verify out of bounds index gets NULL
    Assert_always(NodeStore_dumpTable(store, -1) == NULL);
    Assert_always(NodeStore_dumpTable(store, 3) == NULL);
}

int main(int argc, char** argv)
{
    if (argc > 1 && !strcmp(argv[argc-1], "--genkeys")) {
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

    Allocator_free(alloc);
}
