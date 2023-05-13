


#ifndef _SERVER_H_
#define _SERVER_H_


#include <iostream>
#include <string>
#include <vector>

#include <ws2tcpip.h>    // ����inet_pton��inet_ntop�Ⱥ���
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

#include "../log/Log.h"








namespace myServer
{


struct SocketItem  // ��c�ӿڴ򽻵�ʱ����Ҫ�����÷�װ�ԣ����������class��get��set�Ǽ��䷱����
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
    int clientNum = 4;      // ��ʱ�����ֶ����ø�ֵ    
    int clientID = 0;       // ���ڵȴ��ںŸ�client����
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

        res = listen(servSockItem.socket, clientNum);
        myLog::log(res, -1, "error: myServer::Server::Server() ==> listen()ʧ��");

        clientSockArr = new SocketItem[clientNum];
    }
    
    ~Server()
    {
        for (int i = 0; i < clientNum; i++) {  // �����ⲻ���ƣ�Ҫ�Ľ�����������������
            closesocket(clientSockArr[i].socket);
        }
        delete[] clientSockArr;

        closesocket(servSockItem.socket);
        WSACleanup();
    }
    
    Server(const Server&) = delete;
    Server& operator= (const Server&) = delete;
    // �ƶ����� ??
    // const class ??

    SocketItem accept_server()
    {
        int addrlen = sizeof(clientSockArr[clientID].servaddr);
        clientSockArr[clientID].socket = accept(servSockItem.socket, (struct sockaddr*)&clientSockArr[clientID].servaddr, &addrlen); // �����˵�sockaddr��󶨣������µ�socket
        myLog::log(static_cast<int>(clientSockArr[clientID].socket), -1, "error: myServer::Server::accept_server()ʧ��");
        return clientSockArr[clientID++];   // ע���Ǻ���++  δ����clientID < clientNum ����������������
    }


};



bool SendhttpRequest(myServer::SocketItem& clientSockItem,const char* responseHeader, char* fileName) {
    char responseHeader[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html \r\n\r\n";  // ע�⣺httpͷ��(\r\n\r\n) + html�ļ���������ʽ������Ч�������html�ļ����뵽ͷ���н��д��䣬��html��ʧЧ
    send(clientSockItem.socket, responseHeader, sizeof(responseHeader), 0);

    FILE* fp;
    // fopen_s(&fp, fileaddr.c_str(), "rb");

    // fopen_s(&fp, "web//test//test.html", "rb");
    fopen_s(&fp, "web//login//login.html", "rb");

    // fopen_s(&fp, "web//test//button.html", "rb");

    char buffer[1024];
    int bytesRead;
    while ((bytesRead = fread(buffer, sizeof(char), sizeof(buffer), fp)) > 0)
    {
        send(clientSockItem.socket, buffer, bytesRead, 0);
    }

    fclose(fp);



}


}





#endif // ��_SERVER_H_



