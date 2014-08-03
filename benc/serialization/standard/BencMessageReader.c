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
#include "exception/Jmp.h"
#include "wire/Message.h"
#include "util/Base10.h"

static Object* readGeneric(struct Message* msg, struct Allocator* alloc, struct Except* eh);

static int64_t readInt(struct Message* msg, struct Allocator* alloc, struct Except* eh)
{
    int64_t num = Base10_read(msg, eh);
    if (Message_pop8(msg, eh) != 'e') {
        Except_throw(eh, "Int not terminated with 'e'");
    }
    return num;
}

static String* readString(struct Message* msg, struct Allocator* alloc, struct Except* eh)
{
    int64_t len = Base10_read(msg, eh);
    if (len < 0) {
        Except_throw(eh, "Negative string length");
    }
    if (Message_pop8(msg, eh) != ':') {
        Except_throw(eh, "String not deliniated with a ':'");
    }
    if (len > msg->length) {
        Except_throw(eh, "String too long");
    }
    String* str = String_newBinary(NULL, len, alloc);
    Message_pop(msg, str->bytes, len, eh);
    return str;
}

static List* readList(struct Message* msg, struct Allocator* alloc, struct Except* eh)
{
    struct List_Item* last = NULL;
    for (;;) {
        uint8_t chr = Message_pop8(msg, eh);
        if (chr == 'e') {
            List* out = Allocator_malloc(alloc, sizeof(List));
            *out = last;
            return out;
        }
        Message_push8(msg, chr, eh);

        struct List_Item* item = Allocator_malloc(alloc, sizeof(struct List_Item));
        item->elem = readGeneric(msg, alloc, eh);
        item->next = last;
        last = item;
    }
}

static Dict* readDict(struct Message* msg, struct Allocator* alloc, struct Except* eh)
{
    struct Dict_Entry* last = NULL;
    for (;;) {
        uint8_t chr = Message_pop8(msg, eh);
        if (chr == 'e') {
            Dict* out = Allocator_malloc(alloc, sizeof(Dict));
            *out = last;
            return out;
        }
        Message_push8(msg, chr, eh);

        struct Dict_Entry* entry = Allocator_malloc(alloc, sizeof(struct Dict_Entry));
        entry->key = readString(msg, alloc, eh);
        entry->val = readGeneric(msg, alloc, eh);
        entry->next = last;
        last = entry;
    }
}

static Object* readGeneric(struct Message* msg, struct Allocator* alloc, struct Except* eh)
{
    uint8_t chr = Message_pop8(msg, eh);
    Object* out = Allocator_calloc(alloc, sizeof(Object), 1);
    switch (chr) {
        case 'l': {
            out->type = Object_LIST;
            out->as.list = readList(msg, alloc, eh);
            break;
        }
        case 'd': {
            out->type = Object_DICT;
            out->as.dictionary = readDict(msg, alloc, eh);
            break;
        }
        case 'i': {
            out->type = Object_INTEGER;
            out->as.number = readInt(msg, alloc, eh);
            break;
        }
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9': {
            out->type = Object_STRING;
            Message_push8(msg, chr, eh);
            out->as.string = readString(msg, alloc, eh);
            break;
        }
        default: Except_throw(eh, "Unexpected character in message [%c]", (char)chr);
    }
    return out;
}

Dict* BencMessageReader_read(struct Message* msg, struct Allocator* alloc, struct Except* eh)
{
    if (Message_pop8(msg, eh) != 'd') {
        Except_throw(eh, "Message does not begin with a 'd' to open the dictionary");
    }
    return readDict(msg, alloc, eh);
}

char* BencMessageReader_readNoExcept(struct Message* msg, struct Allocator* alloc, Dict** outPtr)
{
    struct Jmp j;
    Jmp_try(j) {
        Dict* out = BencMessageReader_read(msg, alloc, &j.handler);
        *outPtr = out;
        return NULL;
    } Jmp_catch {
        String* str = String_new(j.message, alloc);
        return str->bytes;
    }
}
