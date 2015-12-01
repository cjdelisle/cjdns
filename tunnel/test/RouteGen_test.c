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
#include "benc/String.h"
#include "benc/Dict.h"
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "util/log/Log.h"
#include "util/log/FileWriterLog.h"
#include "util/platform/Sockaddr.h"
#include "crypto/random/Random.h"
#include "tunnel/RouteGen.h"

static struct Sockaddr* mkSockaddr(char* str, struct Allocator* alloc)
{
    uint8_t buf[64];
    CString_strncpy(buf, str, 64);
    char* slash = CString_strchr(buf, '/');
    if (slash) {
        *slash = '\0';
    }
    struct Sockaddr_storage ss;
    Assert_true(!Sockaddr_parse(buf, &ss));
    return Sockaddr_clone(&ss.addr, alloc);
}

static int getPrefix(char* str)
{
    char* slash = CString_strchr(str, '/');
    Assert_true(slash);
    return atoi(&slash[1]);
}

static void runTest(char** prefixes,
                    char** exemptions,
                    char** expectedOut4,
                    char** expectedOut6,
                    struct Allocator* allocator,
                    struct Log* log)
{
    struct Allocator* alloc = Allocator_child(allocator);
    struct RouteGen* rg = RouteGen_new(alloc, log);
    for (int i = 0; prefixes[i]; i++) {
        RouteGen_addPrefix(rg, mkSockaddr(prefixes[i], alloc), getPrefix(prefixes[i]));
    }
    for (int i = 0; exemptions[i]; i++) {
        RouteGen_addExemption(rg, mkSockaddr(exemptions[i], alloc), getPrefix(exemptions[i]));
    }
}

int main()
{
    struct Allocator* alloc = MallocAllocator_new(1<<20);
    struct Log* log = FileWriterLog_new(stdout, alloc);
    //struct Random* rand = Random_new(alloc, log, NULL);

    struct RouteGen* rg = RouteGen_new(alloc, log);
    RouteGen_addPrefix(rg, mkSockaddr("0.0.0.0", alloc), 0);
    RouteGen_addExemption(rg, mkSockaddr("1.2.3.4", alloc), 32);

    Dict* pfxs = RouteGen_getPrefixes(rg, alloc);
    List* v4 = Dict_getList(pfxs, String_CONST("ipv4"));
    Assert_true(List_size(v4) == 1);
    Assert_true(String_equals(List_getString(v4, 0), String_CONST("0.0.0.0/0")));

    Allocator_free(alloc);
    return 0;
}
