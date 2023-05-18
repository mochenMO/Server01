#pragma once
#ifndef _ROUTE_H_
#define _ROUTE_H_

#include <unordered_map>
#include <vector>
// #include<hash_map>  // 已弃用

#include "RouteABS.h"
#include "../log/Log.h"
#include "../http/Http.h"
#include "../server/Server.h"


#include "../web/login/Login.h"
#include "../web/default/Default.h"



namespace myRoute
{


class Route
{
private:
	std::unordered_map<std::string, RouteABS*> routeMap;   // 注意：RouteABS*是指针
public:

	Route() :routeMap() {
		routeMap.insert({ "/default", new myDefault::Default });
		routeMap.insert({ "/login", new myLogin::Login });
	};
	
	~Route() {
		for (auto itr = routeMap.begin(); itr != routeMap.end(); ++itr) {
			if(itr->second != nullptr) {
				delete itr->second;     // 怎么处理 new[] ??????
			}
		}

	};

	// 能进行拷贝操作吗？(不能，因为unordered_map不能进行拷贝)
	Route(const Route&) = delete;
	Route& operator=(const Route&) = delete;  // 禁用拷贝赋值运算符
	// 能进行移动操作吗？
	// 怎么处理const class？

	RouteABS* get_routeElement(const std::string& pageName) {
		auto res = routeMap.find(pageName);
		// myLog::log(res, routeMap.end(), "error: myRoute::route::get_routeElement()失败 ==> " + pageName + "不存在");	
		if (res == routeMap.end()){
			res = routeMap.find("/error");  // 404页面
		}
		return res->second;  // res的第二个值存的是与该key值绑定的值
	}

	void  set_routeElement(const std::string& pageName, RouteABS* routeElement) {
		auto res = routeMap.find(pageName);
		myLog::log(res, routeMap.end(), "error: myRoute::route::set_routeElement()失败 ==> " + pageName + "不存在");
		// res->second = newFileAddr;  // res的第二个值存的是与该key值绑定的值
		routeMap[pageName] = routeElement;
	}

	bool add_routeElement(const std::string& pageName, RouteABS* routeElement) {
		auto res = routeMap.insert({ pageName, routeElement });
		return res.second;    // res的第二个值是bool,表示是否成功
	}

	void distribute_route(myServer::SocketItem* socketItem, myHttp::HttpRequest& httpRequest, std::vector<void*>& publicRes)
	{
		
		RouteABS* pageRoute;
		// 获得pageName
		std::string pageName = httpRequest.getPageName();

		if (httpRequest.findAttribute("Cookie") == false) {   // 该页面没有Cookie,即没有初始化
			pageRoute = get_routeElement("/login");
		}
		else {   // 该页面有Cookie
			if (httpRequest.getValuebyCookie("userName") == "nulluser") {   // username==nulluser，用户未登录
				pageRoute = get_routeElement("/login");
			}
			else {  // 用户已登录 
				pageRoute = get_routeElement(pageName);
			}
		}
		// 交由指定的页面进行处理
		pageRoute->dealRequest(socketItem, httpRequest, publicRes);
	}

};





}

#endif // !_ROUTE_H_




