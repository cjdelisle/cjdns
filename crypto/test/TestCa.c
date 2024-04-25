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
#include "crypto/CryptoAuth.h"
#include "rust/cjdns_sys/Rffi.h"
#include "util/Hex.h"

struct TestCa_s {
    RTypes_CryptoAuth2_t* ca2;
    struct CryptoAuth* ca;
    bool noise;
};
typedef struct TestCa_Session_pvt_s {
    TestCa_Session_t pub;
    RTypes_CryptoAuth2_Session_t* s2;
    struct CryptoAuth_Session* s;
    struct Iface sPlain;
    struct Iface sCipher;
    struct Iface s2Plain;
    struct Iface s2Cipher;
    Identity
} TestCa_Session_pvt_t;

TestCa_t* TestCa_new(
    Allocator_t *allocator,
    const uint8_t *privateKey,
    EventBase_t* eventBase,
    struct Log* logger,
    struct Random* rand0,
    struct Random* rand1,
    enum TestCa_Config cfg)
{
    TestCa_t* out = Allocator_calloc(allocator, sizeof(TestCa_t), 1);
    if (cfg == TestCa_Config_OLD || cfg == TestCa_Config_OLD_NEW) {
        out->ca = CryptoAuth_new(allocator, privateKey, eventBase, logger, rand0);
    }
    if (cfg == TestCa_Config_OLD_NEW || cfg == TestCa_Config_NOISE) {
        out->ca2 = Rffi_CryptoAuth2_new(allocator, privateKey, rand1);
    }
    if (cfg == TestCa_Config_NOISE) {
        out->noise = true;
    }
    return out;
}

int TestCa_addUser_ipv6(
    String_t *password,
    String_t *login,
    uint8_t *ipv6,
    TestCa_t *ca)
{
    int ret = 0;
    if (ca->ca) {
        ret = CryptoAuth_addUser_ipv6(password, login, ipv6, ca->ca);
    }
    if (ca->ca2) {
        int ret2 = Rffi_CryptoAuth2_addUser_ipv6(password, login, ipv6, ca->ca2);
        if (ca->ca) {
            Assert_true(ret == ret2);
        }
        return ret2;
    }
    return ret;
}

int TestCa_removeUsers(TestCa_t* ca, String_t* user)
{
    int i1 = 0;
    if (ca->ca) {
        i1 = CryptoAuth_removeUsers(ca->ca, user);
    }
    if (ca->ca2) {
        int i2 = Rffi_CryptoAuth2_removeUsers(ca->ca2, user);
        if (ca->ca) {
            Assert_true(i1 == i2);
        }
        return i2;
    }
    return i1;
}

RTypes_StrList_t* TestCa_getUsers(const TestCa_t *ca, Allocator_t *alloc)
{
    RTypes_StrList_t* l1 = NULL;
    if (ca->ca) {
        l1 = CryptoAuth_getUsers(ca->ca, alloc);
    }
    if (ca->ca2) {
        RTypes_StrList_t* l2 = Rffi_CryptoAuth2_getUsers(ca->ca2, alloc);
        Assert_true(l1->len == l2->len);
        for (uintptr_t i = 0; i < l1->len; i++) {
            Assert_true(String_equals(l1->items[i], l2->items[i]));
        }
        return l2;
    }
    return l1;
}

#define PASS 1
#define STOP 2
#define VERIFY 3

#include <stdio.h>

static Iface_DEFUN messagePlaintext(Message_t *msg, struct Iface* iface)
{
    TestCa_Session_pvt_t* sess = Identity_containerOf(iface, TestCa_Session_pvt_t, pub.plaintext);
    Message_t* m2 = NULL;
    if (sess->s2) {
        if (sess->s) {
            m2 = Message_clone(msg, Message_getAlloc(msg));
        } else {
            m2 = msg;
            msg = NULL;
        }
    }
    struct RTypes_Error_t* i1 = NULL;
    if (sess->s) {
        int flag = sess->s2 ? STOP : PASS;
        Er_assert(Message_epushAd(msg, &flag, sizeof flag));
        printf("Send [%d]\n", flag);
        i1 = Iface_send(&sess->sPlain, msg);
    }
    if (sess->s2) {
        if (sess->s) {
            Er_assert(Message_epushAd(m2, &msg, sizeof &msg));
        }
        int flag = sess->s ? VERIFY : PASS;
        Er_assert(Message_epushAd(m2, &flag, sizeof flag));
        printf("Send2 [%d]\n", flag);
        struct RTypes_Error_t* i2 = Iface_send(&sess->s2Plain, m2);
        if (sess->s) {
            Assert_true((i2 == NULL) == (i1 == NULL));
        }
        printf("Send2 done\n");
        return i2;
    }
    return i1;
}

