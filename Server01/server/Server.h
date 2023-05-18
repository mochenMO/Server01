


#ifndef _SERVER_H_
#define _SERVER_H_


#define _CRT_SECURE_NO_WARNINGS  ////// 


#include <iostream>
#include <string>
#include <vector>

#include <ws2tcpip.h>    // ����inet_pton��inet_ntop�Ⱥ���
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

#include "../log/Log.h"
#include "../http/Http.h"







namespace myServer
{


struct SocketItem  // ��c�ӿڴ򽻵�ʱ����Ҫ�����÷�װ�ԣ����������class��get��set�Ǽ��䷱����
{
    SOCKET socket;
    struct sockaddr_in servaddr; 


    void SendhttpRequest(myHttp::HttpResponce& httpResponce, myHttp::HttpRequest& httpRequest) {
        
        httpResponce.CombinatHttpResponce();
        std::string responseHeader = httpResponce.getHttpResponce();
        send(socket, responseHeader.c_str(), responseHeader.length(), 0);
        
        if (httpRequest.getContentType() == "application/json") {
            PosthttpRequest(httpResponce, httpRequest);
        }
        else {
            std::string fileName = httpRequest.getPageName() + httpRequest.getFileName();
            GethttpRequest(httpResponce, httpRequest);
        }
    }


    void PosthttpRequest(myHttp::HttpResponce& httpResponce, myHttp::HttpRequest& httpRequest)
    {
        myJson::Json json = httpResponce.getJson();
        std::string jsonValue = json.getJson();
        send(socket, jsonValue.c_str(), jsonValue.length(), 0);
        std::cout << "�ѷ���json�ļ�" << std::endl;
    }

