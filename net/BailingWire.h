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
#ifndef BailingWire_H
#define BailingWire_H

#include "interface/Interface.h"
#include "interface/SessionManager.h"
#include "memory/Allocator.h"
#include "net/Event.h"
#include "net/EventEmitter.h"
#include "util/Linker.h"
Linker_require("net/BailingWire.c")

/**
 * Called BailingWire because I can't think of what this should be called.
 * Purpose of this module is to take packets from "the inside" which contain ipv6 address and
 * skeleton switch header and find an appropriate CryptoAuth session for them or begin one.
 * If a key for this node cannot be found then the packet will be blocked and a search will be
 * triggered. If the skeleton switch header contains "zero" as the switch label, the packet will
 * also be buffered and a search triggered. If a search is in progress (and another packet is
 * already buffered, the packet will be dropped instead).
 * Incoming messages from the outside will be decrypted and their key and path will be stored.
 */
struct BailingWire
{
    /** Sends and handles packets prepped to/from switch. */
    struct Interface_Two switchIf;

    /**
     * Sends and handles packets with BailingWire_InsideHeader on top.
     * When sending a packet to BailingWire:
     *     header.sh.label_be may be zero
     *     version may be zero
     *     publicKey may be zero
     * If these values are not known, the packet will be taken from the cache or a search will
     * be triggered.
     */
    struct Interface_Two insideIf;

    /**
     * Maximum number of packets to hold in buffer before summarily dropping...
     */
    int maxBufferedMessages;
};

struct BailingWire_InsideHeader
{
    /**
     * The switch header to use.
     * label_be may be zero if unknown.
     * version will be automatically set to the node's current version.
     */
    struct SwitchHeader sh;

    /** Protocol version of peer node, 0 if unknown. */
    uint32_t version;

    /** IPv6 of peer node REQUIRED */
    uint8_t ip6[16];

    /** public key of peer node. */
    uint8_t publicKey[32];

    /** 0 if the publicKey is unknown and 'publicKey' holds nothing of value. */
    uint32_t publicKeyKnown;

    /**
     * Pads out the size of the inside header to prevent the SwitchHeader possibly being
     * clobbered by the cryptoAuth so that the switch header will not be clobbered.
     * Largest overhead:  [ CryptoHeader ][ encrypted Handle (4) ][ encrypted content ..... ]
     */
    uint8_t padding[68];
};
#define BailingWire_InsideHeader_SIZE (CryptoHeader_SIZE + 4 + SwitchHeader_SIZE)
Assert_compileTime(BailingWire_InsideHeader_SIZE == sizeof(struct BailingWire_InsideHeader));

struct BailingWire* BailingWire_new(struct Allocator* alloc,
                                    struct SessionManager* sm,
                                    struct EventEmitter* ee);

#endif
