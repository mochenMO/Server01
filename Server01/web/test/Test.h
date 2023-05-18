#pragma once
#ifndef _TEST_H_
#define _TEST_H_


#include <winsock2.h>
#include <string>


#include "../../route/RouteABS.h"

namespace myTest
{ 


class Test : public myRoute::RouteABS
{
private:
    
public:

	void dealRequest(myServer::SocketItem& clientSockItem, char* url, char* httpRequest, std::vector<void*>& publicRes) {


        char responseHeader[] = "HTTP / 1.1 200 OK\r\nContent-Type: text/html \r\n\r\n";  // ע�⣺httpͷ��(\r\n\r\n) + html�ļ���������ʽ������Ч�������html�ļ����뵽ͷ���н��д��䣬��html��ʧЧ, // �ֶ���+ð��+�ո�+�ֶ�ֵ �ĸ�ʽ
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
};




}



#endif // ! _TEST_H_





