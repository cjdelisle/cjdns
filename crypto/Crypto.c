/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307 USA
 */

#include "Crypto.h"
#include "seccure/protocol.h"
#include "libbenc/benc.h"
#include "libbenc/bencode.h"


/**
 * Cryptographic functions.
 * Private keys are always stored in secure memory.
 * Pointers to private keys are also stored in secure memory.
 * A pointer to a private key sits on the stack temporarily in
 * Crypto_keyGen() but it is zeroed before the function ends.
 *
 * Please send patches and bug reports to calebdelisle(a)lavabitD0Tcom
 */


/** @see Crypto.h */
int Crypto_init()
{
    gcry_error_t err = gcry_control(GCRYCTL_INIT_SECMEM, 1);
    if (gcry_err_code(err)) {
        fprintf(stderr,
                "Cannot enable gcrypt's secure memory management\n");
        return -1;
    }

    err = gcry_control(GCRYCTL_USE_SECURE_RNDPOOL, 1);
    if (gcry_err_code(err)) {
        fprintf(stderr,
                "Cannot enable gcrypt's random number generator\n");
        return -1;
    }

    return 0;
}

/** @see Crypto.h */
KeyPair* Crypto_newKeyPair(const char* curveName)
{
    const struct curve_params* params =
        curve_by_name(curveName, DF_BIN);
    if (params == NULL) {
        return NULL;
    }

    gcry_mpi_t privateKey = get_random_exponent(params);
    struct affine_point publicPoint =
        pointmul(&params->dp.base, privateKey, &params->dp);

    bobj_t* publicKey = bobj_bstr_new(params->pk_len_bin, NULL);
    if (publicKey == NULL) {
        point_release(&publicPoint);
        free((void*)params);
        return NULL;
    }
    compress_to_string(publicKey->as.bstr->bytes, DF_BIN, &publicPoint, params);
    point_release(&publicPoint);

    KeyPair temp = {privateKey, *publicKey, params};
    KeyPair* out = gcry_calloc_secure(sizeof(KeyPair), 1);
    if (out == NULL) {
        free((void*) params);
        return NULL;
    }
    memcpy(out, &temp, sizeof(KeyPair));

    /*
     * These are copied onto the heap so they should be
     * cleared here so as not to leak pointers to the locations
     * of the secure memory holding the actual key.
     */
    memset(&temp, 0, sizeof(KeyPair));
    memset(&privateKey, 0, sizeof(gcry_mpi_t));

    return out;
}

/** @see Crypto.h */
void Crypto_freeKeyPair(KeyPair *keyPair)
{
    free(keyPair->publicKey.as.bstr->bytes);
    gcry_mpi_release(keyPair->privateKey);
    curve_release((struct curve_params*) keyPair->params);
    gcry_free(keyPair);
}

/**
 * Parse a buffer and break into 3.
 * If any inputs are NULL, this will blow up spectacularly.
 *
 * @param outBuffer the place to put the output. This must be
 *                  privateSize + publicSize + curveNameSize + 4
 * @param private the private key.
 * @param privateSize the size of the private key.
 * @param public the public key.
 * @param publicSize the size of the public key.
 * @param curveName the name of the curve.
 * @param curveNameSize the size of the curve name.
 */
static inline void serialize0(char* outBuffer,
                              const char* private, size_t privateSize,
                              const char* public, size_t publicSize,
                              const char* curveName, size_t curveNameSize)
{
    outBuffer[0] = (unsigned char) privateSize & 0xFF;
    memcpy(&outBuffer[1], private, privateSize);

    outBuffer[privateSize + 1] = (unsigned char) publicSize & 0xFF;
    memcpy(&outBuffer[privateSize + 2], public, publicSize);

    outBuffer[privateSize + publicSize + 2] =
        (unsigned char) curveNameSize & 0xFF;
    memcpy(&outBuffer[privateSize + publicSize + 3],
           curveName,
           curveNameSize);
    outBuffer[privateSize + publicSize + curveNameSize + 3] = 0x00;
}

/** @see Crypto.h */
int Crypto_serializeKeyPair(const KeyPair* toSerialize,
                              char** pointToOutput)
{
    if (toSerialize == NULL || pointToOutput == NULL) {
        return -3;
    }

    /* This will be in secure memory. */
    unsigned char* privateBuf = NULL;

    size_t privateSize = 0;
    gcry_error_t err = gcry_mpi_aprint(GCRYMPI_FMT_USG,
                                       &privateBuf,
                                       &privateSize,
                                       toSerialize->privateKey);

    if (gcry_err_code(err) || privateSize == 0 || privateBuf == NULL) {
        return -2;
    }

    size_t publicSize = toSerialize->publicKey.as.bstr->len;

    /* "+ 1" to keep the null at the end of the string. */
    size_t curveNameSize = strlen(toSerialize->params->name) + 1;

    if (publicSize > 0xFE || curveNameSize > 0xFE || privateSize > 0xFE)
    {
        /* limitation of the serialization technique. 0xFF is reserved. */
        return -1;
    }

    size_t outLength = publicSize + curveNameSize + privateSize + 4;

    char* outBuf = gcry_calloc_secure(outLength, 1);

    if (outBuf) {
        serialize0(outBuf,
                   (char*) privateBuf, privateSize,
                   toSerialize->publicKey.as.bstr->bytes, publicSize,
                   toSerialize->params->name, curveNameSize);
    }

    gcry_free(privateBuf);
    privateBuf = NULL;

    *pointToOutput = outBuf;

    return outLength;
}

