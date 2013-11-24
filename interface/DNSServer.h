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
#ifndef DNSServer_H
#define DNSServer_H

#include "interface/addressable/AddrInterface.h"
#include "util/log/Log.h"
#include "interface/RainflyClient.h"
#include "util/platform/Sockaddr.h"
#include "util/Linker.h"
Linker_require("interface/DNSServer.c")

struct DNSServer
{
    int unused;
};

#define DNSServer_addServer_WRONG_ADDRESS_TYPE -1
int DNSServer_addServer(struct DNSServer* dns, struct Sockaddr* server);

struct DNSServer* DNSServer_new(struct AddrInterface* iface,
                                struct Log* logger,
                                struct RainflyClient* rainfly);

#endif
