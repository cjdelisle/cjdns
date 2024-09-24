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
#include "dht/Address.h"
#include "net/ControlHandler.h"
#include "exception/Err.h"
#include "memory/Allocator.h"
#include "net/InterfaceController.h"
#include "switch/EncodingScheme.h"
#include "util/Bits.h"
#include "util/Endian.h"
#include "util/Identity.h"
#include "util/AddrTools.h"
#include "util/Checksum.h"
#include "util/platform/Sockaddr.h"
#include "wire/Control.h"
#include "wire/Error.h"
#include "wire/Message.h"
#define NumberCompress_OLD_CODE
#include "switch/NumberCompress.h"

#include <inttypes.h>

struct ControlHandler_pvt
{
    struct ControlHandler pub;
    struct EncodingScheme* ourEncodingScheme;
    struct Log* log;
    struct Allocator* alloc;
    struct InterfaceController* ifc;
    uint8_t myPublicKey[32];
    struct Iface eventIf;
    struct Address activeSnode;
    Identity
};

/**
 * Expects [ Ctrl ][ Error ][ cause SwitchHeader ][ cause handle ][ cause etc.... ]
 */
#define handleError_MIN_SIZE (Control_Header_SIZE + Control_Error_MIN_SIZE + SwitchHeader_SIZE + 4)
static Iface_DEFUN handleError(Message_t* msg,
                               struct ControlHandler_pvt* ch,
                               uint64_t label,
                               uint8_t* labelStr,
                               struct RouteHeader* rh)
{
    if (Message_getLength(msg) < handleError_MIN_SIZE) {
        Log_info(ch->log, "DROP runt error packet from [%s]", labelStr);
        return Error(msg, "RUNT");
    }
    Err(Message_truncate(msg, handleError_MIN_SIZE));
    Err(Message_epush(msg, &rh->sh, SwitchHeader_SIZE));
    Err(Message_epush32be(msg, 0xffffffff));
    Err(Message_epush32be(msg, PFChan_Core_SWITCH_ERR));
    return Iface_next(&ch->eventIf, msg);
}

/**
 * Expects [ SwitchHeader ][ Ctrl ][ (key)Ping ][ data etc.... ]
 */
