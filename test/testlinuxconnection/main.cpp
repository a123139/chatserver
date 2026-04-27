#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpConnection.h>
#include <muduo/net/Buffer.h>    // 你漏了这个！
#include <muduo/net/InetAddress.h> // 你漏了这个！
#include <iostream>

using namespace muduo;
using namespace muduo::net;

void onConnection(const TcpConnectionPtr& conn)
{
    if (conn->connected())
    {
        std::cout << "客户端连接成功！\n";
    }
    else
    {
        std::cout << "客户端断开连接！\n";
    }
}

void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp time)
{
    std::string msg = buf->retrieveAllAsString();
    std::cout << "收到消息：" << msg << std::endl;
    conn->send(msg);
}

int main()
{
    EventLoop loop;
    TcpServer server(&loop, InetAddress(8888), "ChatServer");

    server.setConnectionCallback(onConnection);
    server.setMessageCallback(onMessage);

    server.start();
    loop.loop();
}