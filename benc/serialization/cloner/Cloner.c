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
#include "benc/serialization/cloner/Cloner.h"
#include "memory/Allocator.h"
#include "benc/List.h"
#include "benc/Dict.h"
#include "benc/String.h"

static Object* clone(Object* orig, struct Allocator* alloc);

static struct List_Item* cloneList(struct List_Item* orig, struct Allocator* alloc)
{
    if (!orig) { return NULL; }
    struct List_Item* out = Allocator_malloc(alloc, sizeof(struct List_Item));
    out->elem = clone(orig->elem, alloc);
    out->next = cloneList(orig->next, alloc);
    return out;
}

static struct Dict_Entry* cloneDict(struct Dict_Entry* orig, struct Allocator* alloc)
{
    if (!orig) { return NULL; }
    struct Dict_Entry* out = Allocator_malloc(alloc, sizeof(struct Dict_Entry));
    out->key = String_clone(orig->key, alloc);
    out->val = clone(orig->val, alloc);
    out->next = cloneDict(orig->next, alloc);
    return out;
}

static Object* clone(Object* orig, struct Allocator* alloc)
{
    Object* out = Allocator_malloc(alloc, sizeof(Object));
    out->type = orig->type;
    switch (orig->type) {
        case Object_INTEGER: out->as.number = orig->as.number; break;
        case Object_STRING: out->as.string = String_clone(orig->as.string, alloc); break;
        case Object_LIST: out->as.list = Cloner_cloneList(orig->as.list, alloc); break;
        case Object_DICT: out->as.dictionary = Cloner_cloneDict(orig->as.dictionary, alloc); break;
        default: Assert_true(0);
    }
    return out;
}

Dict* Cloner_cloneDict(Dict* orig, struct Allocator* alloc)
{
    Dict* out = Allocator_malloc(alloc, sizeof(Dict));
    *out = cloneDict(*orig, alloc);
    return out;
}

List* Cloner_cloneList(List* orig, struct Allocator* alloc)
{
    List* out = Allocator_malloc(alloc, sizeof(List));
    *out = cloneList(*orig, alloc);
    return out;
}
