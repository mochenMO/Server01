#pragma once
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


    void SendhttpRequest(myHttp::HttpResponce& httpResponce) {
        httpResponce.CombinatHttpResponce();
        std::string responseHeader = httpResponce.getHttpResponce();
        
        std::cout << responseHeader << std::endl;
        
        send(socket, responseHeader.c_str(), responseHeader.length(), 0);
        
        if (httpResponce.getContentType() == "application/json") {
            PosthttpRequest(httpResponce);
        }
        else {
            GethttpRequest(httpResponce);
        }
    }


    void PosthttpRequest(myHttp::HttpResponce& httpResponce)
    {
        myJson::Json json = httpResponce.getJson();
        std::string jsonValue = json.getJson();
        send(socket, jsonValue.c_str(), jsonValue.length(), 0);
        std::cout << "�ѷ���json�ļ�" << std::endl;
    }

    void GethttpRequest(myHttp::HttpResponce& httpResponce)
    {
        std::string fileName = "web" + httpResponce.getPageName() + httpResponce.getFileName();
        std::ifstream file(fileName, std::ios::binary);
        if (file.fail()) {
            fileName = "web" + httpResponce.getPageName() + "/res" + httpResponce.getFileName();
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





