pro_scan_recv ?= scan_recv
pro_scan_send ?= scan_send

#CC = /home/zhh/work/YT312/02_YT312门禁控制器/gcc-linaro-arm-linux-gnueabihf-4.7-2013.03-20130313_linux/bin/arm-linux-gnueabihf-gcc
#CC = /home/zhh/work/YT312/02_YT312门禁控制器/gcc-linaro-arm-linux-gnueabihf-4.7-2013.03-20130313_linux/bin/arm-linux-gnueabihf-gcc
CC = /home/zhh/work/yt328_reader/020.源代码/toolchain/gcc-linaro-5.5.0-2017.10-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc
#ARCH=arm
#CFLAGS += -L/home/zhh/work/YT312/02_YT312门禁控制器/gcc-linaro-arm-linux-gnueabihf-4.7-2013.03-20130313_linux/arm-linux-gnueabihf/libc/lib/arm-linux-gnueabihf -lpthread
CFLAGS += -L/home/zhh/work/yt328_reader/020.源代码/toolchain/gcc-linaro-5.5.0-2017.10-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/lib -lpthread
# x86
#LINK_LIBS += -lpthread


CFLAGS += -I../exeShell_obj/ -I../socket_obj -I../misc -I../obj_cstring
all: ${pro_scan_recv} ${pro_scan_send} 
#	$(DEBUGGER) ./${pro_multicast_recv} ${ARGS}

#
TCPIP_SOURCE += ../socket_obj/tcpIp/multicast.c protocol/device_prot.c ../json_parse/jsonlibC/cJSON.c ../socket_obj/link/net_opt.c ../socket_obj/link/net_netlink.c ../misc/mac_opt.c ../obj_cstring/string_opt.c
${pro_scan_recv} :  ${TCPIP_SOURCE} ${LINK_LIBS}

${pro_scan_send} :  ${TCPIP_SOURCE} ${LINK_LIBS}

clean:
	rm ${pro_scan_recv} ${pro_scan_send}

install:
	cp ${pro_scan_recv} ${pro_scan_send} ~/nfs
