#include "fileXOpt.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

// 创建文件
int file_create(const char* file, const mode_t mode) {
    assert(NULL!=file);

    if (-1==creat(file, mode)) {
        return FILEOPT_FAILED_CREAT;
    }
    return FILEOPT_SUCUSSE;
}

// 删除文件
int file_remove(const char* file) {
    assert(NULL!=file);

    if (-1==remove(file)) {
        return FILEOPT_FAILED_REMOVE;
    }
    return FILEOPT_SUCUSSE;
}

// 文件重命名
int file_rename(const char* oldname, const char* newname) {
    assert(NULL!=oldname && NULL!=newname);

    if (-1==rename(oldname, newname)) {
        return FILEOPT_FAILED_RNAME;
    }
    return FILEOPT_SUCUSSE;
}

// 文件移动
int file_move(const char* file, const char* olddir, const char* newdir) {
    assert(NULL!=file && NULL!=olddir && NULL!=newdir);

    rename(olddir"/"file, newdir"/"file);
    return FILEOPT_SUCUSSE;
}

// 创建软链接
int file_link(const char* from, const char* to) {
    assert(NULL!=from && NULL!=to);
    if (-1==link(from, to)) {
        return FILEOPT_FAILED_LINK;
    }
    return FILEOPT_SUCUSSE;
}

// 判断文件是否存在
int file_exist(const char* file) {
    assert(NULL!=file);
    if (-1==access(file, F_OK)) {
        return FILECHK_NOTEXIST;
    }
    return FILECHK_EXIST;
}