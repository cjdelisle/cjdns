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

struct Address* randomAddress()
{
  // horrible hack...
  return Allocator_clone(alloc, (&(struct Address) {
    .ip6.ints = {
      .three = Endian_bigEndianToHost32(0xfc000000 + Random_uint16(rand)),
      .four = Random_uint32(rand),
      .two = Random_uint32(rand),
      .one = Random_uint32(rand)
    },
    .path = 0x13
  }));
}

void test_addNode()
{
    struct Address* myAddr = randomAddress();
    struct NodeStore* store = setUp(myAddr, 8);
    // adding ourselves should be null
    Assert_always(NodeStore_addNode(store, myAddr, 0, Version_CURRENT_PROTOCOL) == NULL);
    // adding somebody else should not
    Assert_always(NodeStore_addNode(store, randomAddress(), 0, Version_CURRENT_PROTOCOL) != NULL);
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

void test_size()
{
    struct NodeStore* store = setUp(randomAddress(), 8);
    Assert_always(NodeStore_size(store) == 0);
    NodeStore_addNode(store, randomAddress(), 0, Version_CURRENT_PROTOCOL);
    Assert_always(NodeStore_size(store) == 1);
}

int main()
{
    alloc = MallocAllocator_new(1<<20);
    struct Writer* writer = FileWriter_new(stdout, alloc);
    logger = WriterLog_new(writer, alloc);
    rand = Random_new(alloc, NULL, NULL);

    test_addNode();
    test_getNodesByAddr();
    test_size();

    Allocator_free(alloc);
}
