

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

#include "route/Route.h"
#include "server/Server.h"
#include "http/Http.h"



//void RequestMainFunc(myServer::SocketItem& clientSockItem)
//{
//    myRoute::Route route{};
//    
//    std::vector<void*> res(10, nullptr);
//    //res[0] = clientsock;
//
//    char* buf = new char[10240]();     // ����http����ĳ������Ϊ10kb���öѿռ�洢,Ҫ��()��ʾ��0��䣩
//    char* url = new char[4096]();     // ����url��Ϣ
//    char* pageName = new char[256]();  // ����ҳ������
//    int len = 0;                       // ����recv�ķ���ֵ
//    
//
//    
//
//    // while (1) {   
//        len = recv(clientSockItem.socket, buf, 10240, 0);   
//        myLog::log(len, -1, "error: recv()ʧ�� ==> clientsock:������ֹͣ");
//        buf[len + 1] = '\0';   // ע�⣺��Ϊrecvÿ�θ���ԭ���ݲ����ڽ�β��'\0'������Ҫ�Լ�����
//        
//       
//        sscanf(buf, "GET %s HTTP/1.1", url);   // ��������ʽ���url����ÿ�λ��url��β��'\0'
//        myHttp::getPageName(url, pageName);    // ���Ŀ��ҳ�������
//
//        std::cout << buf << std::endl;                           // ���http����
//        std::cout << "==> len: " << len << std::endl;            // ���http���󱨵ĳ���
//        std::cout << "==> url: " << url << std::endl;            // ���url
//        std::cout << "==> pageName: " << pageName << std::endl;  // ���Ŀ��ҳ�������
//        std::cout << "==============================================" 
//                  << "============================================\n" << std::endl;  // �ָ��߱��ڲο����
//
//        // ��url����strstr������Ϊurl�ǿ���Я�������ģ�����������������������
//
//
//
//
//        myRoute::RouteABS* routeElement = route.get_routeElement(pageName);   // ����url����ָ����·��
//        routeElement->dealRequest(clientSockItem, url,buf, res);         // ����ָ����·�ɴ���
//
//   // }
//
//
//    delete[] url;
//    delete[] buf;
//
//    // vector ��û������ ???????
//}


void RequestMainFunc(myServer::SocketItem* clientSockItem, myRoute::Route& route, std::vector<void*>& res)
{
    char *recvBuffer = new char[4096]();   // ��()��ʾ��0���
    int len = recv(clientSockItem->socket, recvBuffer, 4096, 0);
    std::cout << recvBuffer << std::endl;

    myHttp::HttpRequest httpRequest(recvBuffer);


    route.distribute_route(clientSockItem, httpRequest, res);

   
    closesocket(clientSockItem->socket);
    delete recvBuffer;
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




