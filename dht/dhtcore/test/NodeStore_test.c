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

#include <stddef.h>
#include <stdio.h>

struct Allocator* alloc = NULL;
struct Random* rand = NULL;
struct Log* logger = NULL;

struct NodeStore* setUp(struct Address* myAddress, uint32_t capacity)
{
    return NodeStore_new(myAddress, capacity, alloc, logger, rand);
}

struct Address* createAddress(uint32_t mostSignificantAddressSpaceWord, uint64_t path)
{
  // horrible hack...
  return Allocator_clone(alloc, (&(struct Address) {
    .ip6.ints = {
      .three = Endian_bigEndianToHost32(0xfc000000 + Random_uint16(rand)),
      .four = Random_uint32(rand),
      .one = mostSignificantAddressSpaceWord,
      .two = Random_uint32(rand)
    },
    .path = path
  }));
}

struct Address* randomAddress()
{
    // TODO make a random valid encoded switch path...
    return createAddress(Random_uint16(rand), 0x15);
}

void test_addNode()
{
    struct Address* myAddr = randomAddress();
    struct NodeStore* store = setUp(myAddr, 8);
    // adding ourselves should be null
    Assert_always(NodeStore_addNode(store, myAddr, 0, Version_CURRENT_PROTOCOL) == NULL);
    // adding somebody else should not
    Assert_always(NodeStore_addNode(store, randomAddress(), 0, Version_CURRENT_PROTOCOL) != NULL);

    // TODO test at capacity and verify worst reach is replaced
}

void test_getNodesByAddr()
{
    struct Address* myAddr = randomAddress();
    struct NodeStore* store = setUp(myAddr, 8);
    struct Address* otherAddr = randomAddress();

    // make sure we can add an addr and get it back
    NodeStore_addNode(store, otherAddr, 0, Version_CURRENT_PROTOCOL);
    struct NodeList* list = NodeStore_getNodesByAddr(otherAddr, 1, alloc, store);
    Assert_always(list->size == 1);
    Assert_always(Address_isSameIp(&list->nodes[0]->address, otherAddr));

    // try for two
    otherAddr->path = 0x17;
    NodeStore_addNode(store, otherAddr, 0, Version_CURRENT_PROTOCOL);
    list = NodeStore_getNodesByAddr(otherAddr, 2, alloc, store);
    Assert_always(list->size == 2);
    Assert_always(Address_isSameIp(&list->nodes[0]->address, otherAddr));
    Assert_always(Address_isSameIp(&list->nodes[1]->address, otherAddr));

    // make sure 1 still works
    list = NodeStore_getNodesByAddr(otherAddr, 1, alloc, store);
    Assert_always(list->size == 1);
}

void test_getPeers()
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

    // returns in reverse distance order ??
    // TODO confirm this is intended behavior, unsure if there is an order
    Assert_always(Address_isSameIp(&list->nodes[0]->address, oneHop2));
    Assert_always(Address_isSameIp(&list->nodes[1]->address, oneHop1));
    Assert_always(Address_isSameIp(&list->nodes[2]->address, oneHop3));

    // verify max parameter works, should keep the xor closest paths
    list = NodeStore_getPeers(target, 2, alloc, store);
    Assert_always(list->size == 2);
    Assert_always(Address_isSameIp(&list->nodes[0]->address, oneHop1));
    Assert_always(Address_isSameIp(&list->nodes[1]->address, oneHop3));
}

void test_getBest()
{
    struct Address* myAddr = createAddress(0x99,0xff);
    struct NodeStore* store = setUp(myAddr, 8);
    struct Address* target = createAddress(0x01, 0x05);
    struct Address* near = createAddress(0x05, 0x04);
    struct Address* far = createAddress(0xff, 0xf0);

    // make sure the distance from my addr to target is greater than
    // the distance between target and near.
    Assert_always( (Address_getPrefix(myAddr) ^ Address_getPrefix(target)) >
                   (Address_getPrefix(target) ^ Address_getPrefix(near)) );

    // add near and far, and then look for best given target
    // need to give reach > 0 to be a valid match
    NodeStore_addNode(store, near, 1, Version_CURRENT_PROTOCOL);
    NodeStore_addNode(store, far, 1, Version_CURRENT_PROTOCOL);
    struct Node* best = NodeStore_getBest(near, store);
    Assert_always(best != NULL);
    Assert_always(Address_isSameIp(&best->address, near));
}

void test_getClosestNodes()
{
    // TOOD
}

void test_updateReach()
{
    // TOOD
}

void test_nonZeroNodes()
{
    // TOOD
}

void test_size()
{
    struct NodeStore* store = setUp(randomAddress(), 8);
    Assert_always(NodeStore_size(store) == 0);
    NodeStore_addNode(store, randomAddress(), 0, Version_CURRENT_PROTOCOL);
    Assert_always(NodeStore_size(store) == 1);
}

void test_getNodeByNetworkAddr()
{
    // TOOD
}

void test_brokenPath()
{
    // TOOD
}

void test_dumpTable()
{
    // TOOD
}

int main()
{
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
