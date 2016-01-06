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
#include "util/Base10.h"

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
    int64_t out;
    Assert_true(!Base10_fromString(&slash[1], &out));
    Assert_true(out <= 128 && out >= 0);
    return (int) out;
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
    Dict* routes = RouteGen_getGeneratedRoutes(rg, alloc);
    List* routes4 = Dict_getList(routes, String_CONST("ipv4"));
    List* routes6 = Dict_getList(routes, String_CONST("ipv6"));
    if (expectedOut4) {
        Assert_true(!expectedOut4[List_size(routes4)]);
        for (int i = 0; i < List_size(routes4); i++) {
            String* str = List_getString(routes4, i);
            Assert_true(str);
            Assert_true(expectedOut4[i]);
            Assert_true(!CString_strncmp(expectedOut4[i], str->bytes, str->len));
        }
    } else {
        Assert_true(!List_size(routes4));
    }
    if (expectedOut6) {
        Assert_true(!expectedOut6[List_size(routes6)]);
        for (int i = 0; List_size(routes6); i++) {
            String* str = List_getString(routes6, i);
            Assert_true(str);
            Assert_true(expectedOut6[i]);
            Assert_true(!CString_strncmp(expectedOut6[i], str->bytes, str->len));
        }
    } else {
        Assert_true(!List_size(routes6));
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


    runTest((char*[]){ "0.0.0.0/0", NULL },
            (char*[]){ "198.167.222.70/32", NULL },
            (char*[]){ "198.167.222.71/32", "198.167.222.68/31", "198.167.222.64/30",
                       "198.167.222.72/29", "198.167.222.80/28", "198.167.222.96/27",
                       "198.167.222.0/26", "198.167.222.128/25", "198.167.223.0/24",
                       "198.167.220.0/23", "198.167.216.0/22", "198.167.208.0/21",
                       "198.167.192.0/20", "198.167.224.0/19", "198.167.128.0/18",
                        "198.167.0.0/17", "198.166.0.0/16", "198.164.0.0/15",
                       "198.160.0.0/14", "198.168.0.0/13", "198.176.0.0/12",
                       "198.128.0.0/11", "198.192.0.0/10", "198.0.0.0/9",
                       "199.0.0.0/8", "196.0.0.0/7", "192.0.0.0/6",
                       "200.0.0.0/5", "208.0.0.0/4", "224.0.0.0/3",
                       "128.0.0.0/2", "0.0.0.0/1", NULL },
              NULL,
              alloc,
              log);

    Allocator_free(alloc);
    return 0;
}
