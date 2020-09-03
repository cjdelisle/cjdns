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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#define Set_NOCREATE
#include "util/Set.h"
#include "util/Bits.h"
#include "util/Tree.h"

#include <stddef.h>
#include <stdlib.h>

struct Entry {
    void* data;
    uint32_t hashCode;
    struct Allocator* alloc;
    struct Set_pvt* set;
    struct {
        struct Entry* rbe_left;
        struct Entry* rbe_right;
        struct Entry* rbe_parent;
        int rbe_color;
    } tree;
};

#define BLOCK_SZ 8
struct Block {
    int usedCount;
    int number;
    struct Block* next;
    struct Allocator* alloc;
    struct Entry entries[BLOCK_SZ];
};

struct Set_Iter
{
    void* val;
    struct Entry* entry;
};

struct Set_pvt
{
    int size;

    struct Allocator* alloc;
    struct Block* block;

    struct ActiveTree {
        struct Entry* rbh_root;
    } activeTree;

    struct FreeTree {
        struct Entry* rbh_root;
    } freeTree;

    Set_Compare_t compare;
    Set_HashCode_t hashCode;

    Identity
};

static int compare(const struct Entry* a, const struct Entry* b)
{
    if (a->hashCode != b->hashCode) { return a->hashCode - b->hashCode; }
    struct Set_pvt* set = Identity_check((struct Set_pvt*) a->set);
    return set->compare(a->data, b->data);
}
RB_GENERATE_STATIC(ActiveTree, Entry, tree, compare)

static int freeCompare(const struct Entry* a, const struct Entry* b)
{
    return a->hashCode - b->hashCode;
}
RB_GENERATE_STATIC(FreeTree, Entry, tree, freeCompare)

static struct Block* blockForEntry(const struct Set_pvt* set, const struct Entry* e)
{
    struct Block* b = set->block;
    uintptr_t ep = (uintptr_t) e;
    while (b) {
        if (ep >= (uintptr_t)&b->entries && ep < (uintptr_t)&b->entries[BLOCK_SZ]) { return b; }
        b = b->next;
    }
    Assert_failure("No matching block for entry");
}

static struct Entry* allocateBlock(struct Set_pvt* set)
{
    struct Allocator* alloc = Allocator_child(set->alloc);
    struct Block* newBlock = Allocator_calloc(alloc, sizeof(struct Block), 1);
    newBlock->alloc = alloc;
    newBlock->next = set->block;
    set->block = newBlock;
    uint32_t num = newBlock->number = (set->block ? set->block->number : -1) + 1;
    for (int i = 0; i < BLOCK_SZ; i++) {
        newBlock->entries[i].set = set;
        newBlock->entries[i].hashCode = num;
        FreeTree_RB_INSERT(&set->freeTree, &newBlock->entries[i]);
    }
    return &newBlock->entries[0];
}

static struct Entry* newEntry(struct Set_pvt* set)
{
    struct Entry* e = RB_MIN(FreeTree, &set->freeTree);
    if (!e) { e = allocateBlock(set); }
    FreeTree_RB_REMOVE(&set->freeTree, e);
    struct Block* b = blockForEntry(set, e);
    b->usedCount++;
    set->size++;
    return e;
}

static void freeBlock(struct Set_pvt* set, struct Block* b)
{
    for (int i = 0; i < BLOCK_SZ; i++) {
        FreeTree_RB_REMOVE(&set->freeTree, &b->entries[i]);
    }
    int ok = 0;
    for (struct Block** bp = &set->block; *bp; bp = &(*bp)->next) {
        if (*bp == b) {
            *bp = b->next;
            ok = 1;
            break;
        }
    }
    Assert_true(ok && "INTERNAL: block was not linked into linked list");
    Allocator_free(b->alloc);
}

static void freeEntry(struct Set_pvt* set, struct Entry* e)
{
    struct Block* b = blockForEntry(set, e);
    ActiveTree_RB_REMOVE(&set->activeTree, e);
    e->data = NULL;
    e->hashCode = b->number;
    if (e->alloc) {
        Allocator_free(e->alloc);
        e->alloc = NULL;
    }
    FreeTree_RB_INSERT(&set->freeTree, e);
    b->usedCount--;
    set->size--;
    if (!b->usedCount) { freeBlock(set, b); }
}

static struct Entry* get(struct Set_pvt* set, void* val, uint32_t hashCode)
{
    struct Entry e = {
        .hashCode = hashCode,
        .data = val,
        .set = set
    };
    return ActiveTree_RB_FIND(&set->activeTree, &e);
}

int Set_addCopy(struct Set* _set, void* val, uint32_t size)
{
    struct Set_pvt* set = Identity_check((struct Set_pvt*) _set);
    uint32_t hashCode = set->hashCode(val);
    struct Entry* e = get(set, val, hashCode);
    if (!e) {
        struct Entry* e = newEntry(set);
        e->hashCode = hashCode;
        ActiveTree_RB_INSERT(&set->activeTree, e);
        struct Block* b = blockForEntry(set, e);
        e->alloc = Allocator_child(b->alloc);
        e->data = Allocator_malloc(e->alloc, size);
        Bits_memcpy(e->data, val, size);
    }
    return set->size;
}

int Set_add(struct Set* _set, void* val)
{
    struct Set_pvt* set = Identity_check((struct Set_pvt*) _set);
    uint32_t hashCode = set->hashCode(val);
    struct Entry* e = get(set, val, hashCode);
    if (!e) {
        struct Entry* e = newEntry(set);
        e->data = val;
        e->hashCode = hashCode;
        ActiveTree_RB_INSERT(&set->activeTree, e);
    }
    return set->size;
}

void Set_iter(struct Set* _set, struct Set_Iter* iter)
{
    struct Set_pvt* set = Identity_check((struct Set_pvt*) _set);
    struct Entry* e = iter->entry = RB_MIN(ActiveTree, &set->activeTree);
    if (e) {
        iter->val = e->data;
        iter->entry = ActiveTree_RB_NEXT(e);
    } else {
        iter->val = NULL;
    }
}

void Set_iterNext(struct Set_Iter* iter)
{
    struct Entry* e = iter->entry;
    if (e) {
        iter->val = e->data;
        iter->entry = ActiveTree_RB_NEXT(e);
    } else {
        iter->val = NULL;
    }
}

void* Set_remove(struct Set* _set, void* val)
{
    struct Set_pvt* set = Identity_check((struct Set_pvt*) _set);
    struct Entry* e = get(set, val, set->hashCode(val));
    void* out = NULL;
    if (e) {
        out = e->data;
        freeEntry(set, e);
    }
    return out;
}

void* Set_get(struct Set* _set, void* val)
{
    struct Set_pvt* set = Identity_check((struct Set_pvt*) _set);
    struct Entry* e = get(set, val, set->hashCode(val));
    void* out = NULL;
    if (e) { out = e->data; }
    return out;
}

struct Set* Set_new(struct Allocator* alloc, Set_HashCode_t hashCode, Set_Compare_t compare)
{
    struct Set_pvt* out = Allocator_calloc(alloc, sizeof(struct Set_pvt), 1);
    Identity_set(out);
    out->alloc = alloc;
    out->compare = compare;
    out->hashCode = hashCode;
    return (struct Set*) out;
}
