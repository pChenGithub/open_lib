#include "mqttClient.h"
#include <stdio.h>
#include <unistd.h>

#if 1
#define SERVERADDR "tcp://etc.hzlinks.cn:1883"
#define CLIENTID    "ZYZH-YT213Y-YTWXFJ-1|64694EC1C73D_temp"
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
    ret = createMqttclient(&client, SERVERADDR, CLIENTID, "64694EC1C73D", "ae2d70f1272f462fa2bba77bcbfb359a");
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
