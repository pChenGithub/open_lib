# !/usr/bin/env python3
# -*- coding:utf-8 -*-

# 批量测试端口号
import sys
import telnetlib


def telnet(host, port):
    """
    测试端口号通不通
    :return:
    """
    try:
    	#  timeout单位s
        telnetlib.Telnet(host=host, port=port, timeout=2)
        print(f"{host} {port}  端口 通")
    except:
        print(f"{host} {port}  端口 不通")
        # 或什么都不打印
        # pass


ips = [
"172.31.6.100", 
"172.31.6.101",
"172.31.6.102",
"172.31.6.103",
"172.31.6.104",
"172.31.6.105",
"172.31.6.106",
"172.31.6.107",
"172.31.6.108",
"192.168.1.130"]


for ip in ips:
	telnet(ip, 23)


