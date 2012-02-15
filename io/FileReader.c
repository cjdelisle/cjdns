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

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

struct Context {
    FILE* toRead;
    bool failed;
    size_t bytesRead;
    struct Reader reader;
};

/** @see Reader->read() */
static int read(void* readInto, size_t length, const struct Reader* reader)
{
    struct Context* context = (struct Context*) reader->context;
    if (context->failed || fread((char*)readInto, 1, length, context->toRead) != length) {
        context->failed = true;
        return -1;
    }
    context->bytesRead += length;
    return 0;
}

/** @see Reader->bytesRead() */
static size_t bytesRead(const struct Reader* reader)
{
    // ftell() is unreliable.
    return ((struct Context*) reader->context)->bytesRead;
}

/** @see Reader->skip() */
static void skip(size_t length, const struct Reader* reader)
{
    struct Context* context = (struct Context*) reader->context;

    #define BUFF_SZ 256
    uint8_t buff[BUFF_SZ];

    // fseek() and ftell() are unreliable.
    size_t amount;
    while ((amount = (length > BUFF_SZ) ? BUFF_SZ : length) && !context->failed) {
        context->failed = read(buff, amount, reader);
        length -= amount;
    }
}

/** @see FileReader.h */
struct Reader* FileReader_new(FILE* toRead, const struct Allocator* allocator)
{
    struct Context* context = allocator->calloc(sizeof(struct Context), 1, allocator);

    context->toRead = toRead;

    struct Reader localReader = {
        .context = context,
        .read = read,
        .skip = skip,
        .bytesRead = bytesRead
    };
    memcpy(&context->reader, &localReader, sizeof(struct Reader));

    return &context->reader;
}
