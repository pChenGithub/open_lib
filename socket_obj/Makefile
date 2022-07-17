#CC = /home/zhh/work/YT312/02_YT312门禁控制器/gcc-linaro-arm-linux-gnueabihf-4.7-2013.03-20130313_linux/bin/arm-linux-gnueabihf-gcc
#CC = /home/zhh/work/YT312/02_YT312门禁控制器/gcc-linaro-arm-linux-gnueabihf-4.7-2013.03-20130313_linux/bin/arm-linux-gnueabihf-gcc
pro ?= test
pro_nl ?= test_nl
pro_gate ?= test_gate
pro_dns ?= test_dns
pro_link ?= test_link
pro_dhcp ?= test_dhcp

#ARCH=arm
#CFLAGS += -L/home/zhh/work/YT312/02_YT312门禁控制器/gcc-linaro-arm-linux-gnueabihf-4.7-2013.03-20130313_linux/arm-linux-gnueabihf/libc/lib/arm-linux-gnueabihf -lpthread
#CFLAGS += -lpthread
CFLAGS += -I../exeShell_obj/

SOURCE_FILE += link/net_link.c link/net_opt.c link/net_netlink.c link/net_dns.c link/net_dhcp.c ../exeShell_obj/mixShell.c


all: ${pro} ${pro_nl} ${pro_gate} ${pro_dns} ${pro_link} ${pro_dhcp} 
	$(DEBUGGER) ./${pro_gate} ${ARGS}

${pro}: test.c ${SOURCE_FILE} -lpthread

${pro_nl} : test_nl.c ${SOURCE_FILE} -lpthread

${pro_gate} : test_gate.c ${SOURCE_FILE} -lpthread

${pro_dns} : test_dns.c ${SOURCE_FILE} -lpthread

${pro_link} : test_link.c ${SOURCE_FILE} -lpthread

${pro_dhcp} : test_dhcp.c ${SOURCE_FILE} -lpthread

clean:
	rm ${pro} ${pro_nl} ${pro_gate} ${pro_dns} ${pro_link} ${pro_dhcp} 
