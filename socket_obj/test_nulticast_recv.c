#include "tcpIp/multicast.h"
#include <unistd.h>
#include <stdio.h>

static void recvmsg(handMulticastArg* arg) {
    // 处理消息
    printf("<== %s\n", arg->recvBuff);
}

int main(int argc, char const *argv[])
{
    /* code */
    int ret = multicast_listen(recvmsg,"224.0.1.1", 666);
    if (ret<0) {
        printf("监听失败, 错误码 %d\n", ret);
    }

    while (1)
    {
        sleep(100);
    }

    return 0;
}
