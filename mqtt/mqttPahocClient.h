#ifndef __MQTTPAHOCCLIENT_H__
#define __MQTTPAHOCCLIENT_H__
#ifdef __cplusplus
extern "C" {
#endif
#include "mqttClient.h"
#include "paho.mqtt.c-1.3.9/src/MQTTClient.h"
#include <pthread.h>

#define MQSYNC      // ʹ��ͬ��ģʽ
#ifndef MQSYNC
#define MQASYNC     // ʹ���첽ģʽ
#endif

#define QOS         1
#define LOCKTIMEOUT_100MS   5   // ������ʱʱ��(100ms)
#define SENDMSG_TIMEOUT_MS  10000L // �ȴ�������Ϣ���ʱ��(ms)

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
