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
#ifndef AddrIface_H
#define AddrIface_H

#include "interface/Iface.h"
#include "util/platform/Sockaddr.h"
#include "exception/Except.h"
#include "wire/Message.h"

/**
 * An AddrInterface, short for "Adderssable Interface" is an interface which
 * sends and accepts an address as the header of the messages sent to and
 * recieved from it.
 */
typedef struct AddrIface
{
    /** As a generic interface. */
    struct Iface* iface;

    /**
     * The address of this node if applicable to the implementation.
     * addr->addrLen will always tell how many of the first bytes of each
     * message are reserved for the address.
     */
    struct Sockaddr* addr;

    struct Allocator* alloc;
} AddrIface_t;

static inline Er_DEFUN(void AddrIface_pushAddr(Message_t* msg, const struct Sockaddr* addr))
{
    Er(Er_fromErr(Message_epush(msg, addr, addr->addrLen)));
    Er_ret();
}

static inline Er_DEFUN(struct Sockaddr* AddrIface_popAddr(Message_t* msg))
{
    struct Sockaddr* out = (struct Sockaddr*) Message_bytes(msg);
    uint16_t len = 0;
    Er(Er_fromErr(Message_epop16h(&len, msg)));
    Er(Er_fromErr(Message_epop(msg, NULL, len - 2)));
    Er_ret(out);
}

#endif
