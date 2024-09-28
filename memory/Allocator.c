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

#include "memory/Allocator.h"
#include "rust/cjdns_sys/Rffi.h"

void Allocator__free(struct Allocator* alloc, const char* file, int line)
{
    Rffi_allocator_free(alloc, file, (uintptr_t) line);
}

int Allocator_isFreeing(Allocator_t* alloc)
{
    return Rffi_allocator_isFreeing(alloc);
}

void* Allocator__malloc(struct Allocator* allocator,
                        unsigned long length,
                        const char* fileName,
                        int lineNum)
{
    return (void*) Rffi_allocator_malloc(allocator, length);
}

void* Allocator__calloc(struct Allocator* alloc,
                        unsigned long length,
                        unsigned long count,
                        const char* fileName,
                        int lineNum)
{
    void* pointer = Allocator__malloc(alloc, length * count, fileName, lineNum);
    Bits_memset(pointer, 0, length * count);
    return pointer;
}

void* Allocator__realloc(struct Allocator* allocator,
                         const void* original,
                         unsigned long size,
                         const char* fileName,
                         int lineNum)
{
    return (void*) Rffi_allocator_realloc(allocator, (uint8_t*) original, size);
}

void* Allocator__clone(struct Allocator* allocator,
                       const void* toClone,
                       unsigned long length,
                       const char* fileName,
                       int lineNum)
{
    void* pointer = Allocator__malloc(allocator, length, fileName, lineNum);
    Bits_memcpy(pointer, toClone, length);
    return pointer;
}

struct Allocator* Allocator__child(struct Allocator* allocator, const char* file, int line)
{
    return Rffi_allocator_child(allocator, file, line);
}

int Allocator_cancelOnFree(struct Allocator_OnFreeJob* toRemove)
{
    /// XXX
    return 1;
}

void Allocator__adopt(struct Allocator* adoptedParent,
                      struct Allocator* childToAdopt,
                      const char* file,
                      int line)
{
    Rffi_allocator_adopt(adoptedParent, childToAdopt);
}

static void onFree(void* voj)
{
    struct Allocator_OnFreeJob* oj = Identity_check((struct Allocator_OnFreeJob*) voj);
    oj->callback(oj);
}

struct Allocator_OnFreeJob* Allocator__onFree(struct Allocator* alloc,
                                              Allocator_OnFreeCallback callback,
                                              void* callbackContext,
                                              const char* file,
                                              int line)
{
    struct Allocator_OnFreeJob* oj = Allocator__malloc(alloc, sizeof(struct Allocator_OnFreeJob), file, line);
    Identity_set(oj);
    oj->callback = callback;
    oj->userData = callbackContext;
    Rffi_allocator_onFree(alloc, onFree, oj, file, line);
    return oj;
}

struct Allocator* Allocator__new(unsigned long sizeLimit,
                                 const char* fileName,
                                 int lineNum)
{
    return Rffi_allocator_newRoot(fileName, lineNum);
}

unsigned long Allocator_bytesAllocated(struct Allocator* allocator)
{
    return 0;
}

void Allocator_snapshot(struct Allocator* alloc, int includeAllocations)
{

}

void Allocator_setCanary(struct Allocator* alloc, uintptr_t value)
{
}
