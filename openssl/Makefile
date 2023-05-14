
target +=  testmd5 rsatest SM2test
PWD = /home/cp/mywork/open_lib/openssl
CFLAGS += -std=c11 -I${PWD}/build_x86/include -L${PWD}/build_x86/lib -lcrypto -lssl

all: ${target}

SM2test: SM2test.c SM2opt.c
	${CC} $^ -o $@ ${CFLAGS}

testmd5: testmd5.c Hashopt.c
	${CC} $^ -o $@ ${CFLAGS}

rsatest: testrsa.c Rsaopt.c
	${CC} $^ -o $@ ${CFLAGS}

%.o: %.c


clean:
	rm *.o ${target}