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
#ifndef Error_H
#define Error_H

/** No error, everything is ok. */
#define Error_NONE                 0

/** The switch label was malformed. */
#define Error_MALFORMED_ADDRESS    1

/** Packet dropped because link is congested. */
#define Error_FLOOD                2

/** Packet dropped because node has oversent its limit. */
#define Error_LINK_LIMIT_EXCEEDED  3

/** Message too big to send. */
#define Error_OVERSIZE_MESSAGE     4

/** Message smaller than expected headers. */
#define Error_UNDERSIZE_MESSAGE    5

/** Authentication failed. */
#define Error_AUTHENTICATION       6

/** Header is invalid or checksum failed. */
#define Error_INVALID              7

/** Message could not be sent to its destination through no fault of the sender. */
#define Error_UNDELIVERABLE        8

/** The route enters and leaves through the same interface in one switch. */
#define Error_LOOP_ROUTE           9

/** The switch is unable to represent the return path. */
#define Error_RETURN_PATH_INVALID 10

static inline char* Error_strerror(int err)
{
    switch (err) {
        case Error_NONE:                return "Error_NONE";
        case Error_MALFORMED_ADDRESS:   return "Error_MALFORMED_ADDRESS";
        case Error_FLOOD:               return "Error_FLOOD";
        case Error_LINK_LIMIT_EXCEEDED: return "Error_LINK_LIMIT_EXCEEDED";
        case Error_OVERSIZE_MESSAGE:    return "Error_OVERSIZE_MESSAGE";
        case Error_UNDERSIZE_MESSAGE:   return "Error_UNDERSIZE_MESSAGE";
        case Error_AUTHENTICATION:      return "Error_AUTHENTICATION";
        case Error_INVALID:             return "Error_INVALID";
        case Error_UNDELIVERABLE:       return "Error_UNDELIVERABLE";
        case Error_LOOP_ROUTE:          return "Error_LOOP_ROUTE";
        case Error_RETURN_PATH_INVALID: return "Error_RETURN_PATH_INVALID";
        default: return "UNKNOWN";
    }
}


#endif
