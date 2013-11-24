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
#ifndef RainflyClient_H
#define RainflyClient_H

#include "benc/Dict.h"
#include "benc/String.h"
#include "memory/Allocator.h"
#include "interface/addressable/AddrInterface.h"
#include "util/log/Log.h"
#include "util/events/EventBase.h"
#include "crypto/random/Random.h"
#include "util/Linker.h"
Linker_require("interface/RainflyClient.c")

#define RainflyClient_DEFAULT_MIN_SIGNATURES 2
#define RainflyClient_DEFAULT_MAX_TRIES 3

struct RainflyClient
{
    int minSignatures;
    int maxTries;
};

enum RainflyClient_ResponseCode {
    RainflyClient_ResponseCode_NO_ERROR = 0,
    RainflyClient_ResponseCode_FORMAT_ERROR = 1,
    RainflyClient_ResponseCode_SERVER_ERROR = 2,
    RainflyClient_ResponseCode_NXDOMAIN = 3,
    RainflyClient_ResponseCode_NOT_IMPLEMENTED = 4,
    RainflyClient_ResponseCode_REFUSED = 5,
    RainflyClient_ResponseCode_XYDOMAIN = 6,
    RainflyClient_ResponseCode_YXRRSET = 7,
    RainflyClient_ResponseCode_NXRRSET = 8,
    RainflyClient_ResponseCode_NOT_AUTH_FOR_ZONE = 9,
};

struct RainflyClient_Lookup
{
    void (* onReply)(struct RainflyClient_Lookup* promise,
                     Dict* value,
                     enum RainflyClient_ResponseCode code);
    void* userData;
    String* domain;
    struct Allocator* alloc;
};

struct RainflyClient_Lookup* RainflyClient_lookup(struct RainflyClient* client, String* domain);

#define RainflyClient_addKey_TOO_MANY_KEYS -1
int RainflyClient_addKey(struct RainflyClient* client, uint8_t key[32]);

#define RainflyClient_addServer_WRONG_ADDRESS_TYPE -1
int RainflyClient_addServer(struct RainflyClient* client, struct Sockaddr* addr);

struct RainflyClient* RainflyClient_new(struct AddrInterface* iface,
                                        struct EventBase* base,
                                        struct Random* rand,
                                        struct Log* logger);

#endif
