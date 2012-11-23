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
#ifndef TUNConfigurator_H
#define TUNConfigurator_H

#include "exception/Except.h"
#include "util/log/Log.h"

/**
 * This is the maximum size that will be accepted as an interface name.
 * If this runs on a system where IFNAMSIZ is less then 16, names larger
 * than that will also be rejected.
 */
#define TUNConfigurator_IFNAMSIZ 16

/**
 * The error code for any function which fails to get a socket for configuring an interface.
 * Multiple functions may return this error.
 */
#define TUNConfigurator_ERROR_GETTING_ADMIN_SOCKET 1

/** If there is an error bringing the interface up. */
#define TUNConfigurator_ERROR_ENABLING_INTERFACE 2

/**
 * Open the TUN device.
 *
 * @param interfaceName the interface name you *want* to use or NULL to let the kernel decide.
 * @param assignedInterfaceName an empty buffer which will be filled in with the interface
 *                              name that is assigned.
 * @param log
 * @param eh if this function fails, it will raise one of the following.
 *           TUNConfigurator_configure_BAD_TUNNEL interfaceName was an invalid tun device name.
 *           TUNConfigurator_configure_INTERNAL Something went wrong internally.
 *           TUNConfigurator_initTun_PERMISSION The user running this process doesn't have
 *                                              permission to open a tun device.
 * @return an opaque pointer which represents the file descriptor for the newly configured tunnel.
 */
#define TUNConfigurator_initTun_INTERNAL -3
#define TUNConfigurator_initTun_BAD_TUNNEL -2
#define TUNConfigurator_initTun_PERMISSION -1
void* TUNConfigurator_initTun(const char* interfaceName,
                              char assignedInterfaceName[TUNConfigurator_IFNAMSIZ],
                              struct Log* logger,
                              struct Except* eh);

/**
 * Set an IPv6 address on an interface, bring the interface up, and configure a route.
 *
 * @param interfaceName the name of the interface to alter.
 * @param address the ip address to set.
 * @param prefixLen the number of bits netmask to include in the route.
 * @param logger
 * @param eh if this function fails, it will raise one of the following.
 *           TUNConfigurator_setIpAddress_INTERNAL Catch all exception code for failures.
 *           TUNConfigurator_ERROR_GETTING_ADMIN_SOCKET Error getting admin socket for interface.
 *           TUNConfigurator_ERROR_ENABLING_INTERFACE If the interface cannot be brought up.
 */
#define TUNConfigurator_setIpAddress_INTERNAL -1
void TUNConfigurator_setIpAddress(const char* interfaceName,
                                  const uint8_t address[16],
                                  int prefixLen,
                                  struct Log* logger,
                                  struct Except* eh);
#endif
