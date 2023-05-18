
#pragma once
#ifndef _DEFAULT_H_
#define _DEFAULT_H_

#include <string>

#include "../../route/RouteABS.h"


namespace myDefault
{


class Default : public myRoute::RouteABS
{
private:

public:
    void dealRequest(myServer::SocketItem* clientSockItem, myHttp::HttpRequest& httpRequest, std::vector<void*>& publicRes)
    {
        myHttp::HttpResponce httpResponce(httpRequest);
        clientSockItem->SendhttpRequest(httpResponce, httpRequest);
    }


};





}



#endif // ! _DEFAULT_H_