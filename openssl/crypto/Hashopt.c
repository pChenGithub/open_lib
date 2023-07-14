#include "Hashopt.h"
#include <openssl/md5.h>
#include <string.h>
#include <stdio.h>

int Md5FileEncode(const char* file, char* md, int mdlen) {
    

}

int Md5StrEncodeToHex(const char* str, char* mdhex, int mdhexlen) {
    int ret = 0;
    MD5_CTX c;
    if (mdhexlen<MD5_DIGEST_LENGTH)
        return -1;
    // 初始化
    MD5_Init(&c);
    // 加入数据
    ret = MD5_Update(&c, str, strlen(str));
    // 计算结果
    ret = MD5_Final(mdhex, &c);
    return 0;
}

int Md5StrEncodeToStr(const char* str, char* mdstr, int mdstrlen) {
    int ret = 0;
    char mdhex[MD5_DIGEST_LENGTH] = {0};
    if (mdstrlen<(2*MD5_DIGEST_LENGTH+1))
        return -1;

    ret = Md5StrEncodeToHex(str, mdhex, MD5_DIGEST_LENGTH);
    if (ret<0)
        return ret;

    for (int i=0;i<MD5_DIGEST_LENGTH;i++)
        sprintf(mdstr+2*i, "%02x", mdhex[i]);
    return 0;
}


