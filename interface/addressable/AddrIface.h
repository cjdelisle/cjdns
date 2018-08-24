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

struct AddrIface_Header
{
    // Caviat emptor: we only guarantee 4 byte alignment in Message structures
    uint32_t recvTime_high;
    uint32_t recvTime_low;

    struct Sockaddr_storage addr;
};
#define AddrIface_Header_SIZE (Sockaddr_OVERHEAD + Sockaddr_MAXSIZE + 8)
Assert_compileTime(AddrIface_Header_SIZE == sizeof(struct AddrIface_Header));

/**
 * An AddrInterface, short for "Adderssable Interface" is an interface which
 * sends and accepts an address as the header of the messages sent to and
 * recieved from it.
 */
struct AddrIface
{
    /** As a generic interface. */
    struct Iface iface;

    /**
     * The address of this node if applicable to the implementation.
     * addr->addrLen will always tell how many of the first bytes of each
     * message are reserved for the address.
     */
    struct Sockaddr* addr;

    struct Allocator* alloc;
};

#endif
