#ifndef __MQTTPAHOCCLIENT_H__
#define __MQTTPAHOCCLIENT_H__
#ifdef __cplusplus
extern "C" {
#endif
#include "mqttClient.h"
#include "paho.mqtt.c-1.3.9/src/MQTTClient.h"
#include <pthread.h>

#define MQSYNC      // 使用同步模式
#ifndef MQSYNC
#define MQASYNC     // 使用异步模式
#endif

#define QOS         1
#define LOCKTIMEOUT_100MS   5   // 拿锁超时时间(100ms)
#define SENDMSG_TIMEOUT_MS  10000L // 等待发送消息完成时间(ms)

typedef struct {
    MQClient base;
    MQTTClient client;
    MQTTClient_connectOptions conn_opts;
    MQTTClient_message pubmsg;
    MQTTClient_deliveryToken token;
    pthread_mutex_t lockmsg;
} MQClientPahoC;

#ifdef __cplusplus
}
#endif
#endif
