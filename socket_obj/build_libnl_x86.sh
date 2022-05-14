#! /bin/sh

echo "build libnl plantform x86"
echo "$PWD"
if [ -d build_x86 ];then
    echo "build dir exist"
else
    mkdir build_x86
fi

cd build_x86 

../libnl-3.5.0/configure --prefix=$PWD/../out/x86

make -j8 && make install

cd -
