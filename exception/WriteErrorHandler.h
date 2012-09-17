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
#ifndef WriteErrorHandler_H
#define WriteErrorHandler_H

#include "benc/Int.h"
#include "benc/serialization/BencSerializer.h"
#include "benc/serialization/standard/StandardBencSerializer.h"
#include "exception/AbortHandler.h"
#include "exception/Except.h"
#include "memory/Allocator.h"
#include "memory/BufferAllocator.h"

#include <stdio.h>

#define WriteErrorHandler_ALLOC_SIZE 1024

struct WriteErrorHandler {
    struct Except asHandler;
    struct Writer* writeTo;
};

static void WriteErrorHandler_callback(char* message, int code, struct Except* handler)
    Gcc_NORETURN;
static void WriteErrorHandler_callback(char* message, int code, struct Except* handler)
{
    struct WriteErrorHandler* weh = (struct WriteErrorHandler*) handler;
    uint8_t buff[1024];
    struct Allocator* alloc = BufferAllocator_new(buff, 1024);
    Dict* d = Dict_new(alloc);
    Dict_putInt(d, String_CONST("code"), code, alloc);
    Dict_putString(d, String_CONST("error"), String_CONST(message), alloc);
    if (StandardBencSerializer_get()->serializeDictionary(weh->writeTo, d)) {
        Except_raise(AbortHandler_INSTANCE, -1, "Failed to raise exception");
    }
    abort();
}

static struct Except* WriteErrorHandler_new(struct Writer* writeTo,
                                            struct Allocator* alloc)
{
    struct WriteErrorHandler* weh =
        alloc->clone(sizeof(struct WriteErrorHandler), alloc, &(struct WriteErrorHandler) {
            .asHandler = {
                .exception = WriteErrorHandler_callback
            },
            .writeTo = writeTo
        });
    return &weh->asHandler;
}

#endif
