#include "tcpIp/multicast.h"
#include "tcpIp/raw_socket.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static int handRsp(char* rsp, int len) {
    printf("接收到消息 %s, 长度 %d\n", rsp, len);
}

int main(int argc, char const *argv[])
{
    /* code */
    char sendbuff[64] = "{"
                        "\"cmdgroup\":\"\","
                        "\"cmd\":\"\","
                        // getinfo  base
                        "\"arg1\":\"\""
                        "}";

    if (argc<3) {
        printf("输入参数错误, 举例 %s <ip> <port>\n", argv[0]);
        return -1;
    }

#if 0
    // "224.0.1.0"
    // 10000
    int ret = multicast_sendmsg_wait(sendbuff,  sizeof(sendbuff), strlen(sendbuff)+1, (char*)(argv[1]), atoi(argv[2]), NULL, handRsp, 100);
    if (ret<0) {
        printf("发送失败, 错误码 %d\n", ret);
    }
#endif

#if 0
// 182 recv
unsigned char arp[] = {
0x00, 0x01, 0x08, 0x00,
0x06, 0x04, 0x00, 0x01,
0x6e, 0xcc, 0x0b, 0xdf, 0x75, 0xf0,
172, 16, 70, 185,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
172,  16,  70,  13,
};
#else 
// 182 to
unsigned char arp[] = {
0x00, 0x01, 0x08, 0x00,
0x06, 0x04, 0x00, 0x01,
0x08, 0x00, 0x27, 0xb6, 0x19, 0x64,
172, 16, 70, 182,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
172,  16,  70,  185,
// ARP结束
};
#endif

// pc ubuntu    08:00:27:b6:19:64     0x08, 0x00, 0x27, 0xb6, 0x19, 0x64
// 185 mac     6E:CC:0B:DF:75:F0      0x6e, 0xcc, 0x0b, 0xdf, 0x75, 0xf0
//unsigned char destmac[6] = {0x08, 0x00, 0x27, 0xb6, 0x19, 0x64};
// 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
unsigned char destmac[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    unsigned char srcmac[6] = {0x08, 0x00, 0x27, 0xb6, 0x19, 0x64};
    int ret = arp_sendmsg_wait(sendbuff, sizeof(sendbuff), destmac, NULL, 100);
    if (ret<0) {
        printf("发送失败, 错误码 %d\n", ret);
    }

    return 0;
}
