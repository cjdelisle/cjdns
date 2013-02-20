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
#include "io/ArrayReader.h"
#include "util/Bits.h"

struct ArrayReader_context {
    char* pointer;
    char* endPointer;
    size_t bytesRead;
};

static int read(void* readInto, size_t length, const struct Reader* reader);
static void skip(size_t byteCount, const struct Reader* reader);
static size_t bytesRead(const struct Reader* reader);

/** @see ArrayReader.h */
struct Reader* ArrayReader_new(const void* bufferToRead,
                               size_t length,
                               const struct Allocator* allocator)
{
    struct Reader* reader =
        Allocator_calloc(allocator, sizeof(struct Reader), 1);
    struct ArrayReader_context* context =
        Allocator_calloc(allocator, sizeof(struct ArrayReader_context), 1);

    context->pointer = (char*) bufferToRead;
    context->endPointer = (char*) bufferToRead + length;

    struct Reader localReader = {
        .context = context,
        .read = read,
        .skip = skip,
        .bytesRead = bytesRead
    };
    Bits_memcpyConst(reader, &localReader, sizeof(struct Reader));

    return reader;
}

/** @see Reader->read() */
static int read(void* readInto, size_t length, const struct Reader* reader)
{
    struct ArrayReader_context* context =
        (struct ArrayReader_context*) reader->context;

    /* Prove that it doesn't run off the end of the buffer or roll over. */
    if (context->pointer + length > context->endPointer
        || context->pointer + length < context->pointer)
    {
        return -1;
    }

    if (length == 0) {
        /* Allow peaking. */
        *((char*)readInto) = *context->pointer;
        return 0;
    }

    Bits_memcpy(readInto, context->pointer, length);
    context->pointer += length;
    context->bytesRead += length;

    return 0;
}

static size_t bytesRead(const struct Reader* reader)
{
    return ((struct ArrayReader_context*) reader->context)->bytesRead;
}

/** @see Reader->skip() */
static void skip(size_t byteCount, const struct Reader* reader)
{
    struct ArrayReader_context* context = (struct ArrayReader_context*) reader->context;
    context->pointer += byteCount;
    context->bytesRead += byteCount;
}
