#pragma once
// 命令
#define CMD_GET_INFO    1

//
int fill_sendInfo();
int parser_getInfo(const char* jsonstr, unsigned char* cmd);
