#! /bin/bash
echo "build yt328 for arm"
PWD=`pwd`
echo "当前目录:${PWD}"
export PATH=/usr/bin:/usr/bin:/bin:/usr/local/games:/usr/games:${PWD}/toolchain/gcc-linaro-5.5.0-2017.10-x86_64_arm-linux-gnueabihf/bin

# 初始化参数
EXESHELL_FILE=$0
SOURCE_DIR=$1

# 项目个性化的一些变量,需要检查不为空
# 默认指定的工程目录
DEFAULT_SOURCE=zyep_dctr_yt327
# pro文件
PRO_FILE=yt327L.pro
# 版本号文件,相对工程路径
VER_FILE=global.h
# 版本号字段
VER_STR=APP_VERSION

#######################################################
# 以下为不修改部分,业务实现
# 检查环境
echo "check qt && toolchain"
if [ -d "toolchain" ];then
	echo "has toolchain"
else
	echo "exe tar toolchain"
	tar xvf toolchain.tar.gz
fi

if [ -d "qt4.8.7" ];then
	echo "has qt4.8.7"
else
	echo "exe tar qt"
	tar xvf qt4.8.7.tar.gz
fi

echo "check input args"
if [ x"" = "x$SOURCE_DIR" ];then
	echo "未指定源码目录,例如$EXESHELL_FILE $DEFAULT_SOURCE"
	exit
fi

# 清理工程
echo "是否清理工程:输入y/n"
read DO_CLEAN
if [ x"y" = "x$DO_CLEAN" ];then
	echo "清理工程:$SOURCE_DIR ..."
	cd $SOURCE_DIR
	make clean
	cd -
else
	echo "跳过清理"
fi

# 编译工程
cd $SOURCE_DIR
echo `pwd`
echo "build qt ..."
../qt4.8.7/bin/qmake $PRO_FILE
make -j8
echo "build qt done"
ls
# 打印应用程序版本号
cat $VER_FILE | grep $VER_STR
cd -