    void GethttpRequest(myHttp::HttpResponce& httpResponce, myHttp::HttpRequest& httpRequest)
    {
        std::string fileName = "web" + httpRequest.getPageName() + httpRequest.getFileName();
        std::ifstream file(fileName, std::ios::binary);
        if (file.fail()) {
            fileName = "web" + httpRequest.getPageName() + "/res" + httpRequest.getFileName();
            file.open(fileName, std::ios::binary);   // image��Դ
            if (file.fail()) {
                std::cout << "��ȡ�ļ�ʧ��: " << fileName << std::endl;
                return;
            }
        }
        
        char buffer[4096];
        while (!file.eof()) {
            file.read(buffer, sizeof(buffer));
            send(socket, buffer, file.gcount(), 0);
        }

        std::cout << "�ѷ���: " << fileName << std::endl;
    }


};


//class Server
//{
//private:
//    SocketItem servSockItem;
//    SocketItem* clientSockArr;
//    struct WSAData wsadata;
//    int clientNum = 40;     // ��ʱ�����ֶ����ø�ֵ    
//    int clientID = 0;       // ���ڵȴ��ںŸ�client����
//public:
//    Server(const char* IPaddr = "127.0.0.1", unsigned short port = 8888) 
//    {
//        int res = 0;
//        res = WSAStartup(MAKEWORD(2, 2), &wsadata);
//        myLog::log(res, -1, "error: myServer::Server::Server() ==> WSAStartup()ʧ��");
//
//        servSockItem.socket = socket(AF_INET, SOCK_STREAM, 0);
//        myLog::log(static_cast<int>(servSockItem.socket), -1, "error: myServer::Server::Server() ==> socket()ʧ��");
//
//
//        servSockItem.servaddr.sin_family = AF_INET;
//        servSockItem.servaddr.sin_port = htons(port);
// 
//        inet_pton(AF_INET, IPaddr, &servSockItem.servaddr.sin_addr.s_addr);  /////
//        res = bind(servSockItem.socket, (struct sockaddr*)&servSockItem.servaddr, sizeof(servSockItem.servaddr));
//        myLog::log(res, -1, "error: myServer::Server::Server() ==> bind()ʧ��");
//
//        res = listen(servSockItem.socket, clientNum);
//        myLog::log(res, -1, "error: myServer::Server::Server() ==> listen()ʧ��");
//
//        clientSockArr = new SocketItem[clientNum];
//    }
//    
//    ~Server()
//    {
//        for (int i = 0; i < clientNum; i++) {  // �����ⲻ���ƣ�Ҫ�Ľ�����������������
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
//    // �ƶ����� ??
//    // const class ??
//
//    SocketItem accept_server()
//    {
//         int addrlen = sizeof(clientSockArr[clientID].servaddr);
//         clientSockArr[clientID].socket = accept(servSockItem.socket, (struct sockaddr*)&clientSockArr[clientID].servaddr, &addrlen); // �����˵�sockaddr��󶨣������µ�socket
//         myLog::log(static_cast<int>(clientSockArr[clientID].socket), -1, "error: myServer::Server::accept_server()ʧ��");
//         return clientSockArr[clientID++];   // ע���Ǻ���++  δ����clientID < clientNum ����������������
//    }
//
//
//    SocketItem accept_server1()
//    {
//
//        SOCKET clientsock;
//        struct sockaddr_in clientaddr;
//        int addrlen = sizeof(clientaddr);
//        clientsock = accept(servSockItem.socket, (struct sockaddr*)&clientaddr, &addrlen); // �����˵�sockaddr��󶨣������µ�socket
//
//        char buffer[1024];
//        int bytesRead;
//        char buf[1024] = { 0 };
//
//        recv(clientsock, buf, 1024, 0);
//        printf("%s\n", buf);  // ���get����
//
//
//        char* url = new char[4096]();     // ����url��Ϣ
//        sscanf(buf, "GET %s HTTP/1.1", url);   // ��������ʽ���url����ÿ�λ��url��β��'\0'
//
//        if (strcmp(url, "/") == 0) {
//
//            char responseHeader[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html \r\n\r\n";  // ע�⣺httpͷ��(\r\n\r\n) + html�ļ���������ʽ������Ч�������html�ļ����뵽ͷ���н��д��䣬��html��ʧЧ
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
//            char responseHeader[] = "HTTP/1.1 200 OK\r\nContent-Type: text/css \r\n\r\n";  // ע�⣺httpͷ��(\r\n\r\n) + html�ļ���������ʽ������Ч�������html�ļ����뵽ͷ���н��д��䣬��html��ʧЧ
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
//            char responseHeader[] = "HTTP/1.1 200 OK\r\nContent-Type: image/png\r\nContent-Length: 120417 \r\n\r\n";  // ע�⣺httpͷ��(\r\n\r\n) + html�ļ���������ʽ������Ч�������html�ļ����뵽ͷ���н��д��䣬��html��ʧЧ
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
        myLog::log(res, -1, "error: myServer::Server::Server() ==> WSAStartup()ʧ��");

        servSockItem.socket = socket(AF_INET, SOCK_STREAM, 0);
        myLog::log(static_cast<int>(servSockItem.socket), -1, "error: myServer::Server::Server() ==> socket()ʧ��");


        servSockItem.servaddr.sin_family = AF_INET;
        servSockItem.servaddr.sin_port = htons(port);

        inet_pton(AF_INET, IPaddr, &servSockItem.servaddr.sin_addr.s_addr);  /////
        res = bind(servSockItem.socket, (struct sockaddr*)&servSockItem.servaddr, sizeof(servSockItem.servaddr));
        myLog::log(res, -1, "error: myServer::Server::Server() ==> bind()ʧ��");

        res = listen(servSockItem.socket, SOMAXCONN);
        myLog::log(res, -1, "error: myServer::Server::Server() ==> listen()ʧ��");
    }

    ~Server()
    {
        closesocket(servSockItem.socket);
        WSACleanup();
    }

    Server(const Server&) = delete;
    Server& operator= (const Server&) = delete;
    // �ƶ����� ??
    // const class ??

    SocketItem* accept_server()
    {
        SocketItem* clientSockItem = new SocketItem();
        int addrlen = sizeof(clientSockItem->servaddr);
        clientSockItem->socket = accept(servSockItem.socket, (struct sockaddr*)&clientSockItem->servaddr, &addrlen); // �����˵�sockaddr��󶨣������µ�socket
        myLog::log(static_cast<int>(clientSockItem->socket), -1, "error: myServer::Server::accept_server()ʧ��");
        return clientSockItem;   
    }
};






}





#endif // ��_SERVER_H_





