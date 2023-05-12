#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include<thread>

#include <ws2tcpip.h>    // 包含inet_pton和inet_ntop等函数
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

#include "../log/Log.h"
#include "../route/Route.h"



namespace myServer
{


struct SocketItem  // 与c接口打交道时，不要无脑用封装性，如果这里用class在get和set是及其繁琐的
{
    SOCKET socket;
    struct sockaddr_in servaddr;
};


class Server
{
private:
    SocketItem servSockItem;
    SocketItem* clientSockArr;
    struct WSAData wsadata;
    int clientNum = 4;      // 暂时不能手动设置改值    
    int clientID = 0;       // 正在等待第号个client连接
public:
    Server(const char* IPaddr = "127.0.0.1", unsigned short port = 8888) 
    {
        int res = 0;
        res = WSAStartup(MAKEWORD(2, 2), &wsadata);
        myLog::log(res, -1, "error: myServer::Server::Server() ==> WSAStartup()失败");

        servSockItem.socket = socket(AF_INET, SOCK_STREAM, 0);
        myLog::log(static_cast<int>(servSockItem.socket), -1, "error: myServer::Server::Server() ==> socket()失败");


        servSockItem.servaddr.sin_family = AF_INET;
        servSockItem.servaddr.sin_port = htons(port);
 
        inet_pton(AF_INET, IPaddr, &servSockItem.servaddr.sin_addr.s_addr);  /////
        res = bind(servSockItem.socket, (struct sockaddr*)&servSockItem.servaddr, sizeof(servSockItem.servaddr));
        myLog::log(res, -1, "error: myServer::Server::Server() ==> bind()失败");

        res = listen(servSockItem.socket, clientNum);
        myLog::log(res, -1, "error: myServer::Server::Server() ==> listen()失败");

        clientSockArr = new SocketItem[clientNum];
    }
    
    ~Server()
    {
        for (int i = 0; i < clientNum; i++) {  // 有问题不完善，要改进？？？？？？？？
            closesocket(clientSockArr[i].socket);
        }
        delete[] clientSockArr;

        closesocket(servSockItem.socket);
        WSACleanup();
    }
    
    Server(const Server&) = delete;
    Server& operator= (const Server&) = delete;
    // 移动操作 ??
    // const class ??

    SocketItem accept_server()
    {
        int addrlen = sizeof(clientSockArr[clientID].servaddr);
        clientSockArr[clientID].socket = accept(servSockItem.socket, (struct sockaddr*)&clientSockArr[clientID].servaddr, &addrlen); // 与服务端的sockaddr相绑定，返回新的socket
        myLog::log(static_cast<int>(clientSockArr[clientID].socket), -1, "error: myServer::Server::accept_server()失败");
        return clientSockArr[clientID++];   // 注意是后置++  未控制clientID < clientNum ？？？？？？？？
    }


};




}


void RequestMainFunc(SOCKET* clientsock)
{
    myRoute::Route route{};
    std::vector<void*> res(10, nullptr);
    res[0] = clientsock;

    char buf[1024] = { 0 };
    int len = 0;
    char url[1024] = { 0 };

    while (1) {   // 为什么顶部的页面栏一直在转圈圈？ 怎么设置顶部的页面栏的图标？



        len = recv(*clientsock, buf, 1024, 0);
        myLog::log(len, -1, "error: recv()失败 ==> clientsock:连接以停止");
        // myLog::log(len, -1, "error: myRoute::route::get_fileAddr()失败 ==> " + "不存在");  // 为什么不行？？？？
        printf("%s\n", buf);  // 输出get请求报

        sscanf(buf, "GET %s HTTP/1.1", url);   // 用正则表达式获得文件路径
        printf(" ==> url: %s\n", url);           // 输出url

     
        myRoute::RouteABS* routeElement = route.get_routeElement(url);
        routeElement->dealRequest(buf, buf, res);

    }


    // delete vector element ???????
}



//int main()
//{
//	struct WSAData wsadata;
//	int res = WSAStartup(MAKEWORD(2, 2), &wsadata);
//	if (res == -1) return -1;
//
//	SOCKET servsock = socket(AF_INET, SOCK_STREAM, 0);
//	if (servsock == -1) return -1;
//
//	struct sockaddr_in servaddr;
//	servaddr.sin_family = AF_INET;
//	servaddr.sin_port = htons(8888);
//	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);   // 同 servaddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
//
//
//	res = bind(servsock, (struct sockaddr*)&servaddr, sizeof(servaddr));
//	if (res == -1) return -1;
//
//	res = listen(servsock, 4);
//	if (res == -1) return -1;
//
//    
//    //while (1) {
//    //    SOCKET clientsock;   // new
//    //    struct sockaddr_in clientaddr;  // new
//    //    int addrlen = sizeof(clientaddr);
//    //    clientsock = accept(servsock, (struct sockaddr*)&clientaddr, &addrlen); // 与服务端的sockaddr相绑定，返回新的socket
//    //    if (clientsock == -1) return -1;
//    //    // 创建一个线程
//    //}
//    
//    SOCKET clientsock;
//    struct sockaddr_in clientaddr;
//    int addrlen = sizeof(clientaddr);
//    clientsock = accept(servsock, (struct sockaddr*)&clientaddr, &addrlen); // 与服务端的sockaddr相绑定，返回新的socket
//    if (clientsock == -1) return -1;
//
//
//    RequestMainFunc(&clientsock);
//
//    /*
//
//    myRoute::Route route{};
//
//    while (1) {   // 为什么顶部的页面栏一直在转圈圈？ 怎么设置顶部的页面栏的图标？
//
//        char buf[1024] = { 0 };
//        int len = 0;
//
//        len = recv(clientsock, buf, 1024, 0);
//        if (len == -1) return 0;
//        printf("%s\n", buf);  // 输出get请求报
//
//
//        char url[1024] = { 0 };
//        sscanf(buf, "GET %s HTTP/1.1", url);   // 用正则表达式获得文件路径
//        printf(" ==> url: %s\n", url);           // 输出url
//
//        std::vector<void*> res(10, nullptr);
//        res[0] = &clientsock;
//
//
//        myRoute::RouteABS* r1 = route.get_routeElement(url);
//        r1->dealRequest(buf, buf, res);
//    }
//    */
//	// Sleep(1000);    // 要延迟一下，浏览器加载页面要时间不能，直接关闭服务器
//
//	closesocket(clientsock);   // 注意释放资源的顺序
//	closesocket(servsock);
//	WSACleanup();
//	return 0;
//}




int main()
{
    

    //while (1) {
    //    SOCKET clientsock;   // new
    //    struct sockaddr_in clientaddr;  // new
    //    int addrlen = sizeof(clientaddr);
    //    clientsock = accept(servsock, (struct sockaddr*)&clientaddr, &addrlen); // 与服务端的sockaddr相绑定，返回新的socket
    //    if (clientsock == -1) return -1;
    //    // 创建一个线程
    //}

    //SOCKET clientsock;
    //struct sockaddr_in clientaddr;
    //int addrlen = sizeof(clientaddr);
    //clientsock = accept(servsock, (struct sockaddr*)&clientaddr, &addrlen); // 与服务端的sockaddr相绑定，返回新的socket
    //if (clientsock == -1) return -1;


    myServer::Server server{};  // myServer::Server server("127.10.10.1",11111);
    myServer::SocketItem clientItem01 = server.accept_server();
    RequestMainFunc(&clientItem01.socket);

    
    return 0;
}

