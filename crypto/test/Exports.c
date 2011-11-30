#define CryptoAuth_addUser Export_CryptoAuth_addUser
#define CryptoAuth_getUser Export_CryptoAuth_getUser
#define CryptoAuth_new Export_CryptoAuth_new
#define CryptoAuth_wrapInterface Export_CryptoAuth_wrapInterface

#include "crypto/CryptoAuth.c"

/**
 * This is for allowing the unit testing of functions internal to CryptoAuth
 * while still allowing them to be inlined in larger functions.
 */

uint32_t Exports_obfuscateNonce(uint32_t nonce_be, void* vWrapper)
{
    return obfuscateNonce(nonce_be, (struct Wrapper*) vWrapper);
}

void Exports_encryptRndNonce(uint8_t nonce[24], struct Message* msg, uint8_t secret[32])
{
    encryptRndNonce(nonce, msg, secret);
}

int Exports_decryptRndNonce(uint8_t nonce[24], struct Message* msg, uint8_t secret[32])
{
    return decryptRndNonce(nonce, msg, secret);
}
