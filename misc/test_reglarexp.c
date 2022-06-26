#include <stdio.h>
#include "regular_exp.h"

int main(int argc, char const *argv[])
{
    int ret = 0;
    /* code */
    char* ip = "172.16.70.00";
    char* url = "baiduc.om.ccc";

    ret = regular_match(ip, REGULAR_STR_IP);
    if (REGULAR_MATCH!=ret) {
        printf("IP正则表达式不匹配，错误码 %d\n", ret);
    }

    ret = regular_match(url, REGULAR_STR_HOST);
    if (REGULAR_MATCH!=ret) {
        printf("域名正则表达式不匹配，错误码 %d\n", ret);
    }
    return 0;
}
