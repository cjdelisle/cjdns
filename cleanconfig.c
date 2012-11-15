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
#include "benc/Dict.h"
#include "benc/serialization/BencSerializer.h"
#include "benc/serialization/json/JsonBencSerializer.h"
#include "memory/MallocAllocator.h"
#include "io/Reader.h"
#include "io/FileReader.h"
#include "io/Writer.h"
#include "io/FileWriter.h"

#include <unistd.h>

int main(int argc, char** argv)
{
    if (isatty(STDIN_FILENO)) {
        printf("Usage: %s < cjdroute.conf > compliant.json\n", argv[0]);
        printf("Cjdns accepts configuration which is not valid json but only outputs json\n"
               "which is valid. This tool deserialies and reserialized a conf file or other "
               "json file.\n");
        printf("In honer of thefinn93, thanks for all of your hard work helping people.\n");
        exit(0);
    }

    struct Allocator* allocator = MallocAllocator_new(1<<20);

    struct Reader* stdinReader = FileReader_new(stdin, allocator);
    Dict config;
    if (JsonBencSerializer_get()->parseDictionary(stdinReader, allocator, &config)) {
        fprintf(stderr, "Failed to parse configuration.\n");
        return -1;
    }

    struct Writer* stdoutWriter = FileWriter_new(stdout, allocator);
    JsonBencSerializer_get()->serializeDictionary(stdoutWriter, &config);

    printf("\n");

    Allocator_free(allocator);
}
