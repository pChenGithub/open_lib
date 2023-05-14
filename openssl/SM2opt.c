#include "SM2opt.h"
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/ec.h>
#include <openssl/ossl_typ.h>
#include <openssl/bn.h>
#include <openssl/crypto.h>
#include <openssl/opensslconf.h>
#include <openssl/obj_mac.h>
#include <openssl/err.h>

int SM2Encode(char* pubKey, char* in, int inlen, char* out) {
    size_t outlen = 0;
    BIO* keybio = NULL;
    EC_KEY *eckey = NULL;
    EVP_PKEY *evpkey = NULL;
    EVP_PKEY_CTX* ctx = NULL;

    //i2d_PublicKey();

    keybio = BIO_new_mem_buf(pubKey, -1);
    evpkey = PEM_read_bio_PUBKEY(keybio, NULL, NULL, NULL);
    //(pubKey, &pub_key);
    //EVP_PKEY_set_alias_type(evpkey, EVP_PKEY_SM2);
    ctx = EVP_PKEY_CTX_new(evpkey, NULL);

    EVP_PKEY_encrypt_init(ctx);
    EVP_PKEY_encrypt(ctx, out, &outlen, in, inlen);

    EVP_PKEY_CTX_free(ctx);
    EVP_PKEY_free(evpkey);
    BIO_free(keybio);
    return outlen;
}

int SM2DeCode(char* priKey, char* in, int inlen, char* out) {
    size_t outlen = 0;
    BIO* keybio = NULL;
    EC_KEY *eckey = NULL;
    EVP_PKEY *evpkey = NULL;
    EVP_PKEY_CTX* ctx = NULL;

    keybio = BIO_new_mem_buf(priKey, -1);
    evpkey = PEM_read_bio_PrivateKey(keybio, NULL, NULL, NULL);
    //(pubKey, &pub_key);
    //EVP_PKEY_set_alias_type(evpkey, EVP_PKEY_SM2);
    ctx = EVP_PKEY_CTX_new(evpkey, NULL);

    EVP_PKEY_decrypt_init(ctx);
    EVP_PKEY_decrypt(ctx, out, &outlen, in, inlen);

    EVP_PKEY_CTX_free(ctx);
    EVP_PKEY_free(evpkey);
    BIO_free(keybio);
    return outlen;
}

int SM2EvpKeyFromCh(unsigned char* keystr) {
    EVP_PKEY *evpkey = NULL;
    i2d_PublicKey(evpkey, &keystr);
}

int SM2EvpKeyToCh(char* keystr, int keyStrLen) {

}

int SM2EvpKeyFromFile(const char* keyFile, EVP_PKEY** evpkey, KEY_TYPE type) {
    BIO* bio = BIO_new_file(keyFile, "r");
    EVP_PKEY* key = EVP_PKEY_new();
    switch (type)
    {
    case _KEY_T_PUB:
        key = PEM_read_bio_PUBKEY(bio, NULL, NULL, NULL);
        break;
    case _KEY_T_PRI:
        key = PEM_read_bio_PrivateKey(bio, NULL, NULL, NULL);
        break;
    default:
        break;
    }

    //EVP_PKEY_set_alias_type(key, EVP_PKEY_SM2);
    BIO_free(bio);
    return 0;
}
