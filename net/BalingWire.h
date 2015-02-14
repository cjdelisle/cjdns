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
#ifndef BalingWire_H
#define BalingWire_H

#include "interface/Interface.h"
#include "net/SessionTable.h"
#include "memory/Allocator.h"
#include "net/Event.h"
#include "net/EventEmitter.h"
#include "wire/SwitchHeader.h"
#include "wire/CryptoHeader.h"
#include "util/Linker.h"
Linker_require("net/BalingWire.c")

/**
 * Called BalingWire because I can't think of what this should be called.
 * Purpose of this module is to take packets from "the inside" which contain ipv6 address and
 * skeleton switch header and find an appropriate CryptoAuth session for them or begin one.
 * If a key for this node cannot be found then the packet will be blocked and a search will be
 * triggered. If the skeleton switch header contains "zero" as the switch label, the packet will
 * also be buffered and a search triggered. If a search is in progress (and another packet is
 * already buffered, the packet will be dropped instead).
 * Incoming messages from the outside will be decrypted and their key and path will be stored.
 */
struct BalingWire
{
    /** Sends and handles packets prepped to/from switch. */
    struct Interface_Two switchIf;

    /**
     * Sends and handles packets with BalingWire_InsideHeader on top.
     * When sending a packet to BalingWire:
     *     header.sh.label_be may be zero
     *     version may be zero
     *     publicKey may be zero
     * If these values are not known, the packet will be taken from the cache or a search will
     * be triggered.
     */
    struct Interface_Two insideIf;

    struct SessionTable* sessionTable;

    /**
     * Maximum number of packets to hold in buffer before summarily dropping...
     */
    #define BalingWire_MAX_BUFFERED_MESSAGES_DEFAULT 30
    int maxBufferedMessages;

    /**
     * Number of milliseconds it takes for metric to halve (value of UINT32_MAX - metric halves)
     * This allows less good routes to supplant better ones if the "better" ones have not been
     * tested in a long time (maybe down).
     */
    #define BalingWire_METRIC_HALFLIFE_MILLISECONDS_DEFAULT 250000
    uint32_t metricHalflifeMilliseconds;
};

struct BalingWire_InsideHeader
{
    /** public key of peer node, 0 if unknown, always send from BailingWire. */
    uint8_t publicKey[32];

    /**
     * The switch header to use.
     * label_be may be zero if unknown.
     * version will be automatically set to the node's current version.
     */
    struct SwitchHeader sh;

    /** Protocol version of peer node, 0 if unknown, sometimes 0 from BailingWire. */
    uint32_t version;

    /**
     * Create a layout which puts the SwitchHeader 24 bytes behind the end of the header
     * allowing it to be in exactly the right place after encryption.
     */
    uint32_t pad;

    /** IPv6 of peer node REQUIRED */
    uint8_t ip6[16];
};
#define BalingWire_InsideHeader_SIZE (56 + SwitchHeader_SIZE)
Assert_compileTime(BalingWire_InsideHeader_SIZE == sizeof(struct BalingWire_InsideHeader));

struct BalingWire* BalingWire_new(struct Allocator* alloc,
                                  struct EventBase* eventBase,
                                  struct CryptoAuth* cryptoAuth,
                                  struct Random* rand,
                                  struct Log* log,
                                  struct EventEmitter* ee);

#endif
