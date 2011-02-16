#include <stdio.h>
#include "crypto/Crypto.c"
#include "libbenc/benc.h"

static const char* control =
        "#aAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAa!bBBBBBBBBBBBBBBBBBBBBBBBBB"
        "BBBBBBb$cCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCc";

static char* private = "aAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAa";
static char* public = "bBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBb";
static char* curveName = "cCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCc";

int serialize0_test()
{
    size_t length = strlen(private) + strlen(public) + strlen(curveName) + 4;
    char outBuffer[length];
    memset(&outBuffer, 'F', length);
    serialize0(outBuffer,
               private, strlen(private),
               public, strlen(public),
               curveName, strlen(curveName));
    outBuffer[length - 1] = '\0';

    if (strcmp(control, outBuffer) != 0) {
        printf("\n\nExpected:\n%s", control);
        printf("\n\nGot:\n%s\n\n\n", outBuffer);
        return -1;
    }
    return 0;
}

int parse0_test()
{
    char* a = NULL;
    size_t aSize = 0;
    char* b = NULL;
    size_t bSize = 0;
    char* c = NULL;
    size_t cSize = 0;

    int out = parse0(control, strlen(control) + 2, &a, &aSize, &b, &bSize, &c, &cSize);
    if (out != 0) {
        printf("Returned error: %d" + out);
        return -1;
    }

    if (aSize != strlen(private)) {
        printf("private is the wrong size, expecting %d got %d",
               (int) strlen(private),
               (int) aSize);
        return -2;
    }
    if (bSize != strlen(public)) {
        printf("public is the wrong size, expecting %d got %d",
               (int) strlen(public),
               (int) bSize);
        return -3;
    }
    if (cSize != strlen(curveName)) {
        printf("curveName is the wrong size, expecting %d got %d",
               (int) strlen(curveName),
               (int) cSize);
        return -4;
    }
    if (memcmp(a, private, aSize) != 0) {
        printf("private key is different");
        return -5;
    }
    if (memcmp(b, public, bSize) != 0) {
        printf("public key is different");
        return -5;
    }
    if (memcmp(c, curveName, cSize) != 0) {
        printf("curve name is different, expected %s, got %s", curveName, c);
        return -5;
    }

    return 0;
}

int Crypto_keySerializeParse_test()
{
    KeyPair* pair = Crypto_newKeyPair("p160");
    if (pair == NULL) {
        printf("No key!\n");
        return -1;
    }
    char* ser = NULL;
    int length = Crypto_serializeKeyPair(pair, &ser);

    if (ser == NULL) {
        printf("No key was generated.\n");
        return -1;
    }

    KeyPair* pair2 = Crypto_parseKeyPair(ser, length);

    if (pair2 == NULL) {
        printf("Failed to parse\n");
        return -1;
    }

    char* ser2 = NULL;
    int length2 = Crypto_serializeKeyPair(pair2, &ser2);

    if (length2 != length) {
        printf("keys are different sizes\n");
        return -1;
    }

    if (memcmp(ser2, ser, length) != 0) {
        printf("keys differ\n");
        return -1;
    }

    printf("\n\n\n\nThis is the key: ");
    size_t i;
    unsigned int chr;
    for (i = 0; i < (size_t) length; i++) {
        chr = (unsigned int) ser[i] & 0xFF;
        /* Nonprinting chars, and \ and " are hex'd */
        if (chr < 0x7f && chr > 0x20 && chr != 0x5c && chr != 0x22) {
            printf("%c", ser[i]);
        } else {
            printf("\\x%.2X", (unsigned int) ser[i] & 0xFF);
        }
    }
    printf("\n\n\n\n");

    return 0;
}

int main()
{
    Crypto_init();
    return
        serialize0_test()
      | parse0_test()
      | Crypto_keySerializeParse_test();
}
