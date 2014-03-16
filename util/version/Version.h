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
#ifndef Version_H
#define Version_H

#include "util/Linker.h"
Linker_require("util/version/Version.c")

#include <stdint.h>

/*
 * Cjdns Protocol Versions
 *
 * The first argument to Version_COMPAT is the new version, the second argument is a list of
 * older versions with which it is compatible. All versions are obviously assumed to be compatible
 * with themselves and any version which is compatible with the most recent version to date is
 * assumed to be compatible with all future versions.
 */
#ifndef Version_COMPAT
    // defined otherwise in Version.c
    #define Version_COMPAT(one, twoRange)
#endif

/*
 * Version 0:
 * January 2012
 *
 * First version.
 */
Version_COMPAT(0, ([]))

/*
 * Version 1:
 * October 2012
 *
 * When you send someone a message through cjdns, it's encrypted.
 * When you send the first message to a router, it has a nice header on it which tells them your key
 * and allows them to establish a cryptographic session with you. For every message after that, they
 * need to remember that session and use it to decrypt the message.
 *
 * The way they remember which session a message is associated with is to look at the switching
 * label from that message and compare that with the label which was used when the first message was
 * sent (with the header).
 *
 * What I didn't think about at the time is that labels change. Nodes find a better path to a
 * destination and expect the same session to work. It would work if the other end knew which
 * session to use but it can't know, the label is different.
 *
 * This is a protocol bug.
 *
 * In my opinion, the best way to fix it is to send an additional header before the crypto nonce
 * which tells the foreign node who it came from. When the handshake message is sent, the node will
 * send a 4 byte integer which the other end will store. Every time the other end sends a
 * non-handshake message to this end, it will prepend that same integer to the encrypted message and
 * on recieving a message, a node will use that number to do a lookup of the correct session to use.
 * The number can be a pointer or index offset so this can be quite fast.
 * Integer 0xFFFFFFFF shall be reserved and handshake messages which contain this value must be
 * ignored or treated as protocol 0 messages.
 *
 * But this is a protocol break.
 *
 * If a node gets handed a number and doesn't know what to do with it, it will think it's a
 * CryptoAuth header and it will fail. New nodes would be able to try a message as the old form if
 * the new form doesn't work but old nodes will just fail if they are ever sent a message in the new
 * form.
 *
 * Proposed Solution:
 * A key will be sent in all findNodes responses in the router, this key will be "np" for nodes'
 * protocol (version).
 * It will contain a string representation of a list of the protocol version numbers for the nodes
 * which it is introducing. The numbers will all be the same number of bytes and the first byte will
 * give that number. The length of the string will always be equal to one, plus the number of nodes
 * times the value of the first byte.
 *
 * This example shows the p which would accompany a findNodes response containing 1 node with
 * protocol version zero and one node with protocol version 1.
 *
 * "np": "\x01\x00\x01"
 *
 * 2:np3:\x01\x00\x01
 *
 * This example shows the p which would accompany a findNodes response containing 1 node with
 * protocol version 300 and two nodes with protocol version 5.
 *
 * "np": "\x02\x01\x2c\x00\x05\x00\x05"
 *
 * 2:np7:\x02\x01\x2c\x00\x05\x00\x05
 *
 * All multi-byte numbers shall be encoded in big endian encoding.
 *
 * Each node will have an internal compatibility matrix giving protocol version numbers which
 * can communicate, a node shall not respond to a findNodes message with a response containing any
 * nodes which are known to be incompatible with the protocol version of the requesting node.
 * Versions which are beyond the highest version number in the compatability matrix will be assumed
 * to have the same compatability as the highest number in the table.
 *
 * All messages shall contain shall contain an "p" key as well but these will have a benc integer
 * representing the protocol version of the sending node.
 *
 * Change to the Ping switch control message:
 * New switch ping messages will all be 8 or more bytes long, they will begin with a magic field
 * and then a version number which is the version of the sending node.
 * The magic will be set to 0x09f91102 for all ping messages and to 0x9d74e35b in the response
 * messages (these numbers shall be big endian encoded). Following this number will be a 4 byte
 * field containing the protocol version. Nodes which only speak protocol version 0 will be
 * identifyable because they echo back 0x09f91102 rather than replacing it and they will be unlikely
 * to send a ping request whose content begins with 0x09f91102.
 *
 * Protocol1.1
 * In protocol0 there was a single pool of sessions shared between the outer and inner layer.
 * In protocol1 it was split because one pool needed to have handles and the other pool didn't.
 * The problem with this is communications do not necessarily travel back and forth along the
 * same path and protocol1 exhibited a pathology wherein one node was direct sending packets to
 * another while the other was routing the responses via an intermediary.
 * There were 2 CryptoAuth sessions between the two nodes and neither session was entering run
 * state. So protocol1 was broken and the new protocol1 (protocol1.1) sends session handles on top
 * of the CryptoAuth handshake headers even if the handshake is in the inner layer (under the Ipv6
 * header). It does not however send handles in the inner layer when the inner layer CryptoAuth
 * session is in HANDSHAKE3 state or above.
 * Protocol1.1 still identifies itself as Protocol1, it will not be able to communicate with
 * protocol0 in some circumstances. If it knows nothing about the other node and it forwards a
 * message via an intermediary, the message will be unreadable at the other end.
 *
 * Protocol1.2
 * Changes in protocol1.1 are reverted as a solution was found in the implementation.
 */
