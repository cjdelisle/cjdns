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
#include "crypto/random/Random.h"
#include "interface/MultiInterface.h"
#include "interface/Interface.h"
#include "interface/InterfaceController.h"
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "wire/Message.h"
#include "benc/String.h"


#define KEY_SIZE 20
#define ENTRY_COUNT 64
#define CYCLES 128

struct Entry {
    uint8_t key[KEY_SIZE];
};

struct Context
{
    struct InterfaceController ic;
    struct Interface* receivedOn;
    struct Message* lastSent;
};


static uint8_t sendMessage(struct Message* msg, struct Interface* iface)
{
    return 0;
}

static uint8_t recieveInternal(struct Message* msg, struct Interface* iface)
{
    struct Context* ctx = iface->receiverContext;
    ctx->receivedOn = iface;
    return 0;
}

static int registerPeer(struct InterfaceController* ic,
                        uint8_t herPublicKey[32],
                        String* password,
                        bool requireAuth,
                        bool transient,
                        struct Interface* iface)
{
    iface->receiveMessage = recieveInternal;
    iface->receiverContext = ic;
    return 0;
}

static enum InterfaceController_PeerState getPeerState(struct Interface* iface)
{
    return InterfaceController_PeerState_HANDSHAKE;
}

int main()
{
    struct Allocator* alloc = MallocAllocator_new(1<<20);
    struct Random* rand = Random_new(alloc, NULL, NULL);

    // mock interface controller.
    struct Context ctx = {
        .ic = {
            .registerPeer = registerPeer,
            .getPeerState = getPeerState
        }
    };

    struct Interface externalIf = {
        .sendMessage = sendMessage,
        .allocator = alloc,
        .senderContext = &ctx
    };

    /*struct MultiInterface* mif = */MultiInterface_new(KEY_SIZE, &externalIf, &ctx.ic, NULL);

    struct Entry* entries = Allocator_malloc(alloc, sizeof(struct Entry) * ENTRY_COUNT);
    Random_bytes(rand, (uint8_t*)entries, ENTRY_COUNT * sizeof(struct Entry));

    struct Interface** ifaces = Allocator_calloc(alloc, sizeof(char*), ENTRY_COUNT);

    // seed the list with some near collisions.
    for (int i = 0; i < 10; i++) {
        int rnd = (((uint32_t*)entries)[i] >> 1) % ENTRY_COUNT;
        ((uint32_t*) (&entries[rnd]))[0] = ((uint32_t*) (&entries[i]))[0];
    }

    for (int i = 0; i < CYCLES; i++) {
        int rnd = ((uint32_t*)entries)[i] % ENTRY_COUNT;
        struct Entry* entry = &entries[rnd];
        struct Interface* iface = ifaces[rnd];

        struct Message* msg;
        Message_STACK(msg, 0, 128);

        Message_push(msg, "hello world", 12, NULL);
        Message_push(msg, entry, 16, NULL);

        externalIf.receiveMessage(msg, &externalIf);

        //printf("Received message for iface [%u] from [%p]\n", rnd, (void*)ctx.receivedOn);
        if (iface) {
            Assert_true(ctx.receivedOn == iface);
        } else {
            ifaces[rnd] = ctx.receivedOn;
        }
    }

    Allocator_free(alloc);
    return 0;
}
