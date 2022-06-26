#include "string_opt.h"
#include <stdio.h>
#include <string.h>

// 字符串的16进制转数值的16进制
// 需要自己控制输入与输出的长度，函数内部只判断输出是不是为0
int hexstrToHexchar(char *out, unsigned int len, const char *in)
{
    if (NULL==out|| 0==len || NULL==in)
        return -1;

    int tmp = 0;
    unsigned int i = 0;
    for (i=0;i<len;i++) {
        tmp = in[2*i];
        if (tmp>='a')
            tmp -= ('a'-10);
        else if (tmp>='A')
            tmp -= ('A'-10);
        else
            tmp -= '0';

        out[i] = tmp*16;

        tmp = in[2*i+1];
        if (tmp>='a')
            tmp -= ('a'-10);
        else if (tmp>'A')
            tmp -= ('A'-10);
        else
            tmp -= '0';

        out[i] += tmp;
    }

    return 0;
}

int isHexItem() {
    return 0;
}

// 数组用16进制字符串表示
int hexToHexstr(char* arrayin, int inlen, char* strout, int outlen) {
    if (NULL==arrayin || NULL==strout)
        return -STROPTERR_CHECKPARAM;

    if (inlen>=(2*outlen))
        return -STROPTERR_CHECKPARAM;

    int i = 0;
    for (i=0;i<inlen;i++) {
        snprintf(strout+2*i, 3, "%02x", arrayin[i]);
        //printf("%s\n", strout);
    }

    strout[outlen-1] = 0;
    return 0;
}

// 去除16进制字符串的后面补0,这里不见查是否是合法的 "112233",
// 会修改传入字符串
int hexstrStriptail(char* hexstr) {
    if (NULL==hexstr)
        return -STROPTERR_CHECKPARAM;

    int len = strlen(hexstr);
    if (len%2)
        return -STROPTERR_CHECKPARAM;

    int i = 0;
    for (i=(len-2);i>=0;i-=2)
    {
        if ('0'!=hexstr[i] || '0'!=hexstr[i+1])
            break;
        hexstr[i] = 0;
    }

    return 0;
}

int hexarrayToint(const char* in, int inlen) {
    if (NULL==in || inlen<=0)
        return -STROPTERR_CHECKPARAM;

    int ret = 0;
    int tmpl = 0;
    int tmph = 0;
    int i = 0;
    const char* pin = in;
    for (i=0;i<inlen;i++) {
        tmpl = (*pin)&0xf;
        tmph = (*pin)>>4;
        if (tmpl>9 || tmph>9)
            return -STROPTERR_CHECKPARAM;

        ret = ret*100+tmph*10+tmpl;
        pin++;
    }

    return ret;
}

int hexToint(const char in) {
    return hexarrayToint(&in, 1);
}




