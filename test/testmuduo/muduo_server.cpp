/*
muduo网络库给用户提供了两个主要的类
TcpServer ： 用于编写服务器程序的
TcpClient ： 用于编写客户端程序的epoll + 线程池
好处：能够把网络I/O的代码和业务代码区分开
                用户的连接和断开       用户的可读写事件
*/

// 引入muduo服务器核心头文件
#include <muduo/net/TcpServer.h>
// 引入事件循环（epoll封装）头文件
#include <muduo/net/EventLoop.h>
// 标准IO流，用于打印日志
#include <iostream>
// 用于std::bind和占位符std::placeholders::_1等
#include <functional>
// 字符串处理
#include <string>

// 使用标准命名空间
using namespace std;
// 使用muduo顶层命名空间
using namespace muduo;
// 使用muduo网络库的命名空间
using namespace muduo::net;
// 使用占位符命名空间，用于std::bind
using namespace placeholders;
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
    // 构造函数：传入事件循环、监听地址、服务器名称
    // _server(loop, listenAddr, nameArg) 初始化 TcpServer
    // _loop(loop) 初始化成员变量 _loop
    ChatServer(EventLoop *loop,               // 事件循环指针（Reactor/EventLoop）
               const InetAddress &listenAddr, // IP+Port（监听地址）
               const string &nameArg)         // 服务器的名字
        : _server(loop, listenAddr, nameArg), _loop(loop)
    {
        // 给服务器注册用户连接的创建和断开回调
        // 当有新连接建立或旧连接断开时，会调用 ChatServer::onConnection
        _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, std::placeholders::_1));

        // 给服务器注册用户读写事件回调
        // 当有数据可读时，会调用 ChatServer::onMessage
        _server.setMessageCallback(std::bind(&ChatServer::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

        // 设置服务器端的线程数量
        // 1 个 I/O 线程（处理监听/accept） + 3 个 worker 线程（处理业务/计算）
        // muduo 内部会基于线程数创建对应的 EventLoop 线程池
        _server.setThreadNum(4);
    }

    // 开启事件循环
    void start()
    {
        // 启动 TcpServer，开始监听端口并运行事件循环
        _server.start();
    }

private:
    // 专门处理用户的连接创建和断开 （epoll listenfd accept）
    // TcpConnectionPtr 是智能指针，封装了客户端的连接 fd 和状态
    void onConnection(const TcpConnectionPtr &conn)
    {
        // 判断连接状态：connected() 为 true 表示连接建立成功
        if (conn->connected())
        {
            // 打印客户端信息：对端IP:端口 -> 本地端IP:端口
            cout << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort() << " is UP." << endl;
        }
        else
        {
            // 连接断开
            cout << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort() << " is DOWN." << endl;
            // 主动关闭连接（muduo 底层会自动关闭 fd，这里可以省略手动 shutdown）
            conn->shutdown(); 
            // 如果是最后一个连接，可选择退出事件循环 _loop->quit();
        }
    }

    // 专门处理用户的读写事件
    // conn  : 连接智能指针
    // buffer: 接收缓冲区（存储客户端发来的数据）
    // time  : 接收到数据的时间戳
    void onMessage(const TcpConnectionPtr &conn, // 连接
                   Buffer *buffer,               // 缓冲区（存储收到的数据）
                   Timestamp time)               // 接收到数据的时间信息
    {
        // 从缓冲区中取出所有数据并转换为 string 类型
        string buf = buffer->retrieveAllAsString();
        
        // 打印收到的数据和时间
        cout << "recv data:" << buf << " time:" << time.toString() << endl;
        
        // Echo 逻辑：将收到的数据原封不动发回给客户端
        conn->send(buf);
    }

    TcpServer _server; // #1 TcpServer 核心对象，封装了服务器的所有网络行为
    EventLoop *_loop;  // #2 事件循环对象指针，对应一个 epoll 实例
};

// main 函数：程序入口
int main()
{
    // 创建一个事件循环对象（主 Reactor，对应一个 epoll fd）
    EventLoop loop;

    // 指定服务器监听地址：127.0.0.1 端口 6000
    InetAddress addr("127.0.0.1", 6000);

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