#define handlePing_MIN_SIZE (Control_Header_SIZE + Control_Ping_MIN_SIZE)
static Iface_DEFUN handlePing(Message_t* msg,
                              struct ControlHandler_pvt* ch,
                              uint64_t label,
                              uint8_t* labelStr,
                              uint16_t messageType_be)
{
    if (Message_getLength(msg) < handlePing_MIN_SIZE) {
        Log_info(ch->log, "DROP runt ping");
        return Error(msg, "RUNT");
    }

    struct Control* ctrl = (struct Control*) Message_bytes(msg);
    Err(Message_eshift(msg, -Control_Header_SIZE));

    // Ping and keyPing share version location
    struct Control_Ping* ping = (struct Control_Ping*) Message_bytes(msg);
    uint32_t herVersion = Endian_bigEndianToHost32(ping->version_be);
    if (!Version_compatibleWithCurrent(herVersion)) {
        Log_debug(ch->log, "DROP ping from incompatible version [%d]", herVersion);
        return Error(msg, "INVALID");
    }

    if (messageType_be == Control_KEYPING_be) {
        Log_debug(ch->log, "got switch keyPing from [%s]", labelStr);
        if (Message_getLength(msg) < Control_KeyPing_HEADER_SIZE) {
            // min keyPing size is longer
            Log_debug(ch->log, "DROP runt keyPing");
            return Error(msg, "RUNT");
        }
        if (Message_getLength(msg) > Control_KeyPing_MAX_SIZE) {
            Log_debug(ch->log, "DROP long keyPing");
            return Error(msg, "INVALID");
        }
        if (ping->magic != Control_KeyPing_MAGIC) {
            Log_debug(ch->log, "DROP keyPing (bad magic)");
            return Error(msg, "INVALID");
        }

        struct Control_KeyPing* keyPing = (struct Control_KeyPing*) Message_bytes(msg);
        keyPing->magic = Control_KeyPong_MAGIC;
        ctrl->header.type_be = Control_KEYPONG_be;
        Bits_memcpy(keyPing->key, ch->myPublicKey, 32);

    } else if (messageType_be == Control_PING_be) {
        // Happens in benchmark.
        //Log_debug(ch->log, "got switch ping from [%s]", labelStr);
        if (ping->magic != Control_Ping_MAGIC) {
            Log_debug(ch->log, "DROP ping (bad magic)");
            return Error(msg, "INVALID");
        }
        ping->magic = Control_Pong_MAGIC;
        ctrl->header.type_be = Control_PONG_be;

    } else {
        Assert_failure("2+2=5");
    }

    ping->version_be = Endian_hostToBigEndian32(Version_CURRENT_PROTOCOL);

    Err(Message_eshift(msg, Control_Header_SIZE));

    ctrl->header.checksum_be = 0;
    ctrl->header.checksum_be = Checksum_engine_be(Message_bytes(msg), Message_getLength(msg));

    Err(Message_eshift(msg, RouteHeader_SIZE));

    struct RouteHeader* routeHeader = (struct RouteHeader*) Message_bytes(msg);
    Bits_memset(routeHeader, 0, RouteHeader_SIZE);
    SwitchHeader_setVersion(&routeHeader->sh, SwitchHeader_CURRENT_VERSION);
    routeHeader->sh.label_be = Endian_hostToBigEndian64(label);
    routeHeader->flags |= RouteHeader_flags_CTRLMSG;

    return Iface_next(&ch->pub.coreIf, msg);
}

/**
 * Expects [ SwitchHeader ][ Ctrl ][ RPath ][ data etc.... ]
 */
#define handleRPathQuery_MIN_SIZE (Control_Header_SIZE + Control_RPath_HEADER_SIZE)
static Iface_DEFUN handleRPathQuery(Message_t* msg,
                                    struct ControlHandler_pvt* ch,
                                    uint64_t label,
                                    uint8_t* labelStr)
{
    Log_debug(ch->log, "Incoming RPATH query");
    if (Message_getLength(msg) < handleRPathQuery_MIN_SIZE) {
        Log_info(ch->log, "DROP runt RPATH query");
        return Error(msg, "RUNT");
    }

    struct Control* ctrl = (struct Control*) Message_bytes(msg);
    struct Control_RPath* rpa = &ctrl->content.rpath;

    if (rpa->magic != Control_RPATH_QUERY_MAGIC) {
        Log_debug(ch->log, "DROP RPATH query (bad magic)");
        return Error(msg, "INVALID");
    }

    ctrl->header.type_be = Control_RPATH_REPLY_be;
    rpa->version_be = Endian_hostToBigEndian32(Version_CURRENT_PROTOCOL);
    rpa->magic = Control_RPATH_REPLY_MAGIC;
    uint64_t label_be = Endian_hostToBigEndian64(label);
    Bits_memcpy(rpa->rpath_be, &label_be, 8);

    ctrl->header.checksum_be = 0;
    ctrl->header.checksum_be = Checksum_engine_be(Message_bytes(msg), Message_getLength(msg));

    Err(Message_eshift(msg, RouteHeader_SIZE));
    struct RouteHeader* routeHeader = (struct RouteHeader*) Message_bytes(msg);
    Bits_memset(routeHeader, 0, RouteHeader_SIZE);
    SwitchHeader_setVersion(&routeHeader->sh, SwitchHeader_CURRENT_VERSION);
    routeHeader->sh.label_be = label_be;
    routeHeader->flags |= RouteHeader_flags_CTRLMSG;
    return Iface_next(&ch->pub.coreIf, msg);
}

