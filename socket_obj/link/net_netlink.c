#include "net_netlink.h"
#include "net_errno.h"
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <net/if.h>

#define NL_THREAD_RUNNING   0x01
typedef struct netlink_listen {
    int flag;
    int socketfd;
    hand_netlink_ev cb[NLEV_MAX];
} NETLINK_LISTEN;

static NETLINK_LISTEN server = {
    // lock
    .flag = 0,
    .socketfd = 0,
    // 回调
    {0},
};

static int parse_linkevent(hand_netlink_ev cb, struct nlmsghdr* msg) {
    if (NULL==cb || NULL==msg)
        return -NETERR_CHECK_PARAM;

    struct ifinfomsg *ifi = NULL;
    struct rtattr* rta = NULL;
    NETLINK_CB_PARAM cbarg;
    memset(&cbarg, 0, sizeof(NETLINK_CB_PARAM));

    // 剥离消息
    if (sizeof(*ifi)>(msg->nlmsg_len-sizeof(*msg)))
    {
        printf("消息不完整\n");
        return -1;
    }

    // 获取nlmsghdr后的地址
    ifi = NLMSG_DATA(msg);
    #if 0
    if ((ifi->ifi_flags & IFF_LOOPBACK) != 0) {
        return -1;
    }
    #endif

    // 获取rtattr
    rta = (struct rtattr *)
        ((char *) ifi + NLMSG_ALIGN(sizeof(*ifi)));
    int len = NLMSG_PAYLOAD(msg, sizeof(*ifi));

    if (!RTA_OK(rta, len))
        return -1;

    cbarg.ifname = RTA_DATA(rta);
    if (ifi->ifi_flags&IFF_RUNNING) {
        cbarg.link = NET_LINK_UP; // down
        #if 0
    } else if (!(msg->nlmsg_flags&IFF_RUNNING)) {
        cbarg.link = NET_LINK_UP; // up
        #endif
    } else {
        cbarg.link = NET_LINK_DOWN; // unkown
    }

    cb(&cbarg);

    return 0;
}

// 监听线程
#define NL_RECVBUFF_LEN 1024
static char recvbuff[NL_RECVBUFF_LEN] = {0};
static void* recv_from_netlink(void* arg) {
    int ret = 0;
    NETLINK_LISTEN* sv = (NETLINK_LISTEN*)arg;
/** 
 * struct nlmsghdr
 * struct ifinfomsg
 * 这里内存4字节对齐
 * struct rtattr
 * 这里放rtattr的内容
*/
    struct nlmsghdr* mlmsg = (struct nlmsghdr*)recvbuff;
    sv->flag |= NL_THREAD_RUNNING;
    while (NL_THREAD_RUNNING&(sv->flag)) {
        //printf("开始等待事件\n");
        memset(recvbuff, 0, NL_RECVBUFF_LEN);
        // flag MSG_WAITALL
        if ((ret=recv(sv->socketfd, recvbuff, NL_RECVBUFF_LEN, 0))<=0)
        {
            printf("接收失败，返回 %d\n", ret);
            continue;
        }

        printf("接收数据长度 %d\n", ret);

        if (!NLMSG_OK(mlmsg, ret))
        {
            // 不是合法的netlink消息
            printf("不是合法的netlink消息，返回\n");
            continue;
        }

        #if 0
        struct rtattr *rta
        RTA_OK(rta, len)
        RTA_DATA(rta)
        // 去除nlmsghdr后的长度
        NLMSG_PAYLOAD(nh, sizeof(*ifi))
        // 返回输入长度的字节对齐长度（这里是4字节对齐）
        NLMSG_ALIGN(sizeof(*ifi)))
        #endif
        
        //printf("recv %d 长度 %d\n", mlmsg->nlmsg_type, mlmsg->nlmsg_len);
        // 根据不同的事件类型，使用不同的回调
        switch (mlmsg->nlmsg_type)
        {
        case RTM_NEWLINK:
            parse_linkevent(sv->cb[NLEV_LINK], mlmsg);
            break;
        default:
            break;
        }
    }
}

int start_netlink_listen() {
    int ret = 0;
    if (NL_THREAD_RUNNING&server.flag)
        return -NETERR_NETLINK_ISRUNNING;

    int socketfd = socket(AF_NETLINK, SOCK_RAW, 0);
    if (socketfd<0)
        return -NETERR_SOCKET_FAIL;

    struct sockaddr_nl addr;
    memset(&addr, 0, sizeof(addr));
    addr.nl_family = AF_NETLINK;
    // 表示网络link状态改变，ipv4地址修改监听
    addr.nl_groups = RTMGRP_LINK | RTMGRP_IPV4_IFADDR;

    if (bind(socketfd, (struct sockaddr*)&addr, sizeof(addr))<0)
        return -NETERR_BIND_FAIL;

    server.socketfd = socketfd;
    pthread_t pid = 0;
    ret = pthread_create(&pid, NULL, recv_from_netlink, &server);
    if (ret<0) {
        ret = -NETERR_PTHREADCREATE_FAIL;
        goto closefd_exit;
    }

    return 0;

closefd_exit:
    close(socketfd);
    return ret;        
}

// 停止netlink事件监听
int stop_netlink_listen() {
    NETLINK_LISTEN* sv = &server;
    // 停止线程
    sv->flag &= (~NL_THREAD_RUNNING);
    pthread_cancel(sv->socketfd);
    // 等待线程退出（加超时）
    pthread_join(sv->socketfd, NULL);
    // 关闭socket
    close(sv->socketfd);
    // 复位全局变量
    memset(sv, 0, sizeof(*sv));
    return 0;
}

int set_netlink_listen_ev(NLEV_TYPE ev, hand_netlink_ev cb) {
    if (ev>=NLEV_MAX || NULL==cb)
        return -NETERR_CHECK_PARAM;

    NETLINK_LISTEN* sv = &server;
    sv->cb[ev] = cb;
    return 0;
}

int rm_netlink_listen_ev(NLEV_TYPE ev) {
    if (ev>=NLEV_MAX)
        return -NETERR_CHECK_PARAM;

    NETLINK_LISTEN* sv = &server;
    sv->cb[ev] = 0;
    return 0;
}

int nl_getGateway() {
    return 0;
}


