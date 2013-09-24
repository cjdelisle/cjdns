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
#include "dht/dhtcore/NodeIndex.h"
#include "util/Assert.h"

static struct Allocator* alloc = NULL;
static struct Random* rand = NULL;

static struct NodeIndex* setUp(uint32_t capacity)
{
    return NodeIndex_new(alloc, capacity);
}

static struct Address* createAddress(uint32_t mostSignificantAddressSpaceWord, uint64_t path)
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

static struct Node* createNode(uint32_t mostSignificantAddressSpaceWord, uint64_t path)
{
   struct Node* node = Allocator_calloc(alloc, sizeof(struct Node), 1);
   Bits_memcpyConst(&node->address,
                    createAddress(mostSignificantAddressSpaceWord, path),
                    sizeof(struct Address));
   return node;
}

static void test_basic()
{
    struct NodeIndex* index = setUp(8);
    struct Node* n1 = createNode(0x99, 0x15);
    struct Node* n2 = createNode(0x01, 0x13);
    struct Node* n3 = createNode(0xff, 0x17);

    // empty lookup should return NULL w/o error
    Assert_always(NULL == NodeIndex_getByIp(index, createAddress(0x00, 0x00)));
    Assert_always(NULL == NodeIndex_getByPath(index, 0x00));

    // add some nodes
    NodeIndex_put(index, n1);
    NodeIndex_put(index, n2);
    NodeIndex_put(index, n3);

    // lookup a non-indexed value should still return NULL
    Assert_always(NULL == NodeIndex_getByIp(index, createAddress(0x00, 0x00)));
    Assert_always(NULL == NodeIndex_getByPath(index, 0x00));

    // should be able to find by ip
    Assert_always(n1 == NodeIndex_getByIp(index, &n1->address)->node);
    Assert_always(n2 == NodeIndex_getByIp(index, &n2->address)->node);
    Assert_always(n3 == NodeIndex_getByIp(index, &n3->address)->node);

    // should be able to find by path
    Assert_always(n1 == NodeIndex_getByPath(index, n1->address.path));
    Assert_always(n2 == NodeIndex_getByPath(index, n2->address.path));
    Assert_always(n3 == NodeIndex_getByPath(index, n3->address.path));

    // remove should work...
    NodeIndex_remove(index, n3);
    Assert_always(NULL == NodeIndex_getByIp(index, &n3->address));
    Assert_always(NULL == NodeIndex_getByPath(index, n3->address.path));

    // try multi-ip
    struct Node* n4 = createNode(0xff, 0x139);
    Bits_memcpy(n4->address.ip6.bytes, n3->address.ip6.bytes, 16);
    NodeIndex_put(index, n4);
    NodeIndex_put(index, n3);
    // should return in the order added
    Assert_always(n4 == NodeIndex_getByIp(index, &n4->address)->node);
    Assert_always(n3 == NodeIndex_getByIp(index, &n4->address)->next->node);

    // remove when multi-ip should remove the correct node
    NodeIndex_remove(index, n3);
    Assert_always(n4 == NodeIndex_getByIp(index, &n4->address)->node);
}

int main()
{
    alloc = MallocAllocator_new(1<<20);
    rand = Random_new(alloc, NULL, NULL);

    test_basic();

    Allocator_free(alloc);
}