static Err_DEFUN writeLlAddr(Message_t* msg, Sockaddr_t* sa) {
    struct Control_LlAddr out = {
        .magic = Control_LlAddr_REPLY_MAGIC,
        .version_be = Endian_hostToBigEndian32(Version_CURRENT_PROTOCOL),
    };
    if (sa->type == Sockaddr_PLATFORM) {
        int fam = Sockaddr_getFamily(sa);
        int port = Sockaddr_getPort(sa);
        if (fam == Sockaddr_AF_INET && port > -1) {
            out.addr.udp4.type = Control_LlAddr_Udp4_TYPE;
            out.addr.udp4.len = sizeof(Control_LlAddr_Udp4_t);
            out.addr.udp4.port_be = Endian_hostToBigEndian16(port);
            uint8_t* p = NULL;
            int len = Sockaddr_getAddress(sa, &p);
            Assert_true(len == sizeof out.addr.udp4.addr && p != NULL);
            Bits_memcpy(&out.addr.udp4.addr, p, sizeof out.addr.udp4.addr);
            return Message_epush(msg, &out, sizeof out);
        } else if (fam == Sockaddr_AF_INET6 && port > -1) {
            out.addr.udp6.type = Control_LlAddr_Udp6_TYPE;
            out.addr.udp6.len = sizeof(Control_LlAddr_Udp6_t);
            out.addr.udp6.port_be = Endian_hostToBigEndian16(port);
            uint8_t* p = NULL;
            int len = Sockaddr_getAddress(sa, &p);
            Assert_true(len == sizeof out.addr.udp6.addr && p != NULL);
            Bits_memcpy(&out.addr.udp6.addr, p, sizeof out.addr.udp6.addr);
            return Message_epush(msg, &out, sizeof out);
        }
    }
    out.addr.other.type = Control_LlAddr_Other_TYPE;
    out.addr.other.len = sizeof(Control_LlAddr_Other_t);
    unsigned int len = sa->addrLen;
    if (len > sizeof out.addr.other.sockaddrHeader) {
        len = sizeof out.addr.other.sockaddrHeader;
    }
    Bits_memcpy(&out.addr.other.sockaddrHeader, &sa, len);
    return Message_epush(msg, &out, sizeof out);
}

/**
 * Expects [ SwitchHeader ][ Ctrl ][ LlAddr ]
 */
#define handleLlAddrQuery_MIN_SIZE (Control_Header_SIZE + Control_LlAddr_HEADER_SIZE)
static Iface_DEFUN handleLlAddrQuery(Message_t* msg,
                                     struct ControlHandler_pvt* ch,
                                     uint64_t label,
                                     uint8_t* labelStr)
{
    Log_debug(ch->log, "Incoming LLADDR query");
    if (Message_getLength(msg) < handleLlAddrQuery_MIN_SIZE) {
        Log_info(ch->log, "DROP runt LLADDR query");
        return Error(msg, "RUNT");
    }

    struct Control_Header hdr;
    Err(Message_epop(msg, &hdr, sizeof hdr));

    struct Control_LlAddr lla;
    Err(Message_epop(msg, &lla, sizeof lla));

    if (lla.magic != Control_LlAddr_QUERY_MAGIC) {
        Log_debug(ch->log, "DROP LLADDR query (bad magic)");
        return Error(msg, "INVALID");
    }

    // If this is not a one-hop packet, it's invalid
    if (!EncodingScheme_isOneHop(ch->ourEncodingScheme, label)) {
        Log_debug(ch->log, "DROP LLADDR query from non-peer");
        return Error(msg, "INVALID");
    }

    // Get the lladdr from the peer who sent the message
    struct Sockaddr* sa = InterfaceController_getPeerLlAddr(
        ch->ifc, Message_getAlloc(msg), label);
    if (!sa) {
        Log_info(ch->log, "LLADDR query peer not found");
        return Error(msg, "INTERNAL");
    }

    Err(writeLlAddr(msg, sa));

    hdr.type_be = Control_LlAddr_REPLY_be;
    hdr.checksum_be = 0;
    Err(Message_epush(msg, &hdr, sizeof hdr));
    hdr.checksum_be = Checksum_engine_be(Message_bytes(msg), Message_getLength(msg));
    Err(Message_epop(msg, NULL, sizeof hdr));
    Err(Message_epush(msg, &hdr, sizeof hdr));

    struct RouteHeader routeHeader = {
        .sh.label_be = Endian_hostToBigEndian64(label),
        .flags = RouteHeader_flags_CTRLMSG,
    };
    SwitchHeader_setVersion(&routeHeader.sh, SwitchHeader_CURRENT_VERSION);

    Err(Message_epush(msg, &routeHeader, sizeof routeHeader));

    return Iface_next(&ch->pub.coreIf, msg);
}

