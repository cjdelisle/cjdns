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
#include "crypto/test/TestCa.h"
#include "rust/cjdns_sys/Rffi.h"

struct TestCa_s {
    RTypes_CryptoAuth2_t* ca2;
    bool noise;
};
typedef struct TestCa_Session_pvt_s {
    TestCa_Session_t pub;
    RTypes_CryptoAuth2_Session_t* s2;
    struct Iface sPlain;
    struct Iface sCipher;
    Identity
} TestCa_Session_pvt_t;

TestCa_t* TestCa_new(
    Allocator_t *allocator,
    const uint8_t *privateKey,
    struct Random* rand0)
{
    TestCa_t* out = Allocator_calloc(allocator, sizeof(TestCa_t), 1);
    out->ca2 = Rffi_CryptoAuth2_new(allocator, privateKey, rand0);
    return out;
}

int TestCa_addUser_ipv6(
    String_t *password,
    String_t *login,
    uint8_t *ipv6,
    TestCa_t *ca)
{
    return Rffi_CryptoAuth2_addUser_ipv6(password, login, ipv6, ca->ca2);
}

int TestCa_removeUsers(TestCa_t* ca, String_t* user)
{
    int i2 = Rffi_CryptoAuth2_removeUsers(ca->ca2, user);
    return i2;
}

RTypes_StrList_t* TestCa_getUsers(const TestCa_t *ca, Allocator_t *alloc)
{
    return Rffi_CryptoAuth2_getUsers(ca->ca2, alloc);
}

#include <stdio.h>

static Iface_DEFUN messagePlaintext(Message_t *msg, struct Iface* iface)
{
    TestCa_Session_pvt_t* sess = Identity_containerOf(iface, TestCa_Session_pvt_t, pub.plaintext);
    struct RTypes_Error_t* i2 = Iface_send(&sess->sPlain, msg);
    printf("Send2 done\n");
    return i2;
}

static Iface_DEFUN messageCiphertext(Message_t *msg, struct Iface* iface)
{
    TestCa_Session_pvt_t* sess = Identity_containerOf(iface, TestCa_Session_pvt_t, pub.ciphertext);
    struct RTypes_Error_t* i2 = Iface_send(&sess->sCipher, msg);
    return i2;
}

static Iface_DEFUN sPlainRecv(Message_t *msg, struct Iface* iface)
{
    TestCa_Session_pvt_t* sess = Identity_containerOf(iface, TestCa_Session_pvt_t, sPlain);
    return Iface_next(&sess->pub.plaintext, msg);
}

static Iface_DEFUN sCipherRecv(Message_t *msg, struct Iface* iface)
{
    TestCa_Session_pvt_t* sess = Identity_containerOf(iface, TestCa_Session_pvt_t, sCipher);
    return Iface_next(&sess->pub.ciphertext, msg);
}

TestCa_Session_t* TestCa_newSession(
    TestCa_t *ca,
    Allocator_t *alloc,
    const uint8_t *herPublicKey,
    bool requireAuth,
    char *name,
    bool useNoise)
{
    TestCa_Session_pvt_t* out = Allocator_calloc(alloc, sizeof(TestCa_Session_pvt_t), 1);
    Identity_set(out);
    out->sCipher.send = sCipherRecv;
    out->sPlain.send = sPlainRecv;
    out->s2 = Rffi_CryptoAuth2_newSession(
        ca->ca2, alloc, herPublicKey, requireAuth, name, ca->noise && useNoise);
    Iface_plumb(&out->sCipher, out->s2->ciphertext);
    Iface_plumb(&out->sPlain, out->s2->plaintext);
    out->pub.plaintext.send = messagePlaintext;
    out->pub.ciphertext.send = messageCiphertext;
    return &out->pub;
}

void TestCa_setAuth(const String_t* password, const String_t* login, TestCa_Session_t* session)
{
    TestCa_Session_pvt_t* sess = Identity_check((TestCa_Session_pvt_t*) session);
    Rffi_CryptoAuth2_setAuth(password, login, sess->s2);
}

void TestCa_resetIfTimeout(TestCa_Session_t* session)
{
    TestCa_Session_pvt_t* sess = Identity_check((TestCa_Session_pvt_t*) session);
    Rffi_CryptoAuth2_resetIfTimeout(sess->s2);
}

void TestCa_reset(TestCa_Session_t* session)
{
    TestCa_Session_pvt_t* sess = Identity_check((TestCa_Session_pvt_t*) session);
    Rffi_CryptoAuth2_reset(sess->s2);
}

RTypes_CryptoAuth_State_t TestCa_getState(TestCa_Session_t* session)
{
    TestCa_Session_pvt_t* sess = Identity_check((TestCa_Session_pvt_t*) session);
    return Rffi_CryptoAuth2_getState(sess->s2);
}

void TestCa_getHerPubKey(TestCa_Session_t* session, uint8_t* buf)
{
    TestCa_Session_pvt_t* sess = Identity_check((TestCa_Session_pvt_t*) session);
    Rffi_CryptoAuth2_getHerPubKey(sess->s2, buf);
}

void TestCa_getPubKey(TestCa_t *ca, uint8_t* buf)
{
    Rffi_CryptoAuth2_getPubKey(ca->ca2, buf);
}