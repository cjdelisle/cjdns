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
#ifndef MultiInterface_H
#define MultiInterface_H

#include "interface/Interface.h"
#include "interface/InterfaceController.h"
#include "util/log/Log.h"
#include "util/Linker.h"
Linker_require("interface/MultiInterface.c")

/*
 * An Interface such as Ethernet or UDP which may connect to multiple peers
 * through the same socket, Interfaces are MultiInterface must send and expect
 * a configurable length key to be sent in front of the message, this key
 * indicates which peer the message is from/to.
 */

struct MultiInterface
{
    /** The interface which is wrapped by this MultiInterface. */
    struct Interface* const iface;

    /**
     * The number of bytes which this interface will send
     * ahead of the message to signal it's origin.
     */
    const int keySize;
};

struct Interface* MultiInterface_ifaceForKey(struct MultiInterface* mIface, void* key);

struct MultiInterface* MultiInterface_new(int keySize,
                                          struct Interface* external,
                                          struct InterfaceController* ic,
                                          struct Log* logger);
#endif
