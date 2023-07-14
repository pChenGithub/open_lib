# YT系列压缩更新包脚本标准
#! /bin/sh

# 获取版本号
version=$1
# 当前目录
PWD=`pwd`

# 脚本配置项
# 版本号格式
VERSION_FORMAT=4.0.21.0929
# 压缩目录
RAR_DIR=data-rar
# 设备类型/运行目录/bin文件
DEVICE_TYPE=yt327L_zytk_dctr
# app文件名称
DEVICE_APP=zytk_pdctr
# 工程目录
DEVICE_DIR=zytk_pdctr
# 输出压缩文件
RAR_OUT=yt327L_dctr40_update_${version}.tar.gz
# 交叉编译工具
GCC_HEAD=arm-linux-gnueabihf
# gcc路径
GCC_DIR=/home/zhh/work/yt328_reader/020.源代码/toolchain/gcc-linaro-5.5.0-2017.10-x86_64_arm-linux-gnueabihf
# 源码路径
SOURCE_CODE_DIR=../zytk_dctr_yt327
# 脚本配置项 end


echo "${PWD}"
echo "打包${RAR_DIR}工作目录文件..."

# 模糊匹配,./开头
if [[ "x$0" == x./* ]];then
	if [ "x${version}" = "x" ];then
		echo "没有带版本号，例如 $0 ${VERSION_FORMAT}"
		exit 0
	fi

	${GCC_HEAD}-gcc -dumpversion
	#echo "xxx $?"
	if [ "x0" != "x$?" ];then
		echo "导入环境变量..."
		#echo "${GCC_DIR}"
		export PATH=${PATH}:${GCC_DIR}/bin
	fi

	# 拷贝执行文件
	cp -vf ${SOURCE_CODE_DIR}/${DEVICE_APP} ${RAR_DIR}/${DEVICE_DIR}/app/${DEVICE_APP}
	cd ${RAR_DIR}

	# 修改版本文件
	echo -e "[SoftVersion]\n${version}\n[SoftType]\n${DEVICE_TYPE}" > ./${DEVICE_DIR}/app/VersionInfo.ini
	${GCC_HEAD}-strip ./${DEVICE_DIR}/app/${DEVICE_APP}

	tar zcf ../${RAR_OUT} * 

	cd -

	echo "${RAR_OUT} 压缩完成"
else
	echo "只支持使用./xxx.sh方式执行脚本"
fi



