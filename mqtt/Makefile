target+=testrecv testsend testreconnect
#CC=/home/zhh/work/gcc-linaro-5.5.0-2017.10-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc
#MQTT_LIB+=build_arm4.7
MQTT_LIB+=build_x86


all: ${target}
#${DEBUGER} ./${target}

testrecv : testrecvsync.c mqttPahocClient.c
	${CC} $^ -o $@ -L./${MQTT_LIB}/src -lpaho-mqtt3c -lpthread

testsend : testsendsync.c mqttPahocClient.c
	${CC} $^ -o $@ -L./${MQTT_LIB}/src -lpaho-mqtt3c -lpthread

testreconnect : testreconnect.c mqttPahocClient.c
	${CC} $^ -o $@ -L./${MQTT_LIB}/src -lpaho-mqtt3c -lpthread

clean:
	rm ${target}
