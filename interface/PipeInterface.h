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
#ifndef PipeInterface_H
#define PipeInterface_H

#include "crypto/random/Random.h"
#include "interface/Interface.h"
#include "exception/Except.h"
#include "memory/Allocator.h"
#include "util/events/EventBase.h"
#include "util/log/Log.h"

/** Raised if there was an error on the pipe. */
#define PipeInterface_DISCONNECTED -1

/** Raised if the other end fails to respond to pings. */
#define PipeInterface_TIMEOUT -2

#define PipeInterface_PADDING 1024
#define PipeInterface_MAX_MESSAGE_SIZE 65535

enum PipeInterface_State
{
    /**
     * If in this state, only ping messages are accepted and syncMagic
     * will be changed to what is sent from the other end if that value is lower.
     * This is how two ends agree on sync magic.
     */
    PipeInterface_State_INITIALIZING,

    /** Normal running state. */
    PipeInterface_State_ESTABLISHED,

    /** Synchronization lost. */
    PipeInterface_State_LOST
};

struct PipeInterface
{
    struct Interface generic;
    struct Except* exceptionHandler;
    enum PipeInterface_State state;
};

void PipeInterface_waitUntilReady(struct PipeInterface* pif);

struct PipeInterface* PipeInterface_new(int inPipe,
                                        int outPipe,
                                        struct EventBase* eventBase,
                                        struct Log* logger,
                                        struct Allocator* alloc,
                                        struct Random* rand);

#endif
