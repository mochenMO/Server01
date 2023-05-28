#pragma once
#ifndef _LOGIN_H_
#define _LOGIN_H_

#include <string>

#include "../../route/RouteABS.h"
#include "../../sql/MySql.h"
#include "../../tool/Tool.h"

using namespace myTool;


namespace myLogin
{


class Login : public myRoute::RouteABS
{
private:

public:
    void dealRequest(myServer::SocketItem* clientSockItem, myHttp::HttpRequest& httpRequest, std::vector<void*>& publicRes)
    {
        myHttp::HttpResponce httpResponce(httpRequest);

        if (httpRequest.getValuebyPost("click") == "login" && httpRequest.findAttribute("Cookie") == true)   // 点击了登录按钮
        {   
            std::string userName = httpRequest.getValuebyPost("username");
            std::string passWord = httpRequest.getValuebyPost("password");
            

            std::string commend = "select * from user_tb where username='" + userName + "'";
            MySql::SqlObject* sqlObject = (MySql::SqlObject*)publicRes[0];
            MySql::SqlData data(3);
            bool res = sqlObject->select_sql(commend.c_str(), data);
            
            //printf("%s", data[2].data);

            if (res == true && passWord == data[2].data && userName.empty() == false) {    // 要用到SQL Server ？？？？？？
                httpResponce.addCookie("username", userName);
            }
        }
        else if (httpRequest.findAttribute("Cookie") == false) {  // 没有cookie
            httpResponce.addCookie("username", "nulluser");
        }

        clientSockItem->SendhttpRequest(httpResponce);
    }


};





}



#endif // ! _LOGIN_H_





