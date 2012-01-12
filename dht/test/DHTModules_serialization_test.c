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
/*
 * This test tests the ability of DHTModules.c to serialize the contexts
 * of the modules. For (de)serialization of the messages go in and out,
 * see SerializationModule_test.c
 */

#include <stdio.h>
#include <string.h>

#include "dht/DHTModules.h"
#include "memory/MemAllocator.h"
#include "memory/BufferAllocator.h"
#include "io/Writer.h"
#include "io/ArrayWriter.h"
#include "io/Reader.h"
#include "io/ArrayReader.h"

static const char* control = "d10:TestModuled2:hi11:Hello Worldee";

static const benc_bstr_t hello = { .bytes = "Hello World!", .len = 12 };
static const benc_bstr_t hi = { .bytes = "hi", .len = 2 };

static Dict* serialize(void* vcontext)
{
    char* buffer = calloc(2048, 1);
    struct MemAllocator* allocator = BufferAllocator_new(buffer, 2048);
    Dict* out = benc_newDictionary(allocator);
    benc_putString(out, &hi, benc_newString((char*) vcontext, allocator), allocator);
    return out;
}

static int testSerialization()
{
    char* context = "Hello World";

    struct DHTModule module = {
        .name = "TestModule",
        .context = context,
        .serialize = serialize
    };

    char buffer[256];
    struct MemAllocator* allocator = BufferAllocator_new(buffer, 256);
    char writeBuffer[64];
    memset(writeBuffer, 0, 64);
    struct Writer* writer = ArrayWriter_new(writeBuffer, 64, allocator);

    struct DHTModuleRegistry* reg = DHTModules_new(allocator);
    DHTModules_register(&module, reg);

    DHTModules_serialize(reg, writer);

    /* This is ok because the output is null padded at the end. */
    printf("The content is: %s\nand the length is: %d\n",
           writeBuffer,
           (int) writer->bytesWritten(writer));

    if (writer->bytesWritten(writer) != strlen(control)) {
        printf("Length mismatch! expected: %d", (int) strlen(control));
    }

    return memcmp(writeBuffer, control, writer->bytesWritten(writer));
}

static void deserialize(const Dict* serialData, void* vcontext)
{
    char* context = (char*) vcontext;
    String* out = benc_lookupString(serialData, &hi);
    memcpy(context, out->bytes, out->len);
}

int testDeserialization()
{
    char context[12];

    struct DHTModule module = {
        .name = "TestModule",
        .context = context,
        .deserialize = deserialize
    };

    char buffer[512];
    struct MemAllocator* allocator = BufferAllocator_new(buffer, 512);
    struct Reader* reader = ArrayReader_new(control, strlen(control), allocator);

    struct DHTModuleRegistry* reg = DHTModules_deserialize(reader, allocator);
    DHTModules_register(&module, reg);

    context[11] = '\0';

    printf("Deserialization output is: %s\n", context);

    return memcmp(context, "Hello World", 11);
}

int main()
{
    return
        testSerialization()
      | testDeserialization();
}
