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

int SM2EVPEncode(char* pubKey, char* in, int inlen, char* out, int outlen) {
    int ret = 0;
    size_t rlen = outlen;
    BIO* keybio = NULL;
    EC_KEY *eckey = NULL;
    EVP_PKEY *evpkey = NULL;
    EVP_PKEY_CTX* ctx = NULL;

    //i2d_PublicKey();

    keybio = BIO_new_mem_buf(pubKey, -1);
    if (NULL==keybio) {
        printf("创建 BIO %d\n", ret);
        return -1;
    }

    evpkey = PEM_read_bio_PUBKEY(keybio, NULL, NULL, NULL);
    if (NULL==evpkey) {
        printf("获取 EVP KEY 失败 %d\n", ret);
        return -1;
    }

    //(pubKey, &pub_key);
    //EVP_PKEY_set_alias_type(evpkey, EVP_PKEY_SM2);
    ctx = EVP_PKEY_CTX_new(evpkey, NULL);
    if (NULL==ctx) {
        printf("创建 EVP_PKEY 失败 %d\n", ret);
        return -1;
    }

    ret = EVP_PKEY_encrypt_init(ctx);
    if (ret<0) {
        printf("初始化失败 %d\n", ret);
        return -1;
    }
    ret = EVP_PKEY_encrypt(ctx, out, &rlen, in, inlen);
    if (ret<0) {
        printf("加密失败 %d\n", ret);
        return -1;
    }

    EVP_PKEY_CTX_free(ctx);
    EVP_PKEY_free(evpkey);
    BIO_free(keybio);
    return rlen;
}

int SM2EVPDeCode(char* priKey, char* in, int inlen, char* out) {
    size_t outlen = 0;
    BIO* keybio = NULL;
    EC_KEY *eckey = NULL;
    EVP_PKEY *evpkey = NULL;
    EVP_PKEY_CTX* ctx = NULL;

    // 创建bio
    keybio = BIO_new_mem_buf(priKey, -1);
    // 从bio读出,创建ec key
    evpkey = PEM_read_bio_PrivateKey(keybio, NULL, NULL, NULL);
    //(pubKey, &pub_key);
    EVP_PKEY_set_alias_type(evpkey, EVP_PKEY_SM2);
    ctx = EVP_PKEY_CTX_new(evpkey, NULL);

    EVP_PKEY_decrypt_init(ctx);
    EVP_PKEY_decrypt(ctx, out, &outlen, in, inlen);

    EVP_PKEY_CTX_free(ctx);
    EVP_PKEY_free(evpkey);
    BIO_free(keybio);
    return outlen;
}

int SM2GetEC(EC_KEY** eckey, unsigned char *key, KEY_TYPE keyType) {
    int ret = 0;
    if (NULL == eckey || NULL == key)
        return -SM2_ERR_PARAM_CHECK;
    // 密钥转成bio对象
    BIO *keybio = BIO_new_mem_buf(key, -1);
    if (NULL==keybio)
        return -SM2_ERR_NEW_BIO;
    switch (keyType)
    {
    case _KEY_T_PUB:
        *eckey = PEM_read_bio_EC_PUBKEY(keybio, NULL, NULL, NULL);
        break;
    case _KEY_T_PRI:
    default:
        // 默认传的是私钥
        *eckey = PEM_read_bio_ECPrivateKey(keybio, NULL, NULL, NULL);
        break;
    }

    if (NULL==(*eckey))
        ret = -SM2_ERR_GET_ECKEY;
    BIO_free(keybio);

    return ret;
}

int SM2GetKeys(char* pubkey, int publen, char* prikey, int prilen) {
    if (NULL==pubkey || NULL==prikey)
        return -SM2_ERR_PARAM_CHECK;
    EC_KEY *ecKey = EC_KEY_new();
    EC_GROUP* ecGroup = EC_GROUP_new_by_curve_name(NID_X9_62_prime256v1);
    EC_KEY_set_group(ecKey, ecGroup);
    // 获取公私钥
    EC_KEY_generate_key(ecKey);
    // 创建bio读取公钥和私钥
    BIO *pri = BIO_new(BIO_s_mem());
    BIO *pub = BIO_new(BIO_s_mem());
    // 读出公私钥
    PEM_write_bio_ECPrivateKey(pri, ecKey, NULL, NULL, 0, NULL, NULL);
    PEM_write_bio_EC_PUBKEY(pub, ecKey);
    // 对密钥补齐(对齐)
    int pri_len = BIO_pending(pri);
    int pub_len = BIO_pending(pub);

    if (pri_len>prilen || pub_len>publen) {
        printf("==> pri_len %d, pub_len %d\n", pri_len, pub_len);
        return -SM2_ERR_BUFF_NOENOUGT;
    }

    memset(pubkey, 0, publen);
    memset(prikey, 0, prilen);
    // 读出指定长度的密钥(bio转字符串格式)
    BIO_read(pri, prikey, pri_len);
    BIO_read(pub, pubkey, pub_len);
    printf("pri_key %s\n", prikey);
    printf("pub_key %s\n", pubkey);

    // 释放资源
    EC_KEY_free(ecKey);
    BIO_free_all(pub);
    BIO_free_all(pri);

    return 0;
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
