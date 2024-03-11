#include "tcpIp/multicast.h"
#include "tcpIp/socket_file.h"
#include "protocol/device_prot.h"
#include "../exeShell_obj/mixShell.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

static RECV_MSG_BODY *body = NULL;
static RECV_TCP_MSG_BODY *tcpbody = NULL;

// 处理日志文件
static int send_logs(handTcpArg* arg) {
    printf("处理设备日志\n");
    // 获取路径
    char cmd[72] = "ps|grep qws|head -n1|awk -F\" \" '{print $4}'|awk -F\"/app\" '{print $1}'";
    exeShell(cmd, cmd, sizeof(cmd));
    printf("应用路径 %s\n", cmd);
    // 去掉最后的\n
    cmd[strlen(cmd)-1] = 0;
    // 压缩日志
    printf("压缩日志\n");
    // tar zcf /data/uploard_log.tar.gz 32byte
    // tar zcf /tmp/uploard_log.tar.gz 31byte
    memmove(cmd+32,  cmd, strlen(cmd)+1);
    memcpy(cmd, "tar zcf /tmp/uploard_log.tar.gz ", 32);
    strcat(cmd, "/log");
    printf("压缩命令 %s\n", cmd);
    exeShell(cmd, NULL, 0);
    // 发送
    printf("开始发送日志\n");
    int ret = sock_send_file(arg->msgfd, "/tmp/uploard_log.tar.gz");
    if (ret<0) {
        return -1;
    }
    return 0;
}
// 处理安装包文件
static int recv_app_pkg(handTcpArg* arg) {
    printf("处理下发的更新包\n");
    int ret = sock_recv_file(arg->msgfd, "/tmp/app_pkg.tar.gz");
    if (ret<0) {
        printf("接收文件失败,错误码 %d\n", ret);
        return -1;
    }

    // ps|grep qws|head -n1|awk -F\" \" '{print $4}'|awk -F\"/app\" '{print $1}' 70byte
    char cmd[72] = "ps|grep qws|head -n1|awk -F\" \" '{print $4}'|awk -F\"/app\" '{print $1}'";
    exeShell(cmd, cmd, sizeof(cmd));
    printf("应用路径 %s\n", cmd);
    // 去掉最后一级目录
    char* pp = NULL;
    char* pc = cmd;
    while(0!=*pc) {
        if ('/'==*pc)
            pp = pc;
        pc++;
    }

    if (NULL!=pp)
        *pp = 0;

    printf("工程路径 %s\n", cmd);
    // 解压压缩文件
    printf("解压更新包\n");
    // tar xf /tmp/app_pkg.tar.gz -C /data
    // tar xf /tmp/app_pkg.tar.gz -C 29byte
    memmove(cmd+30, cmd, strlen(cmd)+1);
    memcpy(cmd, "tar xf /tmp/app_pkg.tar.gz -C ", 30);
    printf("解压命令 %s\n", cmd);
    exeShell(cmd, NULL, 0);
    return 0;
}
// 处理服务bin文件
static int recv_server_bin(handTcpArg* arg) {
    printf("处理下发的服务bin文件\n");
    int ret = sock_recv_file(arg->msgfd, "/tmp/scan_recv_arm");
    if (ret<0) {
        return -1;
    }

    // 777 7*8*8+7*8+7
    ret = chmod("/tmp/scan_recv_arm", 511);
    if (0==ret) {
        // copy文件
        printf("copy 文件\n");
        system("cp /tmp/scan_recv_arm /bin/scan_recv_arm;sync;");
    }
    return 0;
}

// 处理广播指令和udp指令
static void recvmsgbd(handMulticastArg* arg) {
    int ret = 0;
    RECV_MSG_BODY *body = (RECV_MSG_BODY *)arg;
    // 处理消息
    // 解析消息,根据消息准备数据回复,
    DEV_CMD * cmd = (DEV_CMD*)arg->recvBuff;
    // 回复结构体
    DEV_CMD_RSP* rsp = (DEV_CMD_RSP*)body->sendBuff;
    memcpy(&rsp->cmd, cmd, sizeof(DEV_CMD));
    ret = check_base_cmd((DEV_CMD_BASE*)cmd);
    if (ret<0) {
        // 校验出错,错误码是错误原因
        // 当前的版本号告知客户端
        #if 1
        rsp->cmd.base.vMajor = PROT_VMAJOR;
        rsp->cmd.base.vMinor = PROT_VMINOR;
        rsp->cmd.base.vtail = PROT_VPACK;
        #endif
        // 错误码告知
        rsp->cmd.base.code = ret;
        multicast_resp(body, (char*)rsp, sizeof(DEV_CMD_RSP));
        return ;
    }

#if 1
    // 处理广播指令
    switch (cmd->base.cmd) {
    case  CMD_DEVICE_UPDATE: {
        printf("设备更新命令\n");
        rsp->cmd.base.code = CMD_RSP_CODE_REDY;
        // 准备号接收文件,开启tcp服务,端口10002准备接收文件,接收完后解压
        if (NULL!=tcpbody->fn) {
            rsp->cmd.base.code = -CMD_RSP_CODE_CBK_NOTALLOW;
        } else 
            tcpbody->fn = recv_app_pkg;
        // 回复之后,对方就开始发送文件了
    break;
    }
    case CMD_GET_LOGS: {
        printf("设备日志获取命令\n");
        rsp->cmd.base.code = CMD_RSP_CODE_REDY;
        // 准备好发送文件,文件发送方式,客户端请求,服务端发送方式
        if (NULL!=tcpbody->fn) {
            rsp->cmd.base.code = -CMD_RSP_CODE_CBK_NOTALLOW;
        } else 
            tcpbody->fn = send_logs;
        // 回复之后,对方就开始请求文件
    break;
    }
    case CMD_DEV_REBOOT: {
        printf("设备重启获取命令\n");
        rsp->cmd.base.code = CMD_RSP_CODE_OK;
        // 回复之后,对方就开始请求文件
        multicast_resp(body, (char*)rsp, sizeof(DEV_CMD_RSP));
        sleep(2);
        system("sync;reboot");
        return ;
    break;
    }
    case CMD_SERVER_UP: {
        printf("服务更新命令\n");
        rsp->cmd.base.code = CMD_RSP_CODE_REDY;
        // 准备号接收文件,开启tcp服务,端口10002准备接收文件,接收完后解压
        if (NULL!=tcpbody->fn) {
            rsp->cmd.base.code = -CMD_RSP_CODE_CBK_NOTALLOW;
        } else 
            tcpbody->fn = recv_server_bin;
        // 回复之后,对方就开始发送文件了
        break;
    }
    case CMD_DEVICE_SCAN: 
    default:
    {
            // 获取信息,返回
            printf("扫描设备命令\n");
            fill_device_info(&rsp->cmd.devbinfo);
            rsp->cmd.base.code = CMD_RSP_CODE_OK;
            break;
    }
    }

    multicast_resp(body, (char*)rsp, sizeof(DEV_CMD_RSP));
#endif
}

int main(int argc, char const *argv[]) {
    if (argc<3) {
        printf("输入参数错误, 举例 %s <ip> <port>\n", argv[0]);
        return -1;
    }

	int count = 6;
    // 启动组播监听
listen_again:
	sleep(10);
	int ret = multicast_listen_start(&body, recvmsgbd, (char*)(argv[1]), atoi(argv[2]));
    if (ret<0) {
		if (count>0) {
	        printf("监听失败, 错误码 %d, 再一次\n", ret);
			count--;
			goto listen_again;
		}
		printf("监听失败, 错误码 %d\n", ret);
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
