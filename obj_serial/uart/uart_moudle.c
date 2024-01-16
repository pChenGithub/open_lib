#include "uart_moudle.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/select.h>
#include <string.h>
#include <sys/time.h>

static void* wait_uart(void* arg) {
    int ret = 0;
    UART_ENTRY* pentry = (UART_ENTRY*)arg;
    fd_set rfds;
    struct timeval tv;

    FD_ZERO(&rfds);
    FD_SET(pentry->fd, &rfds);
    // 监听串口,分发消息,同步或者异步消息
    while (1) { 
        // 第一个参数,监听最大描述符值加1
        ret = select(pentry->fd+1, &rfds, NULL, NULL, NULL);
        if (ret<0) {
            printf("select 执行失败\n");
            continue ;
        } else if (0==ret) {
            // 超时
            printf("select 等待超时\n");
            continue ;
        }

        ret = read(pentry->fd, pentry->rcvbuff, sizeof(pentry->rcvbuff));
        if (ret<0) {
             printf("read 失败\n");
            continue ;
        }

        // 读取成功
        for (int i=0;i<ret;i++) {
            printf("%02x ", pentry->rcvbuff[i]);
        }
        printf("\n");
    }
    return NULL;
}

int start_uart(UART_ENTRY** entry, const char* devname, UART_MOUDLE_BAUDRATE_TYPE speed, 
    UART_MOUDLE_STOPBIT_TYPE stop, UART_MOUDLE_EVENT_TYPE event) {
    int ret = 0;
    UART_ENTRY* pentry = NULL;
    if (NULL==entry || NULL==devname)
        return -UART_MOUDLE_ERR_CHECKPARAM;

    pentry = calloc(1, sizeof(UART_ENTRY));
    if (NULL==pentry)
        return -UART_MOUDLE_ERR_MALLOC;

    // O_NOCTTY ：告诉内核这个程序不想作为控制终端。如果不指定该标志，任何输入都会影响程序（比如键盘中止信号等）。
    // O_NDELAY ：告诉内核这个程序不关注DCD信号线的状态。如果不指定该标志，当DCD信号线是空电压值的时候，程序将会进入睡眠
    pentry->fd = open(devname, O_RDWR|O_NOCTTY|O_NDELAY);
    if (pentry->fd<0) {
        ret = -UART_MOUDLE_ERR_OPEN;
        goto exit_calloc;
    }
    // 设置成非阻塞
    fcntl(pentry->fd, F_SETFL, FNDELAY);

    ret = set_uart(pentry, speed, stop, event);
    if (ret<0) 
        goto exit_open;

    ret = pthread_create(&(pentry->pid), NULL, wait_uart, pentry);
    if (0!=ret) {
        ret = -UART_MOUDLE_ERR_PTHREAD;
        goto exit_open;
    }
    pthread_detach(pentry->pid);
    printf("创建成功, 线程号 %ld\n", pentry->pid);
    
    *entry = pentry;
    pthread_mutex_init(&(pentry->lockmsg), NULL);
    sem_init(&(pentry->wait_recv), 0, 0);
    return 0;

exit_open:
    close(pentry->fd);
exit_calloc:
    free(pentry);
    return ret;
}

int stop_uart(UART_ENTRY* entry) {
    int ret = 0;
    if (NULL==entry)
        return -UART_MOUDLE_ERR_CHECKPARAM;
    if (entry->pid>0) {
        pthread_cancel(entry->pid);
        pthread_join(entry->pid, NULL);
        printf("回收完成, 线程号 %ld\n", entry->pid);
    }

    if (entry->fd>0) {
        close(entry->fd);
    }

    pthread_mutex_destroy(&(entry->lockmsg));
    sem_destroy(&(entry->wait_recv));
    free(entry);
    return 0;
}

