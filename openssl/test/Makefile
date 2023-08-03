
target +=  testmd5 rsatest SM2test AesTest
PWD = $(shell pwd)
CRYPTO=${PWD}/../crypto
CFLAGS += -std=c11 -I${CRYPTO} -I${PWD}/../build_x86/include -L${PWD}/../build_x86/lib -lcrypto -lssl


all: ${target}

SM2test: SM2test.c ${CRYPTO}/SM2opt.c
	${CC} $^ -o $@ ${CFLAGS}

testmd5: testmd5.c ${CRYPTO}/Hashopt.c
	${CC} $^ -o $@ ${CFLAGS}

rsatest: testrsa.c ${CRYPTO}/Rsaopt.c
	${CC} $^ -o $@ ${CFLAGS}

AesTest: AesTest.c
	${CC} $^ -o $@ ${CFLAGS}

%.o: %.c


clean:
	rm *.o ${target}