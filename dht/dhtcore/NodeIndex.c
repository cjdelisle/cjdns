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

#include "dht/Address.h"
#include "dht/dhtcore/Node.h"
#include "dht/dhtcore/NodeIndex.h"

#include <uv-private/tree.h>

struct IpTree {
  /** pointer to the root of the rb tree */
  struct NodeIndex_ipEntry* rbh_root;

  /** pointer to the first of an array of entries */
  struct NodeIndex_ipEntry* entries;

  /**
   * free / used heap.
   *   0 - (size-1)      : used
   *   (size) - capacity : free
   */
  struct NodeIndex_ipEntry** heap;
};

struct PathTree {
  /** pointer to the root of the rb tree */
  struct NodeIndex_pathEntry* rbh_root;

  /** pointer to the first of an array of entries */
  struct NodeIndex_pathEntry* entries;

  /**
   * free / used heap.
   *   0 - (size-1)      : used
   *   (size) - capacity : free
   */
  struct NodeIndex_pathEntry** heap;
};

struct NodeIndex {
  struct IpTree* byIp;
  struct PathTree* byPath;
  uint32_t size;
  uint32_t capacity;
};

static int comparePath(struct NodeIndex_pathEntry* a, struct NodeIndex_pathEntry* b)
{
    uint64_t path_a = a->node->address.path;
    uint64_t path_b = b->node->address.path;
    if (path_a < path_b) {
        return -1;
    }
    if (path_a > path_b) {
        return 1;
    }
    return 0;
}

static int compareIp(struct NodeIndex_ipEntry* entry_a, struct NodeIndex_ipEntry* entry_b)
{
    struct Address* a = &entry_a->node->address;
    struct Address* b = &entry_b->node->address;
    #define IPCOMPARE(n)                                   \
      if (a->ip6.ints.n < b->ip6.ints.n) {                 \
          return -1;                                       \
      }                                                    \
      if (a->ip6.ints.n > b->ip6.ints.n) {                 \
          return 1;                                        \
      }
    IPCOMPARE(one)
    IPCOMPARE(two)
    IPCOMPARE(three)
    IPCOMPARE(four)
    #undef COMPARE
    return 0;
}

// red black tree indexes
RB_GENERATE_STATIC(PathTree, NodeIndex_pathEntry, indexEntry, comparePath)
RB_GENERATE_STATIC(IpTree, NodeIndex_ipEntry, indexEntry, compareIp)

// TODO allocate entries arrays and use a heap of pointers instead of dynamic alloc for tree nodes
struct NodeIndex* NodeIndex_new(struct Allocator* alloc, const uint32_t capacity)
{
    struct NodeIndex* index = Allocator_clone(alloc, (&(struct NodeIndex) {
        .byIp = Allocator_clone(alloc, (&(struct IpTree) {
           .rbh_root = NULL,
           .entries = Allocator_calloc(alloc, sizeof(struct NodeIndex_ipEntry), capacity),
           .heap = Allocator_calloc(alloc, sizeof(struct NodeIndex_ipEntry*), capacity)
        })),
        .byPath = Allocator_clone(alloc, (&(struct PathTree) {
           .rbh_root = NULL,
           .entries = Allocator_calloc(alloc, sizeof(struct NodeIndex_pathEntry), capacity),
           .heap = Allocator_calloc(alloc, sizeof(struct NodeIndex_pathEntry*), capacity)
        })),
        .capacity = capacity,
        .size = 0
    }));

    // init free pointer heaps
    for (uint32_t i=0; i<capacity; i++) {
        index->byIp->heap[i] = &index->byIp->entries[i];
        index->byPath->heap[i] = &index->byPath->entries[i];
    }

    return index;
}

#define ENTRY_FROM_PATH(name, path)                                       \
    struct Node name##_node = {                                           \
        .address = {                                                      \
            .path = path                                                  \
        }                                                                 \
    };                                                                    \
    struct NodeIndex_pathEntry name = {                                   \
        .node = &name##_node                                              \
    };

