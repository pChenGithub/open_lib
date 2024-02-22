#include "tcpIp/multicast.h"
#include "tcpIp/socket_file.h"
#include "protocol/device_prot.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static RECV_MSG_BODY *body = NULL;
static RECV_TCP_MSG_BODY *tcpbody = NULL;

// 处理安装包文件
static int recv_app_pkg(handTcpArg* arg) {
    printf("处理下发的更新包\n");
    int ret = sock_recv_file(arg->socketfd, "/data/upupupup.tar.gz");
    if (ret<0) {
        return -1;
    }

    // 解压压缩文件
    printf("解压更新包\n");
}

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
            // 准备号接收文件,开启tcp服务,端口10002准备接收文件,接收完后解压
            if (NULL!=tcpbody->fn) {
                rsp->code = -1;
            } else 
                tcpbody->fn = recv_app_pkg;
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

#if 1
    // 启动tcp监听
    ret = tcp_listen_start(&tcpbody, NULL, (char*)(argv[1]), 10002);
    if (ret<0) {
        printf("监听失败, 错误码 %d\n", ret);
    }
#endif

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
