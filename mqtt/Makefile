target+=testrecv testsend


all: ${target}
#${DEBUGER} ./${target}

testrecv : testrecvsync.c mqttPahocClient.c
	${CC} $^ -o $@ -L./build_x86/src -lpaho-mqtt3c -lpthread

testsend : testsendsync.c mqttPahocClient.c
	${CC} $^ -o $@ -L./build_x86/src -lpaho-mqtt3c -lpthread

clean:
	rm ${target}