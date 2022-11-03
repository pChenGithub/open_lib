#ifndef __SM2OPT_H__
#define __SM2OPT_H__
#ifdef __cplusplus
extern "C" {
#endif
#include <openssl/evp.h>
typedef enum {
    _KEY_T_PUB = 0,
    _KEY_T_PRI,
} KEY_TYPE;
int SM2Encode(char *pubKey, char *in, int inlen, char *out);
int SM2DeCode(char* priKey, char* in, int inlen, char* out);
int SM2Sign(char* priKey);
int SM2Verify(char* pubKey);
int SM2EvpKeyFromCh(unsigned char* keystr);
int SM2EvpKeyToCh(char* keystr, int keyStrLen);
int SM2EvpKeyFromFile(const char* keyFile, EVP_PKEY** evpkey, KEY_TYPE type);
#ifdef __cplusplus
}
#endif
#endif
