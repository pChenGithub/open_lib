#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "uart.h"

#define DEVICE_NAME "/dev/ttySLB2"
#define RECV_BUFF_LEN   64
static char recvbuff[RECV_BUFF_LEN] = {0};
int main(int argc, char const *argv[])
{
    // 19200 8bit 偶校验 1停止未
    int fd = uart_open(DEVICE_NAME, BAUDRATE_19200, SBIT_1, EVENT_EVEN);
    if(fd<0) {
        printf("打开串口失败，错误码 %d\n", fd);
        return -1;
    }

    while (1) 
    {
        sleep(1);
        memset(recvbuff, 0, RECV_BUFF_LEN);
        int ret = uart_read(fd, recvbuff, RECV_BUFF_LEN);
        if (ret<=0)
        {
            printf("读串口失败，错误码 %d\n", ret);
            continue;
        }

        printf("获取 %d个字节数据\n", ret);

        for (int i=0;i<ret;i++)
            printf(" %02x",  recvbuff[i]);
        printf("\n");
    }
    uart_close(fd);

    return 0;
}
