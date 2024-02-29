#include "tcpIp/multicast.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static int handRsp(char* rsp, int len) {
    printf("接收到消息 %s, 长度 %d\n", rsp, len);
}

int main(int argc, char const *argv[])
{
    /* code */
    char sendbuff[64] = "{"
                        "\"cmdgroup\":\"\","
                        "\"cmd\":\"\","
                        // getinfo  base
                        "\"arg1\":\"\""
                        "}";

    if (argc<3) {
        printf("输入参数错误, 举例 %s <ip> <port>\n", argv[0]);
        return -1;
    }

    // "224.0.1.0"
    // 10000
    int ret = multicast_sendmsg_wait(sendbuff,  sizeof(sendbuff), strlen(sendbuff)+1, (char*)(argv[1]), atoi(argv[2]), handRsp, 100);
    if (ret<0) {
        printf("发送失败, 错误码 %d\n", ret);
    }

    return 0;
}
