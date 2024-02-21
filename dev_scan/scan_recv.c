#include "tcpIp/multicast.h"
#include "protocol/device_prot.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void recvmsgbd(handMulticastArg* arg) {
    int ret = 0;
    RECV_MSG_BODY *body = (RECV_MSG_BODY *)arg;
    // 处理消息
    // 解析消息,根据消息准备数据回复,
    DEV_CMD * cmd = (DEV_CMD*)arg->recvBuff;

#if 1
    // 处理广播指令
    switch (cmd->cmd) {
    case  CMD_DEVICE_UPDATE:
    break;
    case CMD_DEVICE_SCAN: 
    default:
    {
            // 获取信息,返回
            printf("发送回复信息\n");
            DEVICE_INFO* rsp = (DEVICE_INFO*)body->sendBuff;
            fill_device_info(rsp);
            multicast_resp(body, (char*)rsp, sizeof(DEVICE_INFO));
            break;
    }
    }
#endif
}

static RECV_MSG_BODY *body = NULL;

int main(int argc, char const *argv[]) {
    if (argc<3) {
        printf("输入参数错误, 举例 %s <ip> <port>\n", argv[0]);
        return -1;
    }

    // 启动组播监听
    int ret = multicast_listen_start(&body, recvmsgbd, (char*)(argv[1]), atoi(argv[2]));
    if (ret<0) {
        printf("监听失败, 错误码 %d, 再一次\n", ret);
		sleep(10);
		ret = multicast_listen_start(&body, recvmsgbd, (char*)(argv[1]), atoi(argv[2]));
		if (ret<0) {
			printf("监听失败, 错误码 %d\n", ret);
		}
    }

    // 启动http监听,一些点对点的服务

    
    // 可以监听线程,处理线程间的关系
    // 也可以处理提交任务,处理任务
    while (1) {
        /* code */
        sleep(5);
    }

    multicast_listen_del(body);
    body = NULL;

    return 0;
}
