/** 
 * 实现监听内核事件的模块
 * 监听开启一个独立线程，并提供回调函数返回结果
 * 并且提供跟内核通信的接口
 * 
*/
// link事件的回调参数
#define NET_LINK_UP         0
#define NET_LINK_DOWN       1
#define NET_LINK_UNKNOW     3
typedef struct {
    const char *ifname;
    int link;
} NETLINK_CB_PARAM;

// netlink的监听事件类型
typedef enum {
    NLEV_LINK = 0,
    NLEV_MAX,
} NLEV_TYPE;
typedef int (*hand_netlink_ev)(void* arg);

int start_netlink_listen();
int stop_netlink_listen();
int set_netlink_listen_ev(NLEV_TYPE ev, hand_netlink_ev cb);
int rm_netlink_listen_ev(NLEV_TYPE ev);

// 基于libnl获取网络信息
int nl_getGateway();
