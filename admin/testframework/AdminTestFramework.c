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
#include "admin/Admin.h"
#include "admin/AdminClient.h"
#include "admin/testframework/AdminTestFramework.h"
#include "benc/String.h"
#include "exception/AbortHandler.h"
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "io/FileWriter.h"
#include "io/Writer.h"
#include "util/Assert.h"
#include "util/Log.h"

#include <event2/event.h>

struct AdminTestFramework* AdminTestFramework_setUp()
{
    struct Allocator* alloc = MallocAllocator_new(1<<20);

    struct Writer* logwriter = FileWriter_new(stdout, alloc);
    Assert_always(logwriter);
    struct Log* logger =
        alloc->clone(sizeof(struct Log), alloc, &(struct Log) { .writer = logwriter });

    struct sockaddr_storage addr;
    int addrLen = sizeof(struct sockaddr_storage);
    memset(&addr, 0, sizeof(struct sockaddr_storage));

    struct event_base* eventBase = event_base_new();

    String* password = String_new("abcdefg12345", alloc);
    struct Admin* admin =
        Admin_new(&addr, addrLen, password, NULL, eventBase, AbortHandler_INSTANCE, logger, alloc);

    struct sockaddr_storage* addrPtr;
    String* retPassword;
    Admin_getConnectInfo(&addrPtr, &addrLen, &retPassword, admin);

    Assert_always(String_equals(password, retPassword));

    struct AdminClient* client =
        AdminClient_new(addrPtr, addrLen, retPassword, eventBase, logger, alloc);

    Assert_always(client);

    return alloc->clone(sizeof(struct AdminTestFramework), alloc, &(struct AdminTestFramework) {
        .admin = admin,
        .client = client,
        .alloc = alloc,
        .eventBase = eventBase,
        .logger = logger
    });
}
