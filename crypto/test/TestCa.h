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
#ifndef TESTCA_H
#define TESTCA_H

#include "benc/String.h"
#include "rust/cjdns_sys/RTypes.h"
#include "interface/Iface.h"
#include "util/events/EventBase.h"
#include "crypto/random/Random.h"
#include "util/Linker.h"
Linker_require("crypto/test/TestCa.c")

typedef struct TestCa_s TestCa_t;
typedef struct TestCa_Session_s {
    struct Iface plaintext;
    struct Iface ciphertext;
} TestCa_Session_t;

enum TestCa_Config {
    TestCa_Config_OLD,
    TestCa_Config_OLD_NEW,
    TestCa_Config_NOISE
};

TestCa_t* TestCa_new(
    Allocator_t *allocator,
    const uint8_t *privateKey,
    struct EventBase* eventBase,
    struct Log* logger,
    struct Random* rand0,
    struct Random* rand1,
    enum TestCa_Config cfg);

int TestCa_addUser_ipv6(
    String_t *password,
    String_t *login,
    uint8_t *ipv6,
    TestCa_t *ca);

int TestCa_removeUsers(TestCa_t* context, String_t* user);

RTypes_StrList_t* TestCa_getUsers(const TestCa_t *ca, Allocator_t *alloc);

TestCa_Session_t * TestCa_newSession(
    TestCa_t *ca,
    Allocator_t *alloc,
    const uint8_t *herPublicKey,
    bool requireAuth,
    char *name,
    bool useNoise);

void TestCa_setAuth(const String_t *password, const String_t *login, TestCa_Session_t *caSession);

void TestCa_resetIfTimeout(TestCa_Session_t *session);

void TestCa_reset(TestCa_Session_t *session);

RTypes_CryptoAuth_State_t TestCa_getState(TestCa_Session_t *session);

void TestCa_getHerPubKey(TestCa_Session_t *session, uint8_t* buf);

void TestCa_getPubKey(
    TestCa_t *ca,
    uint8_t* buf);

#endif