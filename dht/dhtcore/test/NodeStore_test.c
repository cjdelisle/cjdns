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
#include "crypto/Key.h"
#include "crypto/random/Random.h"
#include "dht/Address.h"
#include "dht/dhtcore/Node.h"
#include "dht/dhtcore/NodeList.h"
#include "dht/dhtcore/NodeStore.h"
#include "switch/NumberCompress.h"
#include "util/Assert.h"
#include "util/Base32.h"
#include "util/log/FileWriterLog.h"

static uint8_t* ADDRS[] = {
    "v13.0000.0000.0000.001f.usclqxtgkksmgwv10h8h3pltm3zy27bddb20mpsbrvjlcw4d9gl0.k",
    "v13.0000.0000.0000.001b.03ws4vngbq56ymd14vbpd92zdfr0783t7g6u3k4dtb1kuw5m62v0.k",
    "v13.0000.0000.0000.0019.8u2pvwuf1wmf5hwxytckbk4sbyrg3rdnqdwulbgsbmw408grm500.k",
    "v13.0000.0000.0000.0017.bf39dq2mubq17x2lmz8cwgr839s95b6gk7dmcty22uw3dj7v5zy0.k",
    "v13.0000.0000.0000.0015.q402jm870c215kdvf4wy2qvpt4kdrx0b4zyx2vnv2fdfprf41fk0.k",
    "v13.0000.0000.0000.0013.6npk9pfdw09t0ldp0c9usrp8pkhttg0104849ng6j5gsz3w8q3x0.k",
    "v13.0000.0000.0000.00b6.t9lpkc69nwpxpnusc7nlgrrjmzdcjhgf52zhhr9k69t9x6hrz5c0.k",
    "v13.0000.0000.0000.00b2.05t007gun13qnhm5czlkjlp14lpr2v2j6f4g6bmzgbwv5mj9uy60.k",
    "v13.0000.0000.0000.00ae.f5d1l67lb3dl7z41l1lwmh0jsptq382vsyvr999brjdjqutj5m90.k",
    "v13.0000.0000.0000.00aa.61jw1hdru3tnwv3vfpt9vmmbvyhfxc8chd9msf1jhumq2y3h5pn0.k",
    "v13.0000.0000.0000.00a2.684v75l5czfvgmr5qkb60xd7d9l79zxg5nyj5wmbhr8nxm7wzn20.k",
    "v13.0000.0000.0000.009e.th3p5791z6xr24plc3487xfb9tfy4n7n51y8pbhnr9771kluhr10.k",
    "v13.0000.0000.0000.00ba.d40x5rkb8jj5v1521j5l6wd1pu7svzrmyb2kvf1rj7ll0kuydt40.k",
    "v13.0000.0000.0000.001d.rujhjmq178wtfxccuwp3h17uq7u7phfr1t1m1zn80855h2wngl50.k",
    "v13.0000.0000.0000.00a6.0czm5qrryjrhc4dv9zcl148pnbur1869zufrcfw8f9b7vw132yu0.k",
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

static void genAddress(uint8_t* addr, struct Random* rand)
{
    uint8_t publicKey[32];
    uint8_t ip[16];
    uint8_t privateKey[32];
    Key_gen(ip, publicKey, privateKey, rand);
    uint8_t* publicKeyBase32 = &addr[24];
    Assert_true(publicKeyBase32);
    Base32_encode(publicKeyBase32, 53, publicKey, 32);
    publicKeyBase32[52] = '.';
    publicKeyBase32[53] = 'k';
}

static void getPeersTest(uint8_t* addrs[],
                         struct EventBase* base,
                         struct Log* logger,
                         struct Allocator* alloc,
                         struct Random* rand)
{

    uint8_t my_addr[] =
        "v13.0000.0000.0000.0001.XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX.k";
    genAddress(my_addr, rand);

    struct Address* myAddr = Address_fromString(String_new(my_addr, alloc), alloc);
    Assert_true(myAddr);
    struct NodeStore* ns = NodeStore_new(myAddr, alloc, base, logger, NULL);
    NodeStore_setFullVerify(ns, true);
    for (int i = 0; addrs[i]; i++) {
        addNode(ns, addrs[i], alloc);
    }

    struct NodeList* list = NodeStore_getPeers(0, 8, alloc, ns);

    checkList(list, (uint64_t[]){ 0x01,0x13,0x15,0x17,0x19,0x1b,0x1d,0x1f,0 }, logger, alloc);
    list = NodeStore_getPeers(0x1f, 8, alloc, ns);
    checkList(list, (uint64_t[]){ 0x1f,0x9e,0xa2,0xa6,0xaa,0xae,0xb2,0xb6,0 }, logger, alloc);
    list = NodeStore_getPeers(0xb6, 8, alloc, ns);
    checkList(list, (uint64_t[]){ 0xb6,0xba,0 }, logger, alloc);
}

int main(int argc, char** argv)
{
    struct Allocator* alloc = MallocAllocator_new(1<<20);
    struct Log* logger = FileWriterLog_new(stdout, alloc);
    struct EventBase* base = EventBase_new(alloc);
    struct Random* rand = Random_new(alloc, NULL, NULL);

#if defined(ADDRESS_PREFIX) || defined(ADDRESS_PREFIX_BITS)
    // Make all addresses writeable
    for (uint8_t** addr = ADDRS; *addr; addr++) {
        char *addr_rw = Allocator_malloc(alloc, 79);
        Bits_memcpy(addr_rw, *addr, 79);
        *addr = addr_rw;
    }

    for (uint8_t** addr = ADDRS; *addr; addr++) {
        genAddress(*addr, rand);
    }
#endif

    getPeersTest(ADDRS, base, logger, alloc, rand);

    Allocator_free(alloc);
    return 0;
}
