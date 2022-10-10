/**
 * @file mqttClient.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-09
 * 
 * @copyright Copyright (c) 2022
 * �Զ���mqtt�Ĳ����ӿ�
 */
#ifndef __MQTTCLIENT_H__
#define __MQTTCLIENT_H__
#ifdef __cplusplus
extern "C" {
#endif
// �����붨��
#define MQERR_CHECK_PARAM 1   // �������ʧ��
#define MQERR_MALLOC 2        // �����ڴ�ʧ��
#define MQERR_CLIENT_CREATE 3 // �����ͻ���ʧ��
#define MQERR_CLIENT_CONNECT 4 // �ͻ�������ʧ��
#define MQERR_MQTYPE    5 // mqtt��ʹ�����Ͳ���
#define MQERR_CLIENT_DISCONNECT    6 // �ͻ��˶Ͽ�����ʧ��
#define MQERR_CLIENT_DISTROY    7 // ���ٿͻ���ʧ��
#define MQERR_SENDMSG       8 // ����mqtt��Ϣʧ��
#define MQERR_WAITFORSEND   9 // �ȴ�����mqtt��Ϣ���ʧ��
#define MQERR_LOCKFAIL      10 // ����ʧ��
#define MQERR_SETCALLBACK   11 // ���ûص�����ʧ��
#define MQERR_SUBSCRIBE_FAIL    12 // ��������ʧ��
#define MQERR_UNSUBSCRIBE_FAIL  13 // ȡ����������ʧ��


typedef enum {
    MQTYPE_PAHOC = 0,
} MQTYPE;

typedef int (*handleMessage)(const char* topic, const char* payload, const int payloadlen);
typedef int (*handleLost)(const char* cause);
typedef int (*handleComplate)();

typedef struct {
    MQTYPE type;
    handleMessage msgarrive;
    handleLost connectlost;
    handleComplate sendcomplate;
} MQClient;

// ����һ��mqtt�ͻ���
int createMqttclient(MQClient **client, const char *addr, const char *clientid);
// ����mqtt�ͻ���
int destroyMqttclient(MQClient* client);
// ���ûص�����
int setCallBack(MQClient* client, handleMessage msgarrive, handleLost connectlost, handleComplate sendcomplate);
// �ύһ����Ϣ
int pushOneMessage(MQClient* client, const char *topic, const char *message);
// ��������
int subscribeTopic(MQClient* client, const char* topic, int qos);
// ȡ������
int unSubscribeTopic(MQClient* client, const char* topic);
#ifdef __cplusplus
}
#endif

#endif
