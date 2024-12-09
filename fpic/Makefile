outlib_c += share_fuc.c
outlib_o += $(patsubst %.c, %.o, ${outlib_c})
lib_out += libshare_fuc.so

test_c += test_fpic.c
test_bin += test_fpic

CC = /home/zhh/work/yt328_reader/020.源代码/toolchain/gcc-linaro-5.5.0-2017.10-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc

all: lib lib2 test

test: ${test_bin}

${test_bin}: test_fpic.c
	${CC} $^ -o $@  -L. -lshare_fuc2 -L./libshare_x86 -lshare_fuc -fpic

#test_fpic.o: test_fpic.c
#	${CC} $^ -o $@  -L. -lshare_fuc2 -lshare_fuc -fpic


lib: $(lib_out)

${lib_out}: share_fuc.o
	${CC} -shared $^ -o $@

lib2: libshare_fuc2.so

libshare_fuc2.so: share_fuc2.o
	${CC} -shared $^ -o $@

share_fuc2.o: share_fuc2.c
	${CC} $^ -c  -o $@  -lshare_fuc

%.o: %.c
	${CC} $^ -c  -o $@ -fpic

clean: cleanlib cleanlib2 cleantest

cleanlib:
	rm  share_fuc.o  ${lib_out}

cleanlib2:
	rm  share_fuc2.o  libshare_fuc2.so

cleantest:
	rm   ${test_bin}
