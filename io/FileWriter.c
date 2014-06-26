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
#include "io/Writer.h"
#include "io/FileWriter.h"
#include "util/Identity.h"

struct FileWriter_context {
    struct Writer generic;
    FILE* writeTo;
    Identity
};

/** @see Writer->write() */
static int write(struct Writer* writer, const void* toWrite, unsigned long length)
{
    struct FileWriter_context* context = Identity_check((struct FileWriter_context*) writer);

    size_t written = fwrite(toWrite, 1, length, context->writeTo);

    writer->bytesWritten += written;

    return written - length;
}


/** @see ArrayWriter.h */
struct Writer* FileWriter_new(FILE* writeTo, struct Allocator* allocator)
{
    struct FileWriter_context* context = Allocator_clone(allocator, (&(struct FileWriter_context) {
        .generic = {
            .write = write
        },
        .writeTo = writeTo
    }));
    Identity_set(context);

    return &context->generic;
}
