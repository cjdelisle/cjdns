#ifndef CRYPTO_H
#define CRYPTO_H

#include <stdint.h>
#include <gcrypt.h>
#include "crypto/seccure/curves.h"
#include "libbenc/benc.h"

/**
 * Representation of a KeyPair.
 */
struct KeyPair_s {
    /** The private key. */
    const gcry_mpi_t privateKey;

    /** The public key. */
    bobj_t publicKey;

    /** Constants about the crypto algorithm being used. */
    const struct curve_params * const params;
};
typedef struct KeyPair_s KeyPair;

/**
 * Initialize the crypto engine.
 * This must be called before doing anything else.
 * @return 0 if initialization went well, -1 if it failed.
 */
int Crypto_init();

/**
 * Generate a new KeyPair.
 *
 * @param curveName one of "p112", "p128", "p160",
 *                  "p192", "p224", "p256", "p384",
 *                  or "p521", the name of the curve to use.
 * @return a new KeyPair suitable for signing.
 */
KeyPair* Crypto_newKeyPair(const char *curveName);

/**
 * Free a KeyPair.
 *
 * @param toFree this key pair will be destroyed.
 */
void Crypto_freeKeyPair(KeyPair *toFree);

/**
 * Serialize a keyPair.
 *
 * @param toSerialize a KeyPair to convert to a string.
 * @param pointToOutput this pointer will be pointed to a new buffer.
 *                      NOTE: This buffer is allocated in secure memory
 *                            so gcry_free() must be used instead of free()
 * @return a positive number which is the number of bytes in the buffer
 *         if everything went well. -3 if an input was null,
 *         -2 if something failed in gcrypt, -1 if the key
 *         size was over 2032 bits.
 */
int Crypto_serializeKeyPair(const KeyPair* toSerialize,
                              char** pointToOutput);

/**
 * Parse a serialized KeyPair.
 *
 * @param serialized the data to parse from.
 * @param bufferLength length will be detected but this function will fail
 *                     if detected length exceeds this number.
 * @return a new Keypair made from the serialized data.
 */
KeyPair* Crypto_parseKeyPair(const char* serialized, size_t bufferLength);

/**
 * Sign a message.
 *
 * @param message the text to sign.
 * @param keyPair the KeyPair to sign with.
 * @return a string representing a signature
 *         or null if the memory cannot be allocated.
 */
benc_bstr_t* Crypto_sign(const benc_bstr_t message,
                           const KeyPair *keyPair);

/**
 * Verify a signature.
 *
 * @param message the content to check a signature on.
 * @param signature the signature on the message.
 * @param publicKey the signer's public key.
 * @param params constants about the crypto algorithm.
 * @return 1 if and only if the signature is valid and matches
 *         the text, otherwise, 0.
 *
 * NOTE: publicKey is expected to be params->pk_len_bin long
 *       and signature is expected to be params->sig_len_bin long.
 *       If these do not match then 0 will return.
 */
int Crypto_isSignatureValid(const benc_bstr_t message,
                              const benc_bstr_t signature,
                              const benc_bstr_t publicKey,
                              const struct curve_params *params);

#endif
