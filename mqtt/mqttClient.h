#ifndef __MQTTCLIENT_H__
#define __MQTTCLIENT_H__
#ifdef __cplusplus
extern "C"
{
#endif
// 错误码定义
#define MQERR_CHECK_PARAM 1 // 参数检查失败
#define MQERR_MALLOC 2      // 分配内存失败

    typedef struct
    {

    } MQClient;
    typedef int (*handleMessage)();
    // 创建一个mqtt客户端
    int createMqttclient(MQClient **client);
    // 销毁mqtt客户端
    int destroyMqttclient();
    // 提交一条消息
    int pushOneMessage(const char *topic, const char *message);
#ifdef __cplusplus
}
#endif

#endif
