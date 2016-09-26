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
#include "benc/String.h"
#include "crypto/AddressCalc.h"
#include "dht/CJDHTConstants.h"
#include "dht/dhtcore/ReplySerializer.h"
#include "dht/dhtcore/VersionList.h"
#include "dht/Address.h"
#include "memory/Allocator.h"
#include "switch/LabelSplicer.h"

#define NumberCompress_OLD_CODE
#include "switch/NumberCompress.h"

#include "util/Identity.h"
#include "util/log/Log.h"
#include "util/Assert.h"

/**
 * For serializing and parsing responses to getPeers and search requests.
 */
struct Address_List* ReplySerializer_parse(struct Address* fromNode,
                                           Dict* result,
                                           struct Log* log,
                                           bool splicePath,
                                           struct Allocator* alloc)
{
    String* nodes = Dict_getString(result, CJDHTConstants_NODES);

    if (!nodes) { return NULL; }

    if (nodes->len == 0 || nodes->len % Address_SERIALIZED_SIZE != 0) {
        Log_debug(log, "Dropping unrecognized reply");
        return NULL;
    }

    struct VersionList* versions = NULL;
    String* versionsStr = Dict_getString(result, CJDHTConstants_NODE_PROTOCOLS);
    if (versionsStr) {
        versions = VersionList_parse(versionsStr, alloc);
    }
    if (!versions || versions->length != (nodes->len / Address_SERIALIZED_SIZE)) {
        Log_debug(log, "Reply with missing or invalid versions");
        return NULL;
    }

    struct Address_List* out = Address_List_new(versions->length, alloc);

    uint32_t j = 0;
    for (uint32_t i = 0; nodes && i < nodes->len; i += Address_SERIALIZED_SIZE) {

        struct Address addr = { .path = 0 };
        Address_parse(&addr, (uint8_t*) &nodes->bytes[i]);
        addr.protocolVersion = versions->versions[i / Address_SERIALIZED_SIZE];

        // calculate the ipv6
        Address_getPrefix(&addr);

        if (splicePath) {
            // We need to splice the given address on to the end of the
            // address of the node which gave it to us.
            uint64_t path = LabelSplicer_splice(addr.path, fromNode->path);

            if (path == UINT64_MAX) {
                /* common, lots of noise
                uint8_t discovered[60];
                uint8_t fromAddr[60];
                Address_print(discovered, &addr);
                Address_print(fromAddr, fromNode);
                Log_debug(log,
                          "Dropping response [%s] from [%s] because route could not be spliced",
                          discovered, fromAddr);*/
                continue;
            }

            addr.path = path;
        }

        /*#ifdef Log_DEBUG
            uint8_t printedAddr[60];
            Address_print(printedAddr, &addr);
            Log_debug(log, "discovered node [%s]", printedAddr);
        #endif*/

        Address_getPrefix(&addr);
        if (!AddressCalc_validAddress(addr.ip6.bytes)) {
            struct Allocator* tmpAlloc = Allocator_child(alloc);
            String* printed = Address_toString(&addr, tmpAlloc);
            uint8_t ipPrinted[40];
            Address_printIp(ipPrinted, &addr);
            Log_debug(log, "Was told garbage addr [%s] [%s]", printed->bytes, ipPrinted);
            Allocator_free(tmpAlloc);
            // This should never happen, badnode.
            continue;
        }

        Bits_memcpy(&out->elems[j++], &addr, sizeof(struct Address));
    }
    out->length = j;
    return out;
}

void ReplySerializer_serialize(struct Address_List* addrs,
                               Dict* out,
                               struct Address* convertDirectorFor,
                               struct Allocator* alloc)
{
    if (!addrs->length) { return; }
    String* nodes = String_newBinary(NULL, addrs->length * Address_SERIALIZED_SIZE, alloc);
    struct VersionList* versions = VersionList_new(addrs->length, alloc);
    for (int i = 0; i < addrs->length; i++) {
        versions->versions[i] = addrs->elems[i].protocolVersion;
        if (!convertDirectorFor) {
            Address_serialize(&nodes->bytes[i * Address_SERIALIZED_SIZE], &addrs->elems[i]);
        } else {
            struct Address addr;
            Bits_memcpy(&addr, &addrs->elems[i], sizeof(struct Address));
            addr.path = NumberCompress_getLabelFor(addr.path, convertDirectorFor->path);
            Address_serialize(&nodes->bytes[i * Address_SERIALIZED_SIZE], &addr);
        }
    }
    Dict_putStringC(out, "n", nodes, alloc);
    Dict_putStringC(out, "np", VersionList_stringify(versions, alloc), alloc);
}