int set_uart(UART_ENTRY* entry, UART_MOUDLE_BAUDRATE_TYPE speed, 
    UART_MOUDLE_STOPBIT_TYPE stop, UART_MOUDLE_EVENT_TYPE event) {
    int ret = 0;
    struct termios options;
    if (NULL==entry)
        return -UART_MOUDLE_ERR_CHECKPARAM;

    ret = tcgetattr(entry->fd, &options);
    if (0!=ret) {
        return -UART_MOUDLE_ERR_GETATTR;
    }

    switch (speed)
    {
    case BAUDRATE_9600:
        ret = cfsetispeed(&options, B9600);
        ret = ret?ret:cfsetospeed(&options, B9600);
        break;
    case BAUDRATE_19200:
        ret = cfsetispeed(&options, B19200);
        ret = ret?ret:cfsetospeed(&options, B19200);
        break;
    case BAUDRATE_115200:
        ret = cfsetispeed(&options, B115200);
        ret = ret?ret:cfsetospeed(&options, B115200);
        break;
    default:
        break;
    }
    if (0!=ret)
        return -UART_MOUDLE_ERR_SETSPEED;

    options.c_cflag &= ~CSIZE; /* Mask the character size bits */
    options.c_cflag |= CS8;    /* Select 8 data bits */

    // CLOCAL 修改控制模式，保证程序不会占用串口
    // CREAD 修改控制模式，能够从串口读取数据
    options.c_cflag |= (CLOCAL | CREAD);
    // 不使用流控制
    options.c_cflag &= ~CRTSCTS;

     // 无校验
    options.c_cflag &= ~PARENB;
    // 设置停止位
    options.c_cflag &= ~CSTOPB;

    // 设置最少字符和等待时间
    options.c_cc[VMIN] = 1;     // 读数据的最小字节数
    options.c_cc[VTIME]  = 0;   //等待第1个数据，单位是10s

    // 修改输出模式，原始数据输出
    options.c_oflag &= ~OPOST;
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    // 清空终端未完成的数据
    tcflush(entry->fd, TCIFLUSH);
    // 设置新的参数
    ret = tcsetattr(entry->fd, TCSANOW, &options);
    if (0!=ret) {
        return -UART_MOUDLE_ERR_SETATTR;
    }
    return 0;
}

int send_byte_uart_wait(UART_ENTRY* entry, const char* send, int sendlen, char* recv, int recvlen, int waitMs) {
    int ret = 0;
    if (NULL==entry || NULL==send)
        return -UART_MOUDLE_ERR_CHECKPARAM;

    if (NULL==recv || recvlen<=0)
        return -UART_MOUDLE_ERR_CHECKPARAM;

    // 加锁, 设置超时,
    int timeout = UM_LOCKTIMEOUT_10MS;
    while (timeout && 0!=pthread_mutex_trylock(&(entry->lockmsg))) {
        usleep(10000);
        timeout--;
    }
    // 判断是否超时
    if (timeout<=0)
        return -UART_MOUDLE_ERR_TIMEOUT;

    ret = write(entry->fd, send, sendlen);
    if (ret<0) {
        printf("写串口失败\n");
        goto end_exit;
    }

    // 装载超时时间
    struct timespec waittime;
    struct  timeval tv;
    gettimeofday(&tv, NULL);
    //clock_gettime(CLOCK_REALTIME, &waittime);
    waittime.tv_sec = tv.tv_sec+(waitMs/1000);
    waittime.tv_nsec = tv.tv_usec*1000 + (waitMs%1000)*1000;
    //printf("wait recv msg %ld\n", time(NULL));
    // 等待
    if (0!=sem_timedwait(&(entry->wait_recv), &waittime))
    {
        // 超时
        //printf("waitrecv timeout %ld %d\n", time(NULL), errno);
        printf("waitrecv timeout\n");
        ret = -UART_MOUDLE_ERR_RECVTIMEOUT;
        goto end_exit;
    }
    printf("wait message OK\n");

    if (0!=entry->wait_code) {
        ret = entry->wait_code;
        goto end_exit;
    }

end_exit:
    printf("unlock exit\n");
    // 解锁
    pthread_mutex_unlock(&(entry->lockmsg));
    printf("push exit\n");
    
    return ret;
}

int set_uart_ck(UART_ENTRY* entry, hand_uart_msg hand) {
    if (NULL==entry || NULL==hand)
        return -UART_MOUDLE_ERR_CHECKPARAM;
    entry->hand_msg = hand;
    return 0;
}
