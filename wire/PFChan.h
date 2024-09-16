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
#ifndef PFChan_H
#define PFChan_H

#include "util/Assert.h"
#include "wire/RouteHeader.h"
#include "wire/DataHeader.h"
#include "wire/SwitchHeader.h"
#include "wire/Control.h"

struct PFChan_Node
{
    uint8_t ip6[16];

    uint8_t publicKey[32];

    uint64_t path_be;

    /** Quality of path represented by switch label (0:best ffffffff:worst) */
    uint32_t metric_be;

    uint32_t version_be;
};
#define PFChan_Node_SIZE 64
Assert_compileTime(sizeof(struct PFChan_Node) == PFChan_Node_SIZE);

struct PFChan_Msg
{
    struct RouteHeader route;
    struct DataHeader data;
    // ...content follows...
};
#define PFChan_Msg_MIN_SIZE (RouteHeader_SIZE + DataHeader_SIZE)
Assert_compileTime(sizeof(struct PFChan_Msg) == PFChan_Msg_MIN_SIZE);
#pragma GCC poison PFChan_Msg_SIZE

struct PFChan_CtrlMsg
{
    struct RouteHeader route;
    struct Control_Header ctrlHdr;
};
#define PFChan_CtrlMsg_MIN_SIZE (RouteHeader_SIZE + Control_Header_SIZE)
Assert_compileTime(sizeof(struct PFChan_Msg) == PFChan_CtrlMsg_MIN_SIZE);
#pragma GCC poison PFChan_CtrlMsg_SIZE

struct PFChan_Ping
{
    uint64_t cookie;
};
#define PFChan_Ping_SIZE 8
Assert_compileTime(sizeof(struct PFChan_Ping) == PFChan_Ping_SIZE);

struct PFChan_Pathfinder_Connect
{
    /**
     * See PFChan_Pathfinder_Superiority for more information about this field.
     * It is recommended that you pass zero at first and then pass a higher number once your table
     * has populated. Use an PFChan_Pathfinder_SUPERIORITY event to alter it.
     */
    uint32_t superiority_be;

    /** Protocol version of the pathfinder. */
    uint32_t version_be;

    /** Description of the pathfinder in ASCII text. */
    uint8_t userAgent[64];
};
#define PFChan_Pathfinder_Connect_SIZE 72
Assert_compileTime(sizeof(struct PFChan_Pathfinder_Connect) == PFChan_Pathfinder_Connect_SIZE);

/**
 * Sending a PFChan_Pathfinder_SUPERIORITY event will cause PFChan_Core_PATHFINDER event to be sent
 * again for your pathfinder with the updated superiority. Superiority is a way for multiple
 * connected pathfinders to operate together by ones which have lower superiority switching
 * to an "idle" mode, relying on the responses to the DHT pings and searches sent by the higher
 * superiority pathfinder.
 */
struct PFChan_Pathfinder_Superiority
{
    uint32_t superiority_be;
};
#define PFChan_Pathfinder_Superiority_SIZE 4
Assert_compileTime(
    sizeof(struct PFChan_Pathfinder_Superiority) == PFChan_Pathfinder_Superiority_SIZE);

typedef struct PFChan_Pathfinder_ConnectPeer {
    // If ::ffff:XXXX:XXXX then is 4 in 6
    uint8_t ip[16];
    // Public key of the peer
    uint8_t pubkey[32];
    // Null terminated, e.g. "AP_LOGIN: 65535\0"
    uint8_t login[16];
    // Null terminated, e.g. "AP_PASS: 4eqnmtOq/Mo\0"
    uint8_t password[24];
    // Protocol version of the peer
    uint32_t version;
} PFChan_Pathfinder_ConnectPeer_t;
Assert_compileTime(sizeof(PFChan_Pathfinder_ConnectPeer_t) == 92);

enum PFChan_Pathfinder
{
    /** Below the lowest valid value. */
    PFChan_Pathfinder__TOO_LOW = 511,

    /**
     * Must be emitted before any other messages.
     * (Received by: EventEmitter.c)
     */
    PFChan_Pathfinder_CONNECT = 512,

    /**
     * See PFChan_Pathfinder_Superiority for more information about this event.
     * (Received by: EventEmitter.c)
     */
    PFChan_Pathfinder_SUPERIORITY = 513,

    /**
     * Emit to indicate the discovery of a node or a new best path to the node.
     * To reduce traffic load, first request all sessions and then only emit for nodes for which
     * there are active sessions.
     * (Received by: SessionManager.c)
     */
    PFChan_Pathfinder_NODE = 514,