static Iface_DEFUN messageCiphertext(Message_t *msg, struct Iface* iface)
{
    TestCa_Session_pvt_t* sess = Identity_containerOf(iface, TestCa_Session_pvt_t, pub.ciphertext);
    Message_t* m2 = NULL;
    if (sess->s2) {
        if (sess->s) {
            m2 = Message_clone(msg, Message_getAlloc(msg));
        } else {
            m2 = msg;
            msg = NULL;
        }
    }
    struct RTypes_Error_t* i1 = NULL;
    if (sess->s) {
        int flag = sess->s2 ? STOP : PASS;
        Er_assert(Message_epushAd(msg, &flag, sizeof flag));
        i1 = Iface_send(&sess->sCipher, msg);
    }
    if (sess->s2) {
        int flag = PASS;
        if (sess->s) {
            uintptr_t mp = (uintptr_t)msg;
            Er_assert(Message_epushAd(m2, &mp, sizeof &mp));
            flag = VERIFY;
        }
        Er_assert(Message_epushAd(m2, &flag, sizeof flag));
        struct RTypes_Error_t* i2 = Iface_send(&sess->s2Cipher, m2);
        if (sess->s) {
            Assert_true((i2 == NULL) == (i1 == NULL));
        }
        return i2;
    }
    return i1;
}

static bool check(Message_t *msg, TestCa_Session_pvt_t* sess)
{
    int flag = 0;
    Er_assert(Message_epopAd(msg, &flag, sizeof flag));
    if (flag == PASS) {
        printf("Passing message\n");
    } else if (flag == STOP) {
        // do nothing, wait for the next message to come through....
        printf("Stopping message\n");
        return true;
    } else if (flag == VERIFY) {
        uintptr_t m2p = 0;
        Er_assert(Message_epopAd(msg, &m2p, sizeof m2p));
        printf("Verifying message %lx\n", (unsigned long) m2p);
        struct Message* m2 = (struct Message*) m2p;
        if (Message_getLength(msg) != Message_getLength(m2)) {
            Assert_failure("Message_getLength(msg) != m2->length: %d != %d",
                Message_getLength(msg), Message_getLength(m2));
        }
        if (Bits_memcmp(msg->msgbytes, m2->msgbytes, Message_getLength(msg))) {
            const char* msgH = Hex_print(msg->msgbytes, Message_getLength(msg), Message_getAlloc(msg));
            const char* m2H = Hex_print(m2->msgbytes, Message_getLength(m2), Message_getAlloc(m2));
            Assert_failure("msg->bytes != m2->bytes:\n%s\n%s\n", msgH, m2H);
        }
        Assert_true(!Bits_memcmp(msg->msgbytes, m2->msgbytes, Message_getLength(msg)));
    } else {
        Assert_failure("unexpected flag [%d]", flag);
    }
    return false;
}

static Iface_DEFUN sPlainRecv(Message_t *msg, struct Iface* iface)
{
    TestCa_Session_pvt_t* sess = Identity_containerOf(iface, TestCa_Session_pvt_t, sPlain);
    if (check(msg, sess)) { return NULL; }
    return Iface_next(&sess->pub.plaintext, msg);
}

static Iface_DEFUN s2PlainRecv(Message_t *msg, struct Iface* iface)
{
    TestCa_Session_pvt_t* sess = Identity_containerOf(iface, TestCa_Session_pvt_t, s2Plain);
    if (check(msg, sess)) { return NULL; }
    return Iface_next(&sess->pub.plaintext, msg);
}

static Iface_DEFUN sCipherRecv(Message_t *msg, struct Iface* iface)
{
    TestCa_Session_pvt_t* sess = Identity_containerOf(iface, TestCa_Session_pvt_t, sCipher);
    if (check(msg, sess)) { return NULL; }
    return Iface_next(&sess->pub.ciphertext, msg);
}

