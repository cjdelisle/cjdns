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

static void getPeersTest(struct EventBase* base, struct Log* logger, struct Allocator* alloc)
{
    struct Address* myAddr = Address_fromString(String_new(MY_ADDR, alloc), alloc);
    struct NodeStore* ns = NodeStore_new(myAddr, alloc, base, logger, NULL);
    for (int i = 0; ADDRS[i]; i++) {
        addNode(ns, ADDRS[i], alloc);
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

    getPeersTest(base, logger, alloc);

    Allocator_free(alloc);
    return 0;
}
