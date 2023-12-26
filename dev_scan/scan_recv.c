#include "tcpIp/multicast.h"
#include "protocol/prot_json.h"
#include "link/net_opt.h"
#include "mac_opt.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void recvmsgbd(handMulticastArg* arg) {
    RECV_MSG_BODY *body = (RECV_MSG_BODY *)arg;
    // 处理消息
    printf("<== %s\n", arg->recvBuff);
    // 解析消息,根据消息准备数据回复,
    int ret = 0;
    unsigned char cmd = 0;
    ret = parser_getInfo(arg->recvBuff, &cmd);
    if (ret<0) {
        printf("协议错误,错误码 %d\n", ret);
        return;
    }

#if 1
    // 处理广播指令
    switch (cmd) {
    case CMD_GET_INFO: {
        default:
            // 获取信息,返回
            printf("发送回复信息\n");
            memset(body->sendBuff, 0, sizeof(body->sendBuff));
            char *pbuff = body->sendBuff;
            int bufflen = 0;
            // memcpy(body->sendBuff, "IP:172.16.70.182 SN:112233665544", strlen("IP:172.16.70.182 SN:112233665544") + 1);
            memcpy(pbuff, "IP:", 3);
            bufflen += 3;
            pbuff += 3;
            // eth ip
            get_ip("eth0", pbuff, 128-bufflen);
            bufflen = strlen(body->sendBuff);
            pbuff = body->sendBuff + bufflen;
            // mac
            strcat(body->sendBuff, " SN:");
            bufflen += 4;
            pbuff += 4;
            ret = mac_to_snstr( pbuff, 128-bufflen);
            if (ret<0) {
                printf("获取mac失败,错误码 %d\n", ret);
            }
            printf("获取mac %s\n", pbuff);
            multicast_resp(body, body->sendBuff, strlen(body->sendBuff) + 1);
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
