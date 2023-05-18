

/*// 开发任务
*  (1) 改写route.get_routeElement(url);
*  (2) js通信
*  (3) 上传html,js,css,bmg,json
*  (4) json解析
*  (5) F12详细用法
*/



#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <string.h>
#include <vector>

#include "route/Route.h"
#include "server/Server.h"
#include "http/Http.h"



void RequestMainFunc(myServer::SocketItem* clientSockItem, myRoute::Route& route, std::vector<void*>& res)
{
    char *recvBuffer = new char[4096]();   // 加()表示用0填充
    int len = recv(clientSockItem->socket, recvBuffer, 4096, 0);
    std::cout << recvBuffer << std::endl;

    myHttp::HttpRequest httpRequest(recvBuffer);


    route.distribute_route(clientSockItem, httpRequest, res);

   
    closesocket(clientSockItem->socket);
    delete recvBuffer;
    delete clientSockItem;
    std::cout << "请求处理完毕，连接已断开" << std::endl;
    std::cout << "==========================================\n" << std::endl;
}

int main()
{

    // 用std::thread实现一个线程池，accept操作也要一个单独的线程控制吗(先看看笔记)。


    myServer::Server server{};  // myServer::Server server("127.10.10.1",11111);

    myRoute::Route route{};
    std::vector<void*> res(10, nullptr);

    while (1) {
        // myServer::SocketItem clientItem01 = server.accept_server();  // 为什么顶部的页面栏一直在转圈圈？ 怎么设置顶部的页面栏的图标？
        // RequestMainFunc(clientItem01);   // 怎么回收clientItem01 ？？？？？
        
        std::cout << "等待连接中..." << std::endl;
        myServer::SocketItem* sockItem = server.accept_server();
        std::cout << "已连接客户端" << std::endl;
        RequestMainFunc(sockItem, route, res);
    }

    
    


    return 0;
}