Version_COMPAT(1, ([0]))

/*
 * Version 2:
 * February 21, 2013
 *
 * Remove compatibility layer for communicating with version 0 nodes.
 */
Version_COMPAT(2, ([1]))

/*
 * Version 3:
 * August 16, 2013
 *
 * In version 1, handles were introduced so that a session could be looked when a packet came in.
 * During the initiation of a session, the node's handle was placed before the CryptoAuth block
 * unless the message was a layer-3 (forwarded) message in which case it was at the beginning
 * inside of the innermost CryptoAuth block. This handle was transferred to the peer so they could
 * tell how *this* node identifies the session.
 *
 * When a layer-2 (non-forwarded) message was received which was *not* a session initiation packet,
 * the peer's handle was placed before the CryptoAuth block in order for the peer to be able to
 * lookup the session in their table.
 *
 * Unfortunately the handle outside of the CryptoAuth initiation block was not authenticated in any
 * way and a switch could alter it accidently or maliciously causing the wrong session identifier
 * to be stored leading to the session failing until it eventually times out.
 *
 * To fix this bug the handle attached to the initiation message has been moved into the CryptoAuth
 * block where it should have been in the first place. Obviously the peer's handle is still placed
 * outside of the CryptoAuth block so it can serve it's function.
 *
 * As of version 3, implementations must not send handles which are less than 4 so that when they
 * receive them back, they will not be confused with the initial 4 bytes of a CryptoAuth setup
 * packet which is not preceeded by a handle.
 */
Version_COMPAT(3, ([1,2]))

/*
 * Version 4:
 * August 27, 2013
 *
 * This version makes no protocol changes but fixes a nasty bug with forwarding which caused
 * messages to be forwarded to random nodes, updated to encourage nodes to forward via others
 * who do not have the bug.
 */
Version_COMPAT(4, ([1,2,3]))

/*
 * Version 5:
 * September 4, 2013
 *
 * This version introduces a new RPC call for getting directly connected peers from a node.
 * The new RPC call is called "gp" and it takes a target label called "tar" which must be an
 * 8 byte long benc string. It returns a list of peers exactly the same as a search but they
 * must all be direct peers and they are the peers whose labels have smallest XOR distance
 * from "tar".
 */
Version_COMPAT(5, ([1,2,3,4]))

/*
 * Version 6:
 * December 14, 2013
 *
 * Drop support for versions older than 5
 */
Version_COMPAT(6, ([5]))

/*
 * Version 7:
 * March 16, 2014
 *
 * A ceremonial version which has the pathfinder2 implemented.
 * this is still compatible with the same versions and doesn't expressly change the protocol
 * although it is nolonger able to communicate with *older* version 5 nodes which do not advertize
 * their encoding scheme.
 */
Version_COMPAT(7, ([5,6]))

/**
 * The current protocol version.
 */
#define Version_CURRENT_PROTOCOL 7
#define Version_5_COMPAT
#define Version_6_COMPAT

#define Version_MINIMUM_COMPATIBLE 5
#define Version_DEFAULT_ASSUMPTION 5

/**
 * Check the compatibility matrix and return whether two versions are compatible.
 * If a version is not listed on the table, the highest version on the table is
 * substituted for it but if the return value is yes, it is changed to maybe.
 *
 * @param version1 the first version
 * @param version2 the second version
 * @return 1 meaning compatible or 0 meaning incompatible.
 */
int Version_isCompatible(uint32_t one, uint32_t two);

#endif