    /**
     * Send a DHT message to another node.
     * (Received by: UpperDistributor.c)
     */
    PFChan_Pathfinder_SENDMSG = 515,

    /**
     * PFChan_Pathfinder_PING will elicit an PFChan_Core_PONG
     * (Received by: EventEmitter.c)
     */
    PFChan_Pathfinder_PING = 516,

    /**
     * PFChan_Pathfinder_PONG must be sent if core sends a PFChan_Core_PING
     * (Received by: EventEmitter.c)
     */
    PFChan_Pathfinder_PONG = 517,

    // The following events have no content.

    /**
     * Get all sessions.
     * (Received by: SessionManager.c)
     */
    PFChan_Pathfinder_SESSIONS = 518,

    /**
     * Get all peers.
     * (Received by: InterfaceController.c)
     */
    PFChan_Pathfinder_PEERS = 519,

    /**
     * Get all registered pathfinders
     * (Received by: EventEmitter.c)
     */
    PFChan_Pathfinder_PATHFINDERS = 520,

    /**
     * Send from the Pathfinder in order to send off a CTRL message.
     * Send under this, a RouteHeader and the control frame after it.
     * (Received by: UpperDistributor.c)
     */
    PFChan_Pathfinder_CTRL_SENDMSG = 521,

    /**
     * You must send this whenever you have adopted a supernode.
     * Send with address set to all zeros if you lost your supernode.
     * (Received by: ControlHandler.c)
     */
    PFChan_Pathfinder_SNODE = 522,

    /**
     * Send a peer setup request.
     * (Received by: InterfaceController.c)
     */
    PFChan_Pathfinder_CONNECT_PEER = 523,

    PFChan_Pathfinder__TOO_HIGH = 524,
};

typedef struct PFChan_FromPathfinder
{
    enum PFChan_Pathfinder event_be;

    /* Number of the Pathfinder which sent this event, added by EventEmitter.c */
    uint8_t target_be;

    union {
        struct PFChan_Pathfinder_Connect connect;
        struct PFChan_Pathfinder_Superiority superiority;
        struct PFChan_Node node;
        struct PFChan_Msg sendmsg;
        struct PFChan_Ping ping;
        struct PFChan_Ping pong;
        PFChan_Pathfinder_ConnectPeer_t cp;
        uint8_t bytes[1];
    } content;
} PFChan_FromPathfinder_t;

//// ------------------------- Core Events ------------------------- ////

enum PFChan_Core
{
    /** This is below the lowest valid value */
    PFChan_Core__TOO_LOW = 1023,

    /**
     * This message is sent in response to an PFChan_Pathfinder_CONNECT message and is
     * guaranteed to be sent before any other message.
     * (emitted by: EventEmitter.c)
     */
    PFChan_Core_CONNECT = 1024,

    /**
     * Emitted when a pathfinder connects or if PFChan_Pathfinder_PATHFINDERS is sent.
     * (emitted by: EventEmitter.c)
     */
    PFChan_Core_PATHFINDER = 1025,

    /**
     * Emitted when a pathfinder disconnects from the core
     * (emitted by: EventEmitter.c)
     */
    PFChan_Core_PATHFINDER_GONE = 1026,

    /**
     * Emitted if a switch error is received, no matter what type of packet causes it.
     * (emitted by: ControlHandler.c)
     */
    PFChan_Core_SWITCH_ERR = 1027,

    /**
     * Emitted if the core wants the pathfinder to begin searching for a node.
     * (emitted by: SessionManager.c)
     */
    PFChan_Core_SEARCH_REQ = 1028,

    /**
     * Emitted when a peer connects (becomes state ESTABLISHED) or
     * emitted for every peer if PFChan_Pathfinder_PEERS is sent.
     * (emitted by: InterfaceController.c)
     */
    PFChan_Core_PEER = 1029,

    /**
     * Emitted when a peer disconnects (or becomes state UNRESPONSIVE)
     * (emitted by: InterfaceController.c)
     */
    PFChan_Core_PEER_GONE = 1030,

    /**
     * Emitted if a new session begins, also emitted for every active session of
     * PFChan_Pathfinder_SESSIONS is sent.
     * (emitted by: SessionManager.c)
     */
    PFChan_Core_SESSION = 1031,

    /**
     * Emitted when a session ends.
     * (emitted by: SessionManager.c)
     */
    PFChan_Core_SESSION_ENDED = 1032,

    /**
     * Emitted when SessionManager sees an incoming packet with a new path.
     * (emitted by: SessionManager.c)
     */
    PFChan_Core_DISCOVERED_PATH = 1033,

