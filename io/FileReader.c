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
#include "io/Reader.h"
#include "io/FileReader.h"
#include "memory/Allocator.h"
#include "util/Identity.h"

#include <stdio.h>
#include <stdint.h>

struct FileReader {
    struct Reader generic;
    FILE* toRead;
    int failed;
    Identity
};

/** @see Reader->read() */
static int read(struct Reader* reader, void* readInto, unsigned long length)
{
    struct FileReader* context = Identity_check((struct FileReader*) reader);

    int peek = 0;
    if (length == 0) {
        peek = 1;
        length++;
    }

    if (context->failed || fread((char*)readInto, 1, length, context->toRead) != length) {
        context->failed = 1;
        return -1;
    }

    if (peek) {
        ungetc(((char*)readInto)[0], context->toRead);
        length--;
    }

    context->generic.bytesRead += length;
    return 0;
}

/** @see Reader->skip() */
static void skip(struct Reader* reader, unsigned long length)
{
    struct FileReader* context = Identity_check((struct FileReader*) reader);

    #define BUFF_SZ 256
    uint8_t buff[BUFF_SZ];

    // fseek() and ftell() are unreliable.
    size_t amount;
    while ((amount = (length > BUFF_SZ) ? BUFF_SZ : length) && !context->failed) {
        context->failed = read(reader, buff, amount);
        length -= amount;
    }
}

/** @see FileReader.h */
struct Reader* FileReader_new(FILE* toRead, struct Allocator* allocator)
{
    struct FileReader* context = Allocator_clone(allocator, (&(struct FileReader) {
        .generic = {
            .read = read,
            .skip = skip
        },
        .toRead = toRead
    }));
    Identity_set(context);

    return &context->generic;
}
