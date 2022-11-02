#ifndef __SM2OPT_H__
#define __SM2OPT_H__
#ifdef __cplusplus
extern "C" {
#endif
int SM2Encode(char* pubKey, char* in, int inlen, char* out);
int SM2DeCode(char* priKey, char* in, int inlen, char* out);
int SM2Sign(char* priKey);
int SM2Verify(char* pubKey);
#ifdef __cplusplus
}
#endif
#endif
