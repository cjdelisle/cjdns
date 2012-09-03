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

#include "admin/Admin.h"
#include "crypto/Crypto.h"
#include "exception/AbortHandler.h"
#include "io/FileWriter.h"
#include "io/Writer.h"
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"

#include <stdlib.h>
#include <unistd.h>

/*
 * This process is started with 3 parameters, they must all be numeric in base 10.
 * toAngel the pipe which is used to send data back to the angel process.
 * fromAngel the pipe which is used to read incoming data from the angel.
 * maxAlloc the number of bytes of memory which the core is allowed to allocate.
 *
 * Upon initialization, this process will immedietly write 8 bytes to the toAngel pipe.
 * these 8 bytes (syncMagic) will henceforth be used for indicating the border of a frame.
 */
int main(int argc, char** argv)
{
printf("core starting\n");
    uint8_t syncMagic[8];
    randombytes(syncMagic, 8);

    struct Except* eh = AbortHandler_INSTANCE;
    int toAngel;
    int fromAngel;
    int maxAlloc;
    if (argc != 4
        || !(toAngel = atoi(argv[1]))
        || !(fromAngel = atoi(argv[2]))
        || !(maxAlloc = atoi(argv[3]))
        || 8 != write(toAngel, syncMagic, 8))
    {
        Except_raise(eh, -1, "This is internal to cjdns and shouldn't started manually.");
    }

    struct Allocator* alloc = MallocAllocator_new(maxAlloc);
    struct event_base* eventBase = event_base_new();

    struct Writer* logwriter = FileWriter_new(stdout, alloc);
    struct Log* logger = &(struct Log) { .writer = logwriter };

    /*struct Admin* admin = */
    Admin_new(fromAngel, toAngel, alloc, logger, eventBase, NULL, syncMagic);

    event_base_dispatch(eventBase);
}
