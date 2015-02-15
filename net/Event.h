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
#ifndef Event_H
#define Event_H

#include "util/Assert.h"
#include "wire/RouteHeader.h"
#include "wire/DataHeader.h"
#include "wire/SwitchHeader.h"
#include "wire/Control.h"

struct Event_Ip6
{
    uint8_t ipv6[16];
};
#define Event_Ip6_SIZE 16
Assert_compileTime(sizeof(struct Event_Ip6) == Event_Ip6_SIZE);

struct Event_Node
{
    uint8_t ip6[16];

    uint8_t publicKey[32];

    uint64_t path_be;

    /** Quality of path represented by switch label (0:best ffffffff:worst) */
    uint32_t metric_be;

    uint32_t version_be;
};
#define Event_Node_SIZE 64
Assert_compileTime(sizeof(struct Event_Node) == Event_Node_SIZE);

struct Event_Msg
{
    struct RouteHeader route;
    struct DataHeader data;
    // ...content follows...
};
#pragma GCC poison Event_Msg_SIZE // Contains more data...

struct Event_Ping
{
    uint64_t cookie;
};
#define Event_Ping_SIZE 8
Assert_compileTime(sizeof(struct Event_Ping) == Event_Ping_SIZE);

struct Event_Pathfinder_Connect
{
    /**
     * See Event_Pathfinder_Superiority for more information about this field.
     * It is recommended that you pass zero at first and then pass a higher number once your table
     * has populated. Use an Event_Pathfinder_SUPERIORITY event to alter it.
     */
    uint32_t superiority_be;

    /** Protocol version of the pathfinder. */
    uint32_t version_be;

    /** Description of the pathfinder in ASCII text. */
    uint8_t userAgent[64];
};
#define Event_Pathfinder_Connect_SIZE 72
Assert_compileTime(sizeof(struct Event_Pathfinder_Connect) == Event_Pathfinder_Connect_SIZE);

/**
 * Sending a Event_Pathfinder_SUPERIORITY event will cause Event_Core_PATHFINDER event to be sent
 * again for your pathfinder with the updated superiority. Superiority is a way for multiple
 * connected pathfinders to operate together by ones which have lower superiority switching
 * to an "idle" mode, relying on the responses to the DHT pings and searches sent by the higher
 * superiority pathfinder.
 */
struct Event_Pathfinder_Superiority
{
    uint32_t superiority_be;
};
#define Event_Pathfinder_Superiority_SIZE 4
Assert_compileTime(
    sizeof(struct Event_Pathfinder_Superiority) == Event_Pathfinder_Superiority_SIZE);

enum Event_Pathfinder
{
    /**
     * Must be emitted before any other messages.
     * (Received by: EventEmitter.c)
     */
    Event_Pathfinder_CONNECT,

    /**
     * See Event_Pathfinder_Superiority for more information about this event.
     * (Received by: EventEmitter.c)
     */
    Event_Pathfinder_SUPERIORITY,

    /**
     * Emit to indicate the discovery of a node or a new best path to the node.
     * To reduce traffic load, first request all sessions and then only emit for nodes for which
     * there are active sessions.
     * (Received by: SessionManager.c)
     */
    Event_Pathfinder_NODE,

    /**
     * Send a DHT message to another node.
     * TODO
     */
    Event_Pathfinder_SENDMSG,

    /**
     * Event_Pathfinder_PING will elicit an Event_Core_PONG
     * (Received by: EventEmitter.c)
     */
    Event_Pathfinder_PING,

    /**
     * Event_Pathfinder_PONG must be sent if core sends a Event_Core_PING
     * (Received by: EventEmitter.c)
     */
    Event_Pathfinder_PONG,

    // The following events have no content.

    /**
     * Get all sessions.
     * TODO
     */
    Event_Pathfinder_SESSIONS,

    /**
     * Get all peers.
     * (Received by: InterfaceController.c)
     */
    Event_Pathfinder_PEERS,

    /**
     * Get all registered pathfinders
     * (Received by: EventEmitter.c)
     */
    Event_Pathfinder_PATHFINDERS,

    Event_Pathfinder_INVALID
};

static const int Event_Pathfinder_SIZES[] = {
    [Event_Pathfinder_CONNECT]      = 4 + Event_Pathfinder_Connect_SIZE,
    [Event_Pathfinder_SUPERIORITY]  = 4 + Event_Pathfinder_Superiority_SIZE,
    [Event_Pathfinder_NODE]         = 4 + Event_Node_SIZE,
    [Event_Pathfinder_SENDMSG]      = -(4 + (int)sizeof(struct Event_Msg)),
    [Event_Pathfinder_PING]         = 4 + Event_Ping_SIZE,
    [Event_Pathfinder_PONG]         = 4 + Event_Ping_SIZE,
    [Event_Pathfinder_SESSIONS]     = 4,
    [Event_Pathfinder_PEERS]        = 4,
    [Event_Pathfinder_PATHFINDERS]  = 4
};

