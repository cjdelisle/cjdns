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
#ifndef NDPServer_H
#define NDPServer_H

#include "interface/Iface.h"
#include "memory/Allocator.h"
#include "util/log/Log.h"
#include "wire/Ethernet.h"
#include "util/Linker.h"
Linker_require("interface/tuntap/NDPServer.c");

struct NDPServer
{
    struct Iface internal;
};

struct NDPServer* NDPServer_new(struct Iface* external,
                                struct Log* log,
                                uint8_t localMac[Ethernet_ADDRLEN],
                                struct Allocator* alloc);

#endif
