#ifndef  CHATSERVER_H
#define  CHATSERVER_H
// 引入muduo服务器核心头文件
#include <muduo/net/TcpServer.h>
// 引入事件循环（epoll封装）头文件
#include <muduo/net/EventLoop.h>
// 标准IO流，用于打印日志

// 使用标准命名空间
using namespace std;
// 使用muduo顶层命名空间
using namespace muduo;
// 使用muduo网络库的命名空间
using namespace muduo::net;
/*
基于 muduo 网络库开发服务器程序
1. 组合 TcpServer 对象
2. 创建 EventLoop 事件循环对象的指针
3. 明确 TcpServer 构造函数需要什么参数，输出 ChatServer 的构造函数
4. 在当前服务器类的构造函数当中，注册处理连接的回调函数和处理读写事件的回调函数
5. 设置合适的服务端线程数量，muduo 库会自己分配 I/O 线程和 worker 线程
*/
class ChatServer
{
public:
    ChatServer(EventLoop *loop,               // 事件循环指针（Reactor/EventLoop）
               const InetAddress &listenAddr, // IP+Port（监听地址）
               const string &nameArg) ;        // 服务器的名字

    // 开启事件循环
    void start();
private:
    void onConnection(const TcpConnectionPtr &conn);
    void onMessage(const TcpConnectionPtr &conn, // 连接
                   Buffer *buffer,               // 缓冲区（存储收到的数据）
                   Timestamp time);               // 接收到数据的时间信息
    TcpServer _server; // #1 TcpServer 核心对象，封装了服务器的所有网络行为
    EventLoop *_loop;  // #2 事件循环对象指针，对应一个 epoll 实例
};
#endif
