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
#ifndef Iface_H
#define Iface_H

#include <stdint.h>

#include "rust/cjdns_sys/RTypes.h"
#include "wire/Message.h"
#include "util/Defined.h"
#include "util/Identity.h"
#include "util/Linker.h"
Linker_require("interface/Iface.c")

struct Iface;

/**
 * @param thisInterface the interface which contains the sendMessage function pointer.
 * @param message the message
 */
typedef struct RTypes_Error_t* (* Iface_Callback)(Message_t* message, struct Iface* thisInterface);

#define Iface_DEFUN __attribute__ ((warn_unused_result)) struct RTypes_Error_t*

struct Iface
{
    /** Send a message through this interface. */
    Iface_Callback send;

    #ifdef PARANOIA
        /** This is for checking currentMsg Iface_next() has not been called incorrectly. */
        Message_t* currentMsg;
    #endif

    /** Interface to which this one is connected (if connected) */
    struct Iface* connectedIf;

    Identity
} /* Iface_t defined in RffiPrefix.h */;

// This needs to be in a C file in order to be accessible from Rust
Iface_DEFUN Iface_incoming_fromRust(Message_t* message, struct Iface* thisInterface);

void Iface_setIdentity(struct Iface* iface);
void Iface_checkIdentity(struct Iface* iface);

/**
 * Send a message to an Iface.
 * Whatever interface has been *plumbed* to this interface using Iface_plumb() will receive
 * the message. If you are in an Iface_Callback function, you must not use this function to
 * forward the message which you have received as your input, to do so will cause a runtime
 * assertion error, in order to forward the message which you received, you must use Iface_next()
 * and it must be a tail-call.
 */
static inline struct RTypes_Error_t* Iface_send(struct Iface* iface, Message_t* msg)
{
    Iface_checkIdentity(iface);
    struct Iface* conn = iface->connectedIf;
    Iface_checkIdentity(conn);

    #ifdef PARANOIA
        Assert_true(conn);
        Assert_true(conn->send);
        Assert_true(msg);
        Message_t* currentMsg = conn->currentMsg;
        msg->currentIface = conn;
        conn->currentMsg = msg;
    #endif

    struct RTypes_Error_t* ret = conn->send(msg, conn);

    #ifdef PARANOIA
        msg->currentIface = NULL;
        conn->currentMsg = currentMsg;
    #endif

    return ret;
}

/**
 * Forward a message from inside of an Iface_Callback function.
 */
static inline Iface_DEFUN Iface_next(struct Iface* iface, Message_t* msg)
{
    #ifdef PARANOIA
        struct Iface* conn = iface->connectedIf;
        Message_t* currentMsg = conn->currentMsg;
        Assert_true(msg->currentIface);
        Assert_true(msg->currentIface->currentMsg == msg);
        msg->currentIface->currentMsg = NULL;
    #endif

    #ifdef PARANOIA
        // done inside of Iface_send()
        msg->currentIface = NULL;
        conn->currentMsg = NULL;
    #endif

    struct RTypes_Error_t* ret = Iface_send(iface, msg);

    #ifdef PARANOIA
        conn->currentMsg = currentMsg;
    #endif

    return ret;
}

/**
 * Call a function which might use Iface_next()
 * This macro will call a function, passing it a message and other arguments, if the function
 * you are calling might use Iface_next(), you must call it with this macro instead of calling
 * it directly.
 * If you are calling from a Iface_Callback function, you must not use this function to call
 * the next function with the message passed to you. In that case just call the function directly.
 *
 * Why is there code using this weird function rather than simply calling Iface_send() ?
 * Iface_send() only works when you send to *your* iface which is plumbed to the iface that you want
 * the data to go to, this macro will be used when you want to bypass the need to create a local
 * iface and plumb it to the remote one just in order to send one message.
 */
#ifdef PARANOIA
    #define Iface_CALL(func, msg, ...) \
        (__extension__ ({                                    \
            Assert_true(!msg->currentIface);                \
            struct Iface Iface_y = { .currentMsg = msg };   \
            msg->currentIface = &Iface_y;                   \
            struct RTypes_Error_t* ret = func(msg, __VA_ARGS__); \
            msg->currentIface = NULL;                       \
            ret; \
        }))
    // CHECKFILES_IGNORE missing ;
#else
    #define Iface_CALL(func, msg, ...) func(msg, __VA_ARGS__)
#endif

static inline void Iface_plumb(struct Iface* a, struct Iface* b)
{
    Assert_true(!a->connectedIf);
    Assert_true(!b->connectedIf);
    Iface_setIdentity(a);
    Iface_setIdentity(b);
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
