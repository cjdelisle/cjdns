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

#include "benc/serialization/json/JsonBencMessageReader.h"
#include "benc/List.h"
#include "benc/Dict.h"
#include "benc/String.h"
#include "exception/Except.h"
#include "memory/Allocator.h"
#include "wire/Message.h"
#include "exception/Jmp.h"
#include "util/Gcc.h"
#include "util/Hex.h"
#include "util/Base10.h"

#include <stdbool.h>

struct Context {
    struct Message* const msg;
    struct Except* const eh;
    struct Allocator* const alloc;
    const bool lax;
    int line;
    uintptr_t beginningLastLine;
};

static int getColumn(struct Context* ctx)
{
    return (uintptr_t) ctx->msg->bytes - ctx->beginningLastLine;
}

#define ERROR0(ctx, message) \
    Except__throw(Gcc_SHORT_FILE, Gcc_LINE, ctx->eh,    \
        "Error parsing config (line %d column %d): " message, \
        ctx->line, getColumn(ctx))
#define ERROR(ctx, message, ...) \
    Except__throw(Gcc_SHORT_FILE, Gcc_LINE, ctx->eh,    \
        "Error parsing config (line %d column %d): " message, \
        ctx->line, getColumn(ctx), __VA_ARGS__)

static uint8_t peak(struct Context* ctx)
{
    if (!ctx->msg->length) { ERROR0(ctx, "Out of content while reading"); }
    return ctx->msg->bytes[0];
}

static void skip(struct Context* ctx, int num)
{
    if (num > ctx->msg->length) { ERROR0(ctx, "Out of content while reading"); }
    for (int i = 0; i < num; i++) {
        if (ctx->msg->bytes[i] == '\n') {
            ctx->beginningLastLine = (uintptr_t) &ctx->msg->bytes[i];
            ctx->line++;
        }
    }
    Message_shift(ctx->msg, -num, ctx->eh);
}

static bool assertChar(struct Context* ctx, char chr, bool lax)
{
    if (peak(ctx) != chr) {
        if (lax == true) { return false; }
        ERROR(ctx, "Expected char [%c] but got [%c]", chr, peak(ctx));
    }
    return true;
}

static void parseComment(struct Context* ctx)
{
    assertChar(ctx, '/', false);
    skip(ctx, 1);
    uint8_t secondChar = peak(ctx);
    if (secondChar != '/' && secondChar != '*') { ERROR(ctx, "Unexpected char [%c]", secondChar); }
    bool lastCharSplat = false;
    for (;;) {
        skip(ctx, 1);
        uint8_t chr = peak(ctx);
        if (lastCharSplat && secondChar == '*' && chr == '/') {
            // get rid of the trailing *
            skip(ctx, 1);
        } else if (secondChar == '/' && chr == '\n') {
        } else {
            lastCharSplat = (chr == '*');
            continue;
        }
        return;
    }
}

static void parseWhitespaceAndComments(struct Context* ctx)
{
    for (;;) {
        switch (peak(ctx)) {
            case '\n':
            case ' ':
            case '\r':
            case '\t':
                skip(ctx, 1);
                continue;

            case '/':
                parseComment(ctx);
                continue;

            default: break;
        }
        return;
    }
    ERROR0(ctx, "Reached end of message while parsing");
}

static String* parseString(struct Context* ctx)
{
    assertChar(ctx, '"', false);
    int line = ctx->line;
    uintptr_t beginningLastLine = ctx->beginningLastLine;
    int msgLen = ctx->msg->length;

    String* out = NULL;
    uint32_t pos = 0;
    #define PUSHOUT(chr) do { \
        if (out) {                              \
            Assert_true(out->len > pos);        \
            out->bytes[pos] = (chr);            \
        }                                       \
        pos++;                                  \
    } while (0)
    // CHECKFILES_IGNORE expecting a ;

    for (;;) {
        skip(ctx, 1);
        uint8_t bchar = peak(ctx);
        switch (bchar) {
            case '"': {
                skip(ctx, 1);
                if (out) { return out; }
                // got the length, reset and then copy the string next cycle
                ctx->line = line;
                ctx->beginningLastLine = beginningLastLine;
                Message_shift(ctx->msg, msgLen - ctx->msg->length, NULL);
                out = String_newBinary(NULL, pos, ctx->alloc);
                pos = 0;
                continue;
            }
            case '\0':
            case '\n': {
                ERROR0(ctx, "Unterminated string");
            }
            case '\\': {
                skip(ctx, 1);
                uint8_t x = peak(ctx);
                skip(ctx, 1);
                if (x != 'x') {
                    ERROR0(ctx, "Char \\ only allowed if followed by x (as in \\xff)");
                }
                uint8_t high = peak(ctx);
                skip(ctx, 1);
                uint8_t low = peak(ctx);
                int byte = Hex_decodeByte(high, low);
                if (byte < 0 || (byte > 255)) { ERROR0(ctx, "Invalid hex encoding"); }
                PUSHOUT(byte);
                continue;
            }
            default: {
                PUSHOUT(bchar);
                continue;
            }
        }
    }
    #undef PUSHOUT
}

