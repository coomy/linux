#include <iostream>
// libevent头文件
#include <event.h>

using namespace std;
struct event evt;
struct timeval tv;

// 定时事件回调函数
void time_cb(int sock, short event, void *arg)
{
    printf("timer wakeup\n");
    //重新添加定时事件（定时事件触发后默认自动删除）
    event_add((struct event*)arg, &tv);
}

int main()
{
    //初始化
    struct event_base *base = event_init();
    tv.tv_sec = 1; //1s period
    tv.tv_usec = 0;
    //设置定时事件
    evtimer_set(&evt, time_cb, &evt);
    // 添加定时事件
    event_add(&evt, &tv);
    //事件循环
    event_base_dispatch(base);
    return 0;
}