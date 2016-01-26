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
#include "memory/Allocator.h"
#include "net/SwitchAdapter.h"
#include "util/Identity.h"
#include "wire/SwitchHeader.h"
#include "util/Linker.h"

struct SwitchAdapter_pvt
{
    struct SwitchAdapter pub;
    struct Log* log;
    Identity
};

struct Header {
     struct SwitchHeader sh;
     uint32_t handle_be;
};
#define Header_SIZE 16
Assert_compileTime(sizeof(struct Header) == Header_SIZE);

static Iface_DEFUN incomingFromControlIf(struct Message* msg, struct Iface* controlIf)
{
    struct SwitchAdapter_pvt* sa =
        Identity_containerOf(controlIf, struct SwitchAdapter_pvt, pub.controlIf);
    return Iface_next(&sa->pub.switchIf, msg);
}

static Iface_DEFUN incomingFromSessionManagerIf(struct Message* msg, struct Iface* sessionManagerIf)
{
    struct SwitchAdapter_pvt* sa =
        Identity_containerOf(sessionManagerIf, struct SwitchAdapter_pvt, pub.sessionManagerIf);
    return Iface_next(&sa->pub.switchIf, msg);
}

static Iface_DEFUN incomingFromSwitchIf(struct Message* msg, struct Iface* switchIf)
{
    struct SwitchAdapter_pvt* sa =
        Identity_containerOf(switchIf, struct SwitchAdapter_pvt, pub.switchIf);
    if (msg->length < Header_SIZE) {
        Log_debug(sa->log, "DROP runt");
        return 0;
    }
    Assert_true(!(((uintptr_t)msg->bytes) % 4) && "alignment");
    struct Header* hdr = (struct Header*) msg->bytes;

    // The label comes in reversed from the switch because the switch doesn't know that we aren't
    // another switch ready to parse more bits, bit reversing the label yields the source address.
    // (the field is still big endian!)
    hdr->sh.label_be = Bits_bitReverse64(hdr->sh.label_be);

    if (hdr->handle_be == 0xffffffff) {
        return Iface_next(&sa->pub.controlIf, msg);
    }
    return Iface_next(&sa->pub.sessionManagerIf, msg);
}

struct SwitchAdapter* SwitchAdapter_new(struct Allocator* allocator, struct Log* log)
{
    struct Allocator* alloc = Allocator_child(allocator);
    struct SwitchAdapter_pvt* out = Allocator_calloc(alloc, sizeof(struct SwitchAdapter_pvt), 1);
    out->pub.controlIf.send = incomingFromControlIf;
    out->pub.sessionManagerIf.send = incomingFromSessionManagerIf;
    out->pub.switchIf.send = incomingFromSwitchIf;
    out->log = log;
    Identity_set(out);
    return &out->pub;
}
