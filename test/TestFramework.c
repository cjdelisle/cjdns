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
#include "crypto/CryptoAuth.h"
#include "dht/ReplyModule.h"
#include "dht/SerializationModule.h"
#include "io/Writer.h"
#include "io/FileWriter.h"
#include "util/Log.h"
#include "memory/MallocAllocator.h"
#include "memory/Allocator.h"
#include "switch/SwitchCore.h"
#include "test/TestFramework.h"

struct Ducttape* TestFramework_setUp()
{
    uint8_t* privateKey = (uint8_t*) "0123456789abcdefghijklmnopqrstuv";
    uint8_t* publicKey = (uint8_t*)
        "\x3f\x5b\x96\x62\x11\x11\xd8\x9c\x7d\x3f\x51\x71\x68\x78\xfa\xb4"
        "\xc3\xcf\xd9\x7e\x32\x04\x12\xb4\xaf\x7e\x22\x92\xa5\xdf\x31\x71";

    struct event_base* base = event_base_new();

    // Allow it to allocate 4MB
    struct Allocator* allocator = MallocAllocator_new(1<<22);

    struct Writer* logwriter = FileWriter_new(stdout, allocator);
    struct Log* logger = allocator->clone(sizeof(struct Log), allocator, &(struct Log) {
        .writer = logwriter
    });

    struct SwitchCore* switchCore = SwitchCore_new(logger, allocator);
    //struct CryptoAuth* ca = CryptoAuth_new(NULL, allocator, privateKey, base, logger);

    struct DHTModuleRegistry* registry = DHTModuleRegistry_new(allocator);
    ReplyModule_register(registry, allocator);

    struct RouterModule* routerModule =
        RouterModule_register(registry, allocator, publicKey, base, logger, NULL);

    SerializationModule_register(registry, allocator);

    return Ducttape_register(privateKey, registry, routerModule,
                             switchCore, base, allocator, logger, NULL);
}
