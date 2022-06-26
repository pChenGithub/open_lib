#ifndef __FILEXOPT_H__
#define __FILEXOPT_H__

#include <sys/types.h>

#define FILEOPT_SUCUSSE         0
#define FILEOPT_FAILED_CREAT    1
#define FILEOPT_FAILED_REMOVE   2
#define FILEOPT_FAILED_RNAME    3
#define FILEOPT_FAILED_LINK     4

#define FILECHK_EXIST           0
#define FILECHK_NOTEXIST        1
/** 
 * 文件操作:
 * 1、创建文件
 * 2、删除文件
 * 3、重命名文件
 * 4、移动文件
 * 5、创建软链接
 * 6、判断文件是否存在
 * 文件夹操作：
 * 1、创建
 * 2、删除
 * 3、重命名
 * 4、移动
 * 5、创建软链接
 * 6、判断是否存在
 * 
 * */
// 创建文件
int file_create(const char* file, const mode_t mode);
// 删除文件
int file_remove(const char* file);
// 文件重命名
int file_rename(const char* oldname, const char* newname);
// 文件移动(文件重命名？？？)
int file_move(const char* file, const char* olddir, const char* newdir);
// 创建软链接
int file_link(const char* from, const char* to);
// 判断文件是否存在
int file_exist(const char* file);

#endif



