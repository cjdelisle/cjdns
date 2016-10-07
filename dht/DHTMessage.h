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
#ifndef DHTMessage_H
#define DHTMessage_H

#ifdef SUBNODE
    #error "this file should not be included in subnode"
#endif

#include "benc/Dict.h"

/**
 * Maximum number of bytes in a message.
 * Ethernet MTU is 1500 so it's hard to imagine much more.
 */
#define DHTMessage_MAX_SIZE 1536


/**
 * This struct represents a DHT message which will be passed to the
 * modules. The only part of the message which will be available to
 * all modules is Message.peer. Incoming modules will have Message.bytes
 * and Message.length when they come from the network module.
 */
struct DHTMessage;
struct DHTMessage
{
    struct Address* address;

    struct Message* binMessage;

    /** The message as a bencoded dictionary. */
    Dict* asDict;

    struct EncodingScheme* encodingScheme;
    int encIndex;

    int pleaseRespond;

    /**
     * If this message is an outgoing reply, replyTo is the original query.
     * For incoming replies or any queries, it is NULL.
     */
    struct DHTMessage* replyTo;

    /** A memory allocator which will be freed after this message is sent. */
    struct Allocator* allocator;
};

#endif
