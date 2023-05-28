#pragma once
#ifndef _INDEX_H_
#define _INDEX_H_


#include <winsock2.h>
#include <string>


#include "../../route/RouteABS.h"
#include "../../imgAlg/DealImage02.h"
#include "../../tool/Tool.h"

namespace myIndex
{


    class Index : public myRoute::RouteABS
    {
    private:

    public:
        void dealRequest(myServer::SocketItem* clientSockItem, myHttp::HttpRequest& httpRequest, std::vector<void*>& publicRes)
        {
            // myHttp::HttpRequest httpRequest1("GET /error/error.html HTTP/1.1\r\n\r\n");

            //if (httpRequest.getContentType() == "application/json") {

            //    char loadFileName[260] = "web/index/res/testimg1.bmp";
            //    char saveFileName[260] = "web\\index\\res\\testimg1_t.bmp";
            //    myImgAlg::IMAGE im = myImgAlg::Image_load(loadFileName);

            //    

            //    int n = std::stoi(httpRequest.getValuebyPost("length"));
            //    const char* info = httpRequest.getHttpRequest();
            //    
            //    const char* funcInfo = info;
            //    const char* parameterInfo = nullptr;

            //    for (int i = 0; i < n; i++) {

            //       funcInfo = strstr(funcInfo, "funcName");
            //       parameterInfo = strstr(funcInfo, "parameter");
            //       std::string func = myTool::getValuebyKey(funcInfo, "funcName");

            //       if (func == "box3_btn2_1") {
            //           myImgAlg::Transform_color_grayscale(im, GRAY_MODE_WEIGHT);
            //       }
            //       //else if () {

            //       //}


            //       funcInfo += 1;
            //    }


            //     myImgAlg::Image_save(saveFileName, im);
            //     myImgAlg::Image_show(saveFileName);
            //}


            myHttp::HttpResponce httpResponce(httpRequest);
            clientSockItem->SendhttpRequest(httpResponce);
        }
    };




}



#endif // ! _INDEX_H_
