

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

    char* buf = new char[10240]();     // 设置http请求的长度最大为10kb（用堆空间存储,要加()表示用0填充）
    char* url = new char[10240]();     // 保存url信息
    int len = 0;  // 接收recv的返回值
    

    while (1) {   // 为什么顶部的页面栏一直在转圈圈？ 怎么设置顶部的页面栏的图标？
        len = recv(clientSockItem.socket, buf, 10240, 0);   
        myLog::log(len, -1, "error: recv()失败 ==> clientsock:连接以停止");
        buf[len + 1] = '\0';   // 注意：因为recv每次覆盖原数据不会在结尾加'\0'，所以要自己加上
        sscanf(buf, "GET %s HTTP/1.1", url);   // 用正则表达式获得url，它每次会给url结尾加'\0'


        std::cout << buf << std::endl;                         // 输出http请求报
        std::cout << "==> len: " << len << std::endl;          // 输出http请求报的长度
        std::cout << "==> url: " << url << std::endl;          // 输出url
        std::cout << "==============================================" 
                  << "============================================\n" << std::endl;  // 分割线便于参看输出

        // 对url进行strstr处理，因为url是可以携带参数的？？？？？？？？？？？？

        myRoute::RouteABS* routeElement = route.get_routeElement(url);   // 根据url搜索指定的路由
        routeElement->dealRequest(clientSockItem, url,buf, res);         // 交给指定的路由处理

    }


    delete[] url;
    delete[] buf;

    // vector 还没有销毁 ???????
}



int main()
{

    // 用std::thread实现一个线程池，accept操作也要一个单独的线程控制吗(先看看笔记)。

    myServer::Server server{};  // myServer::Server server("127.10.10.1",11111);
    myServer::SocketItem clientItem01 = server.accept_server();
    RequestMainFunc(clientItem01);


    return 0;
}




