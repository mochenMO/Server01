

#ifndef _ROUTE_H_
#define _ROUTE_H_

#include <unordered_map>
#include "RouteABS.h"
#include "../log/log.h"
// #include<hash_map>  // 已弃用


#include "../web/test/Test.h"

namespace myRoute
{


class Route
{
private:
	std::unordered_map<std::string, RouteABS*> routeMap;   // 注意：RouteABS*是指针

public:

	Route() :routeMap() {
		routeMap.insert({ "/", new myTest::Test });
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

	RouteABS* get_routeElement(const std::string& url) {
		auto res = routeMap.find(url);
		myLog::log(res, routeMap.end(), "error: myRoute::route::get_fileAddr()失败 ==> " + url + "不存在");
		return res->second;  // res的第二个值存的是与该key值绑定的值
	}

	void  set_routeElement(const std::string& url, RouteABS* routeElement) {
		auto res = routeMap.find(url);
		myLog::log(res, routeMap.end(), "error: myRoute::route::set_fileAddr()失败 ==> " + url + "不存在");
		// res->second = newFileAddr;  // res的第二个值存的是与该key值绑定的值
		routeMap[url] = routeElement;
	}

	bool add_routeElement(const std::string & url, RouteABS* routeElement) {
		auto res = routeMap.insert({ url, routeElement });
		return res.second;    // res的第二个值是bool,表示是否成功
	}

};





}

#endif // !_ROUTE_H_

