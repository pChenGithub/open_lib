#include "mqttClient.h"
#include <stdio.h>
#include <unistd.h>

#if 1
#define SERVERADDR "tcp://172.16.4.180:30105"
#define CLIENTID    "ZYZH-YT325-MJJCR-1|704A0ECA08CC_temp1"
//#define CLIENTID    "_temp123"
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
    ret = createMqttclient(&client, SERVERADDR, CLIENTID, "sn", "sec");
    if (ret<0)
    {
        printf("创建mqtt客户端失败\n");
        return -1;
    }

    setCallBack(client, msgrecv, lostconnect, NULL);
    ret = subscribeTopic(client, "/sys/ZYZH-YT325-MJJCR-1/704A0ECA08CC/device/rrpc/connect/response", 1);
    if (ret<0)
    {
        printf("mqtt客户端订阅失败\n");
        return -1;
    }

    while (1)
        sleep(100);

    destroyMqttclient(client);
    return 0;
}
