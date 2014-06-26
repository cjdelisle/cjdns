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
#include "benc/String.h"
#include "benc/Dict.h"
#include "benc/serialization/BencSerializer.h"
#include "benc/serialization/standard/StandardBencSerializer.h"
#include "util/Bits.h"
#include "util/CString.h"

#include <stdio.h>

static int parseEmptyList()
{
    char* test = "d" "2:hi" "le" "e";
    struct Allocator* alloc = MallocAllocator_new(1<<20);
    struct Reader* reader = ArrayReader_new(test, CString_strlen(test), alloc);
    Dict d;
    int ret = StandardBencSerializer_get()->parseDictionary(reader, alloc, &d);

    char out[256];
    struct Writer* w = ArrayWriter_new(out, 256, alloc);
    ret |= StandardBencSerializer_get()->serializeDictionary(w, &d);
    ret |= Bits_memcmp(test, out, CString_strlen(test));

    Allocator_free(alloc);
    return ret;
}

int main()
{
    return parseEmptyList();
}
