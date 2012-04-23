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

/** Packet dropped because node has oversent it's limit. */
#define Error_LINK_LIMIT_EXCEEDED  3

/** Message too big to send. */
#define Error_OVERSIZE_MESSAGE     4

/** Message smaller than expected headers. */
#define Error_UNDERSIZE_MESSAGE    5

/** Authentication failed. */
#define Error_AUTHENTICATION       6

/** Header is invalid or checksum failed. */
#define Error_INVALID              7

/** Message could not be sent to it's destination through no fault of the sender. */
#define Error_UNDELIVERABLE        8


#endif
