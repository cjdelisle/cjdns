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
#include "interface/Interface.h"
#include "interface/TUNInterface.h"
#include "benc/String.h"
#include "util/Endian.h"

#include <string.h>
#include <event2/event.h>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <winbase.h>
#include <io.h>

// Windows maps interface names to NDIS names which are 256 bytes
#define IFNAMSIZ 256

#define EPERM WSAEPERM


// Defined extra large so large MTU can be taken advantage of later.
#define MAX_PACKET_SIZE 8192
#define PADDING_SPACE (10240 - MAX_PACKET_SIZE)

// The number of bytes at the beginning of the message which is used
// to contain the type of packet.
#define PACKET_INFO_SIZE 4


static int openTunnel(const char* interfaceName, char assignedInterfaceName[IFNAMSIZ])
{
    return 0;
}

static void closeInterface(void* vcontext)
{
}

static void handleEvent(evutil_socket_t socket, short eventType, void* vcontext)
{
}

static uint8_t sendMessage(struct Message* message, struct Interface* iface)
{
    return 0;
}

struct TUNInterface* TUNInterface_new(void* handle,
                                      struct event_base* base,
                                      struct Allocator* allocator)
{
    return NULL;
}

struct Interface* TUNInterface_asGeneric(struct TUNInterface* tunIf)
{
    return (tunIf) ? &tunIf->iface : NULL;
}
