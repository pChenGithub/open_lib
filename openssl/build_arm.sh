#! /bin/sh
CC=arm-linux-gnueabihf-gcc ./config no-asm -shared --prefix=/home/zhh/work/open_lib/openssl/openssl-openssl-3.0.6/build_328arm
./config no-asm no-async -shared --cross-compile-prefix=arm-linux-gnueabihf- --prefix=/home/zhh/work/open_lib/openssl/openssl-openssl-3.0.6/build_328arm
