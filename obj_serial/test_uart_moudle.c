#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "uart_moudle.h"

#define DEVICE_NAME "/dev/ttySLB2"
UART_ENTRY* gentry = NULL;

int main(int argc, char const *argv[])
{
    // 19200 8bit 偶校验 1停止未
    start_uart(&gentry, DEVICE_NAME, BAUDRATE_19200, SBIT_1, EVENT_ODD);
    set_uart_ck();

    while (1) 
    {
        sleep(100);
    }
    stop_uart(gentry);

    return 0;
}
