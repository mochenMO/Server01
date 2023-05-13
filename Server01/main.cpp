

/*// ��������
*  (1) ��дroute.get_routeElement(url);
*  (2) jsͨ��
*  (3) �ϴ�html,js,css,bmg,json
*  (4) json����
*/





#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <string.h>
#include <vector>

#include "route/Route.h"
#include "server/Server.h"


void RequestMainFunc(myServer::SocketItem& clientSockItem)
{
    myRoute::Route route{};
    
    std::vector<void*> res(10, nullptr);
    //res[0] = clientsock;

    char buf[1024] = { 0 };
    int len = 0;
    char url[1024] = { 0 };

    while (1) {   // Ϊʲô������ҳ����һֱ��תȦȦ�� ��ô���ö�����ҳ������ͼ�ꣿ



        len = recv(clientSockItem.socket, buf, 1024, 0);
        myLog::log(len, -1, "error: recv()ʧ�� ==> clientsock:������ֹͣ");
        // myLog::log(len, -1, "error: myRoute::route::get_fileAddr()ʧ�� ==> " + "������");  // Ϊʲô���У�������
        printf("%s\n", buf);  // ���get����

        sscanf(buf, "GET %s HTTP/1.1", url);   // ��������ʽ����ļ�·��
        printf(" ==> url: %s\n", url);           // ���url


        myRoute::RouteABS* routeElement = route.get_routeElement(url);
        routeElement->dealRequest(clientSockItem, url,buf, res);

    }


    // delete vector element ???????
}



int main()
{

    // ��std::threadʵ��һ���̳߳أ�accept����ҲҪһ���������߳̿�����(�ȿ����ʼ�)��

    myServer::Server server{};  // myServer::Server server("127.10.10.1",11111);
    myServer::SocketItem clientItem01 = server.accept_server();
    RequestMainFunc(clientItem01);


    return 0;
}




