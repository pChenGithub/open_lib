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
        print(f"{host} {port}  端口开放")
    except:
        print(f"{host} {port}  端口未开放")
        # 或什么都不打印
        # pass

ip=sys.argv[1]
port=sys.argv[2]
telnet(ip, port)