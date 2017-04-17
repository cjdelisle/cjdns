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
#include "memory/MallocAllocator.h"
#include "dht/Address.h"
#include "dht/dhtcore/Node.h"
#include "dht/dhtcore/NodeList.h"
#include "dht/dhtcore/NodeStore.h"
#include "switch/NumberCompress.h"
#include "util/Assert.h"
#include "util/log/FileWriterLog.h"

static uint8_t* MY_ADDR =
    "v13.0000.0000.0000.0001.3c95l67xxd0juy9zjn7scz5lnjsrcum33y9ghbnqf64d6z0t7sw0.k";

static uint8_t* ADDRS[] = {
    "v17.0000.0000.0000.0484.3kllsrsp4xrr2n9sx26vugk8f2dh7c1psgl74yjrcmxtl2jldug0.k",
    "v16.0000.0000.0000.00fa.h4quw20xhnz4m40l5td9h6qhug86yghs83w54qd0v7fzywdsh700.k",
    "v16.0000.0000.0000.00f6.zky2fx263y1xp048nylbmkx1cf6wuxtsynwf2ffc8yh3mzr5ufl0.k",
    "v17.0000.0000.0000.00ee.syyrcmn8nfcg2uyjq43nl0nn59l19bhv0799clkc6r2v1dxsswz0.k",
    "v17.0000.0000.0000.00e6.bbck723nkg3cq4vnn1fk9mz5zydpulxjfnqh001hyn3hv80n1qr0.k",
    "v17.0000.0000.0000.00d6.0kt0f1p8mbypdvl0k1j4z1sduwfv41srjz94vd7slq0fuglb8v40.k",
    "v17.0000.0000.0000.00e2.1fsvgdy0ypfdl7zfhjkh2ffjkpbs0g3j1r3zm2fr2yhz6qnu1xm0.k",
    "v17.0000.0000.0000.00d2.84tuvkqvyd4nmh50xz365p6bpuc0khrbc5tq5yjd8hjzbprhlhc0.k",
    "v17.0000.0000.0000.049c.3nc0m38zt7n6j6u9hw1uf0l6qbz2chrw2myurnup5xhv85t9s1c0.k",
    "v17.0000.0000.0000.0494.hzq850g8uuc5rc96mj5ysh1tn07sulfvrmmgj652g55r1ll14rj0.k",
    "v17.0000.0000.0000.0490.jxj2fuv4bluz0xwv6umul7w01vuugpdkprkncvuxdrqquqmjxb00.k",
    "v17.0000.0000.0000.048c.hcvxxvrv130udqqrbt6fsb42mh2vxzm7mx0mfdlmk6qh1sv9y8p0.k",
    "v17.0000.0000.0000.00ce.2k77tt73kysj213msxshcl4rlqlb231pxszszlcdv1zsx5unqht0.k",
    "v16.0000.0000.0000.0488.250vlyjjnj4jwmu0u92fubmjpu453nm2mmk1p02rryxfz3k7d8m0.k",
    "v16.0000.0000.0000.00a2.grpv9js9nfkf5s1n1bb3kv7q4mbd77lwhm3lg1mwcfrszfgxvm70.k",
    "v17.0000.0000.0000.0498.jrgqr5myx44ks7knj2r8l7ng7fdtwghzf6h1v9yczrj0s7us8l50.k",
    "v16.0000.0000.0000.0480.h3wzj7hs03c9cw941dlzlm6ftyhmyl5v58kq82kpz1rd475g6mj0.k",
    "v17.0000.0000.0000.00ea.llh1ul6hd5rv4xz3v0wxsyr8qzvhq2xgwdm1lnvujdy8t1wcgj20.k",
    "v17.0000.0000.0000.00de.4qvj4bt5nyd1qf1522z2ml63nl48s2wtr3qpwqy8hy21qzxx9rg0.k",
    "v17.0000.0000.0000.04ac.pp7yhhkqnzk9rxpkv2c4lz9khqb3hqjj9fr3m73zmzyy3s8tn2u0.k",
    NULL
};

static void addNode(struct NodeStore* ns, uint8_t* address, struct Allocator* alloc)
{
    struct EncodingScheme* scheme = NumberCompress_v3x5x8_defineScheme(alloc);
    struct Address* addr = Address_fromString(String_new(address, alloc), alloc);
    Assert_true(NodeStore_discoverNode(ns, addr, scheme, 0, 100));
}

static void checkList(struct NodeList* list,
                      uint64_t* expectedOutputs,
                      struct Log* logger,
                      struct Allocator* alloc)
{
    int j = 0;
    for (int i = (int)list->size - 1; i >= 0; i--) {
        String* str = Address_toString(&list->nodes[i]->address, alloc);
        Log_info(logger, "output: [%s]", str->bytes);
        Assert_true(expectedOutputs[j]);
        Assert_true(expectedOutputs[j] == list->nodes[i]->address.path);
        j++;
    }
    Assert_true(!expectedOutputs[j]);
}

static void getPeersTest(struct EventBase* base, struct Log* logger, struct Allocator* alloc)
{
    struct Address* myAddr = Address_fromString(String_new(MY_ADDR, alloc), alloc);
    struct NodeStore* ns = NodeStore_new(myAddr, alloc, base, logger, NULL);
    NodeStore_setFullVerify(ns, true);
    for (int i = 0; ADDRS[i]; i++) {
        addNode(ns, ADDRS[i], alloc);
    }

    // Test getting all peers using label
    struct NodeList* list = NodeStore_getPeers(0, 8, NULL, alloc, ns);

    checkList(list, (uint64_t[]){ 0x01,0xa2,0xce,0xd2,0xd6,0xde,0xe2,0xe6,0 }, logger, alloc);
    list = NodeStore_getPeers(0xe6, 8, NULL, alloc, ns);
    checkList(list, (uint64_t[]){ 0xe6,0xea,0xee,0xf6,0xfa,0x480,0x484,0x488,0 }, logger, alloc);
    list = NodeStore_getPeers(0x488, 8, NULL, alloc, ns);
    checkList(list, (uint64_t[]){ 0x488,0x48c,0x490,0x494,0x498,0x49c,0x4ac,0 }, logger, alloc);

    // Test getting all peers pages
    int64_t page = 0;
    list = NodeStore_getPeers(0, 8, &page, alloc, ns);
    checkList(list, (uint64_t[]){ 0x01,0xa2,0xce,0xd2,0xd6,0xde,0xe2,0xe6,0 }, logger, alloc);

    page = 1;
    list = NodeStore_getPeers(0, 8, &page, alloc, ns);
    checkList(list, (uint64_t[]){ 0xea,0xee,0xf6,0xfa,0x480,0x484,0x488,0x48c,0 }, logger, alloc);

    page = 2;
    list = NodeStore_getPeers(0, 8, &page, alloc, ns);
    checkList(list, (uint64_t[]){ 0x490,0x494,0x498,0x49c,0x4ac,0 }, logger, alloc);

    page = 3;
    list = NodeStore_getPeers(0, 8, &page, alloc, ns);
    checkList(list, (uint64_t[]){ 0 }, logger, alloc);
}

int main(int argc, char** argv)
{
    struct Allocator* alloc = MallocAllocator_new(1<<20);
    struct Log* logger = FileWriterLog_new(stdout, alloc);
    struct EventBase* base = EventBase_new(alloc);

    getPeersTest(base, logger, alloc);

    Allocator_free(alloc);
    return 0;
}
