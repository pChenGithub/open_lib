target+=testrecv testsend testreconnect
CC=/home/zhh/work/gcc-linaro-5.5.0-2017.10-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc


all: ${target}
#${DEBUGER} ./${target}

testrecv : testrecvsync.c mqttPahocClient.c
	${CC} $^ -o $@ -L./build_arm/src -lpaho-mqtt3c -lpthread

testsend : testsendsync.c mqttPahocClient.c
	${CC} $^ -o $@ -L./build_arm/src -lpaho-mqtt3c -lpthread

testreconnect : testreconnect.c mqttPahocClient.c
	${CC} $^ -o $@ -L./build_arm/src -lpaho-mqtt3c -lpthread

clean:
	rm ${target}