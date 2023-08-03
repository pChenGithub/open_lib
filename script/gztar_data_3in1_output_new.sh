#! /bin/bash

# 环境变量
#appversion=$1
# 压缩目录
RAR_DIR=data_3in1_output
# 输出压缩包命名
RAR_OUT=usr3in1.tar.gz
# 设备名称
DEV_NAEM=yt328
# 打包工程定义
# "工程描述文字 工程目录名称 app执行文件名称 安装目录名称 工程路径 固件版本号文件(相对工程目录)"
app_def=()
app_def[0]="读卡器reader ZYTK_Reader_YT328_R zytk_reader zytk_reader /home/zhh/work/yt328_reader/020.源代码"
app_def[1]="40一体机(zytk) zytk40_yt328_dctr_new zytk_dctr zytk_dctr /home/zhh/work/ytdctr328/YT328门禁一体机 global.h" 
app_def[2]="易通一体机(zyep) zyep_yt328_dctr_new zyep_dctr zyep_dctr /home/zhh/work/ytdctr328/02_YT328门禁一体机 global.h" 
app_def[3]="易通云一体机(etc) zyetc_yt328_dctr zyetc_dctr zyetc_dctr /home/zhh/work/ytdctr328/YT328一体机"
# 变量定义结束

############################################################################
# 下面为处理业务,不修改内容
echo "打包原始应用(3合1)工作目录文件..."
arm-linux-gnueabihf-gcc -dumpversion
#echo "xxx $?"
if [ "x0" != "x$?" ];then
	echo "导入环境变量..."
	PWD=`pwd`
	echo "${PWD}"
	export PATH=${PATH}:${PWD}/../toolchain/gcc-linaro-5.5.0-2017.10-x86_64_arm-linux-gnueabihf/bin
fi

#if [ "x${appversion}" = "x" ];then
#	echo "没有带版本号，例如 ./gztar_data_3in1_output.sh 21.0929"
#	exit 0
#fi

cd ${RAR_DIR}
#echo "---> ${app_def[0]}"
#echo "---> ${app_def[@]}"
echo "---> ${#app_def[@]}"

for ((i = 0; i < ${#app_def[@]}; i++))
do
	item=(${app_def[i]})
	echo ""
	echo "################## ${item[0]} ######################"
	echo "是否跳过${item[0]}? y/yes/n/no?"
	read inputval

	if [ x"n" = x"$inputval" ];then
		# 开始打包过程
		echo "打包 ${item[0]}"
		# 获取版本号
		vv=`cat ${item[4]}/${item[1]}/${item[5]} | grep APP_VERSION | awk '{print $3}' | sed 's/\"//g'`
		echo "版本号: $vv"
		if [ "x${vv}" = "x" ];then
			echo "没有获取到版本号,失败"
			exit 0
		fi
	
		#echo "写 [SoftVersion]\n${vv}\n[SoftType]\n{$DEV_NAEM}_${item[2]} 到 ./${item[3]}/app/VersionInfo.ini"
		echo -e "[SoftVersion]\n${vv}\n[SoftType]\n${DEV_NAEM}_${item[2]}" > ./${item[3]}/app/VersionInfo.ini
		cp -vf ${item[4]}/${item[1]}/${item[2]} ${item[3]}/app/${item[2]}
		#echo "strip执行文件 ./${item[3]}/app/${item[2]}"
		arm-linux-gnueabihf-strip ./${item[3]}/app/${item[2]}
		echo "发布: ${item[0]},版本号: $vv"
	else
		echo "不发布新的 ${item[0]} 的可执行文件,跳过!"
	fi
	echo "################## ${item[0]} ######################"
	echo ""
done

# 压缩文件
tar zcf ../${RAR_OUT} *

# 还原路径
cd -
echo "${RAR_OUT} 压缩完成"
exit 0



