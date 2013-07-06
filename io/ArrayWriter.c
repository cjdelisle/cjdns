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
#include "ArrayWriter.h"
#include "util/Bits.h"

struct ArrayWriter_context {
    char* beginPointer;
    char* pointer;
    char* endPointer;
    int returnCode;
};

static int write(const void* toWrite, size_t length, const struct Writer* writer);
static uint64_t bytesWritten(const struct Writer* writer);

/** @see ArrayWriter.h */
struct Writer* ArrayWriter_new(void* writeToBuffer,
                               size_t length,
                               const struct Allocator* allocator)
{
    struct Writer* writer =
        Allocator_calloc(allocator, sizeof(struct Writer), 1);
    struct ArrayWriter_context* context =
        Allocator_calloc(allocator, sizeof(struct ArrayWriter_context), 1);

    if (context == NULL || writer == NULL) {
        return NULL;
    }

    context->beginPointer = (char*) writeToBuffer;
    context->pointer = (char*) writeToBuffer;
    context->endPointer = (char*) writeToBuffer + length;

    struct Writer localWriter = {
        .context = context,
        .write = write,
        .bytesWritten = bytesWritten
    };
    Bits_memcpyConst(writer, &localWriter, sizeof(struct Writer));

    return writer;
}

/** @see Writer->write() */
static int write(const void* toWrite, size_t length, const struct Writer* writer)
{
    struct ArrayWriter_context* context =
        (struct ArrayWriter_context*) writer->context;

    /* If there was a previous failure then don't allow any more writing. */
    if (context->returnCode != 0) {
        return context->returnCode;
    }

    /* Prove that it doesn't run off the end of the buffer or roll over. */
    if (context->pointer + length > context->endPointer
        || context->pointer + length < context->pointer)
    {
        context->returnCode = -1;
        return -1;
    }

    Bits_memcpy(context->pointer, toWrite, length);
    context->pointer += length;

    return 0;
}

static uint64_t bytesWritten(const struct Writer* writer)
{
    struct ArrayWriter_context* context =
        (struct ArrayWriter_context*) writer->context;

    return context->pointer - context->beginPointer;
}
