#include "tcpIp/multicast.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>

static void recvmsg(handMulticastArg* arg) {
}

int main(int argc, char const *argv[])
{
    /* code */
    char sendbuff[16] = "hellword";
    int ret = multicast_sendmsg(sendbuff, strlen(sendbuff)+1);
    if (ret<0) {
        printf("发送失败, 错误码 %d\n", ret);
    }
    return 0;
}
