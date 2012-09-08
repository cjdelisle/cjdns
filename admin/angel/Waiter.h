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
#ifndef Waiter_H
#define Waiter_H

#include "exception/Except.h"

#include <event2/event.h>

/**
 * Wait for and read data from a file descriptor.
 * If the FD does not become ready to read within 2 seconds, an excaption is raised.
 * If the read returns negative, an exception is raised.
 *
 * @param output the buffer to read data into.
 * @param bufferSize the amount of space in the buffer.
 * @param fd the file descriptor to read from.
 * @param eventBase a libevent context.
 * @param eh an exception handler in case something goes wrong.
 *           Waiter_getData_TIMEOUT if the file descriptor does not become ready within 2 seconds.
 *           Waiter_getData_READ_FAILED if the read() call returns < 0.
 * @return the number of bytes read.
 */
#define Waiter_getData_TIMEOUT -1
#define Waiter_getData_READ_FAILED -2
uint32_t Waiter_getData(uint8_t* output,
                        uint32_t bufferSize,
                        int fd,
                        struct event_base* eventBase,
                        struct Except* eh);

#endif
