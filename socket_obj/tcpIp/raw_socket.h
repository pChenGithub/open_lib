#pragma once
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    // MAC头
    unsigned char destmac[6];
    unsigned char srcmac[6];
    unsigned char type[2];  // 协议类型 ipv4(0x0800)/ipv6
    // IP头
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
} RAW_MAC_IP_HEAD;

int send_pkg();
int recv_pkg(char* recvbuff, int bufflen);

#ifdef __cplusplus
}
#endif

