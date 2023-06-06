#! /bin/sh

PWD=`pwd`
GCC_PRE=/home/zhh/work/yt312/02_YT312门禁控制器/gcc-linaro-arm-linux-gnueabihf-4.7-2013.03-20130313_linux/bin/arm-linux-gnueabihf-
echo "build ..."
PWD=`pwd`
./configure --enable-cross-compile --prefix=$PWD/../out_arm \
	--arch=arm --target-os=linux --enable-shared \
	--cross-prefix=$GCC_PRE \
	--enable-gpl \
	--extra-cflags=-I/home/cp/out_arm/include \
	--extra-ldflags=-L/home/cp/out_arm/lib \
	#--enable-libx264 \

make -j4 && make install
 