static Iface_DEFUN s2CipherRecv(Message_t *msg, struct Iface* iface)
{
    TestCa_Session_pvt_t* sess = Identity_containerOf(iface, TestCa_Session_pvt_t, s2Cipher);
    if (check(msg, sess)) { return NULL; }
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
    out->s2Cipher.send = s2CipherRecv;
    out->s2Plain.send = s2PlainRecv;
    if (ca->ca) {
        out->s = CryptoAuth_newSession(ca->ca, alloc, herPublicKey, requireAuth, name, false);
        Iface_plumb(&out->sCipher, &out->s->ciphertext);
        Iface_plumb(&out->sPlain, &out->s->plaintext);
    }
    if (ca->ca2) {
        out->s2 = Rffi_CryptoAuth2_newSession(
            ca->ca2, alloc, herPublicKey, requireAuth, name, ca->noise && useNoise);
        Iface_plumb(&out->s2Cipher, out->s2->ciphertext);
        Iface_plumb(&out->s2Plain, out->s2->plaintext);
    }
    out->pub.plaintext.send = messagePlaintext;
    out->pub.ciphertext.send = messageCiphertext;
    return &out->pub;
}

void TestCa_setAuth(const String_t* password, const String_t* login, TestCa_Session_t* session)
{
    TestCa_Session_pvt_t* sess = Identity_check((TestCa_Session_pvt_t*) session);
    if (sess->s) {
        CryptoAuth_setAuth(password, login, sess->s);
    }
    if (sess->s2) {
        Rffi_CryptoAuth2_setAuth(password, login, sess->s2);
    }
}

void TestCa_resetIfTimeout(TestCa_Session_t* session)
{
    TestCa_Session_pvt_t* sess = Identity_check((TestCa_Session_pvt_t*) session);
    if (sess->s) {
        CryptoAuth_resetIfTimeout(sess->s);
    }
    if (sess->s2) {
        Rffi_CryptoAuth2_resetIfTimeout(sess->s2);
    }
}

void TestCa_reset(TestCa_Session_t* session)
{
    TestCa_Session_pvt_t* sess = Identity_check((TestCa_Session_pvt_t*) session);
    if (sess->s) {
        CryptoAuth_reset(sess->s);
    }
    if (sess->s2) {
        Rffi_CryptoAuth2_reset(sess->s2);
    }
}

RTypes_CryptoAuth_State_t TestCa_getState(TestCa_Session_t* session)
{
    TestCa_Session_pvt_t* sess = Identity_check((TestCa_Session_pvt_t*) session);
    RTypes_CryptoAuth_State_t st = 0;
    if (sess->s) {
        st = CryptoAuth_getState(sess->s);
    }
    if (sess->s2) {
        RTypes_CryptoAuth_State_t st2 = Rffi_CryptoAuth2_getState(sess->s2);
        if (sess->s) {
            Assert_true(st2 == st);
        }
        return st2;
    }
    return st;
}

void TestCa_getHerPubKey(TestCa_Session_t* session, uint8_t* buf)
{
    TestCa_Session_pvt_t* sess = Identity_check((TestCa_Session_pvt_t*) session);
    uint8_t hpk1[32];
    if (sess->s) {
        CryptoAuth_getHerPubKey(sess->s, hpk1);
    }
    if (sess->s2) {
        Rffi_CryptoAuth2_getHerPubKey(sess->s2, buf);
        if (sess->s) {
            Assert_true(!Bits_memcmp(hpk1, buf, 32));
        }
        return;
    }
    Bits_memcpy(buf, hpk1, 32);
}

void TestCa_getPubKey(TestCa_t *ca, uint8_t* buf)
{
    uint8_t pk1[32];
    if (ca->ca) {
        CryptoAuth_getPubKey(ca->ca, pk1);
    }
    if (ca->ca2) {
        Rffi_CryptoAuth2_getPubKey(ca->ca2, buf);
        if (ca->ca) {
            Assert_true(!Bits_memcmp(pk1, buf, 32));
        }
        return;
    }
    Bits_memcpy(buf, pk1, 32);
}