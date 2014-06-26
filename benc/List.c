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

int32_t List_size(const List* list)
{
    if (list != NULL) {
        struct List_Item* item = *list;
        int32_t i;
        for (i = 0; item != NULL; i++) {
            item = item->next;
        }
        return i;
    }
    return -1;
}

static List* addObject(List* list, Object* item, struct Allocator* allocator)
{
    if (list == NULL) {
        List* newList = Allocator_calloc(allocator, sizeof(List), 1);
        return addObject(newList, item, allocator);
    }

    struct List_Item* entry = Allocator_malloc(allocator, sizeof(struct List_Item));
    entry->next = *list;
    entry->elem = item;
    *list = entry;

    return list;
}

/** @see Object.h */
List* List_addInt(List* list, int64_t toAdd, struct Allocator* allocator)
{
    Object* obj = Allocator_clone(allocator, (&(Object) {
        .type = Object_INTEGER,
        .as.number = toAdd
    }));
    return addObject(list, obj, allocator);
}

/** @see Object.h */
List* List_addString(List* list, String* toAdd, struct Allocator* allocator)
{
    Object* obj = Allocator_clone(allocator, (&(Object) {
        .type = Object_STRING,
        .as.string = toAdd
    }));
    return addObject(list, obj, allocator);
}

/** @see Object.h */
List* List_addDict(List* list, Dict* toAdd, struct Allocator* allocator)
{
    Object* obj = Allocator_clone(allocator, (&(Object) {
        .type = Object_DICT,
        .as.dictionary = toAdd
    }));
    return addObject(list, obj, allocator);
}

/** @see Object.h */
List* List_addList(List* list, List* toAdd, struct Allocator* allocator)
{
    Object* obj = Allocator_clone(allocator, (&(Object) {
        .type = Object_LIST,
        .as.list = toAdd
    }));
    return addObject(list, obj, allocator);
}

static Object* getObject(const List* list, uint32_t index)
{
    if (list != NULL && *list != NULL) {
        struct List_Item* entry = *list;
        uint32_t i;
        for (i = 0; entry != NULL; i++) {
            if (i == index) {
                return entry->elem;
            }
            entry = entry->next;
        }
    }
    return NULL;
}

/** @see Object.h */
int64_t* List_getInt(const List* list, uint32_t index)
{
    Object* o = getObject(list, index);
    if (o != NULL && o->type == Object_INTEGER) {
        return &(o->as.number);
    }
    return NULL;
}

/** @see Object.h */
String* List_getString(const List* list, uint32_t index)
{
    Object* o = getObject(list, index);
    if (o != NULL && o->type == Object_STRING) {
        return o->as.string;
    }
    return NULL;
}

/** @see Object.h */
Dict* List_getDict(const List* list, uint32_t index)
{
    Object* o = getObject(list, index);
    if (o != NULL && o->type == Object_DICT) {
        return o->as.dictionary;
    }
    return NULL;
}

/** @see Object.h */
List* List_getList(const List* list, uint32_t index)
{
    Object* o = getObject(list, index);
    if (o != NULL && o->type == Object_LIST) {
        return o->as.list;
    }
    return NULL;
}
