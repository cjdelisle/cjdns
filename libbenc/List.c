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
#include "benc.h"

int32_t benc_itemCount(const List* list)
{
    if (list != NULL) {
        benc_list_entry_t* item = *list;
        int32_t i;
        for (i = 0; item != NULL; i++) {
            item = item->next;
        }
        return i;
    }
    return -1;
}

static List* addObject(List* list, Object* item, const struct Allocator* allocator)
{
    if (list == NULL) {
        List* newList = allocator->calloc(sizeof(List), 1, allocator);
        return addObject(newList, item, allocator);
    }

    benc_list_entry_t* entry = allocator->malloc(sizeof(benc_list_entry_t), allocator);
    entry->next = *list;
    entry->elem = item;
    *list = entry;

    return list;
}

/** @see benc.h */
List* benc_addInteger(List* list, Integer toAdd, const struct Allocator* allocator)
{
    Object* obj = allocator->clone(sizeof(Object), allocator, &(bobj_t) {
        .type = BENC_INT,
        .as.int_ = toAdd
    });
    return addObject(list, obj, allocator);
}

/** @see benc.h */
List* benc_addString(List* list, String* toAdd, const struct Allocator* allocator)
{
    Object* obj = allocator->clone(sizeof(Object), allocator, &(bobj_t) {
        .type = BENC_BSTR,
        .as.bstr = toAdd
    });
    return addObject(list, obj, allocator);
}

/** @see benc.h */
List* benc_addDictionary(List* list, Dict* toAdd, const struct Allocator* allocator)
{
    Object* obj = allocator->clone(sizeof(Object), allocator, &(bobj_t) {
        .type = BENC_DICT,
        .as.dictionary = toAdd
    });
    return addObject(list, obj, allocator);
}

/** @see benc.h */
List* benc_addList(List* list, List* toAdd, const struct Allocator* allocator)
{
    Object* obj = allocator->clone(sizeof(Object), allocator, &(bobj_t) {
        .type = BENC_LIST,
        .as.list = toAdd
    });
    return addObject(list, obj, allocator);
}

static Object* getObject(const List* list, uint32_t index)
{
    if (list != NULL && *list != NULL) {
        benc_list_entry_t* entry = *list;
        uint32_t i;
        for (i = 0; i < index && entry != NULL; i++) {
            entry = entry->next;
        }
        return entry->elem;
    }
    return NULL;
}

/** @see benc.h */
Integer* benc_getInteger(const List* list, uint32_t index)
{
    Object* o = getObject(list, index);
    if (o != NULL && o->type == BENC_INT) {
        return &(o->as.int_);
    }
    return NULL;
}

/** @see benc.h */
String* benc_getString(const List* list, uint32_t index)
{
    Object* o = getObject(list, index);
    if (o != NULL && o->type == BENC_BSTR) {
        return o->as.bstr;
    }
    return NULL;
}

/** @see benc.h */
Dict* benc_getDictionary(const List* list, uint32_t index)
{
    Object* o = getObject(list, index);
    if (o != NULL && o->type == BENC_DICT) {
        return o->as.dictionary;
    }
    return NULL;
}

/** @see benc.h */
List* benc_getList(const List* list, uint32_t index)
{
    Object* o = getObject(list, index);
    if (o != NULL && o->type == BENC_LIST) {
        return o->as.list;
    }
    return NULL;
}
