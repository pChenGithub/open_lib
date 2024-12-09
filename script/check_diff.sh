# 检查工程是否有更新的脚本
# 根据自己的工程管理工具修改命令,比如snv diff,git diff

#! /bin/bash

# 配置需要检查的项目
app_def=()
# 工程别名 工程目录名 工程路径
# 328
app_def[0]="328读卡器reader ZYTK_Reader_YT328_R /home/zhh/work/yt328_reader/020.源代码"
app_def[1]="32840一体机(zytk) zytk40_yt328_dctr_new /home/zhh/work/ytdctr328/YT328门禁一体机" 
app_def[2]="328易通一体机(zyep) zyep_yt328_dctr_new /home/zhh/work/ytdctr328/02_YT328门禁一体机" 
app_def[3]="328易通云一体机(etc) zyetc_yt328_dctr /home/zhh/work/ytdctr328/YT328一体机"
# 327L
app_def[4]="327L40一体机(zytk) zytk_dctr_yt327 /home/zhh/work/ytdctr327/YT327L门禁一体机" 
app_def[5]="327L易通一体机(zyep) zyep_dctr_yt327 /home/zhh/work/ytdctr327/02_YT327L门禁一体机" 
app_def[6]="327L易通云一体机(etc) zyetc_yt327L_dctr /home/zhh/work/ytdctr327/YT327L一体机" 
# 检查版本差别的命令
DO_DIFF="svn diff"
#DO_DIFF="git diff"

############################################################################
# 下面为处理业务,不修改内容
echo "检查项目的修改情况"
for ((i = 0; i < ${#app_def[@]}; i++))
do
	item=(${app_def[i]})
	echo ""
	echo "################## ${item[0]} ######################"
	# 进入工程目录,执行diff
	cd ${item[2]}/${item[1]}
	echo `pwd`
	ret=`$DO_DIFF`
	#echo "diff结果: $ret"
	if [ "x" = "x$ret" ];then
		echo "${item[0]} 全部提交,没有更新"
	else
		echo "${item[0]} 有修改,请检查"
		echo "是否查看修改内容(y/Y/n/N):"
		read input
		if [ "xy" = "x$input" ];then
			echo $ret
		fi
	fi
done



