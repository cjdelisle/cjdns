#include <string.h>
#include <stdio.h>
#include "crypto/Crypto.h"
#include "libbenc/benc.h"
#include "memory/BufferAllocator.h"

int Crypto_signVerify_test()
{

    char *message =
        "Governments of the Industrial World, you weary giants of flesh "
        "and steel, I come from Cyberspace, the new home of Mind. On behalf "
        "of the future, I ask you of the past to leave us alone. You are not "
        "welcome among us. You have no sovereignty where we gather.";

    benc_bstr_t bMessage = {strlen(message), message};

    char buffer[512];
    struct MemAllocator* allocator = BufferAllocator_new(buffer, 512);
/*
    char* serializedKP = 
        "\x20\x63\xFB\x68\xDF\xDC\xF6\x47\x65\x97\x0B\x19\x38\xEF\x8C\x97\xDB\x04\x47\x55"
        "\xCA\x58\xF1\xBA\xCE\x2B\xEF\x7D\xD3\xAB\xEA\x82\x03\x21\x01\x1B\x6C\xE7\xE3\xA0"
        "\x4D\x5B\x2C\x80\xD2\xAB\xC3\x12\xD9\xD3\x8B\x2B\xDA\x9F\xEE\x20\x77\x0B\xAB\xE0"
        "\x97\x42\xF5\x4A\xD5\x2A\x4B\x13\x73\x65\x63\x70\x32\x35\x36\x72\x31\x2F\x6E\x69"
        "\x73\x74\x70\x32\x35\x36\x00\x00";*/
    /*char* kp2 = "\x20\xB2\xB3\x93\x61\x88\x48\x2F\x22\x16\x81\x93\xBE\x0B\xD1\x05\x2C\x9C\x7E\x7A\x75\x33\x00\x93\x08\xB5\xFD\x87\x25\xCD\x36\xB1\x62\x21\x00\x4E\x9C\x59\x08\xB8\xD8\xF3\xA1\x86\x9A\x98\xC1\xA7\xC7\x60\x2B\xF3\x3C\x20\x55\x4B\xBA\xCF\xC8\xFC\x09\xBA\xB1\x74\x20\x5E\x70\x13\x73\x65\x63\x70\x32\x35\x36\x72\x31\x2F\x6E\x69\x73\x74\x70\x32\x35\x36\x00\x00";*/

    KeyPair* pair = Crypto_newKeyPair("p256", allocator);

    if (pair == NULL) {
        printf("No key!");
        return -1;
    }

    benc_bstr_t *sig = Crypto_sign(bMessage, pair, allocator);

    if (sig == NULL) {
        printf("failed to sign");
        return -1;
    }

    printf("signature length is %d\n", (int) sig->len);

    if (!Crypto_isSignatureValid(bMessage,
                                 *sig,
                                 *pair->publicKey.as.bstr,
                                 pair->params))
    {
        printf("verification failed.");
        return -1;
    }


    char forgery[300];
    strcpy(forgery, message);
    forgery[10] = 'e';
    benc_bstr_t bForgery = {strlen(forgery), forgery};

    if (Crypto_isSignatureValid(bForgery,
                                *sig,
                                *pair->publicKey.as.bstr,
                                pair->params)) {
        printf("verification passed with bad input!");
        return -1;
    }

    return 0;
}

int main()
{
    Crypto_init();
    return Crypto_signVerify_test();
}
