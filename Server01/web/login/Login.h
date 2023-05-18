#pragma once
#ifndef _LOGIN_H_
#define _LOGIN_H_

#include <string>

#include "../../route/RouteABS.h"


namespace myLogin
{


class Login : public myRoute::RouteABS
{
private:

public:
    void dealRequest(myServer::SocketItem* clientSockItem, myHttp::HttpRequest& httpRequest, std::vector<void*>& publicRes)
    {
        myHttp::HttpResponce httpResponce(httpRequest);

        if (httpRequest.getValuebyPost("click") == "login")   // ����˵�¼��ť
        {   
            std::string userName = httpRequest.getValuebyPost("userName");
            std::string passWord = httpRequest.getValuebyPost("passWord");
            if (userName == "admin" && passWord == "qwert") {    // Ҫ�õ�SQL Server ������������
                httpResponce.addCookie("userName", userName);
            }
        }
        else if (httpRequest.findAttribute("Cookie") == false) {  // û��cookie
            httpResponce.addCookie("userName", "nulluser");
        }

        clientSockItem->SendhttpRequest(httpResponce, httpRequest);
    }


};





}



#endif // ! _LOGIN_H_





