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
#ifndef AdminClient_H
#define AdminClient_H

#include "interface/addressable/AddrIface.h"
#include "benc/String.h"
#include "benc/Dict.h"
#include "memory/Allocator.h"
#include "util/log/Log.h"
#include "util/events/EventBase.h"
#include "util/platform/Sockaddr.h"
#include "util/Linker.h"
Linker_require("client/AdminClient.c");

enum AdminClient_Error
{
    /** Success. */
    AdminClient_Error_NONE = 0,

    /** Response was longer than max message size. */
    AdminClient_Error_OVERLONG_RESPONSE,

    /** Error on recv(), causes result.errno to be set. */
    AdminClient_Error_ERROR_READING_FROM_SOCKET,

    /** No data on socket. */
    AdminClient_Error_SOCKET_NOT_READY,

    /** Failed to deserialize response. */
    AdminClient_Error_DESERIALIZATION_FAILED,

    /** Failed to serialize request. */
    AdminClient_Error_SERIALIZATION_FAILED,

    /** Timeout waiting for response. */
    AdminClient_Error_TIMEOUT,

    /** Requested cookie and response did not contain cookie. */
    AdminClient_Error_NO_COOKIE,
};

/** The biggest message that can be sent or received. */
#define AdminClient_MAX_MESSAGE_SIZE 1023

/** The amount of message padding. */
#define AdminClient_Result_PADDING_SIZE (sizeof(struct Sockaddr_storage))

struct AdminClient_Result
{
    /** The error type of AdminClient_Error_NONE if there was no error. */
    enum AdminClient_Error err;

    /** Space to put the address of the node which the response is being sent to. */
    uint8_t padding[AdminClient_Result_PADDING_SIZE];

    /**
     * When the request is made, this will hold the request bytes,
     * after it will hold the response bytes. If there is an error
     * during the sending of the request, it will still have the request bytes.
     * 1 byte extra to alow for a null terminator.
     */
    uint8_t messageBytes[AdminClient_MAX_MESSAGE_SIZE + 1];

    /** The deserialized response. */
    Dict* responseDict;
};

struct AdminClient_Promise;

typedef void (* AdminClient_ResultHandler)(struct AdminClient_Promise* p,
                                           struct AdminClient_Result* res);

struct AdminClient_Promise
{
    AdminClient_ResultHandler callback;
    void* userData;
    struct Allocator* alloc;
};

struct AdminClient
{
    /** How long to wait for a response from the cjdns node. */
    uint32_t millisecondsToWait;
};

char* AdminClient_errorString(enum AdminClient_Error err);

struct AdminClient_Promise* AdminClient_rpcCall(String* function,
                                                Dict* args,
                                                struct AdminClient* client,
                                                struct Allocator* requestAlloc);


struct AdminClient* AdminClient_new(struct AddrIface* ai,
                                    struct Sockaddr* connectToAddress,
                                    String* adminPassword,
                                    struct EventBase* eventBase,
                                    struct Log* logger,
                                    struct Allocator* alloc);

#endif
