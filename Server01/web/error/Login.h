
#pragma once
#ifndef _LOGIN_H_
#define _LOGIN_H_


#include <winsock2.h>
#include <string>


#include "../../route/RouteABS.h"


namespace myLogin
{


    class Login : public myRoute::RouteABS
    {
    private:

    public:
        //void dealRequest(myServer::SocketItem& clientSockItem, char* httpRequest, std::vector<void*>& publicRes) 
        //{
        //    std::string responseHeader = "HTTP/1.1 200 OK\r\n";
        //    char userName[128] = { 0 };
        //    char passWord[128] = { 0 };
        //    
        //    if (myHttp::findAttribute(httpRequest, "Cookie") == true) {    // application/json
        //        myHttp::getValuebyPost(httpRequest, "userName", userName); 
        //        myHttp::getValuebyPost(httpRequest, "passWord", passWord);
        //    }
        //    
        //    if (strcmp(userName, "admin") == 0 && strcmp(passWord, "qwert") == 0) {
        //        // responseHeader = responseHeader + "Set-Cookie: userName="+ std::string(userName) + ";" + "expires=Mon, 21 Sep 2037 00:00:01 GMT\r\n";
        //    }
        //    else {
        //        // responseHeader = responseHeader + "Set-Cookie: userName=nulluser;" + "expires=Mon, 21 Sep 2037 00:00:01 GMT\r\n";
        //    }
        //    
        //    char fileName[256] = { 0 };
        //    myHttp::getFileName(httpRequest, fileName);

        //    // std::cout << fileName << std::endl;

        //    if (strcmp(fileName, "/") == 0) {
        //        strcpy(fileName, "/login/login.html");
        //    }
        //    responseHeader = responseHeader + "Content-Type:" + myHttp::getContentType(httpRequest) + "\r\n\r\n";

        //    myHttp::SendhttpRequest(clientSockItem, responseHeader.c_str(), responseHeader.length(),fileName);
        //}



        void dealRequest(myServer::SocketItem* clientSockItem, myHttp::HttpRequest& httpRequest, std::vector<void*>& publicRes)
        {
            myHttp::HttpResponce httpResponce(httpRequest);

            // std::string responseHeader = "HTTP/1.1 200 OK\r\n";

            std::string userName;
            std::string passWord;

            if (httpRequest.findAttribute("Cookie") == true) {    // application/json
                // userName = httpRequest.getValuebyPost("userName");
                // passWord = httpRequest.getValuebyPost("passWord");
                std::cout << userName << " " << passWord << std::endl;

                if (userName == "admin" && passWord == "qwert") {
                    // responseHeader = responseHeader + "Set-Cookie: userName="+ std::string(userName) + ";" + "expires=Mon, 21 Sep 2037 00:00:01 GMT\r\n";
                    httpResponce.addCookie("userName", userName);
                }
            }
            else {
                // responseHeader = responseHeader + "Set-Cookie: userName=nulluser;" + "expires=Mon, 21 Sep 2037 00:00:01 GMT\r\n";
                httpResponce.addCookie("userName", "nulluser");
            }

            std::string fileName(128, '\0');
            fileName = httpRequest.getFileName();

            // std::cout << fileName << std::endl;

            if (fileName  == "/") {
                fileName = "/login/login.html";
            }
            // responseHeader = responseHeader + "Content-Type:" + httpRequest.getContentType() + "\r\n\r\n";

           

            clientSockItem->SendhttpRequest_File(httpResponce, fileName);

            //if (httpRequest.getContentType() != myHttp::contentType["json"]) {
            //    clientSockItem->SendhttpRequest_File(httpResponce, fileName);
            //}
            //else {
            //    myJson::Json json;
            //    json.addJsonValue(); /////
            //    clientSockItem->SendhttpRequest_Json(httpResponce, json);
            //}

            //myHttp::S(clientSockItem, responseHeader.c_str(), responseHeader.length(), fileName);
        }


    };




}



#endif // ! _LOGIN_H_