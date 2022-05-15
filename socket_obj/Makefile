CC = /home/zhh/work/YT312/02_YT312门禁控制器/gcc-linaro-arm-linux-gnueabihf-4.7-2013.03-20130313_linux/bin/arm-linux-gnueabihf-gcc
pro ?= test
pro_nl ?= test_nl

ARCH=arm
CFLAGS += -L/home/zhh/work/YT312/02_YT312门禁控制器/gcc-linaro-arm-linux-gnueabihf-4.7-2013.03-20130313_linux/arm-linux-gnueabihf/libc/lib/arm-linux-gnueabihf -lpthread

all: ${pro} ${pro_nl}
#	$(DEBUGGER) ./${pro_nl} ${ARGS}

${pro}: test.c link/net_link.c link/net_opt.c

${pro_nl} : test_nl.c link/net_netlink.c

clean: