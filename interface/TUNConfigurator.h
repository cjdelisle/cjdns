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
#include "util/Log.h"

/**
 * Configure a TUNInterface.
 *
 * @param interface to configure.
 * @param address the ipv6 address in binary form.
 * @param prefixLen the number of bits in the network mask.
 * @param logger
 * @param eh if this function fails, it will raise one of the following.
 *           TUNConfigurator_configure_BAD_TUNNEL interfaceName was an invalid tun device name.
 *           TUNConfigurator_configure_MALFORMED_ADDRESS myIp was an invalid ipv6 address.
 *           TUNConfigurator_configure_INTERNAL Something went wrong internally.
 * @return an opaque pointer which represents the file descriptor for the newly configured tunnel.
 */
#define TUNConfigurator_configure_BAD_TUNNEL -1
#define TUNConfigurator_configure_MALFORMED_ADDRESS -2
#define TUNConfigurator_configure_INTERNAL -3
void* TUNConfigurator_configure(const char* interfaceName,
                                const uint8_t myIp[40],
                                int prefixLen,
                                struct Log* logger,
                                struct Except* eh);

#endif
