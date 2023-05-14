#include "Rsaopt.h"
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/asn1.h>
#include <string.h>
#include <stdio.h>

int RsaGenerateKey() {
    RSA* rsa =  RSA_new();
    BIGNUM* e = BN_new();
    BN_set_word(e, 12345);
    RSA_generate_key_ex(rsa, 1024, e, NULL);
    FILE* fp = fopen("xxx.pem", "w");
    PEM_write_RSAPublicKey(fp, rsa);
    fclose(fp);
    fp = NULL;
    fp = fopen("xxxx2.pem", "w");
    PEM_write_RSAPrivateKey(fp, rsa, NULL, NULL, 0, NULL, NULL);
    fclose(fp);
    fp = NULL;

    BN_free(e);
    RSA_free(rsa);
    return 0;
}



