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

struct ControlHandler
{
    /** This interface expects and sends [ SwitchHeader ][ 0xffffffff ][ CTRL frame ] */
    struct Interface_Two coreIf;

    /**
     * This interface expects and sends [ SwitchHeader ][ 0xffffffff ][ CTRL frame ]
     * May send a pong or an error caused by a ping.
     */
    struct Interface_Two switchPingerIf;
};

struct ControlHandler_pvt
{
    struct ControlHandler pub;
    struct Log* log;
    struct Allocator* alloc;
    struct Router* router;
    Identity
};

/**
 * Expects [ Ctrl ][ Error ][ cause SwitchHeader ][ cause handle ][ cause etc.... ]
 */
#define handleError_MIN_SIZE (Control_HEADER_SIZE + Control_Error_MIN_SIZE + SwitchHeader_SIZE + 4)
static int handleError(struct ControlHandler_pvt* ch,
                       struct Message* msg,
                       uint64_t label,
                       uint8_t* labelStr)
{
    if (msg->length < handleError_MIN_SIZE) {
        Log_info(ch->logger, "DROP runt error packet from [%s]", labelStr);
        return 0;
    }
    struct Control* ctrl = (struct Control*) msg->bytes;
    struct Control_Error* err = ctrl->content.error;
    struct SwitchHeader* causeHeader = &err->cause;
    uint64_t causeLabel = Endian_bigEndianToHost64(causeHeader->label_be);
    uint32_t errorType = Endian_bigEndianToHost32(err->errorType_be);

    if (!NumberCompress_isOneHop(label)) {
        Router_brokenLink(ch->router, label, causeLabel);
    } else (errorType == Error_UNDELIVERABLE) {
        // this is our own InterfaceController complaining
        // because the node isn't responding to pings.
        return 0;
    }

    Log_debug(context->logger,
              "error packet from [%s] [%s]%s",
              labelStr,
              Error_strerror(errorType),
              ((err->causeHandle == 0xffffffff) ? " caused by ctrl" : ""));

    return 0;
}

/**
 * Expects [ SwitchHeader ][ Ctrl ][ (key)Ping ][ data etc.... ]
 */
