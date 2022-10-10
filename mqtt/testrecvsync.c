#include "mqttClient.h"
#include <stdio.h>

#if 0
#define SERVERADDR "tcp://172.16.4.180:30105"
#define CLIENTID    "12345"
#else
#define SERVERADDR "tcp://172.16.70.182:1883"
#define CLIENTID    "12345"
#endif

int msgrecv(const char* topic, const char* payload, const int payloadlen) {

}

int lostconnect(const char* cause) {

}

int main(int argc, char const *argv[])
{
    int ret = 0;
    MQClient* client;
    /* code */
    ret = createMqttclient(&client, SERVERADDR, CLIENTID);
    if (ret<0)
    {
        printf("创建mqtt客户端失败\n");
        return -1;
    }

    setCallBack(client, msgrecv, lostconnect, NULL);
    subscribeTopic(client, "1234", 1);
    subscribeTopic(client, "hello", 1);

    while (1)
        sleep(100);

    int destroyMqttclient(client);
    return 0;
}
