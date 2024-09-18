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
#include "exception/Er.h"
#include "wire/Message.h"
#include "util/Base10.h"

static Er_DEFUN(Object* readGeneric(Message_t* msg, struct Allocator* alloc));

static Er_DEFUN(int64_t readInt(Message_t* msg, struct Allocator* alloc))
{
    int64_t num = Er(Base10_read(msg));
    uint8_t e = '\0';
    Er(Er_fromErr(Message_epop8(&e, msg)));
    if (e != 'e') {
        Er_raise(Message_getAlloc(msg), "Int not terminated with 'e'");
    }
    Er_ret(num);
}

static Er_DEFUN(String* readString(Message_t* msg, struct Allocator* alloc))
{
    int64_t len = Er(Base10_read(msg));
    if (len < 0) {
        Er_raise(alloc, "Negative string length");
    }
    uint8_t flag = '\0';
    Er(Er_fromErr(Message_epop8(&flag, msg)));
    if (flag != ':') {
        Er_raise(alloc, "String not deliniated with a ':'");
    }
    if (len > Message_getLength(msg)) {
        Er_raise(alloc, "String too long");
    }
    String* str = String_newBinary(NULL, len, alloc);
    Er(Er_fromErr(Message_epop(msg, str->bytes, len)));
    Er_ret(str);
}

static Er_DEFUN(List* readList(Message_t* msg, struct Allocator* alloc))
{
    struct List_Item* last = NULL;
    for (;;) {
        uint8_t chr = '\0';
        Er(Er_fromErr(Message_epop8(&chr, msg)));
        if (chr == 'e') {
            List* out = Allocator_malloc(alloc, sizeof(List));
            *out = last;
            Er_ret(out);
        }
        Er(Er_fromErr(Message_epush8(msg, chr)));

        struct List_Item* item = Allocator_malloc(alloc, sizeof(struct List_Item));
        item->elem = Er(readGeneric(msg, alloc));
        item->next = last;
        last = item;
    }
}

static Er_DEFUN(Dict* readDict(Message_t* msg, struct Allocator* alloc))
{
    struct Dict_Entry* last = NULL;
    for (;;) {
        uint8_t chr = '\0';
        Er(Er_fromErr(Message_epop8(&chr, msg)));
        if (chr == 'e') {
            Dict* out = Allocator_malloc(alloc, sizeof(Dict));
            *out = last;
            Er_ret(out);
        }
        Er(Er_fromErr(Message_epush8(msg, chr)));

        struct Dict_Entry* entry = Allocator_malloc(alloc, sizeof(struct Dict_Entry));
        entry->key = Er(readString(msg, alloc));
        entry->val = Er(readGeneric(msg, alloc));
        entry->next = last;
        last = entry;
    }
}

static Er_DEFUN(Object* readGeneric(Message_t* msg, struct Allocator* alloc))
{
    uint8_t chr = '\0';
    Er(Er_fromErr(Message_epop8(&chr, msg)));
    Object* out = Allocator_calloc(alloc, sizeof(Object), 1);
    switch (chr) {
        case 'l': {
            out->type = Object_LIST;
            out->as.list = Er(readList(msg, alloc));
            break;
        }
        case 'd': {
            out->type = Object_DICT;
            out->as.dictionary = Er(readDict(msg, alloc));
            break;
        }
        case 'i': {
            out->type = Object_INTEGER;
            out->as.number = Er(readInt(msg, alloc));
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
            Er(Er_fromErr(Message_epush8(msg, chr)));
            out->as.string = Er(readString(msg, alloc));
            break;
        }
        default: Er_raise(alloc, "Unexpected character in message [%c]", (char)chr);
    }
    Er_ret(out);
}

Er_DEFUN(Dict* BencMessageReader_read(Message_t* msg, struct Allocator* alloc))
{
    uint8_t d = '\0';
    Er(Er_fromErr(Message_epop8(&d, msg)));
    if (d != 'd') {
        Er_raise(alloc, "Message does not begin with a 'd' to open the dictionary");
    }
    Dict* out = Er(readDict(msg, alloc));
    Er_ret(out);
}

const char* BencMessageReader_readNoExcept(
    Message_t* msg, struct Allocator* alloc, Dict** outPtr)
{
    struct Er_Ret* er = NULL;
    Dict* out = Er_check(&er, BencMessageReader_read(msg, alloc));
    if (er) {
        return er->message;
    }
    *outPtr = out;
    return NULL;
}