/**
 * Expects [ SwitchHeader ][ Ctrl ][ SupernodeQuery ][ data etc.... ]
 */
#define handleGetSnodeQuery_MIN_SIZE (Control_Header_SIZE + Control_GetSnode_HEADER_SIZE)
static Iface_DEFUN handleGetSnodeQuery(Message_t* msg,
                                       struct ControlHandler_pvt* ch,
                                       uint64_t label,
                                       uint8_t* labelStr)
{
    Log_debug(ch->log, "incoming getSupernode query");
    if (Message_getLength(msg) < handleGetSnodeQuery_MIN_SIZE) {
        Log_info(ch->log, "DROP runt getSupernode query");
        return Error(msg, "RUNT");
    }

    struct Control* ctrl = (struct Control*) Message_bytes(msg);
    struct Control_GetSnode* snq = &ctrl->content.getSnode;

    if (snq->magic != Control_GETSNODE_QUERY_MAGIC) {
        Log_debug(ch->log, "DROP getSupernode query (bad magic)");
        return Error(msg, "INVALID");
    }

    uint32_t herVersion = Endian_bigEndianToHost32(snq->version_be);
    if (!Version_compatibleWithCurrent(herVersion)) {
        Log_debug(ch->log, "DROP getSupernode query from incompatible version [%d]", herVersion);
        // Nothing wrong with the query but we're just not going to answer it
        return NULL;
    }

    ctrl->header.type_be = Control_GETSNODE_REPLY_be;
    snq->kbps_be = 0xffffffff;
    snq->version_be = Endian_hostToBigEndian32(Version_CURRENT_PROTOCOL);
    snq->magic = Control_GETSNODE_REPLY_MAGIC;
    uint64_t fixedLabel = 0;
    if (ch->activeSnode.path) {
        fixedLabel = NumberCompress_getLabelFor(ch->activeSnode.path, label);
        uint64_t fixedLabel_be = Endian_hostToBigEndian64(fixedLabel);
        Bits_memcpy(snq->pathToSnode_be, &fixedLabel_be, 8);
        Bits_memcpy(snq->snodeKey, ch->activeSnode.key, 32);
        snq->snodeVersion_be = Endian_hostToBigEndian32(ch->activeSnode.protocolVersion);

    } else {
        Log_debug(ch->log, "getSupernode query and we do not have a known snode");
        snq->snodeVersion_be = 0;
        Bits_memset(snq->pathToSnode_be, 0, 8);
        Bits_memset(snq->snodeKey, 0, 32);
    }

    ctrl->header.checksum_be = 0;
    ctrl->header.checksum_be = Checksum_engine_be(Message_bytes(msg), Message_getLength(msg));

    Err(Message_eshift(msg, RouteHeader_SIZE));
    struct RouteHeader* routeHeader = (struct RouteHeader*) Message_bytes(msg);
    Bits_memset(routeHeader, 0, RouteHeader_SIZE);
    SwitchHeader_setVersion(&routeHeader->sh, SwitchHeader_CURRENT_VERSION);
    routeHeader->sh.label_be = Endian_hostToBigEndian64(label);
    routeHeader->flags |= RouteHeader_flags_CTRLMSG;

    Log_debug(ch->log, "Replied to [%" PRIx64 "] with snode [%" PRIx64 "] aka [%" PRIx64 "]",
        label, fixedLabel, ch->activeSnode.path);

    return Iface_next(&ch->pub.coreIf, msg);
}

