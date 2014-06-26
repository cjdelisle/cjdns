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
#include "interface/Interface.h"
#include "interface/FramingInterface.h"
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "util/Endian.h"
#include "util/Bits.h"
#include "util/CString.h"
#include "wire/Error.h"

union MessageLength
{
    uint32_t length_be;
    uint8_t bytes[4];
};

static uint8_t messageOut(struct Message* msg, struct Interface* iface)
{
    struct Message** msgPtr = iface->receiverContext;
    Allocator_adopt((*msgPtr)->alloc, msg->alloc);
    *msgPtr = msg;
    return 0;
}

static void send(struct Interface* sendTo, struct Message* toSend, struct Allocator* cloneWith)
{
    struct Allocator* child = Allocator_child(cloneWith);
    toSend = Message_clone(toSend, child);
    Interface_receiveMessage(sendTo, toSend);
    Allocator_free(child);
}

int main()
{
    struct Allocator* alloc = MallocAllocator_new(1<<20);
    struct Interface dummy = { .sendMessage = NULL };
    struct Interface* fi = FramingInterface_new(1024, &dummy, alloc);
    fi->receiveMessage = messageOut;
    struct Message* output = NULL;
    fi->receiverContext = &output;

    char* text = "Hello World!";
    Assert_true(12 == CString_strlen(text));
    union MessageLength ml = { .length_be = Endian_hostToBigEndian32(12) };

    struct Message* msg;

    {
        // first 2 bytes of length
        Message_STACK(msg, 0, 2);
        Message_push(msg, ml.bytes, 2, NULL);
        send(&dummy, msg, alloc);
    }

    {
        // last 2 bytes of length and first 5 bytes of message "Hello"
        Message_STACK(msg, 0, 7);
        Message_push(msg, text, 5, NULL);
        Message_push(msg, &ml.bytes[2], 2, NULL);
        send(&dummy, msg, alloc);
    }

    Assert_true(output == NULL);
    struct Allocator* child = Allocator_child(alloc);
    output = &(struct Message) { .alloc = child };

    {
        // last 7 bytes of message " World!" and first byte of length of second message.
        Message_STACK(msg, 0, 8);
        Message_push(msg, ml.bytes, 1, NULL);
        Message_push(msg, &text[5], 7, NULL);
        send(&dummy, msg, alloc);
    }

    Assert_true(output && output->length == (int)CString_strlen(text));
    Assert_true(!Bits_memcmp(output->bytes, text, CString_strlen(text)));

    Allocator_free(child);
    child = Allocator_child(alloc);
    output = &(struct Message) { .alloc = child };

    {
        // Send last 3 bytes of length and entire message.
        Message_STACK(msg, 0, 15);
        Message_push(msg, text, 12, NULL);
        Message_push(msg, &ml.bytes[1], 3, NULL);
        send(&dummy, msg, alloc);
    }

    Assert_true(output && output->length == (int)CString_strlen(text));
    Assert_true(!Bits_memcmp(output->bytes, text, CString_strlen(text)));

    Allocator_free(alloc);

    return 0;
}
