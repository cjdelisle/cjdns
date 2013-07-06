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
#define string_strcmp
#include "util/platform/libc/string.h"
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "memory/CanaryAllocator.h"
#include "io/Reader.h"
#include "io/ArrayReader.h"
#include "io/Writer.h"
#include "io/ArrayWriter.h"
#include "benc/Object.h"
#include "benc/serialization/BencSerializer.h"
#include "benc/serialization/standard/StandardBencSerializer.h"
#include "util/Assert.h"

#include <stdio.h>

int expect(char* str, struct Writer* writer, struct Reader* reader)
{
    int ret = 0;
    char buffer[32];
    writer->write("\0", 1, writer);
    reader->read(buffer, strlen(str) + 1, reader);
    if (strcmp(str, buffer) != 0) {
        printf("Expected %s\n Got %s\n", str, buffer);
        return -1;
    }
    return ret;
}

void testSerialize(struct Writer* writer, struct Reader* reader)
{
    Assert_always(!StandardBencSerializer_get()->serializeString(writer, String_CONST("hello")));
    Assert_always(!expect("5:hello", writer, reader));

    Assert_always(!StandardBencSerializer_get()->serializeString(writer, String_CONST("")));
    Assert_always(!expect("0:", writer, reader));
}

void testParse(struct Writer* w, struct Reader* r, struct Allocator* alloc)
{
    char* badBenc = "d2:aq21:RouterModule_pingNode4:argsd4:path39:fcd9:6a75:6c9c7:timeouti4000ee"
                    "6:cookie0:4:hash64:09c6bcd1482df339757c99bbc5e796192968a28562f701fb53a57ed6"
                    "e26b15511:q4:auth4:txid19:43866780dc455e15619e";
    Writer_write(w, badBenc, strlen(badBenc)+1);
    Dict dict;
    Assert_always(StandardBencSerializer_get()->parseDictionary(r, alloc, &dict));
}


int main()
{
    char out[512];
    struct Allocator* alloc = CanaryAllocator_new(MallocAllocator_new(1<<20), NULL);

    struct Writer* writer = ArrayWriter_new(out, 512, alloc);
    struct Reader* reader = ArrayReader_new(out, 512, alloc);

    testSerialize(writer, reader);
    testParse(writer, reader, alloc);

    Allocator_free(alloc);
}
