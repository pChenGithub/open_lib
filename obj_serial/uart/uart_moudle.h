#param once
#include <pthread.h>

#define UART_MOUDLE_ERR_CHECKPARAM   1   // 参数检查失败
#define UART_MOUDLE_ERR_MALLOC              2   // 内存分配失败
#define UART_MOUDLE_ERR_OPEN              3   // 打开串口失败
#define UART_MOUDLE_ERR_PTHREAD              4  // 创建线程失败
#define UART_MOUDLE_ERR_GETATTR              5  // 获取串口属性失败
#define UART_MOUDLE_ERR_SETATTR              6  // 获取串口属性失败
#define UART_MOUDLE_ERR_SETSPEED              7  // 设置串口波特率是吧

typedef enum {
    BAUDRATE_9600 = 0,
    BAUDRATE_19200,
    BAUDRATE_115200,
} UART_MOUDLE_BAUDRATE_TYPE;

typedef enum {
    SBIT_1 = 0,
    SBIT_1_5,
    SBIT_2,
} UART_MOUDLE_STOPBIT_TYPE;

typedef enum {
    EVENT_ODD = 0,
    EVENT_EVEN,
} UART_MOUDLE_EVENT_TYPE;

typedef struct {
    int fd;
    // 设置波特率
    UART_MOUDLE_BAUDRATE_TYPE   baudrate;
    // 设置停止位
    UART_MOUDLE_STOPBIT_TYPE        stopbit;
    // 设置校验格式
    UART_MOUDLE_EVENT_TYPE             event;
    pthread_t pid;
    char devname[16];
} UART_ENTRY;

int start_uart(UART_ENTRY** entry, const char* devname, UART_MOUDLE_BAUDRATE_TYPE speed, 
    UART_MOUDLE_STOPBIT_TYPE stop, UART_MOUDLE_EVENT_TYPE event);
int stop_uart(UART_ENTRY* entry);
int set_uart(UART_ENTRY* entry, UART_MOUDLE_BAUDRATE_TYPE speed, 
    UART_MOUDLE_STOPBIT_TYPE stop, UART_MOUDLE_EVENT_TYPE event);
// 设置回调处理异步消息
int set_uart_ck();
// 发送消息,同步等回复
int send_uart_wait();
// 发送消息,不等,异步
int send_uart_one();