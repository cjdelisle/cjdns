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
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "io/Reader.h"
#include "io/ArrayReader.h"
#include "io/Writer.h"
#include "io/ArrayWriter.h"
#include "benc/Object.h"
#include "benc/serialization/BencSerializer.h"
#include "benc/serialization/standard/StandardBencSerializer.h"
#include "util/CString.h"

#include <stdio.h>

static int expect(char* str, struct Writer* writer, struct Reader* reader, int ret)
{
    char buffer[32];
    Writer_write(writer, "\0", 1);
    Reader_read(reader, buffer, CString_strlen(str) + 1);
    if (CString_strcmp(str, buffer) != 0) {
        printf("Expected %s\n Got %s\n", str, buffer);
        return -1;
    }
    return ret;
}

static int testSerialize(struct Writer* writer, struct Reader* reader)
{
    int ret = 0;

    ret |= StandardBencSerializer_get()->serializeint64_t(writer, 1);
    ret |= expect("i1e", writer, reader, ret);

    ret |= StandardBencSerializer_get()->serializeint64_t(writer, 1000);
    ret |= expect("i1000e", writer, reader, ret);

    ret |= StandardBencSerializer_get()->serializeint64_t(writer, -100);
    ret |= expect("i-100e", writer, reader, ret);

    return ret;
}


int main()
{
    char out[512];
    struct Allocator* alloc = MallocAllocator_new(1<<20);
    struct Writer* writer = ArrayWriter_new(out, 512, alloc);
    struct Reader* reader = ArrayReader_new(out, 512, alloc);

    int ret = testSerialize(writer, reader);
    Allocator_free(alloc);
    return ret;
}
