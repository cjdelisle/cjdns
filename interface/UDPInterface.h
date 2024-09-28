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
#ifndef UDPInterface_H
#define UDPInterface_H

#include "exception/Err.h"
#include "interface/addressable/AddrIface.h"
#include "util/events/EventBase.h"
#include "util/Assert.h"
#include "util/log/Log.h"
#include "util/GlobalConfig.h"
#include "memory/Allocator.h"
#include "util/Linker.h"
Linker_require("interface/UDPInterface.c")

struct UDPInterface_BroadcastHeader
{
    /* Magic (0xfffffffc) */
    uint32_t fffffffc_be;

    /** UDPInterface_CURRENT_VERSION, no communication is possible with different versions. */
    uint8_t version;

    /** padding and for future use. */
    uint8_t zero;

    /** Port used for data communication. */
    uint16_t commPort_be;
};
#define UDPInterface_BroadcastHeader_SIZE 8
Assert_compileTime(
    sizeof(struct UDPInterface_BroadcastHeader) == UDPInterface_BroadcastHeader_SIZE
);

#define UDPInterface_CURRENT_VERSION 0

struct UDPInterface
{
    AddrIface_t generic;
};

/**
 * Create a new UDPInterface
 *
 * @param eventBase the event context
 * @param bindAddr the address and port to bind the socket to
 * @param bcastPort (optional) if specifed, another socket will be created for beacon messages
 *                  if zero then no other socket will be created.
 * @param alloc allocator which will be used to create the interface
 * @param logger
 * @param globalConf for getting the name of the TUN device to avoid bcasting to it
 */
Err_DEFUN UDPInterface_new(
    struct UDPInterface** out,
    EventBase_t* eventBase,
    struct Sockaddr* bindAddr,
    uint16_t beaconPort,
    struct Allocator* alloc,
    struct Log* logger,
    struct GlobalConfig* globalConf);

/**
 * List all devices which can be broadcasted to, this will provide the name of the devices.
 *
 * @param alloc
 */
Err_DEFUN UDPInterface_listDevices(List** out, struct Allocator* alloc);

/**
 * Specify broadcast devices, this function accepts device names, address names and
 * the pseudo-name "all" which means it will use broadcast addresses of all interfaces.
 * To broacdast to a different network, you can simply specify the broadcast address
 * as if it were a device.
 * For example: [ "eth0", "wlan0", "192.168.300.255" ]
 *
 * @param udpif
 * @param devices the list of devices to assign
 */
void UDPInterface_setBroadcastDevices(struct UDPInterface* udpif, List* devices);

/**
 * Get the list of broadcast devices which is set using UDPInterface_setBroadcastDevices().
 * This will return exactly the same list that was set, for the broadcast addresses which
 * were computed to send to, use UDPInterface_getBroadcastAddrs().
 */
List* UDPInterface_getBroadcastDevices(struct UDPInterface* udpif, struct Allocator* alloc);

/**
 * Get the list of broadcast addresses which will be used for beconing, this is computed
 * from the list specified by UDPInterface_setBroadcastDevices().
 */
List* UDPInterface_getBroadcastAddrs(struct UDPInterface* udpif, struct Allocator* alloc);

/**
 * Configure the underlying UDP socket(s) to set DSCP on the traffic they send so that a
 * firewall can recognize them and treat them accordingly. This will set DSCP on both the
 * data socket and the beacon socket.
 */
int UDPInterface_setDSCP(struct UDPInterface* udpif, uint8_t dscp);

int UDPInterface_getFd(struct UDPInterface* udpif);

Err_DEFUN UDPInterface_workerStates(
    Object_t** out,
    struct UDPInterface* udpif,
    Allocator_t* alloc);

#endif