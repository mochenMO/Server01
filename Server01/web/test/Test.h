

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


        char responseHeader[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html \r\n\r\n";  // 注意：http头部(\r\n\r\n) + html文件，这种形式才能生效，如果把html文件插入到头部中进行传输，则html会失效
        send(clientSockItem.socket, responseHeader, sizeof(responseHeader), 0);

        FILE* fp;
        // fopen_s(&fp, fileaddr.c_str(), "rb");

        // fopen_s(&fp, "web//test//test.html", "rb");
        // fopen_s(&fp, "web//login//login.html", "rb");

        fopen_s(&fp, "web//test//button.html", "rb");

        char buffer[1024];
        int bytesRead;
        while ((bytesRead = fread(buffer, sizeof(char), sizeof(buffer), fp)) > 0)
        {
            send(clientSockItem.socket, buffer, bytesRead, 0);
        }

        fclose(fp);




        //char responseHeader2[] = "HTTP/1.1 200 OK\r\nContent-Type: text/css \r\n\r\n";  // 注意：http头部(\r\n\r\n) + html文件，这种形式才能生效，如果把html文件插入到头部中进行传输，则html会失效
        //send(clientSockItem.socket, responseHeader2, sizeof(responseHeader2), 0);

        //FILE* fp1;
        //// fopen_s(&fp, fileaddr.c_str(), "rb");

        //// fopen_s(&fp, "web//test//test.html", "rb");
        //fopen_s(&fp1, "web//login//login.css", "rb");

        //char buffer1[1024];
        //int bytesRead1;
        //while ((bytesRead1 = fread(buffer1, sizeof(char), sizeof(buffer1), fp1)) > 0)
        //{
        //    send(clientSockItem.socket, buffer1, bytesRead1, 0);
        //}


        //std::cout << "sdfdf" << std::endl;

        //fclose(fp1);
	}
};




}



#endif // ! _TEST_H_