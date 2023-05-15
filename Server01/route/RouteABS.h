

#ifndef _ROUTEABS_H_
#define _ROUTEABS_H_

#include <vector>
#include "../server/Server.h"

namespace myRoute
{ 



class RouteABS
{
public:
	virtual void dealRequest(myServer::SocketItem& clientSockItem, char* httpRequest, std::vector<void*>& publicRes) = 0;
	virtual ~RouteABS() = 0 {};
	
	/*// 说明
	* 以目前我的能力水平，既要实现用route统一管理，有要公共资源的共享，能想出以下3种方法
	* (1) std::vector<void*>存任何资源，再类型转成想要的资源，优点是速度快，公共资源访问方便，缺点是要用户自记下各个下标对应的类型，即类型不安全。
	* (2) 在SQL中为每个用户建一个公共资源表，优点安全，缺点读取速度慢
	* (3) 页面间用http协议传输资源，缺点读取速度慢，要进行字符串的匹配，并多写一些处理资源请求的函数
	* (*4) 目前试着实现方法(1)
	*/
};




}

#endif // !_ROUTEABS_H_






