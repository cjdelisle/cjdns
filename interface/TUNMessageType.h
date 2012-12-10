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

#include <stdint.h>

#if defined(BSD) || defined(OSX)
    #define TUNMessageType_push TUNMessageType_push_BSD
    #define TUNMessageType_pop TUNMessageType_pop_BSD
#elif defined(Illumos)
    #define TUNMessageType_push TUNMessageType_push_Illumos
    #define TUNMessageType_pop TUNMessageType_pop_Illumos
#elif defined(Linux) || defined(WIN32)
    // We'll assume that when the WIN32 implementation is written, it works like linux.
    #define TUNMessageType_push TUNMessageType_push_Linux
    #define TUNMessageType_pop TUNMessageType_pop_Linux
#else
    #error No TUNMessageType_push() and TUNMessageType_pop() implementations for your system.
#endif

void TUNMessageType_push(struct Message* message, uint16_t ethertype);

uint16_t TUNMessageType_pop(struct Message* message);

#endif