struct Event_FromPathfinder
{
    enum Event_Pathfinder event_be;

    /* Number of the Pathfinder which sent this event, added by EventEmitter.c */
    uint8_t target_be;

    union {
        struct Event_Pathfinder_Connect connect;
        struct Event_Pathfinder_Superiority superiority;
        struct Event_Node node;
        struct Event_Msg sendmsg;
        struct Event_Ping ping;
        struct Event_Ping pong;
        uint8_t bytes[1];
    } content;
};

//// ------------------------- Core Events ------------------------- ////

enum Event_Core
{
    /**
     * Emitted when a pathfinder connects or if Event_Pathfinder_PATHFINDERS is sent.
     * This message is guaranteed to be sent before any other message and the first instance
     * refers to "you" the pathfinder which just connected.
     * (emitted by: EventEmitter.c)
     */
    Event_Core_PATHFINDER,

    /**
     * Emitted when a pathfinder disconnects from the core
     * (emitted by: EventEmitter.c)
     */
    Event_Core_PATHFINDER_GONE,

    /**
     * Emitted if the core wants the pathfinder to begin searching for a node.
     * (emitted by: SessionManager.c)
     */
    Event_Core_SEARCH_REQ,

    /**
     * Emitted if a switch error is received, no matter what type of packet causes it.
     * (emitted by: ControlHandler.c)
     */
    Event_Core_SWITCH_ERR,

    /**
     * Emitted when a peer connects (becomes state ESTABLISHED) or
     * emitted for every peer if Event_Pathfinder_PEERS is sent.
     * (emitted by: InterfaceController.c)
     */
    Event_Core_PEER,

    /**
     * Emitted when a peer disconnects (or becomes state UNRESPONSIVE)
     * (emitted by: InterfaceController.c)
     */
    Event_Core_PEER_GONE,

    /**
     * Emitted if a new session begins,
     * emitted for every active session of Event_Pathfinder_SESSIONS is sent.
     * TODO
     */ 
    Event_Core_SESSION,

    /** Emitted when a session ends. TODO */
    Event_Core_SESSION_ENDED,

    /** Emitted when SessionManager sees an incoming packet with a new path. TODO */
    Event_Core_DISCOVERED_PATH,

    /** Emitted for each incoming DHT message. TODO */
    Event_Core_MSG,

    /**
     * Emitted from time to time in order to verify the pathfinder is alive.
     * (emitted by: EventEmitter.c)
     */
    Event_Core_PING,

    /**
     * Will be emitted if the pathfinder emits an Event_Pathfinder_PING.
     * (emitted by: EventEmitter.c)
     */
    Event_Core_PONG,

    Event_Core_INVALID
};

struct Event_Core_Pathfinder
{
    /** See struct Event_Pathfinder_Superiority for more information */
    uint32_t superiority_be;

    /** The number of this pathfinder. */
    uint32_t pathfinderId_be;

    /** Description of the pathfinder in ASCII text. */
    uint8_t userAgent[64];
};
#define Event_Core_Pathfinder_SIZE 72
Assert_compileTime(sizeof(struct Event_Core_Pathfinder) == Event_Core_Pathfinder_SIZE);

struct Event_Core_SwitchErr
{
    struct SwitchHeader sh;
    uint32_t ffffffff;
    struct Control_Header ctrlHeader;
    struct Control_Error ctrlErr;
};
#pragma GCC poison Event_Core_SwitchErr_SIZE
Assert_compileTime(sizeof(struct Event_Core_SwitchErr) ==
    (SwitchHeader_SIZE + 4 + Control_Header_SIZE + Control_Error_MIN_SIZE));

#define Event_FromCore_MAGIC 0x1337babe
struct Event_FromCore
{
    enum Event_Core event_be;

    /* Number of the Pathfinder to send this event to, 0xffffffff sends to all. */
    uint8_t target_be;

    union {
        struct Event_Core_Pathfinder pathfinder;
        struct Event_Core_Pathfinder pathfinderGone;
        struct Event_Ip6 searchReq;
        struct Event_Core_SwitchErr switchErr;
        struct Event_Node peer;
        struct Event_Node peerGone;
        struct Event_Node session;
        struct Event_Node sessionEnded;
        struct Event_Node discoveredPath;
        struct Event_Msg msg;
        struct Event_Ping ping;
        struct Event_Ping pong;
        uint8_t bytes[4];
    } content;
};
// Event_FromCore contains a union so it's size is not useful.
#pragma GCC poison Event_FromCore_SIZE

#endif
