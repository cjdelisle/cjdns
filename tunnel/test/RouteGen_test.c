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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
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
    struct Sockaddr_storage ss;
    Assert_true(!Sockaddr_parse(str, &ss));
    return Sockaddr_clone(&ss.addr, alloc);
}

static void runTest0(char** prefixes,
                     char** exceptions4,
                     char** exceptions6,
                     char** expectedOut4,
                     char** expectedOut6,
                     struct Allocator* alloc,
                     struct Log* log)
{
    struct RouteGen* rg = RouteGen_new(alloc, log);
    for (int i = 0; prefixes[i]; i++) {
        RouteGen_addPrefix(rg, mkSockaddr(prefixes[i], alloc));
    }
    for (int i = 0; exceptions4 && exceptions4[i]; i++) {
        RouteGen_addException(rg, mkSockaddr(exceptions4[i], alloc));
    }
    for (int i = 0; exceptions6 && exceptions6[i]; i++) {
        RouteGen_addException(rg, mkSockaddr(exceptions6[i], alloc));
    }
    Dict* routes = RouteGen_getGeneratedRoutes(rg, alloc);
    List* routes4 = Dict_getListC(routes, "ipv4");
    List* routes6 = Dict_getListC(routes, "ipv6");
    if (expectedOut4) {
        for (int i = 0; expectedOut4[i]; i++) {
            Log_debug(log, "%s\n", expectedOut4[i]);
        }
        for (int i = 0; i < List_size(routes4); i++) {
            Log_debug(log, "%s\n", List_getString(routes4, i)->bytes);
        }

        Assert_true(!expectedOut4[List_size(routes4)]);
        for (int i = 0; i < List_size(routes4); i++) {
            String* str = List_getString(routes4, i);
            Assert_true(str);
            Assert_true(expectedOut4[i]);
            if (CString_strncmp(expectedOut4[i], str->bytes, str->len)) {
                Log_error(log, "Fail\nexpected: %s\nGot:      %s\n", expectedOut4[i], str->bytes);
                Assert_failure("fail");
            }
        }
    } else {
        Assert_true(!List_size(routes4));
    }
    if (expectedOut6) {
        for (int i = 0; expectedOut6[i]; i++) {
            Log_debug(log, "%s\n", expectedOut6[i]);
        }
        for (int i = 0; i < List_size(routes6); i++) {
            Log_debug(log, "%s\n", List_getString(routes6, i)->bytes);
        }

        Assert_true(!expectedOut6[List_size(routes6)]);
        for (int i = 0; i < List_size(routes6); i++) {
            String* str = List_getString(routes6, i);
            Assert_true(str);
            Assert_true(expectedOut6[i]);
            if (CString_strncmp(expectedOut6[i], str->bytes, str->len)) {
                Log_error(log, "Fail\nexpected: %s\nGot:      %s\n", expectedOut6[i], str->bytes);
                Assert_failure("fail");
            }
        }
    } else {
        Assert_true(!List_size(routes6));
    }
}

static void runTest(char** prefixes,
                    char** exceptions4,
                    char** exceptions6,
                    char** expectedOut4,
                    char** expectedOut6,
                    struct Allocator* allocator,
                    struct Log* log)
{
    // Check that routes are invertible.
    struct Allocator* alloc = Allocator_child(allocator);
    Log_debug(log, "Forward");
    runTest0(prefixes, exceptions4, exceptions6, expectedOut4, expectedOut6, alloc, log);
    Log_debug(log, "Reverse");
    runTest0(prefixes, expectedOut4, expectedOut6, exceptions4, exceptions6, alloc, log);
    Allocator_free(alloc);
}