static int64_t parseInteger(struct Context* ctx)
{
    return Base10_read(ctx->msg, ctx->eh);
}

static Object* parseGeneric(struct Context* ctx);

static List* parseList(struct Context* ctx)
{
    assertChar(ctx, '[', false);
    skip(ctx, 1);
    struct List_Item* first = NULL;
    struct List_Item* last = NULL;
    for (int i = 0; ; i++) {
        for (;;) {
            parseWhitespaceAndComments(ctx);
            // lax mode, allow ,, extra ,,, commas
            if (!ctx->lax || peak(ctx) != ',') { break; }
            skip(ctx, 1);
        }
        if (peak(ctx) == ']') {
            skip(ctx, 1);
            List* out = Allocator_malloc(ctx->alloc, sizeof(List));
            *out = first;
            return out;
        }
        if (i && assertChar(ctx, ',', ctx->lax)) {
            skip(ctx, 1);
            parseWhitespaceAndComments(ctx);
        }
        struct List_Item* item = Allocator_malloc(ctx->alloc, sizeof(struct List_Item));
        item->elem = parseGeneric(ctx);
        if (last) {
            last->next = item;
        } else {
            first = item;
        }
        last = item;
    }
}

static Dict* parseDict(struct Context* ctx)
{
    assertChar(ctx, '{', false);
    skip(ctx, 1);
    struct Dict_Entry* last = NULL;
    struct Dict_Entry* first = NULL;
    for (int i = 0; ; i++) {
        for (;;) {
            parseWhitespaceAndComments(ctx);
            if (!ctx->lax || peak(ctx) != ',') { break; }
            skip(ctx, 1);
        }
        if (peak(ctx) == '}') {
            skip(ctx, 1);
            Dict* out = Allocator_malloc(ctx->alloc, sizeof(Dict));
            *out = first;
            return out;
        }
        if (i && assertChar(ctx, ',', ctx->lax)) {
            skip(ctx, 1);
            parseWhitespaceAndComments(ctx);
        }
        struct Dict_Entry* entry = Allocator_malloc(ctx->alloc, sizeof(struct Dict_Entry));
        entry->key = parseString(ctx);
        parseWhitespaceAndComments(ctx);
        if (assertChar(ctx, ':', ctx->lax)) {
            skip(ctx, 1);
            parseWhitespaceAndComments(ctx);
        }
        entry->val = parseGeneric(ctx);
        if (last) {
            last->next = entry;
        } else {
            first = entry;
        }
        last = entry;
    }
}

static Object* parseGeneric(struct Context* ctx)
{
    Object* out = Allocator_calloc(ctx->alloc, sizeof(Object), 1);
    switch (peak(ctx)) {
        case '-':
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
            out->type = Object_INTEGER;
            out->as.number = parseInteger(ctx);
            break;
        }
        case '[': {
            out->type = Object_LIST;
            out->as.list = parseList(ctx);
            break;
        }
        case '{': {
            out->type = Object_DICT;
            out->as.dictionary = parseDict(ctx);
            break;
        }
        case '"': {
            out->type = Object_STRING;
            out->as.string = parseString(ctx);
            break;
        }
        default:
            ERROR(ctx, "While looking for something to parse: "
                   "expected one of - 0 1 2 3 4 5 6 7 8 9 [ { \", found [%c]", peak(ctx));
    }
    return out;
}

Dict* JsonBencMessageReader_read(
    struct Message* msg,
    struct Allocator* alloc,
    struct Except* eh,
    bool lax
) {
    struct Context ctx = {
        .msg = msg,
        .eh = eh,
        .alloc = alloc,
        .lax = lax,
        .line = 1,
        .beginningLastLine = (uintptr_t) msg->bytes
    };
    return parseDict(&ctx);
}

char* JsonBencMessageReader_readNoExcept(
    struct Message* msg,
    struct Allocator* alloc,
    Dict** outPtr,
    bool lax
) {
    struct Jmp j;
    Jmp_try(j) {
        Dict* out = JsonBencMessageReader_read(msg, alloc, &j.handler, lax);
        *outPtr = out;
        return NULL;
    } Jmp_catch {
        String* str = String_new(j.message, alloc);
        return str->bytes;
    }
}

