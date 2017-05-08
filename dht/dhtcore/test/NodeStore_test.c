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
    uint8_t* publicKeyBase32 = CString_strstr(addr, "X");
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

    uint8_t* addrs[] = {
        "v13.0000.0000.0000.001f.XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX.k",
        "v13.0000.0000.0000.001b.XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX.k",
        "v13.0000.0000.0000.0019.XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX.k",
        "v13.0000.0000.0000.0017.XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX.k",
        "v13.0000.0000.0000.0015.XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX.k",
        "v13.0000.0000.0000.0013.XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX.k",
        "v13.0000.0000.0000.00b6.XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX.k",
        "v13.0000.0000.0000.00b2.XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX.k",
        "v13.0000.0000.0000.00ae.XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX.k",
        "v13.0000.0000.0000.00aa.XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX.k",
        "v13.0000.0000.0000.00a2.XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX.k",
        "v13.0000.0000.0000.009e.XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX.k",
        "v13.0000.0000.0000.00ba.XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX.k",
        "v13.0000.0000.0000.001d.XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX.k",
        "v13.0000.0000.0000.00a6.XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX.k",
        NULL
    };

    // Make all addresses writeable
    for (uint8_t** addr = addrs; *addr; addr++) {
        char *addr_rw = Allocator_malloc(alloc, 79);
        Bits_memcpy(addr_rw, *addr, 79);
        *addr = addr_rw;
    }

    for (uint8_t** addr = addrs; *addr; addr++) {
        genAddress(*addr, rand);
    }

    getPeersTest(addrs, base, logger, alloc, rand);

    Allocator_free(alloc);
    return 0;
}
