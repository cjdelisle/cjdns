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

#include "benc/Object.h"
#include "benc/String.h"
#include "benc/Dict.h"
#include "benc/List.h"
#include "exception/Except.h"
#include "wire/Message.h"
#include "util/Base10.h"

static void writeGeneric(Object* obj, struct Message* msg, struct Except* eh);

static void writeListItems(struct List_Item* item, struct Message* msg, struct Except* eh)
{
    if (!item) { return; }
    writeListItems(item->next, msg, eh);
    writeGeneric(item->elem, msg, eh);
}

static void writeList(List* l, struct Message* msg, struct Except* eh)
{
    Message_push8(msg, 'e', eh);
    writeListItems(*l, msg, eh);
    Message_push8(msg, 'l', eh);
}

static void writeInt(int64_t num, struct Message* msg, struct Except* eh)
{
    Message_push8(msg, 'e', eh);
    Base10_write(msg, num, eh);
    Message_push8(msg, 'i', eh);
}

static void writeString(String* str, struct Message* msg, struct Except* eh)
{
    Message_push(msg, str->bytes, str->len, eh);
    Message_push8(msg, ':', eh);
    Base10_write(msg, str->len, eh);
}

static void writeDictEntries(struct Dict_Entry* entry, struct Message* msg, struct Except* eh)
{
    if (!entry) { return; }
    writeDictEntries(entry->next, msg, eh);
    writeGeneric(entry->val, msg, eh);
    writeString(entry->key, msg, eh);
}

static void writeDict(Dict* d, struct Message* msg, struct Except* eh)
{
    Message_push8(msg, 'e', eh);
    writeDictEntries(*d, msg, eh);
    Message_push8(msg, 'd', eh);
}

static void writeGeneric(Object* obj, struct Message* msg, struct Except* eh)
{
    switch (obj->type) {
        case Object_STRING:  writeString(obj->as.string, msg, eh); break;
        case Object_DICT:    writeDict(obj->as.dictionary, msg, eh); break;
        case Object_LIST:    writeList(obj->as.list, msg, eh); break;
        case Object_INTEGER: writeInt(obj->as.number, msg, eh); break;
        default: Assert_failure("object of unknown type [%d]", obj->type);
    }
}

void BencMessageWriter_write(Dict* toWrite, struct Message* msg, struct Except* eh)
{
    writeDict(toWrite, msg, eh);

    // lucky
    if (!((uintptr_t)msg->bytes % 8)) { return; }

    Assert_true(Message_pop8(msg, eh) == 'd');
    Assert_true(msg->bytes[0] != 'e' && "Can't serialize empty messages");
    Assert_true(msg->bytes[0] >= '1' && msg->bytes[0] <= '9');

    // put the message into alignment by padding out the number with leading zeros :)
    do { Message_push8(msg, '0', eh); } while ((uintptr_t)msg->bytes % 8);

    Message_pop8(msg, eh);
    Message_push8(msg, 'd', eh);
}
