#include <stdint.h>

#include "wire/Message.h"

uint32_t Exports_obfuscateNonce(uint32_t nonce_be, void* vWrapper);

void Exports_encryptRndNonce(uint8_t nonce[24], struct Message* msg, uint8_t secret[32]);

int Exports_decryptRndNonce(uint8_t nonce[24], struct Message* msg, uint8_t secret[32]);
