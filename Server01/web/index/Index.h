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

        if (httpRequest.getContentType() == "application/json") {


            std::string loadFileName = "web/index/res" + httpRequest.getValuebyPost("imgName");

            int pos = loadFileName.find("?");
            if (pos != std::string::npos) {  // 找到 ?
                loadFileName = loadFileName.substr(0, pos);
            }
       
            char saveFileName[260] = "web\\index\\res\\testimg1_t.bmp";
            myImgAlg::IMAGE im = myImgAlg::Image_load((char*)loadFileName.c_str());

            int n = std::stoi(httpRequest.getValuebyPost("length"));
            //std::cout << n << std::endl;  //////


            const char* info = httpRequest.getHttpRequest();
            const char* funcInfo = info;
            const char* parameterInfo = nullptr;

            for (int i = 0; i < n; i++) {

                funcInfo = strstr(funcInfo, "funcName");
                parameterInfo = strstr(funcInfo, "parameter");
                std::string func = myTool::getValuebyKey(funcInfo, "funcName");
                std::string parameter = myTool::getValuebyKey(funcInfo, "parameter");

                if (func == "box3_btn1_1") {
                    
                    int k = std::stoi(parameter);

                    //std::cout << func << " " << parameter << std::endl;   /////////
                    //std::cout << im.w << " " << im.w * (k / 100.0) << std::endl;  //////////
                    //std::cout << im.h << " " << im.h * (k / 100.0) << std::endl;  //////////

                    im = myImgAlg::Transform_shape_linear(im, im.w * (k / 100.0), im.h * (k / 100.0));
                }
                else if (func == "box3_btn1_2") {
                    int k = std::stoi(parameter);
                    k = k % 360 + 360;  // 处理k是负数的情况
                    im = myImgAlg::Transform_shape_whirl(im, k);
                }              
                else if (func == "box3_btn1_3") {
                    int k = std::stoi(parameter);
                    if (k > 0) {
                        im = Transform_shape_upturn(im, UPTURN_MODE_HORIZONTAL);
                    }
                    else {
                        im = Transform_shape_upturn(im, UPTURN_MODE_VERTICAL);
                    }
                }              
                else if (func == "box3_btn2_1") {
                    myImgAlg::Transform_color_grayscale(im, GRAY_MODE_WEIGHT);
                }              
                else if (func == "box3_btn2_2") {
                    myImgAlg::Transform_color_BW_OSTU(im);
                }              
                else if (func == "box3_btn2_3") {
                    myImgAlg::Transform_color_opposite(im);
                }               
                else if (func == "box3_btn2_4") {
                    myImgAlg::Transform_color_Histogram_all(im);
                }               
                else if (func == "box3_btn3_1") {
                    im = myImgAlg::Wavefiltering_Gauss(im);
                }                
                else if (func == "box3_btn3_2") {
                    im = myImgAlg::Edge_detection_Sobel(im, myImgAlg::KERNELS_Edge_Sobel_X, myImgAlg::KERNELS_Edge_Sobel_Y);
                }               
                else if (func == "box3_btn4_1") {
                    im = myImgAlg::Pooling(im, 2);
                }                
                else if (func == "box3_btn4_2") {
                    im = myImgAlg::Transform_color_BW_Adaptive(im, 9);
                }                
                else if (func == "box3_btn4_3") {
                    Transform_color_BW_grayscale(im, 9);
                }

                funcInfo += 1;
            }


            myImgAlg::Image_save(saveFileName, im);
            // myImgAlg::Image_show(saveFileName);

            myImgAlg::Image_free(im);
        }


        myHttp::HttpResponce httpResponce(httpRequest);
        clientSockItem->SendhttpRequest(httpResponce);
    };

};











};

#endif // ! _INDEX_H_
