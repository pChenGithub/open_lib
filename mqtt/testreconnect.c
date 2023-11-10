#include "mqttClient.h"
#include <stdio.h>
#include <unistd.h>

#if 1
#define SERVERADDR "etc.hzlinks.cn:1883"
#define CLIENTID    "ZYZH-YT325Y-MJJ-2|3AEA7B65519G_temp"
#define USER        "3AEA7B65519G"
#define PASS        "be5b39c2cf9e2825c7cc2e5967aae8fa"
//#define CLIENTID    "_temp123"
#else
#define SERVERADDR "tcp://172.16.70.182:1883"
#define CLIENTID    "12345"
#endif

int msgrecv(const char* topic, const char* payload, const int payloadlen) {

}

static MQClient* client;
static char mqttlost = 1;
int lostconnect(const char *cause)
{
    int ret = 0;
    // 销毁
    destroyMqttclient(client);
    // 重连
    mqttlost = 1;
    return 0;
}

static int buildConnect() {
    int ret = 0;

    /* code */
    ret = createMqttclient(&client, SERVERADDR, CLIENTID, USER, PASS);
    if (ret<0)
    {
        printf("创建mqtt客户端失败\n");
        return -1;
    }

    mqttlost = 0;
    setCallBack(client, msgrecv, lostconnect, NULL);
    ret = subscribeTopic(client, "/sys/ZYZH-YT327L-MJJ-2/3AEA7B65519G/device/rrpc/connect/response", 1);
    if (ret<0)
    {
        printf("mqtt客户端订阅失败\n");
        return -1;
    }

    
}

int main(int argc, char const *argv[])
{
    int ret = 0;

    while (1) {
        if (mqttlost)
            buildConnect();

        sleep(10);
    }

    destroyMqttclient(client);
    return 0;
}

