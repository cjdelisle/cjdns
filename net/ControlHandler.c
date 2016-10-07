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
#include "net/ControlHandler.h"
#include "util/Identity.h"
#include "util/AddrTools.h"
#include "util/Checksum.h"
#include "wire/Control.h"
#include "wire/Error.h"

struct ControlHandler_pvt
{
    struct ControlHandler pub;
    struct Log* log;
    struct Allocator* alloc;
    struct Router* router;
    uint8_t myPublicKey[32];
    struct Iface eventIf;
    Identity
};

/**
 * Expects [ Ctrl ][ Error ][ cause SwitchHeader ][ cause handle ][ cause etc.... ]
 */
#define handleError_MIN_SIZE (Control_Header_SIZE + Control_Error_MIN_SIZE + SwitchHeader_SIZE + 4)
static Iface_DEFUN handleError(struct Message* msg,
                               struct ControlHandler_pvt* ch,
                               uint64_t label,
                               uint8_t* labelStr)
{
    if (msg->length < handleError_MIN_SIZE) {
        Log_info(ch->log, "DROP runt error packet from [%s]", labelStr);
        return NULL;
    }
    Message_shift(msg, SwitchHeader_SIZE + 4, NULL);
    Message_push32(msg, 0xffffffff, NULL);
    Message_push32(msg, PFChan_Core_SWITCH_ERR, NULL);
    return Iface_next(&ch->eventIf, msg);
}

/**
 * Expects [ SwitchHeader ][ Ctrl ][ (key)Ping ][ data etc.... ]
 */
