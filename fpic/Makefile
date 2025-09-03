
CC = /home/cp/tmp/toolchain/gcc-linaro-5.5.0-2017.10-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc

all: lib lib2 test

test: test_fpic

test_fpic: test_fpic.c
	${CC} $^ -o $@  -L. -lshare_fuc2

#test_fpic.o: test_fpic.c
#	${CC} $^ -o $@  -L. -lshare_fuc2 -lshare_fuc -fpic

# -fPIC/-fpic 
%.o: %.c
	${CC} -c -o $@ $< -fPIC 

# lib1
lib: libshare_fuc.so

libshare_fuc.so: share_fuc.o
	${CC} -shared $^ -o $@ -fPIC 

# lib2
lib2: libshare_fuc2.so

libshare_fuc2.so: share_fuc2.o
	${CC} -shared $^ -o $@ -fPIC 

PHONY: clean
clean: cleanlib cleanlib2 cleantest

PHONY: cleanlib
cleanlib:
	rm  share_fuc.o  libshare_fuc.so

PHONY: cleanlib2
cleanlib2:
	rm  share_fuc2.o  libshare_fuc2.so

PHONY: cleantest
cleantest:
	rm   test_fpic
