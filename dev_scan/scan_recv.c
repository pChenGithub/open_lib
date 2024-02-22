#include "tcpIp/multicast.h"
#include "tcpIp/socket_file.h"
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
            printf("设备更新命令\n");
            DEV_CMD_RSP* rsp = (DEV_CMD_RSP*)body->sendBuff;
            memcpy(&rsp->cmd, cmd, sizeof(DEV_CMD));
            rsp->code = CMD_RSP_CODE_REDY;
            // 准备号接收文件,
            // 回复之后,对方就开始发送文件了
            multicast_resp(body, (char*)rsp, sizeof(DEV_CMD_RSP));
    break;
    case CMD_DEVICE_SCAN: 
    default:
    {
            // 获取信息,返回
            printf("扫描设备命令\n");
            DEVICE_INFO* rsp = (DEVICE_INFO*)body->sendBuff;
            fill_device_info(rsp);
            multicast_resp(body, (char*)rsp, sizeof(DEVICE_INFO));
            break;
    }
    }
#endif
}

static void tcprcvbk(handTcpArg* arg) {
    printf("收到一包数据\n");
}

static RECV_MSG_BODY *body = NULL;
static RECV_TCP_MSG_BODY *tcpbody = NULL;

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

    // 启动tcp监听
    ret = tcp_listen_start(&tcpbody, tcprcvbk, (char*)(argv[1]), 10002);
    if (ret<0) {
        printf("监听失败, 错误码 %d\n", ret);
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
