#include <stdio.h>
#include "../crypto/SM2opt.h"

static char pubkey[1024] = {0};
static char prikey[1024] = {0};

static char *srcdata = "123456789";
static char outdata[64] = {0};

int main(int argc, char const *argv[])
{
    int ret = 0;
    EC_KEY *ec = NULL;
    
    ret = SM2GetKeys(pubkey, sizeof(pubkey), prikey, sizeof(prikey));
    if (ret<0)
        printf("获取密钥失败,错误码 %d\n", ret);

    ret = SM2GetEC(&ec, pubkey, _KEY_T_PUB);
    if (ret<0)
        printf("获取曲线失败,错误码 %d\n", ret);

    ret = SM2EVPEncode(pubkey, srcdata, 9, outdata, sizeof(outdata));
    if (ret<0)
        printf("加密失败,错误码 %d\n", ret);

    printf("加密后数据长度 %d\n", ret);
    for (int i = 0;i<ret;i++)
    {
        printf("%02x ", outdata[i]);
    }

        return 0;
}
