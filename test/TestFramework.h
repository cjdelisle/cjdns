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
#ifndef TestFramework_H
#define TestFramework_H

#include "net/NetCore.h"
#include "util/Linker.h"
Linker_require("test/TestFramework.c");

struct TestFramework
{
    struct Allocator* alloc;
    struct Random* rand;
    struct EventBase* eventBase;
    struct Log* logger;
    struct Pathfinder* pathfinder;
    struct Iface* tunIf;
    struct NetCore* nc;

    /** The last message which this node sent. */
    struct Message* lastMsg;

    /**
     * A backup of the last message which this node sent.
     * Used to check if the framework alters the message after sending it.
     */
    struct Message* lastMsgBackup;

    uint8_t* publicKey;
    uint8_t* ip;

    Identity
};

#define TestFramework_KEY_SIZE 8


struct TestFramework* TestFramework_setUp(char* privateKey,
                                          struct Allocator* allocator,
                                          struct EventBase* base,
                                          struct Random* rand,
                                          struct Log* logger);

void TestFramework_linkNodes(struct TestFramework* client,
                             struct TestFramework* server,
                             bool beacon);

void TestFramework_craftIPHeader(struct Message* msg, uint8_t srcAddr[16], uint8_t destAddr[16]);

/** Check if the last message sent was altered after having been sent. */
void TestFramework_assertLastMessageUnaltered(struct TestFramework* tf);

#endif
