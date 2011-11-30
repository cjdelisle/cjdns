#ifndef CRYPTO_AUTH_H
#define CRYPTO_AUTH_H

#include <stdint.h>
#include <stdbool.h>

#include "interface/Interface.h"
#include "libbenc/benc.h"
#include "memory/MemAllocator.h"

struct CryptoAuth;


int32_t CryptoAuth_addUser(String* password,
                           uint8_t authType,
                           void* user,
                           struct CryptoAuth* context);

struct CryptoAuth* CryptoAuth_new(struct MemAllocator* allocator, const uint8_t privateKey[32]);

struct Interface* CryptoAuth_wrapInterface(struct Interface* toWrap,
                                           const uint8_t herPublicKey[32],
                                           String* password,
                                           bool authenticate,
                                           struct CryptoAuth* context);

#endif
