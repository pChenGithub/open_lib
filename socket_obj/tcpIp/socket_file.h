#pragma once
#include "tcpIp_errno.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

// 发送和接收文件
int sock_send_file(SOCKET socketfd, const char* file);
int sock_recv_file(SOCKET socketfd, const char* file);
// 发送和接收

