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
#include "io/ArrayWriter.h"
#include "util/Bits.h"
#include "util/Identity.h"

struct ArrayWriter_context {
    struct Writer generic;
    char* beginPointer;
    char* pointer;
    char* endPointer;
    int returnCode;
    Identity
};

/** @see Writer->write() */
static int write(struct Writer* writer, const void* toWrite, unsigned long length)
{
    struct ArrayWriter_context* context = Identity_check((struct ArrayWriter_context*) writer);

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
    context->generic.bytesWritten += length;

    return 0;
}

/** @see ArrayWriter.h */
struct Writer* ArrayWriter_new(void* writeToBuffer,
                               unsigned long length,
                               struct Allocator* alloc)
{
    struct ArrayWriter_context* context = Allocator_clone(alloc, (&(struct ArrayWriter_context) {
        .generic = {
            .write = write
        },
        .beginPointer = writeToBuffer,
        .pointer = writeToBuffer,
        .endPointer = (char*) writeToBuffer + length
    }));
    Identity_set(context);
    return &context->generic;
}
