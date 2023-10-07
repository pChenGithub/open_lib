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

// 拷贝字符串
int strCopyC(char *buff, int len, const char* str)
{
    if (NULL==buff || len<=0 || NULL==str)
        return -STROPTERR_CHECKPARAM;

    int strlenght = strlen(str);
    strlenght = strlenght<len?strlenght:len-1;

    memcpy(buff, str, strlenght);
    buff[strlenght] = 0;
    return 0;
}

char *ll2Str(char *out, int outlen, long long in)
{
    if (NULL==out || outlen<=0)
        return NULL;
    snprintf(out, outlen, "%lld", in);
    return out;
}

char *l2Str(char *out, int outlen, long in)
{
    if (NULL==out || outlen<=0)
        return NULL;
    snprintf(out, outlen, "%ld", in);
    return out;
}

char *i2Str(char *out, int outlen, int in)
{
    if (NULL==out || outlen<=0)
        return NULL;
    snprintf(out, outlen, "%d", in);
    return out;
}

char *ipByte2Str(char *ipBytes, char *out, int outlen)
{
    if (NULL==ipBytes || NULL==out || outlen<=0)
        return NULL;
    snprintf(out, outlen, "%d.%d.%d.%d",
             ipBytes[0], ipBytes[1], ipBytes[2], ipBytes[3]);
    return out;
}

int ipStr2Byte4(unsigned char *ipBytes, const char *ip)
{
    int ip0=0, ip1=0, ip2=0, ip3=0;
    if (NULL==ipBytes || NULL==ip)
        return -1;
    sscanf(ip, "%d.%d.%d.%d", &ip0, &ip1, &ip2, &ip3);
    ipBytes[0] = ip0;
    ipBytes[1] = ip1;
    ipBytes[2] = ip2;
    ipBytes[3] = ip3;
    return 0;
}

char *getHostFromUrl(const char *url, char *host, int len)
{
    if (NULL==url || NULL==host || len<=0)
        return NULL;

    const char* ps = NULL;
    const char* pe = url;

    while (!('/'==pe[0] && '/'!=pe[1]))
    {
        if (':'==pe[0])
        {
            pe += 3;
            // 获取开头
            ps = pe;
            continue;
        }
        pe++;
    }

    int hostlen = pe-ps;
    if (len<=hostlen)
        return NULL;

    memcpy(host, ps, hostlen);
    host[hostlen] = 0;

    return host;
}

// 按字符截取字符串,copy返回
int cutStr(const char *src, char cutchar, CUT_STR *out, int outlen)
{
    if (NULL==src || NULL==out || outlen<=0)
        return -1;

    int cylen = 0;
    int i = 0;
    char* pstr = src;
    char* sstr = src;
    // 遍历字符串
    while (0!=(*pstr) && i<outlen) {
        if (cutchar!=(*pstr)) {
            pstr++;
            continue ;
        }

        if (sstr==pstr) {
            // 都向后移动
            sstr++;
            pstr++;
            continue ;
        }

        // copy
        cylen = pstr-sstr;
        if (cylen>=out[i].bufflen)
            cylen = out[i].bufflen-1;
        memset(out[i].buff, 0, out[i].bufflen);
        memcpy(out[i].buff, sstr, cylen);
        i++;
        pstr++;
        sstr = pstr;
    }

    return i;
}



