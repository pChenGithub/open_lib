#ifndef __MQTTCLIENT_H__
#define __MQTTCLIENT_H__
#ifdef __cplusplus
extern "C"
{
#endif
// �����붨��
#define MQERR_CHECK_PARAM 1 // �������ʧ��
#define MQERR_MALLOC 2      // �����ڴ�ʧ��

    typedef struct
    {

    } MQClient;
    typedef int (*handleMessage)();
    // ����һ��mqtt�ͻ���
    int createMqttclient(MQClient **client);
    // ����mqtt�ͻ���
    int destroyMqttclient();
    // �ύһ����Ϣ
    int pushOneMessage(const char *topic, const char *message);
#ifdef __cplusplus
}
#endif

#endif
