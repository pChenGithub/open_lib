#ifndef __STRING_OPT_H__
#define __STRING_OPT_H__
#ifdef __cplusplus
extern "C" {
#endif

#define STROPTERR_CHECKPARAM        1   // 参数检查错误

// 16进制字符串,转16进制数组,"112233"转0x112233
int hexstrToHexchar(char *out, unsigned int len, const char *in);
// 16进制数组,转16进制字符串,0x112233转"112233"
int hexToHexstr(char* arrayin, int inlen, char* strout, int outlen);
// 16进制字符串,去除后面的补0
int hexstrStriptail(char* hexstr);
// 输入一个字节,用16进制表示其整数, 0x1617转成1617
int hexarrayToint(const char* in, int inlen);
// 单个字节转整数, 0x16转16
int hexToint(const char in);

// 拷贝字符串,如果buff不够长,会截断
int strCopyC(char* buff, int len, const char* str);
#define STR_COPY(buff, bufflen, str) ({ \
    int strlenght = strlen(str); \
    strlenght = strlenght<bufflen?strlenght:bufflen-1; \
    memcpy(buff, str, strlenght); \
    buff[strlenght] = 0; \
})
// 数字转字符串,需要指定buff,并会返回这个buff
char* ll2Str(char* out, int outlen, long long in);
char* l2Str(char* out, int outlen, long in);
char *i2Str(char* out, int outlen, int in);
// 4字节IP转字符串,ipBytes是char[4]类型
char* ipByte2Str(char* ipBytes, char* out, int outlen);
// IP转4字节数字
int ipStr2Byte4(unsigned char* ipBytes, const char* ip);
// 从url字符串获取主机域名/IP
char* getHostFromUrl(const char* url, char* host, int len);
//***************************************************************
// 字符串切割,失败返回负值,成功返回字符串个数
typedef struct {
    char* buff;
    int bufflen;
} CUT_STR;
int cutStr(const char* src, char cutchar, CUT_STR* out, int outlen);
// 跟cutStr的区别在于,cutStrSelf,会修改源字符串,由一个数组返回字符串的地址
int cutStrSelf(char* src, char cutchar, char** out, int outlen);
#ifdef __cplusplus
}
#endif
#endif

