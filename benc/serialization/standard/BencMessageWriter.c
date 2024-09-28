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

#include "benc/Object.h"
#include "benc/Dict.h"
#include "benc/List.h"
#include "benc/serialization/standard/BencMessageWriter.h"
#include "exception/Err.h"
#include "wire/Message.h"
#include "util/Base10.h"

static Err_DEFUN writeGeneric(Object* obj, Message_t* msg);

static Err_DEFUN writeListItems(struct List_Item* item, Message_t* msg)
{
    if (!item) { return NULL; }
    Err(writeListItems(item->next, msg));
    Err(writeGeneric(item->elem, msg));
    return NULL;
}

static Err_DEFUN writeList(List* l, Message_t* msg)
{
    Err(Message_epush8(msg, 'e'));
    Err(writeListItems(*l, msg));
    Err(Message_epush8(msg, 'l'));
    return NULL;
}

static Err_DEFUN writeInt(int64_t num, Message_t* msg)
{
    Err(Message_epush8(msg, 'e'));
    Err(Base10_write(msg, num));
    Err(Message_epush8(msg, 'i'));
    return NULL;
}

static Err_DEFUN writeString(String* str, Message_t* msg)
{
    Err(Message_epush(msg, str->bytes, str->len));
    Err(Message_epush8(msg, ':'));
    Err(Base10_write(msg, str->len));
    return NULL;
}

static Err_DEFUN writeDictEntries(struct Dict_Entry* entry, Message_t* msg)
{
    if (!entry) { return NULL; }
    Err(writeDictEntries(entry->next, msg));
    Err(writeGeneric(entry->val, msg));
    Err(writeString(entry->key, msg));
    return NULL;
}

static Err_DEFUN writeDict(Dict* d, Message_t* msg)
{
    Err(Message_epush8(msg, 'e'));
    Err(writeDictEntries(*d, msg));
    Err(Message_epush8(msg, 'd'));
    return NULL;
}

static Err_DEFUN writeGeneric(Object* obj, Message_t* msg)
{
    switch (obj->type) {
        case Object_STRING:  Err(writeString(obj->as.string, msg)); break;
        case Object_DICT:    Err(writeDict(obj->as.dictionary, msg)); break;
        case Object_LIST:    Err(writeList(obj->as.list, msg)); break;
        case Object_INTEGER: Err(writeInt(obj->as.number, msg)); break;
        default: Assert_failure("object of unknown type [%d]", obj->type);
    }
    return NULL;
}

Err_DEFUN BencMessageWriter_write(Dict* toWrite, Message_t* msg)
{
    Err(writeDict(toWrite, msg));

    // lucky
    if (!((uintptr_t)Message_bytes(msg) % 8)) { return NULL; }

    uint8_t d = '\0';
    Err(Message_epop8(&d, msg));
    Assert_true(d == 'd');
    Assert_true(Message_bytes(msg)[0] != 'e' && "Can't serialize empty messages");
    Assert_true(Message_bytes(msg)[0] >= '1' && Message_bytes(msg)[0] <= '9');

    // put the message into alignment by padding out the number with leading zeros :)
    do { Err(Message_epush8(msg, '0')); } while ((uintptr_t)Message_bytes(msg) % 8);

    Err(Message_epop8(&d, msg));
    Err(Message_epush8(msg, 'd'));
    return NULL;
}