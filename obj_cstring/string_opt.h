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
char *i2Str(char* out, int outlen, int in);
// 4字节IP转字符串,ipBytes是char[4]类型
char* ipByte2Str(char* ipBytes, char* out, int outlen);

#ifdef __cplusplus
}
#endif
#endif