#define ENTRY_FROM_IP(name, addr)                                         \
    struct Node name##_node;                                              \
    Bits_memcpyConst(&name##_node.address, addr, sizeof(struct Address)); \
    struct NodeIndex_ipEntry name = {                                     \
        .node = &name##_node,                                             \
        .next = NULL                                                      \
    };

struct Node* NodeIndex_getByPath(struct NodeIndex* index, uint64_t path)
{
    ENTRY_FROM_PATH(target, path)
    struct NodeIndex_pathEntry* result = RB_FIND(PathTree, index->byPath, &target);
    return (result) ? result->node : NULL;
}

struct NodeIndex_ipEntry* NodeIndex_getByIp(struct NodeIndex* index, struct Address* addr)
{
    ENTRY_FROM_IP(target, addr)
    return IpTree_RB_FIND(index->byIp, &target);
}

struct Node* NodeIndex_getByMaxPath(struct NodeIndex* index)
{
    struct NodeIndex_pathEntry* result = RB_MAX(PathTree, index->byPath);
    return (result) ? result->node : NULL;
}

static void putIp(struct NodeIndex* index, struct Node* node)
{
    struct NodeIndex_ipEntry* entry = index->byIp->heap[index->size];
    entry->node = node;
    struct NodeIndex_ipEntry* existing = RB_FIND(IpTree, index->byIp, entry);

    if (existing) {
         // tack it on the end of the list failing if we already have it
         while (existing->next) {
             if (existing->node == node) {
                 // dupes not allowed!
                 Assert_failure("Attempted to insert dupe into NodeIndex.");
             }
             existing = existing->next;
         }
         existing->next = entry;
    }
    else {
        // new address so just stick it in the tree
        RB_INSERT(IpTree, index->byIp, entry);
    }
}

static void putPath(struct NodeIndex* index, struct Node* node)
{
    struct NodeIndex_pathEntry* entry = index->byPath->heap[index->size];
    entry->node = node;
    struct NodeIndex_pathEntry* existing = RB_INSERT(PathTree, index->byPath, entry);
    if (existing) {
        Assert_failure("Attempted to insert dupe into NodeIndex.");
    }
}

void NodeIndex_put(struct NodeIndex* index, struct Node* node)
{
    Assert_always(node != NULL);
    Assert_always(index->size < index->capacity);
    putIp(index, node);
    putPath(index, node);
    index->size++;
}

static void removeIp(struct NodeIndex* index, struct Node* node)
{
    struct Address* addr = &node->address;
    ENTRY_FROM_IP(entry, addr)

    // find linked list
    struct NodeIndex_ipEntry* existing = RB_FIND(IpTree, index->byIp, &entry);

    if (existing) {

        // remove if head of the list
        if (existing->node == node) {
            RB_REMOVE(IpTree, index->byIp, existing);
            if (existing->next) {
                // add the rest of the list back into the tree
                RB_INSERT(IpTree, index->byIp, existing->next);
            }
        }
        else {
            // remove from the list
            while (existing->next) {
                struct NodeIndex_ipEntry* next = existing->next;
                if (next->node == node) {
                    existing->next = next->next;
                    existing = next;
                    break;
                }
                existing = next;
            }
        }

        Assert_always(existing->node == node);

        // find the current pointer in the heap
        int pos = -1;
        for (uint32_t i = 0; i < index->size; i++) {
            if (index->byIp->heap[i] == existing) {
                pos = i;
                break;
            }
        }

        // remove it from the heap
        Assert_always(pos >= 0);
        Bits_memmove(index->byIp->heap + pos,
                     index->byIp->heap + pos + 1,
                     sizeof(struct NodeIndex_ipEntry*)*(index->size - pos));

        // clear entry and throw pointer back on the heap
        Bits_memset(existing, 0, sizeof(struct NodeIndex_ipEntry));
        index->byIp->heap[index->size-1] = existing;

    }
}

static bool removePath(struct NodeIndex* index, struct Node* node)
{
    uint64_t path = node->address.path;
    ENTRY_FROM_PATH(target, path)
    struct NodeIndex_pathEntry* existing =  RB_FIND(PathTree, index->byPath, &target);

    if (existing) {
        Assert_always(existing->node == node);
        RB_REMOVE(PathTree, index->byPath, existing);
        // DEBUG
        Assert_always(NULL == RB_FIND(PathTree, index->byPath, &target));

        // find the current pointer in the heap
        int pos = -1;
        for (uint32_t i = 0; i < index->size; i++) {
            if (index->byPath->heap[i] == existing) {
                pos = i;
                break;
            }
        }

        // remove it from the heap
        if (pos < 0) {
            Assert_true(0);
        }
        Bits_memmove(index->byPath->heap + pos,
                     index->byPath->heap + pos + 1,
                     sizeof(struct NodeIndex_pathEntry*)*(index->size - pos));

        // clear entry and throw pointer back on the heap
        Bits_memset(existing, 0, sizeof(struct NodeIndex_pathEntry));
        index->byPath->heap[index->size-1] = existing;
    }

    return existing != NULL;
}


void NodeIndex_remove(struct NodeIndex* index, struct Node* node)
{
    Assert_always(node != NULL);
    if (removePath(index,node)) {
        removeIp(index,node);
        index->size--;
    }
}

void NodeIndex_visit(struct NodeIndex* index, void* context, void (*visit)(void *,struct Node *))
{
    struct NodeIndex_pathEntry* entry = RB_MIN(PathTree, index->byPath);
    while (entry) {
        visit(context, entry->node);
        entry = RB_NEXT(PathTree, index->byPath, entry);
    }
}
