#include "SM2opt.h"
#include <openssl/evp.h>
#include <openssl/pem.h>

int SM2Encode(char* pubKey, char* in, int inlen, char* out) {
    int outlen = 0;
    EC_POINT *ecpoint;
    EC_KEY *eckey = NULL;
    EC_KEY_set_public_key(eckey, ecpoint);
    EVP_PKEY *pub_key = EVP_PKEY_new();

    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(pub_key, NULL);
    EVP_PKEY_encrypt_init(ctx);
    EVP_PKEY_encrypt(ctx, out, &outlen, in, inlen);
    EVP_PKEY_CTX_free(ctx);
    EVP_PKEY_free(pub_key);
    return outlen;
}