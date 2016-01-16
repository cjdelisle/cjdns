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
#ifndef TUNTools_H
#define TUNTools_H

#include "interface/addressable/AddrIface.h"
#include "memory/Allocator.h"
#include "util/log/Log.h"
#include "util/platform/Sockaddr.h"
#include "util/events/EventBase.h"
#include "util/Linker.h"
Linker_require("interface/tuntap/test/TUNTools.c");

struct TUNTools;

typedef Iface_DEFUN (* TUNTools_Callback)(struct Message* msg, struct TUNTools* tt);

Iface_DEFUN TUNTools_genericIP6Echo(struct Message* msg, struct TUNTools* tt);

const uint8_t* TUNTools_testIP6AddrA;
const uint8_t* TUNTools_testIP6AddrB;
const uint8_t* TUNTools_testIP6AddrC;

struct TUNTools
{
    struct Iface tunIface;
    struct Iface udpIface;
    struct Sockaddr* tunDestAddr;
    struct Sockaddr* udpBindTo;
    struct Allocator* alloc;
    struct Log* log;
    struct EventBase* base;
    TUNTools_Callback cb;
    int receivedMessageTUNCount;
};

void TUNTools_echoTest(struct Sockaddr* udpBindTo,
                       struct Sockaddr* tunDestAddr,
                       TUNTools_Callback tunMessageHandler,
                       struct Iface* tun,
                       struct EventBase* base,
                       struct Log* logger,
                       struct Allocator* alloc);

#endif
