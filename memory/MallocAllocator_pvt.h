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
#ifndef MallocAllocator_pvt_H
#define MallocAllocator_pvt_H

#include "memory/MallocAllocator.h"
#include "util/Identity.h"

#include <stdint.h>

struct MallocAllocator_pvt;

struct MallocAllocator_OnFreeJob;
struct MallocAllocator_OnFreeJob {
    struct Allocator_OnFreeJob generic;
    struct MallocAllocator_pvt* alloc;
    struct MallocAllocator_OnFreeJob* next;

    /* prevent async jobs from being called multiple times, nonzero = done */
    int done;

    const char* file;
    int line;
};

struct MallocAllocator_Allocation;
struct MallocAllocator_Allocation {
    struct MallocAllocator_Allocation* next;
    size_t size;
#ifdef MallocAllocator_USE_CANARIES
    long beginCanary;
#endif
};

/** Singly linked list of allocators. */
struct MallocAllocator_List;
struct MallocAllocator_List {
    struct MallocAllocator_pvt* alloc;
    struct MallocAllocator_List* next;
};

struct MallocAllocator_Adoptions {
    struct MallocAllocator_List* parents;
    struct MallocAllocator_List* children;
};

/** Internal state for Allocator. */
struct MallocAllocator_pvt
{
    /** This allocator. */
    struct Allocator pub;

    /**
     * A linked list of the allocations made with this allocator.
     * These are all freed when the allocator is freed.
     */
    struct MallocAllocator_Allocation* allocations;

    /** A linked list of jobs which must be done when this allocator is freed. */
    struct MallocAllocator_OnFreeJob* onFree;

    /**
     * When this allocator is freed, lastSibling->nextSibling will be set to nextSibling
     * removing this allocator from the linked list.
     * GOTCHYA: if this is the first sibling, lastSibling will point to the parent and
     *          in that case, lastSibling->firstChild becomes nextSibling.
     */
    struct MallocAllocator_pvt* lastSibling;

    /** A pointer to the next allocator which is a child of the same parent. */
    struct MallocAllocator_pvt* nextSibling;

    /** The first child allocator, this will be freed when this allocator is freed. */
    struct MallocAllocator_pvt* firstChild;

    /** The root allocator with additional tree-global data. */
    struct MallocAllocator_FirstCtx* rootAlloc;

    /** The number of bytes allocated by *this* allocator (but not it's children). */
    unsigned long allocatedHere;

    /**
     * If this allocator is neither an adopted parent nor an adopted child, this field is NULL,
     * Otherwise it is a linked list of adopted parents and children of this allocator.
     * The structure here is allocated by THIS ALLOCATOR, not by it's parent or child.
     */
    struct MallocAllocator_Adoptions* adoptions;

    /** This is the location where the allocator was created. */
    const char* identFile;
    int identLine;

    #ifdef MallocAllocator_USE_CANARIES
        /** The canary for allocations made with this allocator constant to allow varification. */
        long canary;

        /** The canary which will be used for the next allocator, mutable. */
        long nextCanary;
    #endif

    /** For checking structure integrity. */
    Identity
};

/** The first ("genesis") allocator, not a child of any other allocator. */
struct MallocAllocator_FirstCtx
{
    /** The context for the first allocator. */
    struct MallocAllocator_pvt context;

    /** The number of bytes which can be allocated by this allocator and all of its family. */
    int64_t spaceAvailable;

    /** The number of bytes which can be allocated total. */
    size_t maxSpace;
};

#endif
