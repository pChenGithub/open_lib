#include "mqttPahocClient.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * @brief 
 * 
 * @param context 
 * @param topicName 
 * @param topicLen 
 * @param message 
 * @return int 
 * 处理订阅的消息
 */
static int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: %.*s\n", message->payloadlen, (char*)message->payload);
    // 这里调用回调处理消息
    MQClientPahoC *pclient = (MQClientPahoC *)context;
    if (NULL!=pclient->base.msgarrive)
        pclient->base.msgarrive(topicName, message->payload, message->payloadlen);
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

/**
 * @brief 
 * 
 * @param context 
 * @param cause 
 * 处理连接断开的消息
 */
static void connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
    // 这里开始处理连接丢失消息
    MQClientPahoC *pclient = (MQClientPahoC *)context;
    if (NULL!=pclient->base.connectlost)
        pclient->base.connectlost(cause);
}

/**
 * @brief 
 * 
 * @param context 
 * @param dt 
 */
static void delivered(void *context, MQTTClient_deliveryToken dt)
{
    printf("Message with token value %d delivery confirmed\n", dt);
}

/**
 * @brief
 * 创建一个mqtt客户端,完成建立连接,设置回调
 * client:  返回创建的 MQClient
 */
int createMqttclient(MQClient **client, const char *addr, const char *clientid) {
    int ret = 0;
    if (NULL == client || NULL==addr || NULL==clientid)
        return -MQERR_CHECK_PARAM;
    MQClientPahoC *pclient = (MQClientPahoC *)malloc(sizeof(MQClientPahoC));
    if (NULL == pclient)
        return -MQERR_MALLOC;

    pclient->base.type = MQTYPE_PAHOC;
    ret = MQTTClient_create(&(pclient->client), addr, clientid, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    if (MQTTCLIENT_SUCCESS !=ret)
    {
        printf("create ret errno %d\n", ret);
        ret = -MQERR_CLIENT_CREATE;
        goto create_fail;
    }

    // 同步模式下发送完成无需设置回调
    ret = MQTTClient_setCallbacks(pclient->client, pclient, connlost, msgarrvd, delivered);
    if (MQTTCLIENT_SUCCESS!=ret)
    {
        printf("setcallback ret errno %d\n", ret);
        ret = -MQERR_SETCALLBACK;
        goto setback_fail;
    }

    pclient->base.connectlost = NULL;
    pclient->base.msgarrive = NULL;
    pclient->base.sendcomplate = NULL;

    MQTTClient_message* pubmsg = &(pclient->pubmsg);
    // 初始化pubmsg
    // {{'M', 'Q', 'T', 'M'}, 1, 0, NULL, 0, 0, 0, 0, MQTTProperties_initializer};
    // MQTTProperties_initializer {0,0,0,NULL}
    pubmsg->struct_id[0] = 'M';
    pubmsg->struct_id[1] = 'Q';
    pubmsg->struct_id[2] = 'T';
    pubmsg->struct_id[3] = 'M';

    pubmsg->struct_version = 1;
    pubmsg->payloadlen = 0;
    pubmsg->payload = NULL;
    pubmsg->qos = 0;
    pubmsg->retained = 0;
    pubmsg->dup = 0;
    pubmsg->msgid = 0;

    pubmsg->properties.count = 0;
    pubmsg->properties.max_count = 0;
    pubmsg->properties.length = 0;
    pubmsg->properties.array = NULL;

    /** 
     * { {'M', 'Q', 'T', 'C'}, 8, 60, 1, 1, NULL, NULL, NULL, 30, 0, NULL,\
        0, NULL, MQTTVERSION_DEFAULT, {NULL, 0, 0}, {0, NULL}, -1, 0, NULL, NULL, NULL}
    */
    // 初始化conn_opts
    MQTTClient_connectOptions *conn_opts = &(pclient->conn_opts);
    conn_opts->struct_id[0] = 'M';
    conn_opts->struct_id[1] = 'Q';
    conn_opts->struct_id[2] = 'T';
    conn_opts->struct_id[3] = 'C';

    conn_opts->struct_version = 8;
    conn_opts->keepAliveInterval = 60;
    conn_opts->cleansession = 1;
    conn_opts->reliable = 1;
    conn_opts->will = NULL;
#if 0
    conn_opts->username = "ZYZH-YT325-MJJCR-1";
    conn_opts->password = "9ff33667562cc35790b8db58227b222d";
#else
    conn_opts->username = MQ_USERNAME;
    conn_opts->password = MQ_PASSWORD;
#endif
    conn_opts->connectTimeout = 30;
    conn_opts->retryInterval = 0;

    conn_opts->ssl = NULL;
    conn_opts->serverURIcount = 0;
    conn_opts->serverURIs = NULL;
    conn_opts->MQTTVersion = MQTTVERSION_DEFAULT;

    conn_opts->returned.serverURI = NULL;
    conn_opts->returned.MQTTVersion = 0;
    conn_opts->returned.sessionPresent = 0;

    conn_opts->binarypwd.len = 0;
    conn_opts->binarypwd.data = NULL;

    conn_opts->maxInflightMessages = -1;
    conn_opts->cleanstart = 0;

    conn_opts->httpHeaders = NULL;
    conn_opts->httpProxy = NULL;
    conn_opts->httpsProxy = NULL;

    // 从新设置连接属性
    pclient->conn_opts.keepAliveInterval = 20;
    pclient->conn_opts.cleansession = 1;
    ret = MQTTClient_connect(pclient->client, conn_opts);
    if (MQTTCLIENT_SUCCESS !=ret)
    {
        printf("connect ret errno %d\n", ret);
        ret = -MQERR_CLIENT_CONNECT;
        goto connect_fail;
    }

    *client = (MQClient*)pclient;
    pthread_mutex_init(&(pclient->lockmsg), NULL);
    return 0;
connect_fail:
setback_fail:
    MQTTClient_destroy(&(pclient->client));
create_fail:
    free(pclient);
    pclient = NULL;
    return ret;
}

int destroyMqttclient(MQClient* client) {
    int ret = 0;
    if (NULL == client)
        return -MQERR_CHECK_PARAM;
    if (MQTYPE_PAHOC!=client->type)
        return -MQERR_MQTYPE;

    MQClientPahoC *pclient = (MQClientPahoC *)client;
    ret = MQTTClient_disconnect(client, 10000);
    if (MQTTCLIENT_SUCCESS !=ret)
    {
        printf("disconnect ret errno %d\n", ret);
        return -MQERR_CLIENT_DISCONNECT;
    }

    MQTTClient_destroy(&(pclient->client));
    pthread_mutex_destroy(&(pclient->lockmsg));
    free(pclient);
    return 0;
}

/**
 * @brief 
 * 
 * @param topic 
 * @param message 
 * @return int 
 * 发送一条mqtt消息
 */
int pushOneMessage(MQClient* client, const char *topic, const char *message, RETAINED_TYPE type) {
    int ret = 0;
    if (NULL == client || NULL == topic || NULL == message)
        return -MQERR_CHECK_PARAM;
    if (MQTYPE_PAHOC!=client->type)
        return -MQERR_MQTYPE;

    MQClientPahoC *pclient = (MQClientPahoC *)client;
    MQTTClient_message *pubmsg = &(pclient->pubmsg);
    // 加锁, 设置超时,
    int timeout = LOCKTIMEOUT_100MS;
    while (timeout && 0!=pthread_mutex_trylock(&(pclient->lockmsg))) {
        usleep(100000);
        timeout--;
    }
    // 判断是否超时
    if (timeout<=0)
        return -MQERR_LOCKFAIL;

    pubmsg->payload = (void*)message;
    pubmsg->payloadlen = strlen(message);
    pubmsg->qos = QOS;
    pubmsg->retained = type;
    ret = MQTTClient_publishMessage(pclient->client, topic, pubmsg, &(pclient->token));
    if (MQTTCLIENT_SUCCESS != ret)
    {
        printf("pushmsg ret errno %d\n", ret);
        ret = -MQERR_SENDMSG;
        goto end_exit;
    }

    ret = MQTTClient_waitForCompletion(pclient->client, pclient->token, SENDMSG_TIMEOUT_MS);
    if (MQTTCLIENT_SUCCESS != ret)
    {
        printf("waitforcompletion ret errno %d, token %d\n", ret, pclient->token);
        ret = -MQERR_WAITFORSEND;
        goto end_exit;
    }

    ret = 0;
end_exit:
    // 解锁
    pthread_mutex_unlock(&(pclient->lockmsg));
    return ret;
}

int setCallBack(MQClient* client, handleMessage msgarrive, handleLost connectlost, handleComplate sendcomplate) {
    if (NULL==client || NULL==msgarrive || NULL==connectlost)
        return -MQERR_CHECK_PARAM;
    if (MQTYPE_PAHOC!=client->type)
        return -MQERR_MQTYPE;

    client->msgarrive = msgarrive;
    client->connectlost = connectlost;
#ifdef MQSYNC
    client->sendcomplate = NULL;
#elif (define MQASYNC)
    client->sendcomplate = sendcomplate;
#endif
    return 0;
}

int subscribeTopic(MQClient* client, const char* topic, int qos) {
    int ret = 0;
    if (NULL == client || NULL == topic)
        return -MQERR_CHECK_PARAM;
    if (MQTYPE_PAHOC!=client->type)
        return -MQERR_MQTYPE;

    MQClientPahoC *pclient = (MQClientPahoC *)client;
    ret = MQTTClient_subscribe(pclient->client, topic, qos);
    if (MQTTCLIENT_SUCCESS!=ret)
        return -MQERR_SUBSCRIBE_FAIL;
    return 0;
}

int unSubscribeTopic(MQClient* client, const char* topic) {
    int ret = 0;
    if (NULL == client || NULL == topic)
        return -MQERR_CHECK_PARAM;
    if (MQTYPE_PAHOC!=client->type)
        return -MQERR_MQTYPE;
    MQClientPahoC *pclient = (MQClientPahoC *)client;
    ret = MQTTClient_unsubscribe(pclient->client, topic);
    if (MQTTCLIENT_SUCCESS!=ret)
        return -MQERR_UNSUBSCRIBE_FAIL;
    return 0;
}


