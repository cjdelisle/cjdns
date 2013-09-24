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

  /** pointer to the first of an array of ip entries */
  struct NodeIndex_ipEntry* entries;
};

struct PathTree {
  /** pointer to the root of the rb tree */
  struct NodeIndex_pathEntry* rbh_root;

  /** pointer to the first of an array of path entries */
  struct NodeIndex_pathEntry* entries;
};

struct NodeIndex {
  struct Allocator* alloc;
  struct IpTree* byIp;
  struct PathTree* byPath;
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
    #define IPCOMPARE(n)                                  \
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
    return Allocator_clone(alloc, (&(struct NodeIndex) {
        .alloc = alloc,
        .byIp = Allocator_clone(alloc, (&(struct IpTree) {
           .rbh_root = NULL,
           .entries = NULL //Allocator_calloc(alloc, sizeof(struct NodeIndex_ipEntry), capacity)
        })),
        .byPath = Allocator_clone(alloc, (&(struct IpTree) {
           .rbh_root = NULL,
           .entries = NULL //Allocator_calloc(alloc, sizeof(struct NodeIndex_pathEntry), capacity)
        }))
    }));
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

struct Node* NodeIndex_put(struct NodeIndex* index, struct Node* node)
{
    // insert into path index
    struct NodeIndex_pathEntry* pathEntry = Allocator_calloc(index->alloc,
                                                             sizeof(struct NodeIndex_pathEntry), 1);
    pathEntry->node = node;
    struct NodeIndex_pathEntry* existing = RB_INSERT(PathTree, index->byPath, pathEntry);
    if (existing) {
        Allocator_realloc(index->alloc, existing, 0);
    }

    // insert into ip index
    struct NodeIndex_ipEntry* ipEntry = Allocator_calloc(index->alloc,
                                                          sizeof(struct NodeIndex_ipEntry), 1);
    ipEntry->node = node;
    struct NodeIndex_ipEntry* existingIp = RB_FIND(IpTree, index->byIp, ipEntry);

    if (existingIp) {
         // tack it on the end of the list unless we already have it
         while (existingIp->next) {
             if (existingIp->node == node) {
                 // dupe!  free this up and return
                 Allocator_realloc(index->alloc, ipEntry, 0);
                 return NULL;
             }
             existingIp = existingIp->next;
         }
         existingIp->next = ipEntry;
    }
    else {
        // new address so stick it in the tree
        RB_INSERT(IpTree, index->byIp, ipEntry);
    }

    return NULL;
}

struct Node* NodeIndex_remove(struct NodeIndex* index, struct Node* node)
{
    // remove from path index
    uint64_t path = node->address.path;
    ENTRY_FROM_PATH(target, path)
    struct NodeIndex_pathEntry* existing =  RB_FIND(PathTree, index->byPath, &target);
    struct Node* old = (existing) ? existing->node : NULL;
    if (existing) {
        RB_REMOVE(PathTree, index->byPath, existing);
        Allocator_realloc(index->alloc, existing, 0);
    }

    // remove from ip index
    struct Address* addr = &node->address;
    ENTRY_FROM_IP(ipEntry, addr)
    struct NodeIndex_ipEntry* existingIp = RB_FIND(IpTree, index->byIp, &ipEntry);

    if (!existingIp) {
        // nothing todo
        return NULL;
    }

    if (existingIp->node == node && existingIp->next == NULL) {
        // just remove from tree
        RB_REMOVE(IpTree, index->byIp, existingIp);
        Allocator_realloc(index->alloc, existingIp, 0);
    }
    else {
       // remove from linked list
       while (existingIp->next) {
           struct NodeIndex_ipEntry* tmp = existingIp->next;
           if (tmp->node == node) {
               // drop tmp from the list
               existingIp->next = tmp->next;
               Allocator_realloc(index->alloc, tmp, 0);
               break;
           }
           existingIp = existingIp->next;
       }
    }

    // TODO assert removed nodes match...
    return old;
}

struct Node* NodeIndex_nth(struct NodeIndex* index, uint32_t n)
{
    struct NodeIndex_pathEntry* entry = RB_MIN(PathTree, index->byPath);
    for (uint32_t i=0; i<n && entry != NULL; i++) {
        entry = RB_NEXT(PathTree, index->byPath, entry);
    }
    return (entry) ? entry->node : NULL;
}

void NodeIndex_visit(struct NodeIndex* index, void* context, void (*visit)(void *,struct Node *))
{
    struct NodeIndex_pathEntry* entry = RB_MIN(PathTree, index->byPath);
    while (entry) {
        visit(context, entry->node);
        entry = RB_NEXT(PathTree, index->byPath, entry);
    }
}