/**
 * Parse a buffer and break into 3.
 * If any inputs are NULL, this will blow up spectacularly.
 *
 * @param inBuffer the buffer to read from.
 * @param inBufferLength this function will not read more
 *                       than this much of the buffer.
 * All of the following are assigned
 * @param private a pointer to the private key,
 *                this is allocated from secure memory.
 * @param privateSize the size of the private key.
 * @param public a pointer to the public key.
 * @param publicSize the size of the public key.
 * @param curveName a pointer to the name of the curve.
 * @param curveNameSize the size of the curve name.
 * @return -1 if the buffer does not contain a null where expected.
 *         -2 if memory cannot be allocated.
 *         0 if all goes well.
 */
static inline int parse0(const char* inBuffer, size_t inBufferLength,
                         char** private, size_t* privateSize,
                         char** public, size_t* publicSize,
                         char** curveName, size_t* curveNameSize)
{
    if (inBufferLength == 0) { return -1; }

    size_t privSize = inBuffer[0] & 0xFF;
    if (privSize + 1 >= inBufferLength) { return -1; }
    const char* privPtr = &inBuffer[1];

    size_t pubSize = inBuffer[privSize + 1] & 0xFF;
    if (pubSize + privSize + 2 >= inBufferLength) { return -1; }
    const char* pubPtr = &inBuffer[privSize + 2];

    size_t nameSize = inBuffer[pubSize + privSize + 2] & 0xFF;
    if (privSize + pubSize + nameSize + 3 >= inBufferLength) { return -1; }
    const char* namePtr = &inBuffer[pubSize + privSize + 3];

    if (inBuffer[privSize + pubSize + nameSize + 3] != '\0') {
        return -2;
    }

    char* priv = gcry_calloc_secure(privSize, 1);
    char* pub = calloc(pubSize, 1);
    char* name = calloc(nameSize, 1);

    if (priv == NULL || pub == NULL || name == NULL) {
        if (priv) { gcry_free(priv); }
        if (pub) { free(pub); }
        if (name) { free(name); }
        return -3;
    }

    memcpy(priv, privPtr, privSize);
    memcpy(pub, pubPtr, pubSize);
    memcpy(name, namePtr, nameSize);

    *private = priv;
    *privateSize = privSize;
    *public = pub;
    *publicSize = pubSize;
    *curveName = name;
    *curveNameSize = nameSize;

    return 0;
}

/** @see Crypto.h */
KeyPair* Crypto_parseKeyPair(const char* serialized, size_t bufferLength)
{
    char* private = NULL;
    size_t privateSize = 0;
    char* public = NULL;
    size_t publicSize = 0;
    char* curveName = NULL;
    size_t curveNameSize = 0;

    KeyPair* kp = gcry_calloc_secure(sizeof(KeyPair), 1);

    if (kp == NULL) {
        return NULL;
    }

    int parseError = parse0(serialized, bufferLength,
                            &private, &privateSize,
                            &public, &publicSize,
                            &curveName, &curveNameSize);

    struct curve_params* params = NULL;
    /* Make sure curveName is null terminated. */
    if (!parseError && curveName[curveNameSize - 1] == '\0') {
        params = curve_by_name(curveName, DF_BIN);
    }

    gcry_mpi_t privateKey;
    benc_bstr_t* publicKey = calloc(sizeof(benc_bstr_t), 1);

    gcry_error_t err = 0;
    if (!parseError && publicKey && params) {
        err = gcry_mpi_scan(&privateKey,
                            GCRYMPI_FMT_USG,
                            private, privateSize,
                            NULL);
    }

    if (!parseError
        && params
        && publicKey
        && !gcry_err_code(err))
    {
        KeyPair localKp = {
            .privateKey = privateKey,
            .publicKey = (bobj_t){
                .type = BENC_BSTR,
                .as.bstr = publicKey
            },
            .params = params
        };
        publicKey->bytes = public;
        publicKey->len = publicSize;

        memcpy(kp, &localKp, sizeof(KeyPair));

        /* paranoia. */
        memset(&localKp, 0, sizeof(KeyPair));

        return kp;
    }

    if (params) { curve_release(params); }
    if (private) { gcry_free(private); }
    if (public) { free(public); }
    if (publicKey) { free(publicKey); }
    if (curveName) { free(curveName); }
    if (kp) { gcry_free(kp); }

    return NULL;
}

