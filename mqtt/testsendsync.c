#include "mqttClient.h"
#include <stdio.h>
#include <unistd.h>

#if 0
#define SERVERADDR "tcp://172.16.4.180:30105"
#define CLIENTID    "ZYZH-YT325-MJJCR-1|704A0ECA08CC_temp"
//#define CLIENTID    "_temp123"
#else

#define SERVERADDR "tcp://172.16.70.182:1883"
#define CLIENTID    "12346"
#endif

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

    while (1)
    {
        pushOneMessage(client, "/sys/ZYZH-YT325-MJJCR-1/704A0ECA08CC/device/rrpc/connect/response", "3456", RETAINED_ON);
        sleep(3);
    }

    destroyMqttclient(client);
    return 0;
}
