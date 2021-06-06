#! /bin/sh

echo "build curl plantform x86"

if [ -d build_x86 ];then
    echo "build dir exist"
else
    mkdir build_x86
fi

cd build_x86 

cmake ../curl -DCMAKE_INSTALL_PREFIX=../out/x86

make -j8 && make install

cd -
