pro += test_uart

CFLAGS += -I uart
CC=arm-linux-gnueabihf-gcc

all : ${pro}

${pro} : test_uart.c uart/uart.c


clean :
	rm ${pro}