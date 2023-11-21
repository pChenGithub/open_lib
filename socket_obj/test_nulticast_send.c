#include "tcpIp/multicast.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    /* code */
    char sendbuff[16] = "hellword";

    while (1)
    {
        int ret = multicast_sendmsg(sendbuff, strlen(sendbuff)+1, "224.0.1.0", 9999);
        if (ret<0) {
            printf("发送失败, 错误码 %d\n", ret);
        }

        sleep(10);
    }

    return 0;
}
