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
#include "dht/dhtcore/VersionList.h"
#include "memory/Allocator.h"
#include "io/Reader.h"
#include "io/Writer.h"
#include "io/ArrayWriter.h"
#include "io/ArrayReader.h"
#include "util/Endian.h"
#include <stdio.h>

struct VersionList* VersionList_parse(String* str, struct Allocator* alloc)
{
    const uint8_t numberSize = str->bytes[0];
    if (str->len < 1 || numberSize == 0 || numberSize > 4) {
        return NULL;
    }

    uint32_t length = (str->len - 1) / numberSize;

    if ((length * numberSize) != (str->len - 1)) {
        return NULL;
    }

    struct VersionList* list = VersionList_new(length, alloc);
    struct Reader* r = ArrayReader_new(str->bytes + 1, str->len - 1, alloc);
    for (int i = 0; i < (int)list->length; i++) {
        uint32_t ver = 0;
        Reader_read(r, (uint8_t*) &ver, numberSize);
        ver = Endian_bigEndianToHost32(ver);
        list->versions[i] = ver >> ((4-numberSize) * 8);
    }
    return list;
}

String* VersionList_stringify(struct VersionList* list, struct Allocator* alloc)
{
    uint8_t numberSize = 1;
    uint32_t max = 0xff;
    for (int i = 0; i < (int)list->length; i++) {
        while (list->versions[i] >= max) {
            numberSize++;
            max = max << 8 | 0xff;
        }
    }

    String* out = String_newBinary(NULL, (numberSize * list->length + 1), alloc);

    struct Writer* w = ArrayWriter_new(out->bytes, out->len, alloc);
    Writer_write(w, &numberSize, 1);

    for (int i = 0; i < (int)list->length; i++) {
        uint32_t ver = list->versions[i] << ((4-numberSize) * 8);
        ver = Endian_hostToBigEndian32(ver);
        Writer_write(w, (uint8_t*) &ver, numberSize);
    }
    Writer_write(w, &numberSize, 1);

    return out;
}

struct VersionList* VersionList_new(uint32_t length, struct Allocator* alloc)
{
    struct VersionList* out = Allocator_clone(alloc, (&(struct VersionList) {
        .length = length,
        .alloc = alloc
    }));
    out->versions = Allocator_calloc(alloc, 4, length);
    return out;
}
