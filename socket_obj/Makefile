CC = /home/zhh/work/YT312/02_YT312门禁控制器/gcc-linaro-arm-linux-gnueabihf-4.7-2013.03-20130313_linux/bin/arm-linux-gnueabihf-gcc
pro ?= test
pro_nl ?= test_nl
pro_gate ?= test_gate
pro_dns ?= test_dns
pro_link ?= test_link

ARCH=arm
CFLAGS += -L/home/zhh/work/YT312/02_YT312门禁控制器/gcc-linaro-arm-linux-gnueabihf-4.7-2013.03-20130313_linux/arm-linux-gnueabihf/libc/lib/arm-linux-gnueabihf -lpthread


all: ${pro} ${pro_nl} ${pro_gate} ${pro_dns} ${pro_link} 
#	$(DEBUGGER) ./${pro_nl} ${ARGS}

${pro}: test.c link/net_link.c link/net_opt.c

${pro_nl} : test_nl.c link/net_netlink.c

${pro_gate} : test_gate.c link/net_netlink.c

${pro_dns} : test_dns.c link/net_dns.c

${pro_link} : test_link.c link/net_opt.c

clean:
	rm ${pro} ${pro_nl} ${pro_gate} ${pro_dns} ${pro_link} 
