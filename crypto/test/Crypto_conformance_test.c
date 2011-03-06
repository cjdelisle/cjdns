#include <stdio.h>
#include "crypto/Crypto.h"
#include "libbenc/benc.h"
#include "memory/BufferAllocator.h"

int Crypto_conformance_test()
{
    /*-------------------Conformance data-------------------*/

    char* message =
        "Governments of the Industrial World, you weary giants of flesh "
        "and steel, I come from Cyberspace, the new home of Mind. On behalf "
        "of the future, I ask you of the past to leave us alone. You are not "
        "welcome among us. You have no sovereignty where we gather.";

    char* publicKey =
        "\x59\xBA\xCF\x4E\x8F\x76\x12\x81\x24\xC1\x32\x4F\xF8\x69\xF4\xBD"
        "\xB9\x37\x04\x97\xD4\x0D\xF0\x64\xDF\xE2\xA6\x4E\x5A\xB7\xB6\xBC"
        "\x83\x8F\x47\x96\x27\x50\x12\x6A\x63\xC7\x1B\x1F\x16\x1A\xCA\x76"
        "\xEA\xE1\x02\x07\xAC\x9C\x70\xD2\x8B\x7C\x6A\xEE\xD4\x83\x4A\x67";

    char* privateKey =
        "\x11\x39\xBA\x48\x73\x0A\x03\xBB\x84\xD3\x19\x9F\x8F\x11\x9F\xE7"
        "\xE2\x73\x5A\x99\xE8\xA0\xCD\xF0\x57\x41\x78\xE5\xDC\xFF\x6F\x5B";

    char* signature =
        "\x7F\x57\xA0\x9D\xAB\x75\xE0\x3F\x48\xE9\x5A\x6D\x0F\x2F\x97\xD1"
        "\x07\x19\xED\xAE\x4A\xA7\x97\x2B\x24\x81\x29\xBF\x9C\xD5\xF6\x5E"
        "\x88\xF4\x48\x85\x46\xDA\x7B\xF5\x3B\x7F\xB3\x37\x7A\x0C\x4F\x96"
        "\x9E\x54\x6B\x4E\x65\xF2\x5C\x9D\x29\x9C\x2F\x09\x15\xC1\x7A\x5D";

    /*------------------------Tests------------------------*/

    size_t privateKeyLength = 32;
    size_t publicKeyLength = 65;

    /* Craft the cjdns crypto format serialized keypair from the public and private keys. */
    unsigned char keyPair[121];
    keyPair[0] = (unsigned char) privateKeyLength;
    memcpy(keyPair + 1, privateKey, privateKeyLength);
    keyPair[privateKeyLength + 1] = (unsigned char) publicKeyLength;
    memcpy(keyPair + 1 + privateKeyLength + 1, publicKey, publicKeyLength);
    /* An internally used hint to tell the program what private key it has. */
    const char* curve = "secp256r1/nistp256\x00";
    /* length of the curve type hint including null pad. */
    size_t curveNameLength = 19;
    keyPair[privateKeyLength + 1 + publicKeyLength + 1] = (unsigned char) curveNameLength;
    memcpy(keyPair + 1 + privateKeyLength + 1 + publicKeyLength + 1, curve, strlen(curve) + 1);
    keyPair[120] = '\0';

    benc_bstr_t bMessage = {strlen(message), message};

    KeyPair* pair = Crypto_parseKeyPair((char*)keyPair, 121);

    if (pair == NULL) {
        printf("No key!\n");
        return -1;
    }

    char buffer[512];
    struct MemAllocator* allocator = BufferAllocator_new(buffer, 512);
    benc_bstr_t* sig = Crypto_sign(bMessage, pair, allocator);

    if (sig == NULL) {
        printf("failed to sign\n");
        return -1;
    }

    if (sig->len != 64 || memcmp(signature, sig->bytes, 64) != 0) {
        printf("Signature is not the same!\n");
        return -1;
    }

    if (!Crypto_isSignatureValid(bMessage,
                                 (benc_bstr_t){ .len = 64, .bytes = signature },
                                 (benc_bstr_t){ .len = 65, .bytes = publicKey },
                                 pair->params))
    {
        printf("verification failed.");
        return -1;
    }

    printf("\n\n\n\nThis is the sig: ");
    size_t i;
    for (i = 0; i < (size_t) sig->len; i++) {
        printf("\\x%.2X", (unsigned int) sig->bytes[i] & 0xFF);
    }
    printf("\n\n\n\n");

    return 0;
}

int main()
{
    Crypto_init();
    return Crypto_conformance_test();
}
