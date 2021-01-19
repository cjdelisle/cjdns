#ifndef rffi_H
#define rffi_H

// This file is generated from src/rffi.rs using cbindgen

#include "RffiPrefix.h"

typedef struct Rffi_CryptoAuth2_Session_t Rffi_CryptoAuth2_Session_t;

typedef struct Rffi_CryptoAuth2_t Rffi_CryptoAuth2_t;

RTypes_IfWrapper_t Rffi_testwrapper_create(Allocator_t *a);

RTypes_IfWrapper_t Rffi_android_create(Allocator_t *a);

int Rffi_CryptoAuth2_addUser_ipv6(String_t *password,
                                  String_t *login,
                                  uint8_t *ipv6,
                                  Rffi_CryptoAuth2_t *ca);

int Rffi_CryptoAuth2_removeUsers(Rffi_CryptoAuth2_t *context, String_t *user);

RTypes_StrList_t *Rffi_CryptoAuth2_getUsers(const Rffi_CryptoAuth2_t *ca, Allocator_t *alloc);

Rffi_CryptoAuth2_t *Rffi_CryptoAuth2_new(Allocator_t *allocator,
                                         const uint8_t *privateKey,
                                         Random *random);

Rffi_CryptoAuth2_Session_t *Rffi_CryptoAuth2_newSession(Rffi_CryptoAuth2_t *ca,
                                                        Allocator_t *alloc,
                                                        const uint8_t *herPublicKey,
                                                        bool requireAuth,
                                                        char *name,
                                                        bool useNoise);

int Rffi_CryptoAuth2_encrypt(Rffi_CryptoAuth2_Session_t *session, Message_t *msg);

int Rffi_CryptoAuth2_decrypt(Rffi_CryptoAuth2_Session_t *sess, Message_t *msg);

void Rffi_CryptoAuth2_setAuth(const String_t *password,
                              const String_t *login,
                              Rffi_CryptoAuth2_Session_t *caSession);

void Rffi_CryptoAuth2_resetIfTimeout(Rffi_CryptoAuth2_Session_t *session);

void Rffi_CryptoAuth2_reset(Rffi_CryptoAuth2_Session_t *caSession);

RTypes_CryptoAuth_State_t Rffi_CryptoAuth2_getState(Rffi_CryptoAuth2_Session_t *session);

void Rffi_CryptoAuth2_getHerPubKey(const Rffi_CryptoAuth2_Session_t *session, uint8_t *pkOut);

void Rffi_CryptoAuth2_getHerIp6(const Rffi_CryptoAuth2_Session_t *session, uint8_t *ipOut);

String_t *Rffi_CryptoAuth2_getName(const Rffi_CryptoAuth2_Session_t *session, Allocator_t *alloc);

void Rffi_CryptoAuth2_getPubKey(Rffi_CryptoAuth2_t *ca, uint8_t *pkOut);

void Rffi_CryptoAuth2_stats(const Rffi_CryptoAuth2_Session_t *session,
                            RTypes_CryptoStats_t *statsOut);

void Rffi_panic(const char *msg);

#endif /* rffi_H */
