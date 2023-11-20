#include "tcpIp/multicast.h"
#include <unistd.h>
#include <stdio.h>

static void recvmsg(handMulticastArg* arg) {
    // 处理消息
    printf("<== %s\n", arg->recvBuff);
}

static RECV_MSG_BODY *body = NULL;

int main(int argc, char const *argv[])
{
    /* code */
    while (1) {
        int ret = multicast_listen_start(&body, recvmsg,"224.0.1.1", 10000);
        if (ret<0) {
            printf("监听失败, 错误码 %d\n", ret);
        }

        usleep(10000);
        multicast_listen_del(body);
        //body = NULL;
        usleep(10000);
    }
    return 0;
}
