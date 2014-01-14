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
#include "util/Identity.h"

struct ArrayReader_context {
    struct Reader generic;
    const char* pointer;
    const char* endPointer;
    Identity
};

/** @see Reader->read() */
static int read(struct Reader* reader, void* readInto, unsigned long length)
{
    struct ArrayReader_context* context = Identity_check((struct ArrayReader_context*) reader);

    // Prove that it doesn't run off the end of the buffer or roll over.
    if (context->pointer + length > context->endPointer
        || context->pointer + length < context->pointer)
    {
        return -1;
    }

    if (length == 0) {
        // Allow peaking.
        *((char*)readInto) = *context->pointer;
        return 0;
    }

    Bits_memcpy(readInto, context->pointer, length);
    context->pointer += length;
    reader->bytesRead += length;

    return 0;
}

/** @see Reader->skip() */
static void skip(struct Reader* reader, unsigned long byteCount)
{
    struct ArrayReader_context* context = Identity_check((struct ArrayReader_context*) reader);
    context->pointer += byteCount;
    reader->bytesRead += byteCount;
}

/** @see ArrayReader.h */
struct Reader* ArrayReader_new(const void* bufferToRead,
                               unsigned long length,
                               struct Allocator* alloc)
{
    struct ArrayReader_context* context = Allocator_clone(alloc, (&(struct ArrayReader_context) {
        .generic = {
            .read = read,
            .skip = skip
        },
        .pointer = bufferToRead,
        .endPointer = (char*) bufferToRead + length
    }));
    Identity_set(context);
    return &context->generic;
}
