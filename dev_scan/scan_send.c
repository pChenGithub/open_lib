#include "tcpIp/multicast.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>

static void handRsp(char* rsp, int len) {
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

    int ret = multicast_sendmsg_wait(sendbuff, strlen(sendbuff)+1, "224.0.1.0", 10000, handRsp, 2000);
    if (ret<0) {
        printf("发送失败, 错误码 %d\n", ret);
    }

    return 0;
}
