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
#include "wire/Message.h"

static uint8_t transferMessage(struct Message* msg, struct Interface* iface)
{
    struct Interface* other = (struct Interface*) iface->receiverContext;
    return other->sendMessage(msg, other);
}

/**
 * Connect two interfaces together like a double female adapter.
 * Any traffic coming in on one will be forwarded to the other.
 *
 * @param a one interface.
 * @param b another interface.
 */
void InterfaceConnector_connect(struct Interface* a, struct Interface* b)
{
    a->receiveMessage = transferMessage;
    a->receiverContext = b;
    b->receiveMessage = transferMessage;
    b->receiverContext = a;
}
