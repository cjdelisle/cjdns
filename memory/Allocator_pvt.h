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
#ifndef Allocator_pvt_H
#define Allocator_pvt_H

#include "memory/Allocator.h"
#include "util/Identity.h"

#include <stdint.h>

struct Allocator_pvt;

struct Allocator_OnFreeJob_pvt;
struct Allocator_OnFreeJob_pvt {
    struct Allocator_OnFreeJob pub;
    struct Allocator_pvt* alloc;
    struct Allocator_OnFreeJob_pvt* next;

    /* prevent async jobs from being called multiple times, nonzero = done */
    int done;

    int line;
    const char* file;

    /** Set by allocator. */
    Identity
};

struct Allocator_Allocation_pvt;
struct Allocator_Allocation_pvt {
    struct Allocator_Allocation pub;
    struct Allocator_Allocation_pvt* next;
#ifdef Allocator_USE_CANARIES
    unsigned long beginCanary;
#endif
};

/** Singly linked list of allocators. */
struct Allocator_List;
struct Allocator_List {
    struct Allocator_pvt* alloc;
    struct Allocator_List* next;
};

struct Allocator_Adoptions {
    struct Allocator_List* parents;
    struct Allocator_List* children;
};

/** Internal state for Allocator. */
struct Allocator_pvt
{
    /** This allocator. */
    struct Allocator pub;

    /**
     * A linked list of the allocations made with this allocator.
     * These are all freed when the allocator is freed.
     */
    struct Allocator_Allocation_pvt* allocations;

    /** A linked list of jobs which must be done when this allocator is freed. */
    struct Allocator_OnFreeJob_pvt* onFree;

    /** The parent of this allocator, self-pointer if this is a root allocator. */
    struct Allocator_pvt* parent;

    /**
     * When this allocator is freed, lastSibling->nextSibling will be set to nextSibling
     * removing this allocator from the linked list.
     */
    struct Allocator_pvt* lastSibling;

    /** A pointer to the next allocator which is a child of the same parent. */
    struct Allocator_pvt* nextSibling;

    /** The first child allocator, this will be freed when this allocator is freed. */
    struct Allocator_pvt* firstChild;

    /** The root allocator with additional tree-global data. */
    struct Allocator_FirstCtx* rootAlloc;

    /** The number of bytes allocated by *this* allocator (but not it's children). */
    unsigned long allocatedHere;

    /**
     * If this allocator is neither an adopted parent nor an adopted child, this field is NULL,
     * Otherwise it is a linked list of adopted parents and children of this allocator.
     * The structure here is allocated by THIS ALLOCATOR, not by it's parent or child.
     */
    struct Allocator_Adoptions* adoptions;

    #ifdef Allocator_USE_CANARIES
        /** The canary for allocations made with this allocator constant to allow varification. */
        unsigned long canary;

        /** The canary which will be used for the next allocator, mutable. */
        unsigned long nextCanary;
    #endif

    /** For checking structure integrity. */
    Identity
};

/** The first ("genesis") allocator, not a child of any other allocator. */
struct Allocator_FirstCtx
{
    /** The context for the first allocator. */
    struct Allocator_pvt context;

    Allocator_Provider provider;

    Allocator_Provider_CONTEXT_TYPE* providerContext;

    /** The number of bytes which can be allocated by this allocator and all of its family. */
    int64_t spaceAvailable;

    /** The number of bytes which can be allocated total. */
    int64_t maxSpace;

    Identity
};

#endif
