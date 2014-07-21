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
#include "interface/Aligner.h"
#include "interface/InterfaceWrapper.h"
#include "util/Bits.h"
#include "util/Identity.h"
#include "wire/Message.h"

struct Aligner_pvt
{
    struct Aligner pub;
    struct Interface* wrapped;
    uint32_t alignmentBytes;
    Identity
};

static int isAligned(uint8_t* pointer, uint32_t alignmentBytes)
{
    return !( ((uintptr_t)(pointer)) % alignmentBytes );
}

static void alignMessage(struct Message* msg, uint32_t alignmentBytes)
{
    if (isAligned(msg->bytes, alignmentBytes)) { return; }
    uint8_t* bytes = msg->bytes;
    int length = msg->length;
    do {
        Message_push8(msg, 0, NULL);
    } while (!isAligned(msg->bytes, alignmentBytes));
    Bits_memmove(msg->bytes, bytes, length);
    msg->length = length;
}

static uint8_t receiveMessage(struct Message* msg, struct Interface* iface)
{
    struct Aligner_pvt* al = Identity_check((struct Aligner_pvt*)iface->receiverContext);
    alignMessage(msg, al->alignmentBytes);
    Interface_receiveMessage(&al->pub.generic, msg);
    return 0;
}

static uint8_t sendMessage(struct Message* msg, struct Interface* iface)
{
    struct Aligner_pvt* al = Identity_check((struct Aligner_pvt*)iface);
    alignMessage(msg, al->alignmentBytes);
    return Interface_sendMessage(al->wrapped, msg);
}

struct Aligner* Aligner_new(struct Interface* external,
                            struct Allocator* alloc,
                            uint32_t alignmentBytes)
{
    Assert_true(Bits_popCountx32(alignmentBytes) == 1 && "alignmentBytes must be a power of 2");
    struct Aligner_pvt* out = Allocator_clone(alloc, (&(struct Aligner_pvt) {
        .wrapped = external,
        .alignmentBytes = alignmentBytes
    }));
    Identity_set(out);
    InterfaceWrapper_wrap(external, sendMessage, receiveMessage, &out->pub.generic);
    return &out->pub;
}
