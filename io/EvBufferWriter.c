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
#include "io/EvBufferWriter.h"
#include "util/Bits.h"

#include <event2/buffer.h>

struct Context {
    struct evbuffer* buffer;
    size_t amount;
    int returnCode;
    struct Writer writer;
};

static int write(const void* toWrite, size_t length, const struct Writer* writer);
static uint64_t bytesWritten(const struct Writer* writer);

/** @see EvBufferWriter.h */
struct Writer* EvBufferWriter_new(struct evbuffer* buffer, const struct Allocator* allocator)
{
    struct Context* context = allocator->calloc(sizeof(struct Context), 1, allocator);

    context->amount = 0;
    context->returnCode = 0;
    context->buffer = buffer;

    struct Writer localWriter = {
        .context = context,
        .write = write,
        .bytesWritten = bytesWritten
    };
    Bits_memcpyConst(&context->writer, &localWriter, sizeof(struct Writer));

    return &context->writer;
}

/** @see Writer->write() */
static int write(const void* toWrite, size_t length, const struct Writer* writer)
{
    struct Context* context = (struct Context*) writer->context;

    // If there was a previous failure then don't allow any more writing.
    if (context->returnCode) {
        return context->returnCode;
    }
    context->returnCode = evbuffer_add(context->buffer, toWrite, length);

    if (!context->returnCode) {
        context->amount += length;
    }

    return context->returnCode;
}

static uint64_t bytesWritten(const struct Writer* writer)
{
    return ((struct Context*) writer->context)->amount;
}
