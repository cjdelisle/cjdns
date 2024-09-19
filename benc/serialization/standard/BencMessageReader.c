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
#include "benc/serialization/standard/BencMessageReader.h"
#include "exception/Err.h"
#include "rust/cjdns_sys/RTypes.h"
#include "rust/cjdns_sys/Rffi.h"
#include "wire/Message.h"
#include "util/Base10.h"

static Err_DEFUN readGeneric(Object** outP, Message_t* msg, struct Allocator* alloc);

static Err_DEFUN readInt(int64_t* outP, Message_t* msg, struct Allocator* alloc)
{
    int64_t num = -1;
    Err(Base10_read(&num, msg));
    uint8_t e = '\0';
    Err(Message_epop8(&e, msg));
    if (e != 'e') {
        Err_raise(Message_getAlloc(msg), "Int not terminated with 'e'");
    }
    *outP = num;
    return NULL;
}

static Err_DEFUN readString(String** outP, Message_t* msg, struct Allocator* alloc)
{
    int64_t len = -1;
    Err(Base10_read(&len, msg));
    if (len < 0) {
        Err_raise(alloc, "Negative string length");
    }
    uint8_t flag = '\0';
    Err(Message_epop8(&flag, msg));
    if (flag != ':') {
        Err_raise(alloc, "String not deliniated with a ':'");
    }
    if (len > Message_getLength(msg)) {
        Err_raise(alloc, "String too long");
    }
    String* str = String_newBinary(NULL, len, alloc);
    Err(Message_epop(msg, str->bytes, len));
    *outP = str;
    return NULL;
}

static Err_DEFUN readList(List** outP, Message_t* msg, struct Allocator* alloc)
{
    struct List_Item* last = NULL;
    for (;;) {
        uint8_t chr = '\0';
        Err(Message_epop8(&chr, msg));
        if (chr == 'e') {
            List* out = Allocator_malloc(alloc, sizeof(List));
            *out = last;
            *outP = out;
            return NULL;
        }
        Err(Message_epush8(msg, chr));

        struct List_Item* item = Allocator_malloc(alloc, sizeof(struct List_Item));
        Err(readGeneric(&item->elem, msg, alloc));
        item->next = last;
        last = item;
    }
}

static Err_DEFUN readDict(Dict** outP, Message_t* msg, struct Allocator* alloc)
{
    struct Dict_Entry* last = NULL;
    for (;;) {
        uint8_t chr = '\0';
        Err(Message_epop8(&chr, msg));
        if (chr == 'e') {
            Dict* out = Allocator_malloc(alloc, sizeof(Dict));
            *out = last;
            *outP = out;
            return NULL;
        }
        Err(Message_epush8(msg, chr));

        struct Dict_Entry* entry = Allocator_malloc(alloc, sizeof(struct Dict_Entry));
        Err(readString(&entry->key, msg, alloc));
        Err(readGeneric(&entry->val, msg, alloc));
        entry->next = last;
        last = entry;
    }
}

static Err_DEFUN readGeneric(Object** outP, Message_t* msg, struct Allocator* alloc)
{
    uint8_t chr = '\0';
    Err(Message_epop8(&chr, msg));
    Object* out = Allocator_calloc(alloc, sizeof(Object), 1);
    switch (chr) {
        case 'l': {
            out->type = Object_LIST;
            Err(readList(&out->as.list, msg, alloc));
            break;
        }
        case 'd': {
            out->type = Object_DICT;
            Err(readDict(&out->as.dictionary, msg, alloc));
            break;
        }
        case 'i': {
            out->type = Object_INTEGER;
            Err(readInt(&out->as.number, msg, alloc));
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
            Err(Message_epush8(msg, chr));
            Err(readString(&out->as.string, msg, alloc));
            break;
        }
        default: Err_raise(alloc, "Unexpected character in message [%c]", (char)chr);
    }
    *outP = out;
    return NULL;
}

Err_DEFUN BencMessageReader_read(Dict** outP, Message_t* msg, struct Allocator* alloc)
{
    uint8_t d = '\0';
    Err(Message_epop8(&d, msg));
    if (d != 'd') {
        Err_raise(alloc, "Message does not begin with a 'd' to open the dictionary");
    }
    Err(readDict(outP, msg, alloc));
    return NULL;
}

const char* BencMessageReader_readNoExcept(
    Message_t* msg, struct Allocator* alloc, Dict** outPtr)
{
    RTypes_Error_t* er = BencMessageReader_read(outPtr, msg, alloc);
    if (er) {
        return Rffi_printError(er, alloc);
    }
    return NULL;
}
