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
#include "benc/Dict.h"
#include "benc/serialization/BencSerializer.h"
#include "benc/serialization/json/JsonBencSerializer.h"
#include "benc/serialization/standard/StandardBencSerializer.h"
#include "memory/MallocAllocator.h"
#include "io/Reader.h"
#include "io/FileReader.h"
#include "io/Writer.h"
#include "io/FileWriter.h"
#include "util/platform/libc/string.h"

#include <unistd.h>

int main(int argc, char** argv)
{
    if (isatty(STDIN_FILENO)) {
        printf("Usage: %s < input.benc > output.json\n", argv[0]);
        printf("Or:    %s -r < input.json > output.benc\n", argv[0]);
        exit(0);
    }

    struct Allocator* allocator = MallocAllocator_new(1<<20);

    const struct BencSerializer* parser = StandardBencSerializer_get();
    const struct BencSerializer* serializer = JsonBencSerializer_get();

    if (argc > 1 && !strcmp(argv[0], "-r")) {
        const struct BencSerializer* p = parser;
        parser = serializer;
        serializer = p;
    }

    struct Reader* stdinReader = FileReader_new(stdin, allocator);
    Dict config;
    if (parser->parseDictionary(stdinReader, allocator, &config)) {
        fprintf(stderr, "Failed to parse configuration.\n");
        return -1;
    }

    struct Writer* stdoutWriter = FileWriter_new(stdout, allocator);
    serializer->serializeDictionary(stdoutWriter, &config);

    printf("\n");

    Allocator_free(allocator);
}