/**
 * Sign a message, implementation 0.
 *
 * @param message a char buffer containing the message to sign.
 * @param messageLength how much to read from the buffer.
 * @param privateKey sign using this key.
 * @param outputBuffer write the signature to this buffer.
 * @param params constants about the crypto algorithm.
 */
static inline int sign0(const char* message, size_t messageLength,
                        const gcry_mpi_t privateKey,
                        char* outputBuffer, size_t outBufferLength,
                        const struct curve_params* params)
{
    if (message == NULL
        || messageLength == 0
        || outputBuffer == NULL
        || outBufferLength == 0
        || params == NULL)
    {
        return -1;
    }

    /* ECDSA_sign() expects exactly 64 bytes so use SHA512. */
    gcry_md_hd_t hashFunction;
    gcry_error_t err = gcry_md_open(&hashFunction, GCRY_MD_SHA512, 0);
    if (gcry_err_code(err)) {
        /* Failed to setup SHA512 */
    } else {
        gcry_md_write(hashFunction, message, messageLength);
        char* hashReturn = (char*) gcry_md_read(hashFunction, 0);
        if (hashReturn == NULL) {
            /* Failed to do hash. */
        } else {
            gcry_mpi_t sig = ECDSA_sign(hashReturn, privateKey, params);
            serialize_mpi(outputBuffer, outBufferLength, DF_BIN, sig);
            gcry_mpi_release(sig);
            return 0;
        }
        gcry_md_close(hashFunction);
    }
    return -1;
}

/** @see Crypto.h */
benc_bstr_t* Crypto_sign(const benc_bstr_t message,
                           const KeyPair* const keyPair)
{
    if (keyPair == NULL) {
        return NULL;
    }
    benc_bstr_t* sig = benc_bstr_new(keyPair->params->sig_len_bin, NULL);
    if (sig == NULL) {
        return NULL;
    }
    if (sign0(message.bytes,
              message.len,
              keyPair->privateKey,
              sig->bytes,
              sig->len,
              keyPair->params) < 0)
    {
        benc_bstr_free(sig);
        return NULL;
    }
    return sig;
}

/**
 * Verify a signature.
 *
 * @param message the content to check a signature on.
 * @param messageLength the amount of message to read.
 * @param signature the signature on the message.
 * @param publicKey the signer's public key.
 * @param params constants about the crypto algorithm.
 * @return 1 if and only if the signature is valid and matches the text,
 *         otherwise, 0.
 *
 * NOTE: publicKey is expected to be params->pk_len_bin long
 *       and signature is expected to be params->sig_len_bin long.
 *       This function _will_ read that much of the buffers.
 */
static inline int isSignatureValid0(const char* message, size_t messageLength,
                                    const char* signature,
                                    const char* publicKey,
                                    const struct curve_params* params)
{
    if (message == NULL
        || messageLength == 0
        || publicKey == NULL
        || signature == NULL
        || params == NULL)
    {
        return false;
    }

    struct affine_point Q;
    /*
     * Do a bunch of things to the public key
     * which I have not reviewed well enough.
     */
    if (decompress_from_string(&Q, publicKey, DF_BIN, params) == 0) {
        /* Invalid key. */
        return false;
    }

    /* Read the signature into a platform independent var blah blah. */
    gcry_mpi_t mpi_signature;
    gcry_mpi_scan(&mpi_signature,
                  GCRYMPI_FMT_USG,
                  signature, params->sig_len_bin,
                  NULL);
    gcry_mpi_set_flag(mpi_signature, GCRYMPI_FLAG_SECURE);

    /* ECDSA_verify() expects exactly 64 bytes so use SHA512. */
    gcry_md_hd_t hashFunction;
    gcry_error_t err = gcry_md_open(&hashFunction, GCRY_MD_SHA512, 0);
    if (gcry_err_code(err)) {
        /* Failed to setup SHA512 */
    } else {
        gcry_md_write(hashFunction, message, messageLength);
        char* hashReturn = (char*) gcry_md_read(hashFunction, 0);
        if (hashReturn == NULL) {
            /* Failed to do hash. */
        } else {
            return ECDSA_verify(hashReturn, &Q, mpi_signature, params);
        }
        gcry_md_close(hashFunction);
    }
    point_release(&Q);

    /* This free's hashReturn. */
    gcry_mpi_release(mpi_signature);

    return false;
}

/** @see Crypto.h */
int Crypto_isSignatureValid(const benc_bstr_t message,
                              const benc_bstr_t signature,
                              const benc_bstr_t publicKey,
                              const struct curve_params *params)
{
    if (publicKey.len != (size_t) params->pk_len_bin
        || signature.len != (size_t) params->sig_len_bin) {
        return false;
    }

    return isSignatureValid0(message.bytes,
                             message.len,
                             signature.bytes,
                             publicKey.bytes,
                             params);
}
