#include "tcpIp/multicast.h"
#include <unistd.h>
#include <stdio.h>

static void recvmsgbg(handMulticastArg* arg) {
    // 处理消息
    printf("<== %s\n", arg->recvBuff);
}

static RECV_MSG_BODY *body = NULL;

int main(int argc, char const *argv[])
{
    /* code */
   
        int ret = multicast_listen_start(&body, recvmsgbg,"224.0.1.0", 9999);
        if (ret<0) {
            printf("监听失败, 错误码 %d\n", ret);
        }

 while (1) {
        sleep(100);
    }

    multicast_listen_del(body);
    return 0;
}
