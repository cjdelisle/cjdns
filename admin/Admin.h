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
#ifndef Admin_H
#define Admin_H

#include "interface/Iface.h"
#include "interface/addressable/AddrIface.h"
#include "benc/Dict.h"
#include "exception/Except.h"
#include "memory/Allocator.h"
#include "util/log/Log.h"
#include "util/UniqueName.h"
#include "util/events/EventBase.h"
#include "util/Linker.h"
Linker_require("admin/Admin.c");

#include <stdbool.h>

typedef void (* Admin_Function)
    (Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc);

struct Admin
{
    int unused;
};

struct Admin_FunctionArg
{
    char* name;
    char* type;
    bool required;
};

#define Admin_MAX_REQUEST_SIZE 512

// This must not exceed PipeInterface_MAX_MESSAGE_SIZE
#define Admin_MAX_RESPONSE_SIZE 65536

/**
 * @param arguments an array of struct Admin_FunctionArg specifying what functions are available
 *                  and of those, which are required.
 *        Example C code:
 *            Admin_registerFunction("AuthorizedPasswords_add", addPass, ctx, true,
 *                ((struct Admin_FunctionArg[]) {
 *                    { .name = "password", .required = 1, .type = "String" },
 *                    { .name = "authType", .required = 0, .type = "Int" }
 *                }), admin);
 */
void Admin_registerFunctionWithArgCount(char* name,
                                        Admin_Function callback,
                                        void* callbackContext,
                                        bool needsAuth,
                                        struct Admin_FunctionArg* arguments,
                                        int argCount,
                                        struct Admin* admin);
#define Admin_registerFunction(name, cb, ctx, needsAuth, args, admin) \
    Admin_registerFunctionWithArgCount(                                                           \
        name, cb, ctx, needsAuth, args, (sizeof(args) / sizeof(struct Admin_FunctionArg)), admin)

#define Admin_sendMessage_CHANNEL_CLOSED -1
int Admin_sendMessage(Dict* message, String* txid, struct Admin* admin);

struct Admin* Admin_new(struct AddrIface* ai,
                        struct Log* logger,
                        struct EventBase* eventBase,
                        String* password);
#else
#include "util/UniqueName.h"
#endif
