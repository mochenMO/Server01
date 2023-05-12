#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <string.h>
#include <vector>


#include <ws2tcpip.h>    // 包含inet_pton和inet_ntop等函数
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

#include "../log/Log.h"
#include "../route/Route.h"




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



int main()
{
	struct WSAData wsadata;
	int res = WSAStartup(MAKEWORD(2, 2), &wsadata);
	if (res == -1) return -1;

	SOCKET servsock = socket(AF_INET, SOCK_STREAM, 0);
	if (servsock == -1) return -1;

	struct sockaddr_in servaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8888);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);   // 同 servaddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);


	res = bind(servsock, (struct sockaddr*)&servaddr, sizeof(servaddr));
	if (res == -1) return -1;

	res = listen(servsock, 4);
	if (res == -1) return -1;

    
    //while (1) {
    //    SOCKET clientsock;   // new
    //    struct sockaddr_in clientaddr;  // new
    //    int addrlen = sizeof(clientaddr);
    //    clientsock = accept(servsock, (struct sockaddr*)&clientaddr, &addrlen); // 与服务端的sockaddr相绑定，返回新的socket
    //    if (clientsock == -1) return -1;
    //    // 创建一个线程
    //}
    
    SOCKET clientsock;
    struct sockaddr_in clientaddr;
    int addrlen = sizeof(clientaddr);
    clientsock = accept(servsock, (struct sockaddr*)&clientaddr, &addrlen); // 与服务端的sockaddr相绑定，返回新的socket
    if (clientsock == -1) return -1;


    RequestMainFunc(&clientsock);

    /*

    myRoute::Route route{};

    while (1) {   // 为什么顶部的页面栏一直在转圈圈？ 怎么设置顶部的页面栏的图标？

        char buf[1024] = { 0 };
        int len = 0;

        len = recv(clientsock, buf, 1024, 0);
        if (len == -1) return 0;
        printf("%s\n", buf);  // 输出get请求报


        char url[1024] = { 0 };
        sscanf(buf, "GET %s HTTP/1.1", url);   // 用正则表达式获得文件路径
        printf(" ==> url: %s\n", url);           // 输出url

        std::vector<void*> res(10, nullptr);
        res[0] = &clientsock;


        myRoute::RouteABS* r1 = route.get_routeElement(url);
        r1->dealRequest(buf, buf, res);
    }
    */
	// Sleep(1000);    // 要延迟一下，浏览器加载页面要时间不能，直接关闭服务器

	closesocket(clientsock);   // 注意释放资源的顺序
	closesocket(servsock);
	WSACleanup();
	return 0;
}


/*// 参考代码

#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")  // 链接 Winsock2 库

int main()
{
    WSADATA wsaData;                    // 存储 Winsock 库初始化信息的结构体
    SOCKET listenSocket = INVALID_SOCKET;// 监听客户端连接请求的套接字对象
    SOCKET clientSocket = INVALID_SOCKET;// 与客户端进行数据交换的套接字对象
    struct addrinfo* result = NULL;     // 待连接主机的地址信息列表
    struct addrinfo hints;              // 连接的地址信息结构体
    int ret;

    // 初始化 Winsock 库
    ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (ret != 0)
    {
        printf("WSAStartup failed with error: %d\n", ret);
        return 1;
    }

    // 填充初始地址信息结构体（仅支持 IPv4）
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;          // IPv4 地址族
    hints.ai_socktype = SOCK_STREAM;    // 流式套接字
    hints.ai_protocol = IPPROTO_TCP;    // TCP 协议
    hints.ai_flags = AI_PASSIVE;        // 被动连接模式

    // 获取待连接的本地 IP 地址和端口号
    ret = getaddrinfo(NULL, "8080", &hints, &result);
    if (ret != 0)
    {
        printf("getaddrinfo failed with error: %d\n", ret);
        WSACleanup();
        return 1;
    }

    // 创建套接字对象并绑定到本地 IP 地址和端口号上
    listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listenSocket == INVALID_SOCKET)
    {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }
    ret = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (ret == SOCKET_ERROR)
    {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }
    freeaddrinfo(result);

    // 监听本地 IP 地址上指定的端口，等待连接请求
    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    // 循环处理客户端请求
    while (true)
    {
        // 接收客户端连接请求，并创建新的套接字对象用于接收数据
        clientSocket = accept(listenSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET)
        {
            printf("accept failed with error: %d\n", WSAGetLastError());
            closesocket(listenSocket);
            WSACleanup();
            return 1;
        }

        // 组织 HTTP 响应报文并发送给客户端
        char responseHeader[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
        send(clientSocket, responseHeader, sizeof(responseHeader), 0);

        // 发送指定的 HTML 文件内容（注意要使用二进制方式读取文件）
        FILE* fp;
        ret = fopen_s(&fp, "test.html", "rb");
        if (ret != 0)
        {
            printf("open file 'test.html' failed with error: %d\n", ret);
            continue;
        }
        char buffer[1024];
        int bytesRead;
        while ((bytesRead = fread(buffer, sizeof(char), sizeof(buffer), fp)) > 0)
        {
            send(clientSocket, buffer, bytesRead, 0);
        }
        fclose(fp);

        // 关闭与客户端的连接和套接字对象
        closesocket(clientSocket);
    }

    // 关闭监听套接字对象并完成 Winsock 库的清理工作
    closesocket(listenSocket);
    WSACleanup();

    return 0;
}
*/