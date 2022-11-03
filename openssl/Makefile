
target += SM2test
PWD = /home/zhh/work/open_lib/openssl

all: ${target}

SM2test: SM2test.c SM2opt.c
	${CC} -std=c11 -I ${PWD}/build_x86/include -L${PWD}/build_x86/lib64 -lcrypto -lssl $^ -o $@

%.o: %.c