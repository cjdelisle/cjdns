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

#include "benc/serialization/json/JsonBencMessageReader.h"
#include "benc/Object.h"
#include "exception/Err.h"
#include "memory/Allocator.h"
#include "rust/cjdns_sys/RTypes.h"
#include "rust/cjdns_sys/Rffi.h"
#include "wire/Message.h"

#include <stdbool.h>

Err_DEFUN JsonBencMessageReader_read(
    Dict_t** out,
    Message_t* msg,
    struct Allocator* alloc,
    bool lax
) {
    return Rffi_Benc_decodeJson(out, msg, lax, alloc);
}

Err_DEFUN JsonBencMessageReader_write(
    Dict_t* input,
    Message_t* msg,
    struct Allocator* alloc
) {
    return Rffi_Benc_encodeJson(input, msg, alloc);
}

const char* JsonBencMessageReader_readNoExcept(
    Message_t* msg,
    struct Allocator* alloc,
    Dict_t** out,
    bool lax)
{
    RTypes_Error_t* er = JsonBencMessageReader_read(out, msg, alloc, lax);
    if (er) {
        return Rffi_printError(er, alloc);
    }
    return NULL;
}