int main(void);
int main(void)
{
    struct Allocator* alloc = MallocAllocator_new(1<<20);
    struct Log* log = FileWriterLog_new(stdout, alloc);

    runTest((char*[]){ "0.0.0.0/0", NULL },
            (char*[]){ "77.66.55.44/32", NULL },
            NULL,
            (char*[]){ "77.66.55.45/32", "77.66.55.46/31", "77.66.55.40/30", "77.66.55.32/29",
                       "77.66.55.48/28", "77.66.55.0/27", "77.66.55.64/26", "77.66.55.128/25",
                       "77.66.54.0/24", "77.66.52.0/23", "77.66.48.0/22", "77.66.56.0/21",
                       "77.66.32.0/20", "77.66.0.0/19", "77.66.64.0/18", "77.66.128.0/17",
                       "77.67.0.0/16", "77.64.0.0/15", "77.68.0.0/14", "77.72.0.0/13",
                       "77.80.0.0/12", "77.96.0.0/11", "77.0.0.0/10", "77.128.0.0/9",
                       "76.0.0.0/8", "78.0.0.0/7", "72.0.0.0/6", "64.0.0.0/5",
                       "80.0.0.0/4", "96.0.0.0/3", "0.0.0.0/2", "128.0.0.0/1", NULL },
            NULL,
            alloc,
            log);

    runTest((char*[]){ "77.66.0.0/16", NULL },
            (char*[]){ "77.66.55.44/32", NULL },
            NULL,
            (char*[]){ "77.66.55.45/32", "77.66.55.46/31", "77.66.55.40/30", "77.66.55.32/29",
                       "77.66.55.48/28", "77.66.55.0/27", "77.66.55.64/26", "77.66.55.128/25",
                       "77.66.54.0/24", "77.66.52.0/23", "77.66.48.0/22", "77.66.56.0/21",
                       "77.66.32.0/20", "77.66.0.0/19", "77.66.64.0/18", "77.66.128.0/17", NULL },
            NULL,
            alloc,
            log);

    runTest0((char*[]){ "77.66.0.0/16", "77.99.0.0/16", NULL },
             (char*[]){ "77.66.55.44/32", NULL },
             NULL,
             (char*[]){ "77.66.55.45/32", "77.66.55.46/31", "77.66.55.40/30", "77.66.55.32/29",
                        "77.66.55.48/28", "77.66.55.0/27", "77.66.55.64/26", "77.66.55.128/25",
                        "77.66.54.0/24", "77.66.52.0/23", "77.66.48.0/22", "77.66.56.0/21",
                        "77.66.32.0/20", "77.66.0.0/19", "77.66.64.0/18", "77.66.128.0/17",
                        "77.99.0.0/16", NULL },
             NULL,
             alloc,
             log);

    runTest((char*[]){ "0.0.0.0/0", NULL },
            (char*[]){ "77.66.55.44/32", "133.33.66.9/32", NULL },
            NULL,
            (char*[]){ "77.66.55.45/32", "133.33.66.8/32", "77.66.55.46/31", "133.33.66.10/31",
                       "77.66.55.40/30", "133.33.66.12/30", "77.66.55.32/29", "133.33.66.0/29",
                       "77.66.55.48/28", "133.33.66.16/28", "77.66.55.0/27", "133.33.66.32/27",
                       "77.66.55.64/26", "133.33.66.64/26", "77.66.55.128/25", "133.33.66.128/25",
                       "77.66.54.0/24", "133.33.67.0/24", "77.66.52.0/23", "133.33.64.0/23",
                       "77.66.48.0/22", "133.33.68.0/22", "77.66.56.0/21", "133.33.72.0/21",
                       "77.66.32.0/20", "133.33.80.0/20", "77.66.0.0/19", "133.33.96.0/19",
                       "77.66.64.0/18", "133.33.0.0/18", "77.66.128.0/17", "133.33.128.0/17",
                       "77.67.0.0/16", "133.32.0.0/16", "77.64.0.0/15", "133.34.0.0/15",
                       "77.68.0.0/14", "133.36.0.0/14", "77.72.0.0/13", "133.40.0.0/13",
                       "77.80.0.0/12", "133.48.0.0/12", "77.96.0.0/11", "133.0.0.0/11",
                       "77.0.0.0/10", "133.64.0.0/10", "77.128.0.0/9", "133.128.0.0/9",
                       "76.0.0.0/8", "132.0.0.0/8", "78.0.0.0/7", "134.0.0.0/7",
                       "72.0.0.0/6", "128.0.0.0/6", "64.0.0.0/5", "136.0.0.0/5",
                       "80.0.0.0/4", "144.0.0.0/4", "96.0.0.0/3", "160.0.0.0/3",
                       "0.0.0.0/2", "192.0.0.0/2", NULL },
              NULL,
              alloc,
              log);

    runTest((char*[]){ "0.0.0.0/0", NULL },
            (char*[]){ "77.33.55.44/32", "77.66.55.44/32", NULL },
            NULL,
            (char*[]){ "77.33.55.45/32", "77.66.55.45/32", "77.33.55.46/31", "77.66.55.46/31",
                       "77.33.55.40/30", "77.66.55.40/30", "77.33.55.32/29", "77.66.55.32/29",
                       "77.33.55.48/28", "77.66.55.48/28", "77.33.55.0/27", "77.66.55.0/27",
                       "77.33.55.64/26", "77.66.55.64/26", "77.33.55.128/25", "77.66.55.128/25",
                       "77.33.54.0/24", "77.66.54.0/24", "77.33.52.0/23", "77.66.52.0/23",
                       "77.33.48.0/22", "77.66.48.0/22", "77.33.56.0/21", "77.66.56.0/21",
                       "77.33.32.0/20", "77.66.32.0/20", "77.33.0.0/19", "77.66.0.0/19",
                       "77.33.64.0/18", "77.66.64.0/18", "77.33.128.0/17", "77.66.128.0/17",
                       "77.32.0.0/16", "77.67.0.0/16", "77.34.0.0/15", "77.64.0.0/15",
                       "77.36.0.0/14", "77.68.0.0/14", "77.40.0.0/13", "77.72.0.0/13",
                       "77.48.0.0/12", "77.80.0.0/12", "77.0.0.0/11", "77.96.0.0/11",
                       "77.128.0.0/9", "76.0.0.0/8", "78.0.0.0/7", "72.0.0.0/6",
                       "64.0.0.0/5", "80.0.0.0/4", "96.0.0.0/3", "0.0.0.0/2",
                       "128.0.0.0/1", NULL },
              NULL,
              alloc,
              log);

    runTest((char*[]){ "77.33.0.0/16", "77.66.0.0/16", NULL },
            (char*[]){ "77.33.55.44/32", "77.66.55.44/32", NULL },
            NULL,
            (char*[]){ "77.33.55.45/32", "77.66.55.45/32", "77.33.55.46/31", "77.66.55.46/31",
                       "77.33.55.40/30", "77.66.55.40/30", "77.33.55.32/29", "77.66.55.32/29",
                       "77.33.55.48/28", "77.66.55.48/28", "77.33.55.0/27", "77.66.55.0/27",
                       "77.33.55.64/26", "77.66.55.64/26", "77.33.55.128/25", "77.66.55.128/25",
                       "77.33.54.0/24", "77.66.54.0/24", "77.33.52.0/23", "77.66.52.0/23",
                       "77.33.48.0/22", "77.66.48.0/22", "77.33.56.0/21", "77.66.56.0/21",
                       "77.33.32.0/20", "77.66.32.0/20", "77.33.0.0/19", "77.66.0.0/19",
                       "77.33.64.0/18", "77.66.64.0/18", "77.33.128.0/17", "77.66.128.0/17",
                       NULL },
            NULL,
            alloc,
            log);


    runTest((char*[]){ "77.66.0.0/16", NULL },
            (char*[]){ "77.66.55.44/30", NULL },
            NULL,
            (char*[]){ "77.66.55.40/30", "77.66.55.32/29",
                       "77.66.55.48/28", "77.66.55.0/27", "77.66.55.64/26", "77.66.55.128/25",
                       "77.66.54.0/24", "77.66.52.0/23", "77.66.48.0/22", "77.66.56.0/21",
                       "77.66.32.0/20", "77.66.0.0/19", "77.66.64.0/18", "77.66.128.0/17", NULL },
            NULL,
            alloc,
            log);


    runTest((char*[]){ "::/0", NULL },
            NULL,
            (char*[]){ "2a00:1450:400d:807::200e/128", NULL },
            NULL,
            (char*[]){ "2a00:1450:400d:807::200f/128",
                       "2a00:1450:400d:807::200c/127", "2a00:1450:400d:807::2008/126",
                       "2a00:1450:400d:807::2000/125", "2a00:1450:400d:807::2010/124",
                       "2a00:1450:400d:807::2020/123", "2a00:1450:400d:807::2040/122",
                       "2a00:1450:400d:807::2080/121", "2a00:1450:400d:807::2100/120",
                       "2a00:1450:400d:807::2200/119", "2a00:1450:400d:807::2400/118",
                       "2a00:1450:400d:807::2800/117", "2a00:1450:400d:807::3000/116",
                       "2a00:1450:400d:807::/115", "2a00:1450:400d:807::4000/114",
                       "2a00:1450:400d:807::8000/113", "2a00:1450:400d:807::1:0/112",
                       "2a00:1450:400d:807::2:0/111", "2a00:1450:400d:807::4:0/110",
                       "2a00:1450:400d:807::8:0/109", "2a00:1450:400d:807::10:0/108",
                       "2a00:1450:400d:807::20:0/107", "2a00:1450:400d:807::40:0/106",
                       "2a00:1450:400d:807::80:0/105", "2a00:1450:400d:807::100:0/104",
                       "2a00:1450:400d:807::200:0/103", "2a00:1450:400d:807::400:0/102",
                       "2a00:1450:400d:807::800:0/101", "2a00:1450:400d:807::1000:0/100",
                       "2a00:1450:400d:807::2000:0/99", "2a00:1450:400d:807::4000:0/98",
                       "2a00:1450:400d:807::8000:0/97", "2a00:1450:400d:807:0:1::/96",
                       "2a00:1450:400d:807:0:2::/95", "2a00:1450:400d:807:0:4::/94",
                       "2a00:1450:400d:807:0:8::/93", "2a00:1450:400d:807:0:10::/92",
                       "2a00:1450:400d:807:0:20::/91", "2a00:1450:400d:807:0:40::/90",
                       "2a00:1450:400d:807:0:80::/89", "2a00:1450:400d:807:0:100::/88",
                       "2a00:1450:400d:807:0:200::/87", "2a00:1450:400d:807:0:400::/86",
                       "2a00:1450:400d:807:0:800::/85", "2a00:1450:400d:807:0:1000::/84",
                       "2a00:1450:400d:807:0:2000::/83", "2a00:1450:400d:807:0:4000::/82",
                       "2a00:1450:400d:807:0:8000::/81", "2a00:1450:400d:807:1::/80",
                       "2a00:1450:400d:807:2::/79", "2a00:1450:400d:807:4::/78",
                       "2a00:1450:400d:807:8::/77", "2a00:1450:400d:807:10::/76",
                       "2a00:1450:400d:807:20::/75", "2a00:1450:400d:807:40::/74",
                       "2a00:1450:400d:807:80::/73", "2a00:1450:400d:807:100::/72",
                       "2a00:1450:400d:807:200::/71", "2a00:1450:400d:807:400::/70",
                       "2a00:1450:400d:807:800::/69", "2a00:1450:400d:807:1000::/68",
                       "2a00:1450:400d:807:2000::/67", "2a00:1450:400d:807:4000::/66",
                       "2a00:1450:400d:807:8000::/65", "2a00:1450:400d:806::/64",
                       "2a00:1450:400d:804::/63", "2a00:1450:400d:800::/62",
                       "2a00:1450:400d:808::/61", "2a00:1450:400d:810::/60",
                       "2a00:1450:400d:820::/59", "2a00:1450:400d:840::/58",
                       "2a00:1450:400d:880::/57", "2a00:1450:400d:900::/56",
                       "2a00:1450:400d:a00::/55", "2a00:1450:400d:c00::/54",
                       "2a00:1450:400d::/53", "2a00:1450:400d:1000::/52",
                       "2a00:1450:400d:2000::/51", "2a00:1450:400d:4000::/50",
                       "2a00:1450:400d:8000::/49", "2a00:1450:400c::/48",
                       "2a00:1450:400e::/47", "2a00:1450:4008::/46",
                       "2a00:1450:4000::/45", "2a00:1450:4010::/44", "2a00:1450:4020::/43",
                       "2a00:1450:4040::/42", "2a00:1450:4080::/41", "2a00:1450:4100::/40",
                       "2a00:1450:4200::/39", "2a00:1450:4400::/38", "2a00:1450:4800::/37",
                       "2a00:1450:5000::/36", "2a00:1450:6000::/35", "2a00:1450::/34",
                       "2a00:1450:8000::/33", "2a00:1451::/32", "2a00:1452::/31",
                       "2a00:1454::/30", "2a00:1458::/29", "2a00:1440::/28",
                       "2a00:1460::/27", "2a00:1400::/26", "2a00:1480::/25", "2a00:1500::/24",
                       "2a00:1600::/23", "2a00:1000::/22", "2a00:1800::/21", "2a00::/20",
                       "2a00:2000::/19", "2a00:4000::/18", "2a00:8000::/17", "2a01::/16",
                       "2a02::/15", "2a04::/14", "2a08::/13", "2a10::/12", "2a20::/11",
                       "2a40::/10", "2a80::/9", "2b00::/8", "2800::/7", "2c00::/6", "2000::/5",
                       "3000::/4", "::/3", "4000::/2", "8000::/1", NULL },
            alloc,
            log);

    runTest((char*[]){ "2a00:1450:400d:807::2100/119", NULL },
            NULL,
            (char*[]){ "2a00:1450:400d:807::200e/128", NULL },
            NULL,
            (char*[]){ "2a00:1450:400d:807::200f/128",
                       "2a00:1450:400d:807::200c/127", "2a00:1450:400d:807::2008/126",
                       "2a00:1450:400d:807::2000/125", "2a00:1450:400d:807::2010/124",
                       "2a00:1450:400d:807::2020/123", "2a00:1450:400d:807::2040/122",
                       "2a00:1450:400d:807::2080/121", "2a00:1450:400d:807::2100/120", NULL },
            alloc,
            log);

    runTest((char*[]){ "2a00:1450:400d:807::2100/119", NULL },
            NULL,
            (char*[]){ "2a00:1450:400d:807::2000/128", "2a00:1450:400d:807::200e/128", NULL },
            NULL,
            (char*[]){ "2a00:1450:400d:807::2001/128", "2a00:1450:400d:807::200f/128",
                       "2a00:1450:400d:807::2002/127", "2a00:1450:400d:807::200c/127",
                       "2a00:1450:400d:807::2004/126", "2a00:1450:400d:807::2008/126",
                       "2a00:1450:400d:807::2010/124", "2a00:1450:400d:807::2020/123",
                       "2a00:1450:400d:807::2040/122", "2a00:1450:400d:807::2080/121",
                       "2a00:1450:400d:807::2100/120", NULL },
            alloc,
            log);

    Allocator_free(alloc);
    return 0;
}
