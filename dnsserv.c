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
#define string_strlen
#define string_strcmp
#include "util/Base32.h"
#include "util/platform/libc/string.h"
#include "memory/MallocAllocator.h"
#include "util/events/EventBase.h"
#include "io/FileWriter.h"
#include "io/FileReader.h"
#include "benc/serialization/BencSerializer.h"
#include "benc/serialization/json/JsonBencSerializer.h"
#include "util/log/WriterLog.h"
#include "util/platform/Sockaddr.h"
#include "interface/addressable/UDPAddrInterface.h"
#include "interface/DNSServer.h"
#include "interface/RainflyClient.h"
#include "util/Assert.h"
#include "util/platform/libc/string.h"

static void genconf()
{
    printf("{\n"
            "    \"dns\": {\n"
            "        \"bind\": \"[::]:5353\",\n"
            "        \"authorities\": [\n"
            "            \"7kuc3jcyql3cm8lx5zdj8vc0tkz8679kyx83utbm1ub5bxpf4mf1.mittens.h\",\n"
            "            \"tvlxu5rbcj76rfdmsw9xd3kjn79fhv6kpvl2hzv98637j4rdj1b1.tom.h\",\n"
            "            \"kkxfwnm3upf0jv35jq4lx0dn0z3m9bh71gv84cdjlcp68w1qckt1.maru.h\",\n"
            "            \"02wmqfu7v0kdq17fwv68hk646bdvhcr8ybk2ycy7ddzv21n5nb60.scruffy.h\"\n"
            "        ],\n"
            "        \"servers\": [\n"
            "            \"[fc71:ec46:57a0:2bbc:537d:b680:3630:93e4]:9001\",\n"
            "            \"[fc8e:9a1c:27c3:281b:29b1:1a04:3701:c125]:9001\",\n"
            "            \"[fcad:0450:4a40:9778:14e2:e442:6678:3161]:9001\",\n"
            "            \"[fc2f:baa8:4a89:2db5:6789:aa75:07e6:4cb2]:9001\"\n"
            "        ]\n"
            "        \"minSignatures\":2\n"
            "    }\n"
            "}\n");
}

int main(int argc, char** argv)
{
    if (argc > 1 && !strcmp("--genconf", argv[argc-1])) {
        genconf();
        return 0;
    }

    struct Allocator* alloc = MallocAllocator_new(1<<22);
    struct EventBase* base = EventBase_new(alloc);
    struct Writer* logWriter = FileWriter_new(stdout, alloc);
    struct Log* logger = WriterLog_new(logWriter, alloc);
    struct Random* rand = Random_new(alloc, logger, NULL);

    struct Reader* stdinReader = FileReader_new(stdin, alloc);
    Dict config;
    if (JsonBencSerializer_get()->parseDictionary(stdinReader, alloc, &config)) {
        Log_critical(logger, "Failed to parse configuration");
        return -1;
    }

    Dict* dns = Dict_getDict(&config, String_CONST("dns"));
    if (!dns) {
        Log_critical(logger, "No DNS in configuration");
        return -1;
    }

    struct Sockaddr_storage addr;
    Assert_true(!Sockaddr_parse("::", &addr));
    struct AddrInterface* ifaceB = UDPAddrInterface_new(base, &addr.addr, alloc, NULL, logger);
    struct RainflyClient* client = RainflyClient_new(ifaceB, base, rand, logger);

    String* bind = Dict_getString(dns, String_CONST("bind"));
    Assert_true(!Sockaddr_parse(bind ? bind->bytes : "[::]:5353", &addr));
    struct AddrInterface* iface = UDPAddrInterface_new(base, &addr.addr, alloc, NULL, logger);
    DNSServer_new(iface, logger, client);

    List* auth = Dict_getList(dns, String_CONST("authorities"));
    for (int i = 0; i < (int)List_size(auth); i++) {
        String* str = List_getString(auth, i);
        if (!str) {
            Log_warn(logger, "Element [%d] in [dns.authorities] list of wrong type", i);
            continue;
        }

        uint8_t key[32] = {0};
        if (str->len < 52 || Base32_decode(key, 32, str->bytes, 52) != 32) {
            Log_warn(logger, "Failed to parse key [%s]", str->bytes);
            continue;
        }

        if (RainflyClient_addKey(client, key)) {
            Log_warn(logger, "Failed to add key to RainflyClient [%s]", str->bytes);
        }
    }

    List* servers = Dict_getList(dns, String_CONST("servers"));
    for (int i = 0; i < (int)List_size(servers); i++) {
        String* str = List_getString(servers, i);
        if (!str) {
            Log_warn(logger, "Element [%d] in [dns.servers] list of wrong type", i);
            continue;
        }

        struct Sockaddr_storage node;
        if (Sockaddr_parse(str->bytes, &node)) {
            Log_warn(logger, "Failed to parse server name [%s]", str->bytes);
            continue;
        }

        if (RainflyClient_addServer(client, &node.addr)) {
            Log_warn(logger, "Failed to add server to RainflyClient [%s]", str->bytes);
        }
    }

    EventBase_beginLoop(base);
}

