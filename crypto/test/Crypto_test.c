#include <stdio.h>
#include "crypto/Crypto.h"
#include "libbenc/benc.h"

int Crypto_signVerify_test()
{

    char *message =
        "Governments of the Industrial World, you weary giants of flesh "
        "and steel, I come from Cyberspace, the new home of Mind. On behalf "
        "of the future, I ask you of the past to leave us alone. You are not "
        "welcome among us. You have no sovereignty where we gather.";

    benc_bstr_t bMessage = {strlen(message), message};

    KeyPair *pair = Crypto_newKeyPair("p160");
    if (pair == NULL) {
        printf("No key!");
        return -1;
    }

    benc_bstr_t *sig = Crypto_sign(bMessage, pair);

    if (sig == NULL) {
        printf("failed to sign");
        return -1;
    }

    if (!Crypto_isSignatureValid(bMessage,
                                   *sig,
                                   *pair->publicKey.as.bstr,
                                   pair->params)) {
        printf("verification falied.");
        return -1;
    }

    char *forgery = strdup(message);
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