/**
 * Handle an incoming control message from a switch.
 *
 * @param context the ducttape context.
 * @param message the control message, this should be alligned on the beginning of the content,
 *                that is to say, after the end of the switch header.
 * @param switchHeader the header.
 * @param switchIf the interface which leads to the switch.
 * @param isFormV8 true if the control message is in the form specified by protocol version 8+
 */
static Iface_DEFUN incomingFromCore(Message_t* msg, struct Iface* coreIf)
{
    struct ControlHandler_pvt* ch = Identity_check((struct ControlHandler_pvt*) coreIf);

    struct RouteHeader routeHdr;
    Err(Message_epop(msg, &routeHdr, RouteHeader_SIZE));
    uint8_t labelStr[20];
    uint64_t label = Endian_bigEndianToHost64(routeHdr.sh.label_be);
    AddrTools_printPath(labelStr, label);
    // happens in benchmark
    // Log_debug(ch->log, "ctrl packet from [%s]", labelStr);

    if (Message_getLength(msg) < 4 + Control_Header_SIZE) {
        Log_info(ch->log, "DROP runt ctrl packet from [%s]", labelStr);
        return Error(msg, "RUNT");
    }

    Assert_true(routeHdr.flags & RouteHeader_flags_CTRLMSG);

    if (Checksum_engine_be(Message_bytes(msg), Message_getLength(msg))) {
        Log_info(ch->log, "DROP ctrl packet from [%s] with invalid checksum", labelStr);
        return Error(msg, "INVALID");
    }

    struct Control* ctrl = (struct Control*) Message_bytes(msg);

    if (ctrl->header.type_be == Control_ERROR_be) {
        return handleError(msg, ch, label, labelStr, &routeHdr);

    } else if (ctrl->header.type_be == Control_KEYPING_be
            || ctrl->header.type_be == Control_PING_be)
    {
        return handlePing(msg, ch, label, labelStr, ctrl->header.type_be);

    } else if (ctrl->header.type_be == Control_KEYPONG_be
            || ctrl->header.type_be == Control_PONG_be
            || ctrl->header.type_be == Control_LlAddr_REPLY_be)
    {
        Log_debug(ch->log, "got switch pong [%d] from [%s]",
            ctrl->header.type_be, labelStr);
        Err(Message_epush(msg, &routeHdr, RouteHeader_SIZE));
        return Iface_next(&ch->pub.switchPingerIf, msg);

    } else if (ctrl->header.type_be == Control_GETSNODE_QUERY_be) {
        return handleGetSnodeQuery(msg, ch, label, labelStr);

    } else if (ctrl->header.type_be == Control_GETSNODE_REPLY_be
            || ctrl->header.type_be == Control_RPATH_REPLY_be)
    {
        Log_debug(ch->log, "got %s REPLY from [%s]",
            (ctrl->header.type_be == Control_GETSNODE_REPLY_be) ? "GETSNODE" : "RPATH",
            labelStr);
        Err(Message_epush(msg, &routeHdr, RouteHeader_SIZE));
        Err(Message_epush32be(msg, 0xffffffff));
        Err(Message_epush32be(msg, PFChan_Core_CTRL_MSG));
        return Iface_next(&ch->eventIf, msg);

    } else if (ctrl->header.type_be == Control_RPATH_QUERY_be) {
        return handleRPathQuery(msg, ch, label, labelStr);

    } else if (ctrl->header.type_be == Control_LlAddr_QUERY_be) {
        return handleLlAddrQuery(msg, ch, label, labelStr);
    }

    Log_info(ch->log, "DROP control packet of unknown type from [%s], type [%d]",
             labelStr, Endian_bigEndianToHost16(ctrl->header.type_be));

    return Error(msg, "INVALID");
}

