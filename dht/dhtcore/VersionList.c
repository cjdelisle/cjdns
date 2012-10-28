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
        r->read((uint8_t*) &ver, numberSize, r);
        list->versions[i] = Endian_bigEndianToHost32(ver);
    }
    return list;
}

String* VersionList_stringify(struct VersionList* list, struct Allocator* alloc)
{
    uint8_t numberSize = 0;
    uint32_t max = 0;
    for (int i = 0; i < (int)list->length; i++) {
        if (list->versions[i] >= max) {
            numberSize++;
            max = UINT32_MAX >> (32 - (numberSize * 8));
        }
    }

    String* out = String_newBinary(NULL, (numberSize * list->length + 1), alloc);
    out->bytes[0] = numberSize;

    struct Writer* w = ArrayWriter_new(out->bytes + 1, out->len - 1, alloc);
    for (int i = 0; i < (int)list->length; i++) {
        uint32_t ver = Endian_hostToBigEndian32(list->versions[i]);
        w->write((uint8_t*) &ver, numberSize, w);
    }

    return out;
}

struct VersionList* VersionList_new(uint32_t length, struct Allocator* alloc)
{
    struct VersionList* out = alloc->clone(sizeof(struct VersionList), alloc,
        &(struct VersionList) {
            .length = length,
            .alloc = alloc
        });
    out->versions = alloc->calloc(4, length, alloc);
    return out;
}
