


#ifndef _SERVER_H_
#define _SERVER_H_


#define _CRT_SECURE_NO_WARNINGS  ////// 


#include <iostream>
#include <string>
#include <vector>

#include <ws2tcpip.h>    // 包含inet_pton和inet_ntop等函数
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

#include "../log/Log.h"








namespace myServer
{


struct SocketItem  // 与c接口打交道时，不要无脑用封装性，如果这里用class在get和set是及其繁琐的
{
    SOCKET socket;
    struct sockaddr_in servaddr;
};


//class Server
//{
//private:
//    SocketItem servSockItem;
//    SocketItem* clientSockArr;
//    struct WSAData wsadata;
//    int clientNum = 40;     // 暂时不能手动设置改值    
//    int clientID = 0;       // 正在等待第号个client连接
//public:
//    Server(const char* IPaddr = "127.0.0.1", unsigned short port = 8888) 
//    {
//        int res = 0;
//        res = WSAStartup(MAKEWORD(2, 2), &wsadata);
//        myLog::log(res, -1, "error: myServer::Server::Server() ==> WSAStartup()失败");
//
//        servSockItem.socket = socket(AF_INET, SOCK_STREAM, 0);
//        myLog::log(static_cast<int>(servSockItem.socket), -1, "error: myServer::Server::Server() ==> socket()失败");
//
//
//        servSockItem.servaddr.sin_family = AF_INET;
//        servSockItem.servaddr.sin_port = htons(port);
// 
//        inet_pton(AF_INET, IPaddr, &servSockItem.servaddr.sin_addr.s_addr);  /////
//        res = bind(servSockItem.socket, (struct sockaddr*)&servSockItem.servaddr, sizeof(servSockItem.servaddr));
//        myLog::log(res, -1, "error: myServer::Server::Server() ==> bind()失败");
//
//        res = listen(servSockItem.socket, clientNum);
//        myLog::log(res, -1, "error: myServer::Server::Server() ==> listen()失败");
//
//        clientSockArr = new SocketItem[clientNum];
//    }
//    
//    ~Server()
//    {
//        for (int i = 0; i < clientNum; i++) {  // 有问题不完善，要改进？？？？？？？？
//            closesocket(clientSockArr[i].socket);
//        }
//        delete[] clientSockArr;
//
//        closesocket(servSockItem.socket);
//        WSACleanup();
//    }
//    
//    Server(const Server&) = delete;
//    Server& operator= (const Server&) = delete;
//    // 移动操作 ??
//    // const class ??
//
//    SocketItem accept_server()
//    {
//         int addrlen = sizeof(clientSockArr[clientID].servaddr);
//         clientSockArr[clientID].socket = accept(servSockItem.socket, (struct sockaddr*)&clientSockArr[clientID].servaddr, &addrlen); // 与服务端的sockaddr相绑定，返回新的socket
//         myLog::log(static_cast<int>(clientSockArr[clientID].socket), -1, "error: myServer::Server::accept_server()失败");
//         return clientSockArr[clientID++];   // 注意是后置++  未控制clientID < clientNum ？？？？？？？？
//    }
//
//
//    SocketItem accept_server1()
//    {
//
//        SOCKET clientsock;
//        struct sockaddr_in clientaddr;
//        int addrlen = sizeof(clientaddr);
//        clientsock = accept(servSockItem.socket, (struct sockaddr*)&clientaddr, &addrlen); // 与服务端的sockaddr相绑定，返回新的socket
//
//        char buffer[1024];
//        int bytesRead;
//        char buf[1024] = { 0 };
//
//        recv(clientsock, buf, 1024, 0);
//        printf("%s\n", buf);  // 输出get请求报
//
//
//        char* url = new char[4096]();     // 保存url信息
//        sscanf(buf, "GET %s HTTP/1.1", url);   // 用正则表达式获得url，它每次会给url结尾加'\0'
//
//        if (strcmp(url, "/") == 0) {
//
//            char responseHeader[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html \r\n\r\n";  // 注意：http头部(\r\n\r\n) + html文件，这种形式才能生效，如果把html文件插入到头部中进行传输，则html会失效
//            send(clientsock, responseHeader, sizeof(responseHeader), 0);
//
//            FILE* fp;
//            fopen_s(&fp, "web//test//button.html", "rb");
//
//            while ((bytesRead = fread(buffer, sizeof(char), sizeof(buffer), fp)) > 0)
//            {
//                send(clientsock, buffer, bytesRead, 0);
//            }
//            fclose(fp);
//        
//        }
//        else if (strcmp(url, "/test.css") == 0){
//            char responseHeader[] = "HTTP/1.1 200 OK\r\nContent-Type: text/css \r\n\r\n";  // 注意：http头部(\r\n\r\n) + html文件，这种形式才能生效，如果把html文件插入到头部中进行传输，则html会失效
//            send(clientsock, responseHeader, sizeof(responseHeader), 0);
//
//            FILE* fp;
//            fopen_s(&fp, "web//test//test.css", "rb");
//
//            while ((bytesRead = fread(buffer, sizeof(char), sizeof(buffer), fp)) > 0)
//            {
//                send(clientsock, buffer, bytesRead, 0);
//            }
//            fclose(fp);
//        
//        }
//        else if (url, "res/wallpaper.png") {
//            char responseHeader[] = "HTTP/1.1 200 OK\r\nContent-Type: image/png\r\nContent-Length: 120417 \r\n\r\n";  // 注意：http头部(\r\n\r\n) + html文件，这种形式才能生效，如果把html文件插入到头部中进行传输，则html会失效
//            send(clientsock, responseHeader, sizeof(responseHeader), 0);
//       
//            FILE* fp;
//            fopen_s(&fp, "web//test//res//wallpaper.png", "rb");
//
//            while ((bytesRead = fread(buffer, sizeof(char), sizeof(buffer), fp)) > 0)
//            {
//                send(clientsock, buffer, bytesRead, 0);
//            }
//            fclose(fp);
//
//        }
//          
//        else
//            return clientSockArr[clientID];
//
//        return clientSockArr[clientID];
//
//    }
//
//};



class Server
{
private:
    SocketItem servSockItem;
    struct WSAData wsadata;
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

        res = listen(servSockItem.socket, SOMAXCONN);
        myLog::log(res, -1, "error: myServer::Server::Server() ==> listen()失败");
    }

    ~Server()
    {
        closesocket(servSockItem.socket);
        WSACleanup();
    }

    Server(const Server&) = delete;
    Server& operator= (const Server&) = delete;
    // 移动操作 ??
    // const class ??

    SocketItem* accept_server()
    {
        SocketItem* clientSockItem = new SocketItem();
        int addrlen = sizeof(clientSockItem->servaddr);
        clientSockItem->socket = accept(servSockItem.socket, (struct sockaddr*)&clientSockItem->servaddr, &addrlen); // 与服务端的sockaddr相绑定，返回新的socket
        myLog::log(static_cast<int>(clientSockItem->socket), -1, "error: myServer::Server::accept_server()失败");
        return clientSockItem;   
    }
};






}





#endif // ！_SERVER_H_





