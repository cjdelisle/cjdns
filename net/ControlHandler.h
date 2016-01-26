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
#ifndef ControlHandler_H
#define ControlHandler_H

#include "memory/Allocator.h"
#include "util/log/Log.h"
#include "net/EventEmitter.h"
#include "util/Linker.h"
Linker_require("net/ControlHandler.c");

struct ControlHandler
{
    /** This interface expects and sends [ SwitchHeader ][ 0xffffffff ][ CTRL frame ] */
    struct Iface coreIf;

    /**
     * This interface expects and sends [ SwitchHeader ][ 0xffffffff ][ CTRL frame ]
     * May send a pong or an error caused by a ping.
     */
    struct Iface switchPingerIf;
};

struct ControlHandler* ControlHandler_new(struct Allocator* alloc,
                                          struct Log* logger,
                                          struct EventEmitter* ee,
                                          uint8_t myPublicKey[32]);

#endif
