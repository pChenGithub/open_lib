#include "raw_socket.h"
#include "tcpIp_errno.h"
#include <sys/socket.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ether.h>		//ETH_P_ALL
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>

typedef struct {
    unsigned char destmac[6];
    unsigned char srcmac[6];
    unsigned char type[2];  // 协议类型 ipv4(0x0800)/ipv6
} RAW_MAC_HEAD;

typedef struct {
    unsigned char ver_len;                              // 版本号和IP头长度,以4字节为单位
    unsigned char dsf;
    unsigned char toltal_len[2];                    // 总长度,IP头和数据总长度

    unsigned char identification[2];
    unsigned char flags;
    unsigned char frame_offset[2];
    unsigned char ttl;                                          // 跃点数
    unsigned char protocol;                             // 协议类型 udp(17) igmp(2)
    unsigned char head_sum_check[2];    // 头部和校验
    unsigned char src_addr[4];
    unsigned char dest_addr[4];
} RAW_IP_HEAD;

int send_pkg() {
    int fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (fd<0)
        return -TCPIPERR_SOCKET_CREATE;

    return 0;
}

#define PRINTF_HEX(buff, len) {\
    for (int i=0;i<len;i++) \
        printf("%02x", buff[i]); \
    printf("\n"); \
}

int recv_pkg(char* recvbuff, int bufflen) {
    int ret = 0;
    int fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    if (fd<0)
        return -TCPIPERR_SOCKET_CREATE;

    struct sockaddr_in srcaddr;
	socklen_t addr_length = sizeof(struct sockaddr_in);

    char ipsrc[16] = {0};
    char destsrc[16] = {0};
    while (1) {
        ret = recvfrom(fd, recvbuff, bufflen, 0, (struct sockaddr *)(&srcaddr), &addr_length);
        if (-1==ret) {
            printf("接收消息失败, 错误码 %d\n", errno);
            ret = -TCPIPERR_RECV_DATA;
        }
        printf("\n\n接收消息长度 %d\n", ret);
        RAW_MAC_IP_HEAD* phead = (RAW_MAC_IP_HEAD*)recvbuff;
        printf("目标mac:");
        PRINTF_HEX(phead->destmac, sizeof(phead->destmac));
        printf("源mac:");
        PRINTF_HEX(phead->srcmac, sizeof(phead->srcmac));

        inet_ntop(AF_INET, phead->src_addr, ipsrc, sizeof(ipsrc));
        inet_ntop(AF_INET, phead->dest_addr, destsrc, sizeof(destsrc));
        printf("源地址 %s\n", ipsrc);
        printf("目的地址 %s\n", destsrc);
        sleep(1);
    }

    close(fd);
    return ret;
}