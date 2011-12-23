#ifndef EXPORTS_H
#define EXPORTS_H

#include <stdint.h>

#include "crypto/CryptoAuth_struct.h"
#include "wire/Message.h"

uint32_t Exports_obfuscateNonce(uint32_t* nonce_be, void* vWrapper);

void Exports_encryptRndNonce(uint8_t nonce[24], struct Message* msg, uint8_t secret[32]);

int Exports_decryptRndNonce(uint8_t nonce[24], struct Message* msg, uint8_t secret[32]);

uint8_t Exports_encryptHandshake(struct Message* message, struct Wrapper* wrapper);

void Exports_receiveMessage(struct Message* received, struct Interface* interface);

#endif
