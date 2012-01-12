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
#include <string.h>
#include <stdio.h>

#include "memory/MemAllocator.h"
#include "memory/BufferAllocator.h"
#include "io/Reader.h"
#include "io/ArrayReader.h"
#include "io/Writer.h"
#include "io/ArrayWriter.h"
#include "libbenc/benc.h"
#include "libbenc/serialization/BencSerializer.h"
#include "libbenc/serialization/standard/StandardBencSerializer.h"

int expect(char* str, struct Writer* writer, struct Reader* reader, int ret)
{
    char buffer[32];
    writer->write("\0", 1, writer);
    reader->read(buffer, strlen(str) + 1, reader);
    if (strcmp(str, buffer) != 0) {
        printf("Expected %s\n Got %s\n", str, buffer);
        return -1;
    }
    return ret;
}

int testSerialize(struct Writer* writer, struct Reader* reader)
{
    int ret = 0;

    ret |= benc_getStandardBencSerializer()->serializeInteger(writer, 1);
    ret |= expect("i1e", writer, reader, ret);

    ret |= benc_getStandardBencSerializer()->serializeInteger(writer, 1000);
    ret |= expect("i1000e", writer, reader, ret);

    ret |= benc_getStandardBencSerializer()->serializeInteger(writer, -100);
    ret |= expect("i-100e", writer, reader, ret);

    return ret;
}


int main()
{
    char buffer[512];
    char out[512];
    struct MemAllocator* alloc = BufferAllocator_new(buffer, 512);
    struct Writer* writer = ArrayWriter_new(out, 512, alloc);
    struct Reader* reader = ArrayReader_new(out, 512, alloc);

    return testSerialize(writer, reader);
}
