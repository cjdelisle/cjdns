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
#ifndef TAPWrapper_H
#define TAPWrapper_H

#include "interface/Iface.h"
#include "memory/Allocator.h"
#include "util/log/Log.h"
#include "wire/Ethernet.h"
#include "util/Linker.h"
Linker_require("interface/tuntap/TAPWrapper.c");

struct TAPWrapper
{
    struct Iface internal;

    /** This is the peer's MAC address (zero before initialization). */
    uint8_t peerAddress[Ethernet_ADDRLEN];
};

/**
 * This is the address which we will report to the peer as our address.
 * As per IEEE-802 it is non-multicast and non-locally-assigned, and it begins with fc :)
 */
#define TAPWrapper_LOCAL_MAC "\xfc\x00\x00\x00\x00\x00"

struct TAPWrapper* TAPWrapper_new(struct Iface* external,
                                  struct Log* log,
                                  struct Allocator* alloc);

#endif
