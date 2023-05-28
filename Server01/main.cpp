

/*// ��������
*  (1) ��дroute.get_routeElement(url);
*  (2) jsͨ��
*  (3) �ϴ�html,js,css,bmg,json
*  (4) json����
*  (5) F12��ϸ�÷�
*/



#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <string.h>
#include <vector>
// #include <msdasql.h>

#include "route/Route.h"
#include "server/Server.h"
#include "http/Http.h"
#include "sql/MySql.h"


void RequestMainFunc(myServer::SocketItem* clientSockItem, myRoute::Route& route, std::vector<void*>& res)
{
    char *recvBuffer = new char[10240]();   // ��()��ʾ��0���
    int len = recv(clientSockItem->socket, recvBuffer, 10239, 0);
    std::cout << recvBuffer << std::endl;
    

    myHttp::HttpRequest httpRequest(recvBuffer);


    route.distribute_route(clientSockItem, httpRequest, res);

   
    closesocket(clientSockItem->socket);
    delete[] recvBuffer;
    delete clientSockItem;
    std::cout << "��������ϣ������ѶϿ�" << std::endl;
    std::cout << "==========================================\n" << std::endl;
}

int main()
{

    // ��std::threadʵ��һ���̳߳أ�accept����ҲҪһ���������߳̿�����(�ȿ����ʼ�)��


    myServer::Server server{};  // myServer::Server server("127.10.10.1",11111);

    myRoute::Route route{};
    std::vector<void*> res(10, nullptr);


    MySql::SqlObject sqlObject("mytest", "mo", "qwert");
    res[0] = &sqlObject;

    while (1) {
        // myServer::SocketItem clientItem01 = server.accept_server();  // Ϊʲô������ҳ����һֱ��תȦȦ�� ��ô���ö�����ҳ������ͼ�ꣿ
        // RequestMainFunc(clientItem01);   // ��ô����clientItem01 ����������
        
        std::cout << "�ȴ�������..." << std::endl;
        myServer::SocketItem* sockItem = server.accept_server();
        std::cout << "�����ӿͻ���" << std::endl;
        RequestMainFunc(sockItem, route, res);
    }

    
    


    return 0;
}




