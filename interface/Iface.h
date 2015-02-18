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
#ifndef Iface_H
#define Iface_H

#include <stdint.h>

#include "wire/Message.h"

struct Iface;

/**
 * @param thisInterface the interface which contains the sendMessage function pointer.
 * @param message the message
 */
typedef struct Iface* (* Iface_Callback)(struct Message* message, struct Iface* thisInterface);

#define Iface_DEFUN __attribute__ ((warn_unused_result)) struct Iface*

struct Iface
{
    /** Send a message through this interface. */
    Iface_Callback send;

    #ifdef PARANOIA
        /** This is for checking currentMsg Iface_next() has not been called incorrectly. */
        struct Message* currentMsg;
    #endif

    /** Interface to which this one is connected (if connected) */
    struct Iface* connectedIf;
};

static inline void Iface_send(struct Iface* iface, struct Message* msg)
{
    do {
        struct Iface* conn = iface->connectedIf;
        if (!conn) { return; }
        #ifdef PARANOIA
            Assert_true(conn->send);
            Assert_true(msg);
            msg->currentIface = iface;
            iface->currentMsg = msg;
        #endif
        #ifdef PARANOIA
            struct Iface* ifaceNext = conn->send(msg, conn);
            msg->currentIface = NULL;
            iface->currentMsg = NULL;
            iface = ifaceNext;
        #else
            iface = conn->send(conn, msg);
        #endif
        #ifndef STUPID_OPTIMIZATIONS
            Assert_true(!iface);
        #endif
    } while (iface);
}

/**
 * This is a hack high-performance version of Iface_send() which can only be used as a tail call.
 * You MUST pass the same msg which was passed to you, otherwise it will fail.
 */
static inline Iface_DEFUN Iface_next(struct Iface* iface, struct Message* msg)
{
    #ifdef PARANOIA
        Assert_true(msg->currentIface);
        Assert_true(msg->currentIface->currentMsg == msg);
    #endif

    #ifdef STUPID_OPTIMIZATIONS
        return iface;
    #endif

    Iface_send(iface, msg);
    return NULL;
}

#define Iface_CALL(func, msg, ...) \
    do {                                                      \
        struct Iface* Iface_x = func(msg, __VA_ARGS__);       \
        if (Iface_x) { Iface_send(Iface_x, msg); }            \
    } while (0)
// CHECKFILES_IGNORE missing ;

static inline void Iface_plumb(struct Iface* a, struct Iface* b)
{
    Assert_true(!a->connectedIf);
    Assert_true(!b->connectedIf);
    a->connectedIf = b;
    b->connectedIf = a;
}

static inline void Iface_unplumb(struct Iface* a, struct Iface* b)
{
    Assert_true(a->connectedIf == b);
    Assert_true(b->connectedIf == a);
    a->connectedIf = NULL;
    b->connectedIf = NULL;
}

#endif
