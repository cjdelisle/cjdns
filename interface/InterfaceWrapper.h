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
#ifndef InterfaceWrapper_H
#define InterfaceWrapper_H

#include "interface/Interface.h"
#include "memory/Allocator.h"

static void InterfaceWrapper_wrap(struct Interface* toWrap,
                                  Interface_CALLBACK(sendMessage),
                                  Interface_CALLBACK(receiveMessage),
                                  struct Interface* output)
{
    toWrap->receiveMessage = receiveMessage;
    toWrap->receiverContext = output;

    Bits_memcpyConst(output, (&(struct Interface) {
        .sendMessage = sendMessage,
    }), sizeof(struct Interface));
}


#endif
