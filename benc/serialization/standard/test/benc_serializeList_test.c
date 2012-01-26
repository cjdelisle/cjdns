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

#include "memory/Allocator.h"
#include "memory/BufferAllocator.h"
#include "io/Reader.h"
#include "io/ArrayReader.h"
#include "io/Writer.h"
#include "io/ArrayWriter.h"
#include "benc/Object.h"
#include "benc/serialization/BencSerializer.h"
#include "benc/serialization/standard/StandardBencSerializer.h"

int parseEmptyList()
{
    char* test = "d" "2:hi" "le" "e";
    char buffer[512];
    struct Allocator* alloc = BufferAllocator_new(buffer, 512);
    struct Reader* reader = ArrayReader_new(test, strlen(test), alloc);
    Dict d;
    int ret = List_getStandardBencSerializer()->parseDictionary(reader, alloc, &d);
    if (ret) {
        return ret;
    }
    char out[256];
    struct Writer* w = ArrayWriter_new(out, 256, alloc);
    ret = List_getStandardBencSerializer()->serializeDictionary(w, &d);
    if (ret) {
        return ret;
    }
    return memcmp(test, out, strlen(test));
}

int main()
{
    return parseEmptyList();
}
