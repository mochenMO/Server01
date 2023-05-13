#pragma once
#ifndef _HTTP_H_
#define _HTTP_H_

//#include<iostream>
//#include<string>



namespace myHttp
{


bool SendhttpRequest(,const char* fileName)
{
    char responseHeader[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html \r\n\r\n";  // 注意：http头部(\r\n\r\n) + html文件，这种形式才能生效，如果把html文件插入到头部中进行传输，则html会失效
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




}







#endif  // !_HTTP_H_


