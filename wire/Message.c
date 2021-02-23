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
#include "wire/Message.h"
#include "util/UniqueName.h"

struct Message* Message_new(uint32_t messageLength,
                                          uint32_t amountOfPadding,
                                          struct Allocator* alloc)
{
    uint8_t* buff = Allocator_malloc(alloc, messageLength + amountOfPadding);
    struct Message* out = Allocator_calloc(alloc, sizeof(struct Message), 1);
    out->_ad = buff;
    out->_adLen = 0;
    out->bytes = &buff[amountOfPadding];
    out->length = out->capacity = messageLength;
    out->padding = amountOfPadding;
    out->_alloc = alloc;
    return out;
}

void Message_setAssociatedFd(struct Message* msg, int fd)
{
    if (fd == -1) {
        msg->_associatedFd = 0;
    } else if (fd == 0) {
        msg->_associatedFd = -1;
    } else {
        msg->_associatedFd = fd;
    }
}

int Message_getAssociatedFd(struct Message* msg)
{
    if (msg->_associatedFd == -1) {
        return 0;
    } else if (msg->_associatedFd == 0) {
        return -1;
    } else {
        return msg->_associatedFd;
    }
}

struct Message* Message_clone(struct Message* toClone, struct Allocator* alloc)
{
    Assert_true(toClone->capacity >= toClone->length);
    int32_t len = toClone->capacity + toClone->padding + toClone->_adLen;
    uint8_t* allocation = Allocator_malloc(alloc, len + 8);
    while (((uintptr_t)allocation % 8) != (((uintptr_t)toClone->bytes - toClone->padding - toClone->_adLen) % 8)) {
        allocation++;
    }
    Bits_memcpy(allocation, toClone->bytes - toClone->padding - toClone->_adLen, len);
    return Allocator_clone(alloc, (&(struct Message) {
        .length = toClone->length,
        .padding = toClone->padding,
        .bytes = allocation + toClone->_adLen + toClone->padding,
        ._ad = allocation + toClone->_adLen,
        ._adLen = toClone->_adLen,
        .capacity = toClone->capacity,
        ._alloc = alloc
    }));
}