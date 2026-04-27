#include "chatserver.hpp"
#include "chatservice.hpp"
#include <iostream>
#include <signal.h>
using namespace std;

// 处理服务器ctrl+c结束后，重置user的状态信息
void resetHandler(int)
{
    ChatService::instance()->reset();
    exit(0);
}

// main 函数：程序入口
int main(int argc, char **argv)
{

    if (argc < 3)
    {
        cerr << "command invalid! example: ./ChatServer 127.0.0.1 6000" << endl;
        exit(-1);
    }
    // 解析通过命令行参数传递的ip和port
    char *ip = argv[1];
    uint16_t port = atoi(argv[2]);

    signal(SIGINT, resetHandler);

    // 创建一个事件循环对象（主 Reactor，对应一个 epoll fd）
    EventLoop loop;
    // 指定服务器监听地址
    InetAddress addr(ip, port);
    // 创建 ChatServer 对象
    // 传入主 loop、监听地址、服务器名称
    ChatServer server(&loop, addr, "ChatServer");
    // 启动服务器
    server.start();
    // 开启事件循环
    // loop.loop() 等价于执行 epoll_wait，以阻塞方式等待新用户连接和已连接用户的读写事件
    loop.loop();

    return 0;
}