# YT系列压缩初始包脚本标准
#! /bin/sh

# 获取版本号
version=$1
# 当前目录
PWD=`pwd`

# 脚本配置项
# 压缩目录
RAR_DIR=output_initpkg
# 设备类型/运行目录/bin文件
DEVICE_TYPE=zyep_smartdc
# app文件名称
DEVICE_APP=zyep_smartdc
# 工程目录
DEVICE_DIR=zyep_smartdc
# 输出压缩文件
RAR_OUT=usr.tar.gz
# 交叉编译工具
GCC_HEAD=arm-linux-gnueabihf
# gcc路径
GCC_DIR=${PWD}/../gcc-linaro-arm-linux-gnueabihf-4.7-2013.03-20130313_linux
# 脚本配置项 end


echo "${PWD}"
echo "打包${RAR_DIR}工作目录文件..."

# 模糊匹配,./开头
if [[ "x$0" == x./* ]];then
	if [ "x${version}" = "x" ];then
		echo "没有带版本号，例如 $0 5.1.21.0929"
		sleep 5
		exit 0
	fi

	${GCC_HEAD}-gcc -dumpversion
	#echo "xxx $?"
	if [ "x0" != "x$?" ];then
		echo "导入环境变量..."
		#echo "${GCC_DIR}"
		export PATH=${PATH}:${GCC_DIR}/bin
	fi

	cd ${RAR_DIR}

	# 修改版本文件
	echo "[SoftVersion]\n${version}\n[SoftType]\n${DEVICE_TYPE}" > ./${DEVICE_DIR}/app/VersionInfo.ini
	${GCC_HEAD}-strip ./${DEVICE_DIR}/app/${DEVICE_APP}

	tar zcf ../${RAR_OUT} * 

	cd -

	echo "${RAR_OUT} 压缩完成"
else
	echo "只支持使用./xxx.sh方式执行脚本"
fi