#define handlePing_MIN_SIZE (Control_HEADER_SIZE + Control_Ping_MIN_SIZE)
static int handlePing(struct ControlHandler_pvt* ch,
                      struct Message* msg,
                      uint64_t label,
                      uint8_t* labelStr,
                      uint16_t messageType_be)
{
    if (msg->length < handlePing_MIN_SIZE) {
        Log_info(ch->log, "DROP runt ping");
        return 0;
    }

    struct Control* ctrl = (struct Control*) msg->bytes;
    Message_shift(msg, -Control_HEADER_SIZE, NULL);

    // Ping and keyPing share version location
    struct Control_Ping* ping = (struct Control_Ping*) message->bytes;
    uint32_t herVersion = Endian_bigEndianToHost32(ping->version_be);
    if (!Version_isCompatible(Version_CURRENT_PROTOCOL, herVersion)) {
        Log_debug(ch->log, "DROP ping from incompatible version [%s]", herVersion);
        return 0;
    }

    if (messageType_be == Control_KEYPING_be) {
        if (msg->length < Control_KeyPing_HEADER_SIZE) {
            // min keyPing size is longer
            Log_debug(ch->log, "DROP runt keyPing");
            return 0;
        }
        if (msg->length > Control_KeyPing_MAX_SIZE) {
            Log_debug(ch->log, "DROP long keyPing");
            return 0;
        }
        if (ping->magic != Control_KeyPing_MAGIC) {
            Log_debug(ch->log, "DROP keyPing (bad magic)");
            return 0;
        }

        struct Control_KeyPing* keyPing = (struct Control_KeyPing*) msg->bytes;
        keyPing->magic = Control_KeyPong_MAGIC;
        ctrl->type_be = Control_KEYPONG_be;
        Bits_memcpyConst(keyPing->key, ch->myAddr.key, 32);

    } else if (messageType_be == Control_PING_be) {
        if (ping->magic != Control_Ping_MAGIC) {
            Log_debug(ch->log, "DROP ping (bad magic)");
            return 0;
        }
        ping->magic = Control_Pong_MAGIC;
        ctrl->type_be = Control_PONG_be;

    } else {
        Assert_failure("2+2=5");
    }

    ping->version_be = Endian_hostToBigEndian32(Version_CURRENT_PROTOCOL);

    Message_shift(message, Control_HEADER_SIZE, NULL);

    ctrl->checksum_be = 0;
    ctrl->checksum_be = Checksum_engine(message->bytes, message->length);

    Message_shift(message, SwitchHeader_SIZE, NULL);

        Log_debug(context->logger, "got switch ping from [%s]", labelStr);
        SwitchHeader_setLabelShift(switchHeader, 0);
        SwitchHeader_setCongestion(switchHeader, 0);
        Interface_receiveMessage(switchIf, message);
//////

        Message_shift(message, -Control_HEADER_SIZE, NULL);

        if (message->length < Control_KeyPing_HEADER_SIZE
            || message->length > Control_KeyPing_MAX_SIZE)
        {
            Log_info(context->logger, "DROP incorrect size keyping");
            return Error_INVALID;
        }

        struct Control_KeyPing* keyPing = (struct Control_KeyPing*) message->bytes;

        #ifdef Log_DEBUG
            struct Address herAddr = {
                .protocolVersion = Endian_bigEndianToHost32(keyPing->version_be),
                .path = label
            };
            Bits_memcpyConst(herAddr.key, keyPing->key, 32);
            String* addrStr = Address_toString(&herAddr, message->alloc);
            Log_debug(context->logger, "got switch keyPing from [%s]", addrStr->bytes);
        #endif

        keyPing->magic = Control_KeyPong_MAGIC;
        uint32_t herVersion = Endian_bigEndianToHost32(keyPing->version_be);
        keyPing->version_be = Endian_hostToBigEndian32(Version_CURRENT_PROTOCOL);
        Bits_memcpyConst(keyPing->key, context->myAddr.key, 32);
        Message_shift(message, Control_HEADER_SIZE, NULL);

        ctrl->type_be = Control_KEYPONG_be;
        ctrl->checksum_be = 0;
        ctrl->checksum_be = Checksum_engine(message->bytes, message->length);

        Message_shift(message, 4, NULL);
        Assert_true(((uint32_t*)message->bytes)[0] == 0xffffffff);

        Message_shift(message, SwitchHeader_SIZE, NULL);
        SwitchHeader_setLabelShift(switchHeader, 0);
        SwitchHeader_setCongestion(switchHeader, 0);
        Interface_receiveMessage(switchIf, message);

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
static int incomingFromCore(struct Interface_Two* coreIf, struct Message* msg)
{
    struct ControlHandler_pvt* ch = Identity_check((struct ControlHandler_pvt*) coreIf);

    struct SwitchHeader switchHeader;
    Message_pop(msg, &switchHeader, SwitchHeader_SIZE, NULL);
    uint8_t labelStr[20];
    uint64_t label = Endian_bigEndianToHost64(switchHeader.label_be);
    AddrTools_printPath(labelStr, label);
    Log_debug(ch->logger, "ctrl packet from [%s]", labelStr);

    if (msg->length < SwitchHeader_SIZE + 4 + Control_HEADER_SIZE) {
        Log_info(ch->logger, "DROP runt ctrl packet from [%s]", labelStr);
        return Error_NONE;
    }

    Assert_true(Message_pop32(msg, NULL) == 0xffffffff);

    if (Checksum_engine(msg->bytes, msg->length)) {
        Log_info(ch->logger, "DROP ctrl packet from [%s] with invalid checksum", labelStr);
        return Error_NONE;
    }

    struct Control* ctrl = (struct Control*) msg->bytes;

    switch (ctrl->type_be) {
        case Control_ERROR_be: return handleError(ch, msg, label, labelStr);

        case Control_KEYPING_be:
        case Control_PING_be: return handlePing(ch, msg, label, labelStr, ctrl->type_be);

        case Control_KEYPONG_be:
        case Control_PONG_be: return handlePong(ch, msg, label, labelStr, ctrl->type_be);
    }

    } else if (ctrl->type_be == Control_PONG_be) {
        pong = true;
    } else if (ctrl->type_be == Control_PING_be) {

        Message_shift(message, -Control_HEADER_SIZE, NULL);

        if (message->length < Control_Ping_MIN_SIZE) {
            Log_info(context->logger, "DROP runt ping");
            return Error_INVALID;
        }
        struct Control_Ping* ping = (struct Control_Ping*) message->bytes;
        uint32_t herVersion = Endian_bigEndianToHost32(ping->version_be);
        ping->magic = Control_Pong_MAGIC;
        ping->version_be = Endian_hostToBigEndian32(Version_CURRENT_PROTOCOL);
        Message_shift(message, Control_HEADER_SIZE, NULL);

        ctrl->type_be = Control_PONG_be;
        ctrl->checksum_be = 0;
        ctrl->checksum_be = Checksum_engine(message->bytes, message->length);

        if (isFormV8) {
            Message_shift(message, 4, NULL);
            Assert_true(((uint32_t*)message->bytes)[0] == 0xffffffff);
        } else if (herVersion >= 8) {
            changeToVersion8(message);
        }
        Message_shift(message, SwitchHeader_SIZE, NULL);

        Log_debug(context->logger, "got switch ping from [%s]", labelStr);
        SwitchHeader_setLabelShift(switchHeader, 0);
        SwitchHeader_setCongestion(switchHeader, 0);
        Interface_receiveMessage(switchIf, message);

    } else if (ctrl->type_be == Control_KEYPONG_be) {
        pong = true;
    } else if (ctrl->type_be == Control_KEYPING_be) {

        Message_shift(message, -Control_HEADER_SIZE, NULL);

        if (message->length < Control_KeyPing_HEADER_SIZE
            || message->length > Control_KeyPing_MAX_SIZE)
        {
            Log_info(context->logger, "DROP incorrect size keyping");
            return Error_INVALID;
        }

        struct Control_KeyPing* keyPing = (struct Control_KeyPing*) message->bytes;

        #ifdef Log_DEBUG
            struct Address herAddr = {
                .protocolVersion = Endian_bigEndianToHost32(keyPing->version_be),
                .path = label
            };
            Bits_memcpyConst(herAddr.key, keyPing->key, 32);
            String* addrStr = Address_toString(&herAddr, message->alloc);
            Log_debug(context->logger, "got switch keyPing from [%s]", addrStr->bytes);
        #endif

        keyPing->magic = Control_KeyPong_MAGIC;
        uint32_t herVersion = Endian_bigEndianToHost32(keyPing->version_be);
        keyPing->version_be = Endian_hostToBigEndian32(Version_CURRENT_PROTOCOL);
        Bits_memcpyConst(keyPing->key, context->myAddr.key, 32);
        Message_shift(message, Control_HEADER_SIZE, NULL);

        ctrl->type_be = Control_KEYPONG_be;
        ctrl->checksum_be = 0;
        ctrl->checksum_be = Checksum_engine(message->bytes, message->length);

        Message_shift(message, 4, NULL);
        Assert_true(((uint32_t*)message->bytes)[0] == 0xffffffff);

        Message_shift(message, SwitchHeader_SIZE, NULL);
        SwitchHeader_setLabelShift(switchHeader, 0);
        SwitchHeader_setCongestion(switchHeader, 0);
        Interface_receiveMessage(switchIf, message);

    } else {
        Log_info(context->logger,
                  "DROP control packet of unknown type from [%s], type [%d]",
                  labelStr, Endian_bigEndianToHost16(ctrl->type_be));
    }

    if (pong) {
        Log_debug(context->logger, "got switch pong from [%s]", labelStr);
        // Shift back over the header
        Message_shift(message, 4 + SwitchHeader_SIZE, NULL);
        return Interface_send(&context->pub.switchPingerIf, message);
    }
    return Error_NONE;
}

// Forward from switch pinger directly to core.
static int incomingFromSwitchPinger(struct Interface_Two* switchPingerIf, struct Message* msg)
{
    struct ControlHandler_pvt* ch =
        Identity_check(&((struct ControlHandler_pvt*) switchPingerIf)[-1]);
    return Interface_send(ch->coreIf, msg);
}

struct ControlHandler* ControlHandler_new(struct Allocator* alloc,
                                          struct Log* logger,
                                          struct Router* router)
{
    struct ControlHandler_pvt* ch = Allocator_calloc(alloc, sizeof(struct ControlHandler_pvt), 1);
    ch->alloc = alloc;
    ch->log = logger;
    ch->router = router;
    ch->pub.coreIf.send = incomingFromCore;
    ch->pub.switchPoingerIf.send = incomingFromSwitchPinger;
}
