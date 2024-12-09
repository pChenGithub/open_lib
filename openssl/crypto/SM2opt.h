#ifndef __SM2OPT_H__
#define __SM2OPT_H__
#ifdef __cplusplus
extern "C" {
#endif
#include <openssl/evp.h>

#define SM2_ERR_PARAM_CHECK     1   // 参数校验失败
#define SM2_ERR_NEW_BIO         2   // 创建bio失败
#define SM2_ERR_GET_ECKEY       3   // 创建椭圆曲线失败
#define SM2_ERR_BUFF_NOENOUGT   4   // 传入空间不足

typedef enum {
    _KEY_T_PUB = 0,
    _KEY_T_PRI,
} KEY_TYPE;
// sm2加密
int SM2EVPEncode(char *pubKey, char *in, int inlen, char *out, int outlen);
// sm2解密
int SM2EVPDeCode(char* priKey, char* in, int inlen, char* out);
// sm2加签
int SM2Sign(char* priKey);
// sm2验签
int SM2Verify(char* pubKey);
// 获取椭圆曲线EC
int SM2GetEC(EC_KEY** eckey, unsigned char *key, KEY_TYPE keyType);
// 获取密钥
int SM2GetKeys(char* pubkey, int publen, char* prikey, int prilen);
//
int SM2EvpKeyFromCh(unsigned char* keystr);
//
int SM2EvpKeyToCh(char* keystr, int keyStrLen);
//
int SM2EvpKeyFromFile(const char* keyFile, EVP_PKEY** evpkey, KEY_TYPE type);
#ifdef __cplusplus
}
#endif
#endif
