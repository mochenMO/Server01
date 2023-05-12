

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

	void dealRequest(char* inputHttpReq, char* outputHttpReq, std::vector<void*> publicRes) {


        SOCKET clientsock = *((SOCKET*)publicRes[0]);

        char responseHeader[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html \r\n\r\n";  // ע�⣺httpͷ��(\r\n\r\n) + html�ļ���������ʽ������Ч�������html�ļ����뵽ͷ���н��д��䣬��html��ʧЧ
        send(clientsock, responseHeader, sizeof(responseHeader), 0);

        FILE* fp;
        // fopen_s(&fp, fileaddr.c_str(), "rb");

        fopen_s(&fp, "web//test//test.html", "rb");

        char buffer[1024];
        int bytesRead;
        while ((bytesRead = fread(buffer, sizeof(char), sizeof(buffer), fp)) > 0)
        {
            send(clientsock, buffer, bytesRead, 0);
        }
        fclose(fp);
	}
};




}



#endif // ! _TEST_H_