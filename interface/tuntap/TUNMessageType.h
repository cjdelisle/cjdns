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
#ifndef TUNMessageType_H
#define TUNMessageType_H

#include "wire/Message.h"

static inline void TUNMessageType_push(struct Message* message,
                                       uint16_t ethertype,
                                       struct Except* eh)
{
    Message_shift(message, 4, eh);
    ((uint16_t*) message->bytes)[0] = 0;
    ((uint16_t*) message->bytes)[1] = ethertype;
}

static inline uint16_t TUNMessageType_pop(struct Message* message, struct Except* eh)
{
    Message_shift(message, -4, eh);
    return ((uint16_t*) message->bytes)[-1];
}

#endif
