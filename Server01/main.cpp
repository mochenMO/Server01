

/*// 开发任务
*  (1) 改写route.get_routeElement(url);
*  (2) js通信
*  (3) 上传html,js,css,bmg,json
*  (4) json解析
*/





#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <string.h>
#include <vector>

#include "route/Route.h"
#include "server/Server.h"


void RequestMainFunc(myServer::SocketItem& clientSockItem)
{
    myRoute::Route route{};
    
    std::vector<void*> res(10, nullptr);
    //res[0] = clientsock;

    char buf[1024] = { 0 };
    int len = 0;
    char url[1024] = { 0 };

    while (1) {   // 为什么顶部的页面栏一直在转圈圈？ 怎么设置顶部的页面栏的图标？



        len = recv(clientSockItem.socket, buf, 1024, 0);
        myLog::log(len, -1, "error: recv()失败 ==> clientsock:连接以停止");
        // myLog::log(len, -1, "error: myRoute::route::get_fileAddr()失败 ==> " + "不存在");  // 为什么不行？？？？
        printf("%s\n", buf);  // 输出get请求报

        sscanf(buf, "GET %s HTTP/1.1", url);   // 用正则表达式获得文件路径
        printf(" ==> url: %s\n", url);           // 输出url


        myRoute::RouteABS* routeElement = route.get_routeElement(url);
        routeElement->dealRequest(clientSockItem, url,buf, res);

    }


    // delete vector element ???????
}



int main()
{

    // 用std::thread实现一个线程池，accept操作也要一个单独的线程控制吗(先看看笔记)。

    myServer::Server server{};  // myServer::Server server("127.10.10.1",11111);
    myServer::SocketItem clientItem01 = server.accept_server();
    RequestMainFunc(clientItem01);


    return 0;
}




