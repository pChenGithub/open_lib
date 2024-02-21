#include "socket_file.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>

#define FFR_CODE_OK 0
#define FFR_CODE_ERR_SIZE 1 // 文件大小信息异常
#define FFR_CODE_ERR_INDEX 2 // 文件帧序号异常

typedef struct {
    long filesize;
    int framecount;
    int laseframesize;
    int index;
    int code;
} FILE_FRAME_RSP;

#define FRAME_BUFF_SIZE 4
typedef struct {
    long filesize;
    int framecount;
    int laseframesize;
    // 变量
    int index;
    unsigned char buff[FRAME_BUFF_SIZE];
} FILE_FRAME;

int sock_send_file(SOCKET socketfd, const char* file) {
    int ret = 0;
    if (NULL==file)
        return -TCPIPERR_CHECK_PARAM;

    // 打开文件，统计大小，
    int fd = open(file, O_RDWR);
    if (fd<0)
        return -TCPIPERR_OPENFILE;

    struct stat file_stat;
    ret = fstat(fd, &file_stat);
    if (ret<0) {
        ret = -TCPIPERR_FSTAT;
        goto close_exit;
    }

    // 准备发送帧
    FILE_FRAME* fframe = calloc(1, sizeof(FILE_FRAME));
    fframe->filesize = file_stat.st_size;
    fframe->framecount = fframe->filesize/FRAME_BUFF_SIZE;
    fframe->laseframesize = fframe->filesize % FRAME_BUFF_SIZE;
    if (fframe->laseframesize)
        fframe->framecount++;

    int sendsize = sizeof(FILE_FRAME);
    // 准备读取文件，发送
    while((ret=read(fd, fframe->buff, FRAME_BUFF_SIZE))>0) {
        if (FRAME_BUFF_SIZE!=ret)
            sendsize = fframe->buff-(unsigned char*)fframe + ret;
        ret = send(socketfd, (const char*)fframe, sendsize, 0);
        if (ret<0) {
            ret = -TCPIPERR_SEND_DATA;
            goto free_exit;
        }

#if 1
        // 等待回复，设置超时
        FILE_FRAME_RSP rsp;
        ret = recv(socketfd, (char*)(&rsp), sizeof(FILE_FRAME_RSP), 0);
        if (ret<0) {
            ret = -TCPIPERR_RECV_DATA;
            goto free_exit;
        }
#endif

        // 校验回复
        if (fframe->filesize!=rsp.filesize || fframe->framecount!=rsp.framecount
                || fframe->laseframesize!=rsp.laseframesize) {
            ret = -TCPIPERR_RSP;
            goto free_exit;
        }

        if (rsp.index!=fframe->index) {
            ret = -TCPIPERR_RSP_INDEX;
            goto free_exit;
        }

        if (FFR_CODE_OK!=rsp.code) {
            ret = -TCPIPERR_RSP_CODE;
            goto free_exit;
        }

        // 发送的index从0开始
        fframe->index ++;
    }

    if (ret<0) {
        // 读取文件失败
        ret = -TCPIPERR_READ_FILE;
    }

    // 发送文件完成
free_exit:
    free(fframe);
close_exit:
    close(fd);
    return ret;
}

int sock_recv_file(SOCKET socketfd, const char* file) {
    int ret = 0;
    if (NULL==file)
        return -TCPIPERR_CHECK_PARAM;

    // 打开文件
    int fd = open(file, O_RDWR);
    if (fd<0)
        return -TCPIPERR_OPENFILE;

    // 准备接收帧
    FILE_FRAME* fframe = calloc(1, sizeof(FILE_FRAME));
    FILE_FRAME_RSP rsp = {
        // 在接到第一帧数据之前，前面3个参数未知，index为0，期待接到序号为0的帧
        0,0,0,0,FFR_CODE_OK,
    };
    // 等待数据，超时？？
    do {
        ret = recv(socketfd, (char*)fframe, sizeof(FILE_FRAME), 0);
        if (ret<=0) {
            ret = -TCPIPERR_RECV_DATA;
            goto free_exit;
        }

        // index为0的帧初始化参数
        if (0==fframe->index) {
            rsp.filesize = fframe->filesize;
            rsp.framecount = fframe->framecount;
            rsp.laseframesize = fframe->laseframesize;
        }

        // 校验帧
        if (fframe->filesize!=rsp.filesize || fframe->framecount!=rsp.framecount
                || fframe->laseframesize!=rsp.laseframesize) {
            rsp.code = FFR_CODE_ERR_SIZE;
            send(socketfd, (const char*)&rsp, sizeof(FILE_FRAME_RSP), 0);

            ret = -TCPIPERR_RSP;
            goto free_exit;
        }

        if (rsp.index!=fframe->index) {
            rsp.code = FFR_CODE_ERR_INDEX;
            send(socketfd, (const char*)&rsp, sizeof(FILE_FRAME_RSP), 0);

            ret = -TCPIPERR_RSP_INDEX;
            goto free_exit;
        }

        // 回复
        ret = send(socketfd, (const char*)&rsp, sizeof(FILE_FRAME_RSP), 0);
        if (ret<0) {
            ret = -TCPIPERR_SEND_DATA;
            goto free_exit;
        }
        // 期待下一帧
        rsp.index ++;
    } while(1);

free_exit:
    free(fframe);
//close_exit:
    close(fd);
    return ret;
}
