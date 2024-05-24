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
echo "build X11"

echo enable_malloc0returnsnull=yes>arm-linux.cache
# arm-linux-gnueabihf-gcc
#./configure CC=arm-linux-gnueabihf-gcc --host=arm-linux --prefix=$OUT_DIR --includedir=$OUT_DIR/include --libdir=$OUT_DIR/lib --cache-file=arm-linux.cache
./configure CC=arm-linux-gnueabihf-gcc --host=arm-linux --prefix=$OUT_DIR CFLAGS=-I$OUT_DIR/include LDFLAGS=-L$OUT_DIR/lib --cache-file=arm-linux.cache

make -j4 && make install 

cd -

echo "done"

