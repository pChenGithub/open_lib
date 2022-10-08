#include "mqttClient.h"
#include "src/MQTTClient.h"
#include <stdio.h>

/**
 * @brief
 * 创建一个mqtt客户端,完成建立连接,设置回调
 * client:  返回创建的 MQClient
 */
int createMqttclient(MQClient **client)
{
    if (NULL == client)
        return -MQERR_CHECK_PARAM;
    MQClient *pclient = (MQClient *)malloc(sizeof(MQClient));
    if (NULL == pclient)
        return -MQERR_MALLOC;

    if (MQTTCLIENT_SUCCESS != MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL))
        return -1;

    return 0;
}