

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

    char* buf = new char[10240]();     // ����http����ĳ������Ϊ10kb���öѿռ�洢,Ҫ��()��ʾ��0��䣩
    char* url = new char[10240]();     // ����url��Ϣ
    int len = 0;  // ����recv�ķ���ֵ
    

    while (1) {   // Ϊʲô������ҳ����һֱ��תȦȦ�� ��ô���ö�����ҳ������ͼ�ꣿ
        len = recv(clientSockItem.socket, buf, 10240, 0);   
        myLog::log(len, -1, "error: recv()ʧ�� ==> clientsock:������ֹͣ");
        buf[len + 1] = '\0';   // ע�⣺��Ϊrecvÿ�θ���ԭ���ݲ����ڽ�β��'\0'������Ҫ�Լ�����
        sscanf(buf, "GET %s HTTP/1.1", url);   // ��������ʽ���url����ÿ�λ��url��β��'\0'


        std::cout << buf << std::endl;                         // ���http����
        std::cout << "==> len: " << len << std::endl;          // ���http���󱨵ĳ���
        std::cout << "==> url: " << url << std::endl;          // ���url
        std::cout << "==============================================" 
                  << "============================================\n" << std::endl;  // �ָ��߱��ڲο����

        // ��url����strstr������Ϊurl�ǿ���Я�������ģ�����������������������

        myRoute::RouteABS* routeElement = route.get_routeElement(url);   // ����url����ָ����·��
        routeElement->dealRequest(clientSockItem, url,buf, res);         // ����ָ����·�ɴ���

    }


    delete[] url;
    delete[] buf;

    // vector ��û������ ???????
}



int main()
{

    // ��std::threadʵ��һ���̳߳أ�accept����ҲҪһ���������߳̿�����(�ȿ����ʼ�)��

    myServer::Server server{};  // myServer::Server server("127.10.10.1",11111);
    myServer::SocketItem clientItem01 = server.accept_server();
    RequestMainFunc(clientItem01);


    return 0;
}




