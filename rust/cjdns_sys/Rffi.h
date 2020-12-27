#ifndef rffi_H
#define rffi_H

// This file is generated from src/rffi.rs using cbindgen

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "cffi.h"

typedef struct Rffi_CryptoAuth2_Session_t Rffi_CryptoAuth2_Session_t;

typedef struct Rffi_CryptoAuth2_t Rffi_CryptoAuth2_t;

typedef struct Vec_Vec_u8 Vec_Vec_u8;

typedef struct {
  Iface_t *internal;
  Iface_t *external;
} Rffi_IfWrapper_t;

Rffi_IfWrapper_t Rffi_testwrapper_create(Allocator_t *a);

Rffi_IfWrapper_t Rffi_android_create(Allocator_t *a);

int Rffi_CryptoAuth2_addUser_ipv6(String_t *password,
                                  String_t *login,
                                  uint8_t *ipv6,
                                  Rffi_CryptoAuth2_t *ca);

Vec_Vec_u8 *Rffi_CryptoAuth2_getUsers(const Rffi_CryptoAuth2_t *ca, Allocator_t *alloc);

Rffi_CryptoAuth2_t *Rffi_CryptoAuth2_new(Allocator_t *allocator, const uint8_t *privateKey);

Rffi_CryptoAuth2_Session_t *Rffi_CryptoAuth2_newSession(Rffi_CryptoAuth2_t *ca,
                                                        Allocator_t *alloc,
                                                        const uint8_t *herPublicKey,
                                                        bool requireAuth,
                                                        char *name);

int Rffi_CryptoAuth2_encrypt(Rffi_CryptoAuth2_Session_t *session, Message_t *msg);

int Rffi_CryptoAuth2_decrypt(Rffi_CryptoAuth2_Session_t *sess, Message_t *msg);

void Rffi_CryptoAuth2_setAuth(const String_t *password,
                              const String_t *login,
                              Rffi_CryptoAuth2_Session_t *caSession);

void Rffi_CryptoAuth2_resetIfTimeout(Rffi_CryptoAuth2_Session_t *session);

void Rffi_CryptoAuth2_reset(Rffi_CryptoAuth2_Session_t *caSession);

#endif /* rffi_H */
