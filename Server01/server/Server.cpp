#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <string.h>
#include <vector>


#include <ws2tcpip.h>    // ����inet_pton��inet_ntop�Ⱥ���
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
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);   // ͬ servaddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);


	res = bind(servsock, (struct sockaddr*)&servaddr, sizeof(servaddr));
	if (res == -1) return -1;

	res = listen(servsock, 4);
	if (res == -1) return -1;

    
    //while (1) {
    //    SOCKET clientsock;   // new
    //    struct sockaddr_in clientaddr;  // new
    //    int addrlen = sizeof(clientaddr);
    //    clientsock = accept(servsock, (struct sockaddr*)&clientaddr, &addrlen); // �����˵�sockaddr��󶨣������µ�socket
    //    if (clientsock == -1) return -1;
    //    // ����һ���߳�
    //}
    
    SOCKET clientsock;
    struct sockaddr_in clientaddr;
    int addrlen = sizeof(clientaddr);
    clientsock = accept(servsock, (struct sockaddr*)&clientaddr, &addrlen); // �����˵�sockaddr��󶨣������µ�socket
    if (clientsock == -1) return -1;


    RequestMainFunc(&clientsock);

    /*

    myRoute::Route route{};

    while (1) {   // Ϊʲô������ҳ����һֱ��תȦȦ�� ��ô���ö�����ҳ������ͼ�ꣿ

        char buf[1024] = { 0 };
        int len = 0;

        len = recv(clientsock, buf, 1024, 0);
        if (len == -1) return 0;
        printf("%s\n", buf);  // ���get����


        char url[1024] = { 0 };
        sscanf(buf, "GET %s HTTP/1.1", url);   // ��������ʽ����ļ�·��
        printf(" ==> url: %s\n", url);           // ���url

        std::vector<void*> res(10, nullptr);
        res[0] = &clientsock;


        myRoute::RouteABS* r1 = route.get_routeElement(url);
        r1->dealRequest(buf, buf, res);
    }
    */
	// Sleep(1000);    // Ҫ�ӳ�һ�£����������ҳ��Ҫʱ�䲻�ܣ�ֱ�ӹرշ�����

	closesocket(clientsock);   // ע���ͷ���Դ��˳��
	closesocket(servsock);
	WSACleanup();
	return 0;
}


/*// �ο�����

#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")  // ���� Winsock2 ��

int main()
{
    WSADATA wsaData;                    // �洢 Winsock ���ʼ����Ϣ�Ľṹ��
    SOCKET listenSocket = INVALID_SOCKET;// �����ͻ�������������׽��ֶ���
    SOCKET clientSocket = INVALID_SOCKET;// ��ͻ��˽������ݽ������׽��ֶ���
    struct addrinfo* result = NULL;     // �����������ĵ�ַ��Ϣ�б�
    struct addrinfo hints;              // ���ӵĵ�ַ��Ϣ�ṹ��
    int ret;

    // ��ʼ�� Winsock ��
    ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (ret != 0)
    {
        printf("WSAStartup failed with error: %d\n", ret);
        return 1;
    }

    // ����ʼ��ַ��Ϣ�ṹ�壨��֧�� IPv4��
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;          // IPv4 ��ַ��
    hints.ai_socktype = SOCK_STREAM;    // ��ʽ�׽���
    hints.ai_protocol = IPPROTO_TCP;    // TCP Э��
    hints.ai_flags = AI_PASSIVE;        // ��������ģʽ

    // ��ȡ�����ӵı��� IP ��ַ�Ͷ˿ں�
    ret = getaddrinfo(NULL, "8080", &hints, &result);
    if (ret != 0)
    {
        printf("getaddrinfo failed with error: %d\n", ret);
        WSACleanup();
        return 1;
    }

    // �����׽��ֶ��󲢰󶨵����� IP ��ַ�Ͷ˿ں���
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

    // �������� IP ��ַ��ָ���Ķ˿ڣ��ȴ���������
    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    // ѭ������ͻ�������
    while (true)
    {
        // ���տͻ����������󣬲������µ��׽��ֶ������ڽ�������
        clientSocket = accept(listenSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET)
        {
            printf("accept failed with error: %d\n", WSAGetLastError());
            closesocket(listenSocket);
            WSACleanup();
            return 1;
        }

        // ��֯ HTTP ��Ӧ���Ĳ����͸��ͻ���
        char responseHeader[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
        send(clientSocket, responseHeader, sizeof(responseHeader), 0);

        // ����ָ���� HTML �ļ����ݣ�ע��Ҫʹ�ö����Ʒ�ʽ��ȡ�ļ���
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

        // �ر���ͻ��˵����Ӻ��׽��ֶ���
        closesocket(clientSocket);
    }

    // �رռ����׽��ֶ������ Winsock ���������
    closesocket(listenSocket);
    WSACleanup();

    return 0;
}
*/