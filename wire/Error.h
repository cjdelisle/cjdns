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
#ifndef Error_H
#define Error_H

#include "rust/cjdns_sys/Rffi.h"
#include "wire/Message.h"

enum Error_e {
    /** No error, everything is ok. */
    Error_NONE =                0,

    /** The switch label was malformed. */
    Error_MALFORMED_ADDRESS =   1,

    /** Packet dropped because link is congested. */
    Error_FLOOD =               2,

    /** Packet dropped because node has oversent its limit. */
    Error_LINK_LIMIT_EXCEEDED = 3,

    /** Message too big to send. */
    Error_OVERSIZE_MESSAGE =    4,

    /** Message smaller than expected headers. */
    Error_RUNT =                5,

    /** Authentication failed. */
    Error_AUTHENTICATION =      6,

    /** Header is invalid or checksum failed. */
    Error_INVALID =             7,

    /** Message could not be sent to its destination through no fault of the sender. */
    Error_UNDELIVERABLE =       8,

    /** The route enters and leaves through the same interface in one switch. */
    Error_LOOP_ROUTE =          9,

    /** The switch is unable to represent the return path. */
    Error_RETURN_PATH_INVALID = 10,

    /** Not invalid, but not something the code is able to handle. */
    Error_UNHANDLED =           11,

    /** Too many messages, cannot handle. */
    Error_OVERFLOW =            12,

    /** Something went wrong, it should not have happened. */
    Error_INTERNAL =            13,
};

#define Error(m, ...) \
    Rffi_error_fl( \
        String_printf(Message_getAlloc(m), __VA_ARGS__)->bytes, \
        Gcc_SHORT_FILE, \
        Gcc_LINE, \
        Message_getAlloc(m))

#endif