#define handlePing_MIN_SIZE (Control_Header_SIZE + Control_Ping_MIN_SIZE)
static Iface_DEFUN handlePing(struct Message* msg,
                              struct ControlHandler_pvt* ch,
                              uint64_t label,
                              uint8_t* labelStr,
                              uint16_t messageType_be)
{
    if (msg->length < handlePing_MIN_SIZE) {
        Log_info(ch->log, "DROP runt ping");
        return NULL;
    }

    struct Control* ctrl = (struct Control*) msg->bytes;
    Message_shift(msg, -Control_Header_SIZE, NULL);

    // Ping and keyPing share version location
    struct Control_Ping* ping = (struct Control_Ping*) msg->bytes;
    uint32_t herVersion = Endian_bigEndianToHost32(ping->version_be);
    if (!Version_isCompatible(Version_CURRENT_PROTOCOL, herVersion)) {
        Log_debug(ch->log, "DROP ping from incompatible version [%d]", herVersion);
        return NULL;
    }

    if (messageType_be == Control_KEYPING_be) {
        Log_debug(ch->log, "got switch keyPing from [%s]", labelStr);
        if (msg->length < Control_KeyPing_HEADER_SIZE) {
            // min keyPing size is longer
            Log_debug(ch->log, "DROP runt keyPing");
            return NULL;
        }
        if (msg->length > Control_KeyPing_MAX_SIZE) {
            Log_debug(ch->log, "DROP long keyPing");
            return NULL;
        }
        if (ping->magic != Control_KeyPing_MAGIC) {
            Log_debug(ch->log, "DROP keyPing (bad magic)");
            return NULL;
        }

        struct Control_KeyPing* keyPing = (struct Control_KeyPing*) msg->bytes;
        keyPing->magic = Control_KeyPong_MAGIC;
        ctrl->header.type_be = Control_KEYPONG_be;
        Bits_memcpy(keyPing->key, ch->myPublicKey, 32);

    } else if (messageType_be == Control_PING_be) {
        Log_debug(ch->log, "got switch ping from [%s]", labelStr);
        if (ping->magic != Control_Ping_MAGIC) {
            Log_debug(ch->log, "DROP ping (bad magic)");
            return NULL;
        }
        ping->magic = Control_Pong_MAGIC;
        ctrl->header.type_be = Control_PONG_be;

    } else {
        Assert_failure("2+2=5");
    }

    ping->version_be = Endian_hostToBigEndian32(Version_CURRENT_PROTOCOL);

    Message_shift(msg, Control_Header_SIZE, NULL);

    ctrl->header.checksum_be = 0;
    ctrl->header.checksum_be = Checksum_engine(msg->bytes, msg->length);

    Message_push32(msg, 0xffffffff, NULL);

    Message_shift(msg, SwitchHeader_SIZE, NULL);

    struct SwitchHeader* switchHeader = (struct SwitchHeader*) msg->bytes;
    Bits_memset(switchHeader, 0, SwitchHeader_SIZE);
    SwitchHeader_setVersion(switchHeader, SwitchHeader_CURRENT_VERSION);
    switchHeader->label_be = Endian_hostToBigEndian64(label);

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
static Iface_DEFUN incomingFromCore(struct Message* msg, struct Iface* coreIf)
{
    struct ControlHandler_pvt* ch = Identity_check((struct ControlHandler_pvt*) coreIf);

    struct SwitchHeader switchHeader;
    Message_pop(msg, &switchHeader, SwitchHeader_SIZE, NULL);
    uint8_t labelStr[20];
    uint64_t label = Endian_bigEndianToHost64(switchHeader.label_be);
    AddrTools_printPath(labelStr, label);
    Log_debug(ch->log, "ctrl packet from [%s]", labelStr);

    if (msg->length < 4 + Control_Header_SIZE) {
        Log_info(ch->log, "DROP runt ctrl packet from [%s]", labelStr);
        return NULL;
    }

    Assert_true(Message_pop32(msg, NULL) == 0xffffffff);

    if (Checksum_engine(msg->bytes, msg->length)) {
        Log_info(ch->log, "DROP ctrl packet from [%s] with invalid checksum", labelStr);
        return NULL;
    }

    struct Control* ctrl = (struct Control*) msg->bytes;

    if (ctrl->header.type_be == Control_ERROR_be) {
        return handleError(msg, ch, label, labelStr);

    } else if (ctrl->header.type_be == Control_KEYPING_be
            || ctrl->header.type_be == Control_PING_be)
    {
        return handlePing(msg, ch, label, labelStr, ctrl->header.type_be);

    } else if (ctrl->header.type_be == Control_KEYPONG_be
            || ctrl->header.type_be == Control_PONG_be)
    {
        Log_debug(ch->log, "got switch pong from [%s]", labelStr);
        // Shift back over the header
        Message_shift(msg, 4 + SwitchHeader_SIZE, NULL);
        return Iface_next(&ch->pub.switchPingerIf, msg);
    }

    Log_info(ch->log, "DROP control packet of unknown type from [%s], type [%d]",
             labelStr, Endian_bigEndianToHost16(ctrl->header.type_be));

    return NULL;
}

// Forward from switch pinger directly to core.
static Iface_DEFUN incomingFromSwitchPinger(struct Message* msg, struct Iface* switchPingerIf)
{
    struct ControlHandler_pvt* ch =
        Identity_containerOf(switchPingerIf, struct ControlHandler_pvt, pub.switchPingerIf);
    return Iface_next(&ch->pub.coreIf, msg);
}

struct ControlHandler* ControlHandler_new(struct Allocator* allocator,
                                          struct Log* logger,
                                          struct EventEmitter* ee,
                                          uint8_t myPublicKey[32])
{
    struct Allocator* alloc = Allocator_child(allocator);
    struct ControlHandler_pvt* ch = Allocator_calloc(alloc, sizeof(struct ControlHandler_pvt), 1);
    ch->alloc = alloc;
    ch->log = logger;
    Bits_memcpy(ch->myPublicKey, myPublicKey, 32);
    ch->pub.coreIf.send = incomingFromCore;
    ch->pub.switchPingerIf.send = incomingFromSwitchPinger;
    EventEmitter_regCore(ee, &ch->eventIf, 0);
    Identity_set(ch);
    return &ch->pub;
}
