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
#include "memory/Allocator.h"
#include "benc/List.h"
#include "benc/Dict.h"
#include "benc/String.h"

#include <stddef.h>

List* List_new(struct Allocator* alloc)
{
    return Allocator_calloc(alloc, sizeof(List*), 1);
}

int32_t List_size(const List* list)
{
    Assert_true(list);
    struct List_Item* item = *list;
    uint32_t i = 0;
    for (; item; i++) {
        item = item->next;
    }
    return i;
}

static void addObject(List* list, Object* item, struct Allocator* allocator)
{
    Assert_true(list);

    struct List_Item* entry = Allocator_malloc(allocator, sizeof(struct List_Item));
    entry->next = *list;
    entry->elem = item;
    *list = entry;
}

#define ADD(list, asType, typeName, thing, alloc) \
    addObject(list, Allocator_clone(alloc, (&(Object) { \
        .type = typeName,                               \
        .as.asType = thing                              \
    })), alloc)

/** @see Object.h */
void List_addInt(List* list, int64_t toAdd, struct Allocator* allocator)
{
    ADD(list, number, Object_INTEGER, toAdd, allocator);
}

/** @see Object.h */
void List_addString(List* list, String* toAdd, struct Allocator* allocator)
{
    ADD(list, string, Object_STRING, toAdd, allocator);
}

/** @see Object.h */
void List_addDict(List* list, Dict* toAdd, struct Allocator* allocator)
{
    ADD(list, dictionary, Object_DICT, toAdd, allocator);
}

/** @see Object.h */
void List_addList(List* list, List* toAdd, struct Allocator* allocator)
{
    ADD(list, list, Object_LIST, toAdd, allocator);
}

static Object* getObject(const List* list, uint32_t index)
{
    Assert_true(list);
    struct List_Item* entry = *list;
    for (uint32_t i = 0; entry; i++) {
        if (i == index) {
            return entry->elem;
        }
        entry = entry->next;
    }
    return NULL;
}

/** @see Object.h */
int64_t* List_getInt(const List* list, uint32_t index)
{
    Object* o = getObject(list, index);
    if (o && o->type == Object_INTEGER) {
        return &(o->as.number);
    }
    return NULL;
}

#define GET(list, index, asType, typeName) \
    do {                                                   \
        Object* o = getObject(list, index);                \
        if (o && o->type == typeName) {                    \
            return o->as.asType;                           \
        }                                                  \
        return NULL;                                       \
    } while (0)
// CHECKFILES_IGNORE // expecting a ; or bracket

/** @see Object.h */
String* List_getString(const List* list, uint32_t index)
{
    GET(list, index, string, Object_STRING);
}

/** @see Object.h */
Dict* List_getDict(const List* list, uint32_t index)
{
    GET(list, index, dictionary, Object_DICT);
}

/** @see Object.h */
List* List_getList(const List* list, uint32_t index)
{
    GET(list, index, list, Object_LIST);
}
