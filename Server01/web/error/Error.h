#pragma once
#ifndef _ERROR_H_
#define _ERROR_H_


#include <winsock2.h>
#include <string>


#include "../../route/RouteABS.h"


namespace myError
{


class Error : public myRoute::RouteABS
{
private:

public:
    void dealRequest(myServer::SocketItem* clientSockItem, myHttp::HttpRequest& httpRequest, std::vector<void*>& publicRes)
    {
        myHttp::HttpResponce httpResponce(httpRequest);
        httpResponce.setPageName("/error");

        if (httpResponce.getContentType() == "text/html") {
            httpResponce.setFileName("/error.html");
        }

        clientSockItem->SendhttpRequest(httpResponce);
    }
};




}



#endif // ! _ERROR_H_








