#ifndef CRYPTO_H
#define CRYPTO_H

#include <stdint.h>
#include <gcrypt.h>
#include "crypto/seccure/curves.h"
#include "libbenc/benc.h"

/**
 * Representation of a KeyPair.
 *
struct KeyPair_s {
    ** The private key. *
    const gcry_mpi_t privateKey;

    ** The public key. *
    bobj_t publicKey;

    ** Constants about the crypto algorithm being used. *
    const struct curve_params * const params;
};
typedef struct KeyPair_s KeyPair;*/

/**
 * Initialize the random number generator.
 * This must be called before doing anything else.
 */
void Crypto_init();

/**
 * Generate a new KeyPair.
 *
 * @param curveName one of "p112", "p128", "p160",
 *                  "p192", "p224", "p256", "p384",
 *                  or "p521", the name of the curve to use.
 * @param allocator a means of getting memory. The private key will be allocated
 *                  in gcrypt provided secure memory no matter what type this
 *                  allocator is. However all parts of the key including the private
 *                  key will be freed when this allocator is freed.
 * @return a new KeyPair suitable for signing.
 */
/*
KeyPair* Crypto_newKeyPair(const char *curveName,
                           const struct MemAllocator* allocator);
*/
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
/*
int Crypto_serializeKeyPair(const KeyPair* toSerialize,
                            char** pointToOutput);
*/
/**
 * Parse a serialized KeyPair.
 *
 * @param serialized the data to parse from.
 * @param bufferLength length will be detected but this function will fail
 *                     if detected length exceeds this number.
 * @return a new Keypair made from the serialized data.
 */
//KeyPair* Crypto_parseKeyPair(const char* serialized, size_t bufferLength);

/**
 * Sign a message.
 *
 * @param message the text to sign.
 * @param keyPair the KeyPair to sign with.
 * @param allocator a means to get memory to put the signature in.
 * @return a string representing a signature
 *         or null if the memory cannot be allocated.
 */
/*
benc_bstr_t* Crypto_sign(const benc_bstr_t message,
                         const KeyPair* const keyPair,
                         const struct MemAllocator* allocator);
*/
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
/*
uint32_t Crypto_isSignatureValid(const benc_bstr_t message,
                                 const benc_bstr_t signature,
                                 const benc_bstr_t publicKey,
                                 const struct curve_params *params);
*/
/**
 * Generate a SHA-1 hash of a message.
 * This function will produce a benc_bstr_t object of length 20
 * allocated in the provided allocator.
 *
 * @param hashThis a bencoded string to be hashed.
 * @param allocator a means of getting memory for storing the output.
 * @return a bencoded string containing the hash output in binary.
 */
benc_bstr_t* Crypto_sha1sum(const benc_bstr_t* hashThis,
                            const struct MemAllocator* allocator);

/**
 * Generate a SHA-256 hash of a message.
 * This function will produce a String struct of length 32
 * allocated in the provided allocator.
 *
 * @param hashThis a bencoded string to be hashed.
 * @param allocator a means of getting memory for storing the output.
 * @return a bencoded string containing the hash output in binary.
 */
benc_bstr_t* Crypto_sha256sum(const benc_bstr_t* hashThis,
                            const struct MemAllocator* allocator);

/**
 * Overwrite a string with random bytes.
 * The size of the string will dictate the number of random bytes written.
 *
 * @param toRandomize the String to scramble.
 */
void Crypto_randomize(String* toRandomize);

#endif
