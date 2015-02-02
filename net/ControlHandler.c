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

    struct SwitchHeader* switchHeader = (struct SwitchHeader*) msg->bytes;
    Assert_true(msg->length >= SwitchHeader_SIZE);
    uint8_t labelStr[20];
    uint64_t label = Endian_bigEndianToHost64(switchHeader->label_be);
    AddrTools_printPath(labelStr, label);
    Log_debug(ch->logger, "ctrl packet from [%s]", labelStr);

    if (msg->length < SwitchHeader_SIZE + 4 + Control_HEADER_SIZE) {
        Log_info(ch->logger, "DROP runt ctrl packet from [%s]", labelStr);
        return Error_NONE;
    }

    Message_shift(msg, -SwitchHeader_SIZE, NULL);
    Assert_true(Message_pop32(msg, NULL) == 0xffffffff);
    struct Control* ctrl = (struct Control*) msg->bytes;

    if (Checksum_engine(msg->bytes, msg->length)) {
        Log_info(ch->logger, "DROP ctrl packet from [%s] with invalid checksum", labelStr);
        return Error_NONE;
    }

    bool pong = false;
    if (ctrl->type_be == Control_ERROR_be) {
        if (msg->length < Control_Error_MIN_SIZE) {
            Log_info(ch->logger, "DROP runt error packet from [%s]", labelStr);
            return Error_NONE;
        }

        uint64_t path = Endian_bigEndianToHost64(switchHeader->label_be);
        if (!NumberCompress_isOneHop(path)) {
            uint64_t labelAtStop = Endian_bigEndianToHost64(ctrl->content.error.cause.label_be);
            Router_brokenLink(ch->router, path, labelAtStop);
        }

        if (ctrl->content.error.causeHandle == 0xffffffff) {
            // Determine whether the "cause" packet is a control message.
            if (message->length < Control_Error_MIN_SIZE + Control_HEADER_SIZE) {
                Log_info(context->logger,
                         "error packet from [%s] containing runt cause packet",
                         labelStr);
                return Error_NONE;
            }
            struct Control* causeCtrl = (struct Control*) &(&ctrl->content.error.cause)[1];
            if (causeCtrl->type_be != Control_PING_be && causeCtrl->type_be != Control_KEYPING_be) {
                #ifdef Log_INFO
                    uint32_t errorType =
                        Endian_bigEndianToHost32(ctrl->content.error.errorType_be);
                    Log_info(context->logger,
                              "error packet from [%s] caused by [%s] packet ([%s])",
                              labelStr,
                              Control_typeString(causeCtrl->type_be),
                              Error_strerror(errorType));
                #endif
            } else {
                if (LabelSplicer_isOneHop(label)
                    && ctrl->content.error.errorType_be
                        == Endian_hostToBigEndian32(Error_UNDELIVERABLE))
                {
                    // this is our own InterfaceController complaining
                    // because the node isn't responding to pings.
                    return Error_NONE;
                }
                Log_debug(context->logger,
                           "error packet from [%s] in response to ping, err [%u], length: [%u].",
                           labelStr,
                           Endian_bigEndianToHost32(ctrl->content.error.errorType_be),
                           message->length);
                // errors resulting from pings are forwarded back to the pinger.
                pong = true;
            }
        } else {
            uint32_t errorType = Endian_bigEndianToHost32(ctrl->content.error.errorType_be);
            if (errorType != Error_RETURN_PATH_INVALID) {
                // Error_RETURN_PATH_INVALID is impossible to prevent so will appear all the time.
                Log_info(context->logger,
                         "error packet from [%s] [%s]",
                         labelStr,
                         Error_strerror(errorType));
            }
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
