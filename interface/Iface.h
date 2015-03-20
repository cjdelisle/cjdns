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
#include "util/Defined.h"

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

/**
 * Send a message to an Iface.
 * Whatever interface has been *plumbed* to this interface using Iface_plumb() will receive
 * the message. If you are in an Iface_Callback function, you must not use this function to
 * forward the message which you have received as your input, to do so will cause a runtime
 * assertion error, in order to forward the message which you received, you must use Iface_next()
 * and it must be a tail-call.
 */
static inline void Iface_send(struct Iface* iface, struct Message* msg)
{
    do {
        struct Iface* conn = iface->connectedIf;

        #ifdef PARANOIA
            Assert_true(conn);
            Assert_true(conn->send);
            Assert_true(msg);
            struct Message* currentMsg = conn->currentMsg;
            msg->currentIface = conn;
            conn->currentMsg = msg;
        #endif

        iface = conn->send(msg, conn);

        #ifdef PARANOIA
            msg->currentIface = NULL;
            conn->currentMsg = currentMsg;
        #endif

        if (!Defined(Iface_OPTIMIZE)) {
            Assert_true(!iface);
        }
    } while (iface);
}

/**
 * Forward a message from inside of an Iface_Callback function.
 * This function must be a tail-call, you must return the value returned to you.
 * If you do anything between the call to this function and your return of it's return value,
 * the order in which that thing happens is undefined.
 *
 * Consider the following (bad) code:
 *     struct Iface* retVal = Iface_next(iface, msg);
 *     x++;
 *     return retVal;
 *
 * If Iface_OPTIMIZE is enabled, it becomes equivilant to:
 *     x++;
 *     struct Iface* retVal = Iface_next(iface, msg);
 *     return retVal;
 *
 * So simplify your life and follow the basic rule of always returning directly the value
 * from this function, IE: return Iface_next(iface, msg);
 */
static inline Iface_DEFUN Iface_next(struct Iface* iface, struct Message* msg)
{
    #ifdef PARANOIA
        struct Iface* conn = iface->connectedIf;
        struct Message* currentMsg = conn->currentMsg;
        Assert_true(msg->currentIface);
        Assert_true(msg->currentIface->currentMsg == msg);
        msg->currentIface->currentMsg = NULL;
    #endif

    if (Defined(Iface_OPTIMIZE)) {
        #ifdef PARANOIA
            msg->currentIface = conn;
            conn->currentMsg = msg;
        #endif
        return iface;
    }

    #ifdef PARANOIA
        // done inside of Iface_send()
        msg->currentIface = NULL;
        conn->currentMsg = NULL;
    #endif

    Iface_send(iface, msg);

    #ifdef PARANOIA
        conn->currentMsg = currentMsg;
    #endif

    return NULL;
}

/**
 * Call a function which might use Iface_next()
 * This macro will call a function, passing it a message and other arguments, if the function
 * you are calling might use Iface_next(), you must call it with this macro instead of calling
 * it directly.
 * If you are calling from a Iface_Callback function, you must not use this function to call
 * the next function with the message passed to you. In that case just call the function directly.
 */
#ifdef PARANOIA
    #define Iface_CALL(func, msg, ...) \
        do {                                                \
            Assert_true(!msg->currentIface);                \
            struct Iface Iface_y = { .currentMsg = msg };   \
            msg->currentIface = &Iface_y;                   \
            struct Iface* Iface_x = func(msg, __VA_ARGS__); \
            msg->currentIface = NULL;                       \
            if (Iface_x) { Iface_send(Iface_x, msg); }      \
        } while (0)
    // CHECKFILES_IGNORE missing ;
#else
    #define Iface_CALL(func, msg, ...) \
        do {                                                 \
            struct Iface* Iface_x = func(msg, __VA_ARGS__);  \
            if (Iface_x) { Iface_send(Iface_x, msg); }       \
        } while (0)
    // CHECKFILES_IGNORE missing ;
#endif

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
