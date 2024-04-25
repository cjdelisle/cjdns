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
#include "benc/String.h"
#include "benc/Dict.h"
#include "benc/List.h"
#include "benc/serialization/standard/BencMessageWriter.h"
#include "exception/Er.h"
#include "wire/Message.h"
#include "util/Base10.h"

static Er_DEFUN(void writeGeneric(Object* obj, Message_t* msg));

static Er_DEFUN(void writeListItems(struct List_Item* item, Message_t* msg))
{
    if (!item) { Er_ret(); }
    Er(writeListItems(item->next, msg));
    Er(writeGeneric(item->elem, msg));
    Er_ret();
}

static Er_DEFUN(void writeList(List* l, Message_t* msg))
{
    Er(Message_epush8(msg, 'e'));
    Er(writeListItems(*l, msg));
    Er(Message_epush8(msg, 'l'));
    Er_ret();
}

static Er_DEFUN(void writeInt(int64_t num, Message_t* msg))
{
    Er(Message_epush8(msg, 'e'));
    Er(Base10_write(msg, num));
    Er(Message_epush8(msg, 'i'));
    Er_ret();
}

static Er_DEFUN(void writeString(String* str, Message_t* msg))
{
    Er(Message_epush(msg, str->bytes, str->len));
    Er(Message_epush8(msg, ':'));
    Er(Base10_write(msg, str->len));
    Er_ret();
}

static Er_DEFUN(void writeDictEntries(struct Dict_Entry* entry, Message_t* msg))
{
    if (!entry) { Er_ret(); }
    Er(writeDictEntries(entry->next, msg));
    Er(writeGeneric(entry->val, msg));
    Er(writeString(entry->key, msg));
    Er_ret();
}

static Er_DEFUN(void writeDict(Dict* d, Message_t* msg))
{
    Er(Message_epush8(msg, 'e'));
    Er(writeDictEntries(*d, msg));
    Er(Message_epush8(msg, 'd'));
    Er_ret();
}

static Er_DEFUN(void writeGeneric(Object* obj, Message_t* msg))
{
    switch (obj->type) {
        case Object_STRING:  Er(writeString(obj->as.string, msg)); break;
        case Object_DICT:    Er(writeDict(obj->as.dictionary, msg)); break;
        case Object_LIST:    Er(writeList(obj->as.list, msg)); break;
        case Object_INTEGER: Er(writeInt(obj->as.number, msg)); break;
        default: Assert_failure("object of unknown type [%d]", obj->type);
    }
    Er_ret();
}

Er_DEFUN(void BencMessageWriter_write(Dict* toWrite, Message_t* msg))
{
    Er(writeDict(toWrite, msg));

    // lucky
    if (!((uintptr_t)Message_bytes(msg) % 8)) { Er_ret(); }

    char d = Er(Message_epop8(msg));
    Assert_true(d == 'd');
    Assert_true(Message_bytes(msg)[0] != 'e' && "Can't serialize empty messages");
    Assert_true(Message_bytes(msg)[0] >= '1' && Message_bytes(msg)[0] <= '9');

    // put the message into alignment by padding out the number with leading zeros :)
    do { Er(Message_epush8(msg, '0')); } while ((uintptr_t)Message_bytes(msg) % 8);

    Er(Message_epop8(msg));
    Er(Message_epush8(msg, 'd'));
    Er_ret();
}
