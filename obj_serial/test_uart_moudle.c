#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "uart_moudle.h"

#define DEVICE_NAME "/dev/ttyUSB0"
UART_ENTRY* gentry = NULL;

static void hand_msg() {
}

static char recv[64] = {0};
int main(int argc, char const *argv[])
{
    int ret = 0;
    // 19200 8bit 偶校验 1停止未
    ret = start_uart(&gentry, DEVICE_NAME, BAUDRATE_19200, SBIT_1, EVENT_ODD);
    if (ret<0) {
        printf("启动串口失败, 错误码 %d\n", ret);
        return -1;
    }
    ret = set_uart_ck(gentry, hand_msg);
    if (ret<0) {
        printf("设置回调失败, 错误码 %d\n", ret);
        return -1;
    }

    // 最后一个0是结尾
    char send[] = {0x7E, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0xAA, 0x00, 0x01, 0xA3, 0x7A, 0x7E};
    send_byte_uart_wait(gentry, send, sizeof(send), recv, sizeof(recv), 1000);

    while (1) 
    {
        sleep(100);
    }
    stop_uart(gentry);

    return 0;
}
