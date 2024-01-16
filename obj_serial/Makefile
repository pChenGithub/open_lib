pro1 += test_uart
pro2 += test_uart_moudle

pro += ${pro1} ${pro2}

CFLAGS += -I uart
#CC=/home/zhh/work/ytdctr327/02_YT327L门禁一体机/toolchain/gcc-linaro-5.5.0-2017.10-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc

all : ${pro}

${pro1} : test_uart.c uart/uart.c

${pro2} : test_uart_moudle.c uart/uart_moudle.c -lpthread


clean :
	rm ${pro}