#! /bin/bash
PWD_DIR=`pwd`
OBJ_DIR=$1
OUT_DIR=$PWD_DIR/out_arm

if [ x"source" = x"$0" ];then
	echo "refuse source, please ./$0"
	exit 0
fi

if [ x"" = x"$OBJ_DIR" ];then
	echo "git a obj dir, ./$0 <obj dir>"
	exit 0
fi

cd $OBJ_DIR
echo "dir in:`pwd`"
echo "build xproto"

# arm-linux-gnueabihf-gcc
./configure CC=arm-linux-gnueabihf-gcc --host=arm-linux --prefix=$OUT_DIR CFLAGS=-I$OUT_DIR/include PKG_CONFIG_PATH=$OUT_DIR/lib/pkgconfig
#./configure CC=arm-linux-gnueabihf-gcc --host=arm-linux --prefix=$OUT_DIR

make -j4 && make install 

cd -

echo "done"

