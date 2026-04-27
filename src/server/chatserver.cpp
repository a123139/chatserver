#include "chatserver.hpp"
#include "chatservice.hpp"
#include"json.hpp"
#include <iostream>
#include <functional>
#include <string>

// 使用占位符命名空间，用于std::bind
using namespace placeholders;
using json=nlohmann::json;
ChatServer::ChatServer(EventLoop *loop,               // 事件循环指针（Reactor/EventLoop）
                       const InetAddress &listenAddr, // IP+Port（监听地址）
                       const string &nameArg)         // 服务器的名字
    : _server(loop, listenAddr, nameArg), _loop(loop)
{
    _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, std::placeholders::_1));
    _server.setMessageCallback(std::bind(&ChatServer::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    _server.setThreadNum(4);
}

void ChatServer::start()
{
    _server.start();
}

void ChatServer::onConnection(const TcpConnectionPtr &conn)
{
    // 判断连接状态：connected() 为 true 表示连接建立成功
    if (conn->connected())
    {
        // 打印客户端信息：对端IP:端口 -> 本地端IP:端口
        cout << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort() << " is UP." << endl;
    }
    else
    {
        ChatService::instance()->clientcloseexception(conn);
        conn->shutdown();
    }
}

// 上报读写事件相关信息的回调函数
void ChatServer::onMessage(const TcpConnectionPtr &conn,
                           Buffer *buffer,
                           Timestamp time)
{
    string buf = buffer->retrieveAllAsString();
    // 数据的反序列化
    json js = json::parse(buf);
    // 达到的目的：完全解耦   网络模块的代码   业务模块的代码
    // 通过js["msgid"] 获取=>业务handler=>conn js time
    auto msgHandler = ChatService::instance()->getHandler(js["msgid"].get<int>());
    msgHandler(conn, js, time);
}