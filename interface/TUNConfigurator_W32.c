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
#include "interface/TUNConfigurator.h"
#include "interface/TUNInterface.h"

void* TUNConfigurator_initTun(const char* interfaceName,
                              char assignedInterfaceName[TUNConfigurator_IFNAMSIZ],
                              struct Log* logger,
                              struct Except* eh)
{
    return NULL;
}

void TUNConfigurator_addIp4Address(const char* interfaceName,
                                   const uint8_t address[4],
                                   int prefixLen,
                                   struct Log* logger,
                                   struct Except* eh)
{
    Except_raise(eh, TUNConfigurator_addIp4Address_INTERNAL, "unimplemented");
}

void TUNConfigurator_addIp6Address(const char* interfaceName,
                                   const uint8_t address[16],
                                   int prefixLen,
                                   struct Log* logger,
                                   struct Except* eh)
{
}

void TUNConfigurator_setMTU(const char* interfaceName,
                            uint32_t mtu,
                            struct Log* logger,
                            struct Except* eh)
{
}
