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

#include "interface/TUNInterface.h"

/**
 * Configure a TUNInterface.
 *
 * @param interface to configure.
 * @param address the ipv6 address in binary form.
 * @param prefixLen the number of bits in the network mask.
 * @return 0 if successful, non-zero otherwise.
 */
int TUNConfigurator_configure(struct TUNInterface* interface,
                              uint8_t address[16],
                              int prefixLen);

#endif
