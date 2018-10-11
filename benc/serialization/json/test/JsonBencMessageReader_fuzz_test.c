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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "benc/serialization/json/JsonBencMessageReader.h"
#include "crypto/random/Random.h"
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "util/Bits.h"
#include "util/Identity.h"
#include "util/log/FileWriterLog.h"
#include "util/CString.h"
#include "util/events/EventBase.h"
#include "crypto/random/test/DeterminentRandomSeed.h"
#include "test/FuzzTest.h"
#include "util/Js.h"

Js({ file.lib = require('benc/serialization/json/test/JsonBencMessageReader_fuzzGen.js'); })

struct FuzzTest* CJDNS_FUZZ_MK(struct Allocator* alloc)
{
    int len = 0;
    Js({ return 'len=' + file.lib.defaultConfLen(); });
    struct Message* msg = Message_new(len, 0, alloc);
    uint8_t* x = msg->bytes;
    Js({ return file.lib.defaultConf("x"); });
    struct FuzzTest* out = Allocator_calloc(alloc, sizeof(struct FuzzTest), 1);
    out->name = "JsonBencMessageReader_default";
    out->fuzz = msg;
    return out;
}

void CJDNS_FUZZ_MAIN(void* vctx, struct Message* fuzz)
{
    struct Allocator* alloc = (struct Allocator*) vctx;
    Dict* out = NULL;
    char* res = JsonBencMessageReader_readNoExcept(fuzz, alloc, &out, true);
    if (res) { return; }
    res = JsonBencMessageReader_readNoExcept(fuzz, alloc, &out, false);
}

void* CJDNS_FUZZ_INIT(struct Allocator* alloc, struct Random* rand)
{
    return alloc;
}