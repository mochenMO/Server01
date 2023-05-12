#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include<thread>

#include <ws2tcpip.h>    // ����inet_pton��inet_ntop�Ⱥ���
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

#include "../log/Log.h"
#include "../route/Route.h"



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




}


void RequestMainFunc(SOCKET* clientsock)
{
    myRoute::Route route{};
    std::vector<void*> res(10, nullptr);
    res[0] = clientsock;

    char buf[1024] = { 0 };
    int len = 0;
    char url[1024] = { 0 };

    while (1) {   // Ϊʲô������ҳ����һֱ��תȦȦ�� ��ô���ö�����ҳ������ͼ�ꣿ



        len = recv(*clientsock, buf, 1024, 0);
        myLog::log(len, -1, "error: recv()ʧ�� ==> clientsock:������ֹͣ");
        // myLog::log(len, -1, "error: myRoute::route::get_fileAddr()ʧ�� ==> " + "������");  // Ϊʲô���У�������
        printf("%s\n", buf);  // ���get����

        sscanf(buf, "GET %s HTTP/1.1", url);   // ��������ʽ����ļ�·��
        printf(" ==> url: %s\n", url);           // ���url

     
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
//	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);   // ͬ servaddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
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
//    //    clientsock = accept(servsock, (struct sockaddr*)&clientaddr, &addrlen); // �����˵�sockaddr��󶨣������µ�socket
//    //    if (clientsock == -1) return -1;
//    //    // ����һ���߳�
//    //}
//    
//    SOCKET clientsock;
//    struct sockaddr_in clientaddr;
//    int addrlen = sizeof(clientaddr);
//    clientsock = accept(servsock, (struct sockaddr*)&clientaddr, &addrlen); // �����˵�sockaddr��󶨣������µ�socket
//    if (clientsock == -1) return -1;
//
//
//    RequestMainFunc(&clientsock);
//
//    /*
//
//    myRoute::Route route{};
//
//    while (1) {   // Ϊʲô������ҳ����һֱ��תȦȦ�� ��ô���ö�����ҳ������ͼ�ꣿ
//
//        char buf[1024] = { 0 };
//        int len = 0;
//
//        len = recv(clientsock, buf, 1024, 0);
//        if (len == -1) return 0;
//        printf("%s\n", buf);  // ���get����
//
//
//        char url[1024] = { 0 };
//        sscanf(buf, "GET %s HTTP/1.1", url);   // ��������ʽ����ļ�·��
//        printf(" ==> url: %s\n", url);           // ���url
//
//        std::vector<void*> res(10, nullptr);
//        res[0] = &clientsock;
//
//
//        myRoute::RouteABS* r1 = route.get_routeElement(url);
//        r1->dealRequest(buf, buf, res);
//    }
//    */
//	// Sleep(1000);    // Ҫ�ӳ�һ�£����������ҳ��Ҫʱ�䲻�ܣ�ֱ�ӹرշ�����
//
//	closesocket(clientsock);   // ע���ͷ���Դ��˳��
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
    //    clientsock = accept(servsock, (struct sockaddr*)&clientaddr, &addrlen); // �����˵�sockaddr��󶨣������µ�socket
    //    if (clientsock == -1) return -1;
    //    // ����һ���߳�
    //}

    //SOCKET clientsock;
    //struct sockaddr_in clientaddr;
    //int addrlen = sizeof(clientaddr);
    //clientsock = accept(servsock, (struct sockaddr*)&clientaddr, &addrlen); // �����˵�sockaddr��󶨣������µ�socket
    //if (clientsock == -1) return -1;


    myServer::Server server{};  // myServer::Server server("127.10.10.1",11111);
    myServer::SocketItem clientItem01 = server.accept_server();
    RequestMainFunc(&clientItem01.socket);

    
    return 0;
}

