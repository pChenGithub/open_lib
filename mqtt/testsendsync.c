#include "mqttClient.h"

#if 0
#define SERVERADDR "tcp://172.16.4.180:30105"
#define CLIENTID    "12346"
#else
#define SERVERADDR "tcp://172.16.70.182:1883"
#define CLIENTID    "12346"
#endif

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

    while (1)
    {
        pushOneMessage(client, "1234", "3456");
        sleep(3);
    }

    int destroyMqttclient(client);
    return 0;
}
