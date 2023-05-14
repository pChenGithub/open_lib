#ifndef __HASHOPT_H__
#define __HASHOPT_H__
#ifdef __cplusplus
extern "C" {
#endif
int Md5FileEncode(const char* file, char* md, int mdlen);
int Md5StrEncodeToHex(const char* str, char* mdhex, int mdhexlen);
int Md5StrEncodeToStr(const char* str, char* mdstr, int mdstrlen);
#ifdef __cplusplus
}
#endif
#endif

