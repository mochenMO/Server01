

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



//void RequestMainFunc(myServer::SocketItem& clientSockItem)
//{
//    myRoute::Route route{};
//    
//    std::vector<void*> res(10, nullptr);
//    //res[0] = clientsock;
//
//    char* buf = new char[10240]();     // 设置http请求的长度最大为10kb（用堆空间存储,要加()表示用0填充）
//    char* url = new char[4096]();     // 保存url信息
//    char* pageName = new char[256]();  // 保存页面名称
//    int len = 0;                       // 接收recv的返回值
//    
//
//    
//
//    // while (1) {   
//        len = recv(clientSockItem.socket, buf, 10240, 0);   
//        myLog::log(len, -1, "error: recv()失败 ==> clientsock:连接以停止");
//        buf[len + 1] = '\0';   // 注意：因为recv每次覆盖原数据不会在结尾加'\0'，所以要自己加上
//        
//       
//        sscanf(buf, "GET %s HTTP/1.1", url);   // 用正则表达式获得url，它每次会给url结尾加'\0'
//        myHttp::getPageName(url, pageName);    // 获得目标页面的名称
//
//        std::cout << buf << std::endl;                           // 输出http请求报
//        std::cout << "==> len: " << len << std::endl;            // 输出http请求报的长度
//        std::cout << "==> url: " << url << std::endl;            // 输出url
//        std::cout << "==> pageName: " << pageName << std::endl;  // 输出目标页面的名称
//        std::cout << "==============================================" 
//                  << "============================================\n" << std::endl;  // 分割线便于参看输出
//
//        // 对url进行strstr处理，因为url是可以携带参数的？？？？？？？？？？？？
//
//
//
//
//        myRoute::RouteABS* routeElement = route.get_routeElement(pageName);   // 根据url搜索指定的路由
//        routeElement->dealRequest(clientSockItem, url,buf, res);         // 交给指定的路由处理
//
//   // }
//
//
//    delete[] url;
//    delete[] buf;
//
//    // vector 还没有销毁 ???????
//}


void RequestMainFunc(myServer::SocketItem* clientSockItem, myRoute::Route& route, std::vector<void*>& res)
{
    char recvBuffer[4096] = { 0 };   // 加()表示用0填充
    int len = recv(clientSockItem->socket, recvBuffer, sizeof(recvBuffer), 0);
    std::cout << recvBuffer << std::endl;

    route.distribute_route(*clientSockItem, recvBuffer, res);

   
    closesocket(clientSockItem->socket);
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