    /**
     * Emitted for each incoming DHT message.
     * (emitted by: UpperDistributor.c)
     */
    PFChan_Core_MSG = 1034,

    /**
     * Emitted from time to time in order to verify the pathfinder is alive.
     * Must be responded to by an PFChan_Pathfinder_PONG.
     * (emitted by: EventEmitter.c)
     */
    PFChan_Core_PING = 1035,

    /**
     * Will be emitted if the pathfinder emits an PFChan_Pathfinder_PING.
     * (emitted by: EventEmitter.c)
     */
    PFChan_Core_PONG = 1036,

    /**
     * Will be emitted by the core when a control message (response) is incoming.
     * TODO(cjd): This doesn't cover all control message types yet.
     */
    PFChan_Core_CTRL_MSG = 1037,

    /**
     * Will be emitted when the core has a path to a node but the session is not setup.
     * Structure is a PFChan_Node
     * (emitted by: SessionManager.c)
     */
    PFChan_Core_UNSETUP_SESSION = 1038,

    /**
     * Will be emitted once every 3 seconds to inform pathfinders of the link state of
     * the peering links, contains an array of PFChan_LinkState_Entry.
     * (emitted by: InterfaceController.c)
     */
    PFChan_Core_LINK_STATE = 1039,

    PFChan_Core__TOO_HIGH = 1040,
};

// All values are in host order
struct PFChan_LinkState_Entry {
    uint64_t peerLabel;
    uint64_t sumOfPackets;
    uint64_t sumOfDrops;
    uint64_t sumOfKb;
};
#define PFChan_LinkState_Entry_SIZE 32
Assert_compileTime(sizeof(struct PFChan_LinkState_Entry) == PFChan_LinkState_Entry_SIZE);

struct PFChan_Core_SearchReq
{
    uint8_t ipv6[16];

    uint32_t pad;

    uint32_t version_be;
};
#define PFChan_Core_SearchReq_SIZE 24
Assert_compileTime(sizeof(struct PFChan_Core_SearchReq) == PFChan_Core_SearchReq_SIZE);

struct PFChan_Core_Pathfinder
{
    /** See struct PFChan_Pathfinder_Superiority for more information */
    uint32_t superiority_be;

    /** The number of this pathfinder. */
    uint32_t pathfinderId_be;

    /** Description of the pathfinder in ASCII text. */
    uint8_t userAgent[64];
};
#define PFChan_Core_Pathfinder_SIZE 72
Assert_compileTime(sizeof(struct PFChan_Core_Pathfinder) == PFChan_Core_Pathfinder_SIZE);

struct PFChan_Core_Connect
{
    /** The core's version (Version.h). */
    uint32_t version_be;

    /** This pathfinder's ID. */
    uint32_t pathfinderId_be;

    /** The public key of this cjdns node. */
    uint8_t publicKey[32];
};
#define PFChan_Core_Connect_SIZE 40
Assert_compileTime(sizeof(struct PFChan_Core_Connect) == PFChan_Core_Connect_SIZE);

struct PFChan_Core_SwitchErr
{
    struct SwitchHeader sh;
    struct Control_Header ctrlHeader;
    struct Control_Error ctrlErr;
    struct SwitchHeader shAtErrorHop;
};
#pragma GCC poison PFChan_Core_SwitchErr_SIZE
#define PFChan_Core_SwitchErr_MIN_SIZE \
    (SwitchHeader_SIZE + Control_Header_SIZE + Control_Error_MIN_SIZE + SwitchHeader_SIZE)
Assert_compileTime(sizeof(struct PFChan_Core_SwitchErr) == PFChan_Core_SwitchErr_MIN_SIZE);

struct PFChan_FromCore
{
    enum PFChan_Core event_be;

    /* Number of the Pathfinder to send this event to, 0xffffffff sends to all. */
    uint8_t target_be;

    union {
        struct PFChan_Core_Connect connect;
        struct PFChan_Core_Pathfinder pathfinder;
        struct PFChan_Core_Pathfinder pathfinderGone;
        struct PFChan_Core_SwitchErr switchErr;
        struct PFChan_Core_SearchReq searchReq;
        struct PFChan_Node peer;
        struct PFChan_Node peerGone;
        struct PFChan_Node session;
        struct PFChan_Node sessionEnded;
        struct PFChan_Node discoveredPath;
        struct PFChan_Msg msg;
        struct PFChan_Ping ping;
        struct PFChan_Ping pong;
        struct PFChan_LinkState_Entry linkState;
        uint8_t bytes[4];
    } content;
};
// PFChan_FromCore contains a union so it's size is not useful.
#pragma GCC poison PFChan_FromCore_SIZE

#endif
