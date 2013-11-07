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
#include "dht/Address.h"
#include "dht/CJDHTConstants.h"
#include "dht/DHTMessage.h"
#include "dht/DHTModule.h"
#include "dht/DHTModuleRegistry.h"
#include "benc/String.h"
#include "util/Identity.h"
#include "switch/NumberCompress.h"
#include "switch/EncodingScheme.h"

/**
 * The encoding scheme module tags each message with our encoding scheme
 * representation as well as the number of the smallest encoding form which
 * can represent the return path to the querying node.
 */

struct EncodingSchemeModule_pvt
{
    struct DHTModule module;

    /** Our scheme. */
    String* schemeDefinition;
    struct EncodingScheme* scheme;

    Identity
};

static inline struct EncodingSchemeModule_pvt* moduleForVContext(void* vcontext)
{
    return Identity_cast((struct EncodingSchemeModule_pvt*)
        (((char*)vcontext) - offsetof(struct EncodingSchemeModule_pvt, module.context)));
}

static int handleIncoming(struct DHTMessage* message, void* vcontext)
{
    return 0;
}

static int handleOutgoing(struct DHTMessage* dmesg, void* vcontext)
{
    if (dmesg->replyTo == NULL) {
        return 0;
    }

    struct EncodingSchemeModule_pvt* ctx = moduleForVContext(vcontext);

    // Send our encoding scheme definition
    Dict_putString(dmesg->asDict,
                   CJDHTConstants_ENC_SCHEME,
                   ctx->schemeDefinition,
                   dmesg->allocator);

    // And tell the asker which encoding form their interface uses:
    int totalBits = NumberCompress_bitsUsedForLabel(dmesg->address->path);
    int schemeNum = -1;
    for (int i =  0; i < ctx->scheme->count; i++) {
        if (totalBits == (ctx->scheme->forms[i].prefixLen + ctx->scheme->forms[i].bitCount)) {
            schemeNum = i;
            break;
        }
    }
    Assert_true(schemeNum > -1);
    Dict_putInt(dmesg->asDict, CJDHTConstants_ENC_INDEX, schemeNum, dmesg->allocator);

    return 0;
}

void EncodingSchemeModule_register(struct DHTModuleRegistry* reg, struct Allocator* alloc)
{
    struct EncodingScheme* scheme = NumberCompress_defineScheme(alloc);
    String* schemeDefinition = EncodingScheme_serialize(scheme, alloc);

    struct EncodingSchemeModule_pvt* ctx =
        Allocator_clone(alloc, (&(struct EncodingSchemeModule_pvt) {
            .module = {
                .name = "EncodingSchemeModule",
                .handleIncoming = handleIncoming,
                .handleOutgoing = handleOutgoing
            },
            .scheme = scheme,
            .schemeDefinition = schemeDefinition
        }));
    Identity_set(ctx);

    DHTModuleRegistry_register(&ctx->module, reg);
}
