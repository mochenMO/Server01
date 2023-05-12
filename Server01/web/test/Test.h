

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

        char responseHeader[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html \r\n\r\n";  // 注意：http头部(\r\n\r\n) + html文件，这种形式才能生效，如果把html文件插入到头部中进行传输，则html会失效
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