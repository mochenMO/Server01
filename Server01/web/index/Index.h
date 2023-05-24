#pragma once
#ifndef _INDEX_H_
#define _INDEX_H_


#include <winsock2.h>
#include <string>


#include "../../route/RouteABS.h"


namespace myIndex
{


    class Index : public myRoute::RouteABS
    {
    private:

    public:
        void dealRequest(myServer::SocketItem* clientSockItem, myHttp::HttpRequest& httpRequest, std::vector<void*>& publicRes)
        {
            myHttp::HttpRequest httpRequest1("GET /error/error.html HTTP/1.1\r\n\r\n");
            myHttp::HttpResponce httpResponce(httpRequest);
            clientSockItem->SendhttpRequest(httpResponce, httpRequest1);
        }
    };




}



#endif // ! _INDEX_H_
