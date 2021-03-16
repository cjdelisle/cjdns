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
#ifndef TUNMessageType_H
#define TUNMessageType_H

#include "util/Defined.h"
#include "wire/Message.h"

static inline Er_DEFUN(void TUNMessageType_push(struct Message* message,
                                       uint16_t ethertype))
{
    Er(Message_eshift(message, 4));
    ((uint16_t*) message->msgbytes)[0] = 0;
    ((uint16_t*) message->msgbytes)[1] = ethertype;
    Er_ret();
}

static inline Er_DEFUN(uint16_t TUNMessageType_pop(struct Message* message))
{
    Er(Message_eshift(message, -4));
    Er_ret( ((uint16_t*) message->msgbytes)[-1] );
}

enum TUNMessageType {
    // Ethertype header, used by linux
    TUNMessageType_ETHERTYPE = 0,

    // No header, used by android and sunos, ipv4 and ipv6 only
    TUNMessageType_NONE  = 1,

    // address family header, used by BSD
    TUNMessageType_AF  = 2,
};

static inline enum TUNMessageType TUNMessageType_guess()
{
    if (Defined(android) || Defined(sunos)) {
        return TUNMessageType_NONE;
    } else if (Defined(linux) || Defined(win32)) {
        return TUNMessageType_ETHERTYPE;
    } else {
        return TUNMessageType_AF;
    }
}

#endif