// Forward from switch pinger directly to core.
static Iface_DEFUN incomingFromSwitchPinger(Message_t* msg, struct Iface* switchPingerIf)
{
    struct ControlHandler_pvt* ch =
        Identity_containerOf(switchPingerIf, struct ControlHandler_pvt, pub.switchPingerIf);
    return Iface_next(&ch->pub.coreIf, msg);
}

static Iface_DEFUN changeSnode(Message_t* msg, struct Iface* eventIf)
{
    struct ControlHandler_pvt* ch =
        Identity_containerOf(eventIf, struct ControlHandler_pvt, eventIf);
    enum PFChan_Pathfinder ev = 0;
    Err(Message_epop32be(&ev, msg));
    Assert_true(ev == PFChan_Pathfinder_SNODE);
    uint32_t discard = 0;
    Err(Message_epop32be(&discard, msg));

    struct PFChan_Node node;
    Err(Message_epop(msg, &node, PFChan_Node_SIZE));
    Assert_true(!Message_getLength(msg));

    uint64_t path = Endian_bigEndianToHost64(node.path_be);
    uint32_t protocolVersion = Endian_bigEndianToHost32(node.version_be);

    const char* log = NULL;
    if (!ch->activeSnode.path) {
        if (node.path_be) {
            log = "Found snode";
        } else {
            // didn't know the snode before, still don't
            return NULL;
        }
    } else if (!node.path_be) {
        // We had one, now we don't
        log = "Removing snode";
    } else {
        if (Bits_memcmp(ch->activeSnode.key, node.publicKey, 32)) {
            log = "Changing snode";
        } else if (path != ch->activeSnode.path) {
            log = "Changing snode path";
        } else if (ch->activeSnode.protocolVersion != protocolVersion) {
            log = "Changing snode protocolVersion";
        } else {
            // Nothing has changed
            return NULL;
        }
    }

    struct Address old = {
        .protocolVersion = ch->activeSnode.protocolVersion,
        .path = ch->activeSnode.path,
    };
    Bits_memcpy(old.key, ch->activeSnode.key, 32);

    Bits_memcpy(ch->activeSnode.key, node.publicKey, 32);
    ch->activeSnode.path = path;
    ch->activeSnode.protocolVersion = protocolVersion;

    struct Address addr = { .protocolVersion = protocolVersion, .path = path, };
    Bits_memcpy(addr.key, node.publicKey, 32);

    Log_debug(ch->log, "%s [%s] -> [%s]", log,
        Address_toStringKey(&old, Message_getAlloc(msg))->bytes,
        Address_toStringKey(&addr, Message_getAlloc(msg))->bytes);

    return NULL;
}

struct ControlHandler* ControlHandler_new(struct Allocator* allocator,
                                          struct Log* logger,
                                          struct EventEmitter* ee,
                                          uint8_t myPublicKey[32],
                                          struct InterfaceController* ifc)
{
    struct Allocator* alloc = Allocator_child(allocator);
    struct ControlHandler_pvt* ch = Allocator_calloc(alloc, sizeof(struct ControlHandler_pvt), 1);
    ch->ourEncodingScheme = NumberCompress_defineScheme(alloc);
    ch->alloc = alloc;
    ch->log = logger;
    ch->ifc = ifc;
    Bits_memcpy(ch->myPublicKey, myPublicKey, 32);
    ch->pub.coreIf.send = incomingFromCore;
    ch->pub.switchPingerIf.send = incomingFromSwitchPinger;
    ch->eventIf.send = changeSnode;
    EventEmitter_regCore(ee, &ch->eventIf, PFChan_Pathfinder_SNODE);
    Identity_set(ch);
    return &ch->pub;
}
