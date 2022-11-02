#ifndef __SM2OPT_H__
#define __SM2OPT_H__
#ifdef __cplusplus
extern "C" {
#endif
int SM2Encode(char* pubKey, char* in, int inlen, char* out);
int SM2DeCode();
#ifdef __cplusplus
}
#endif
#endif
