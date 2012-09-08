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
#ifndef AngelChan_H
#define AngelChan_H

#include "util/Assert.h"

#include <stdint.h>

/*       *Channels* between angel and core process
 *
 * Channels are identified by number; they are opened by a non empty message,
 * and closed by an empty message. A close on an open channel must be ACKed with
 * an empty message before it can be reopened.
 *
 * Channels are in one of three states: OPEN, CLOSED, WAIT_FOR_CLOSE
 *  - OPEN:
 *    + on empty message: send close, -> CLOSED
 *    + <close>: send close -> WAIT_FOR_CLOSE
 *    + <send data>: send the non-empty data
 *  - CLOSED (start state):
 *    + ignore empty messages
 *    + on non empty message: -> OPEN
 *    + <send data>: send the non-empty data -> OPEN
 *  - WAIT_FOR_CLOSE
 *    + ignore non empty messages (optionally send close again if this channel is invalid)
 *    + on empty message: -> CLOSED
 *    + CANNOT <send data> on this channel
 *
 * A channel is a byte stream; the message framing must not be used to identify
 * frames in the byte stream of a channel.
 * (Right now bencoding messages is the way to go)
 *
 * If one end doesn't want to handle a channel, it responds with an immediate close
 * on non empty messages, but it drops empty messages; such "invalid" channel are
 * never in the OPEN state.
 *
 * For valid channels (channels that are not closed immediately on open) you
 * have to keep track of the WAIT_FOR_CLOSE (and OPEN) state, but you can free all
 * data in the CLOSED state.
 *
 * By not ACKing a close message you force the other end to stay in the WAIT_FOR_CLOSE
 * state (and block reopening the channel).
 */
#pragma pack(4)
struct AngelChan_MessageHeader
{
    /**
     * Magic bytes which are sent at the beginning of
     * each frame to make sure the connection is still synchronized.
     * This is randomly generated at process startup.
     */
    uint64_t syncMagic;

    /**
     * length of the message data following the header
     * zero length: close(d) communication channel
     */
    uint32_t length;

    /**
     * channel types:
     *   0x00000000 - 0x0000ffff: admin connections
     *      data exchanged: continous byte stream to/from the tcp connection
     *      byte stream represents requests/responses;
     *      each request/response is bencoded
     */
    uint32_t channelNum;
};
#define AngelChan_MessageHeader_SIZE 16
Assert_compileTime(sizeof(struct AngelChan_MessageHeader) == AngelChan_MessageHeader_SIZE);


/* zero state ("default") is CLOSED */
enum AngelChan_ChannelState {
    AngelChan_ChannelState_OPEN = 1,
    AngelChan_ChannelState_CLOSED = 0,
    AngelChan_ChannelState_WAIT_FOR_CLOSE = 2
};

/* maximum message size for framing on the channels */
#define AngelChan_MAX_MESSAGE_SIZE (1<<16)

/* maximum message size for api requests (buffer size per active channel) */
#define AngelChan_MAX_API_REQUEST_SIZE (1<<16)

/* max connections to the admin tcp socket */
#define AngelChan_MAX_CONNECTIONS 64

/* the max size of the first initialization messages which are sent across the pipe. */
#define AngelChan_INITIAL_CONF_BUFF_SIZE 1024

#endif
