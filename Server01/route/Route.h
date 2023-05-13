

#ifndef _ROUTE_H_
#define _ROUTE_H_

#include <unordered_map>
#include "RouteABS.h"
#include "../log/log.h"
// #include<hash_map>  // ������


#include "../web/test/Test.h"

namespace myRoute
{


class Route
{
private:
	std::unordered_map<std::string, RouteABS*> routeMap;   // ע�⣺RouteABS*��ָ��

public:

	Route() :routeMap() {
		routeMap.insert({ "/", new myTest::Test });
	};

	~Route() {
		for (auto itr = routeMap.begin(); itr != routeMap.end(); ++itr) {
			if(itr->second != nullptr) {
				delete itr->second;     // ��ô���� new[] ??????
			}
		}

	};

	// �ܽ��п���������(���ܣ���Ϊunordered_map���ܽ��п���)
	Route(const Route&) = delete;
	Route& operator=(const Route&) = delete;  // ���ÿ�����ֵ�����
	// �ܽ����ƶ�������
	// ��ô����const class��

	RouteABS* get_routeElement(const std::string& url) {
		auto res = routeMap.find(url);
		myLog::log(res, routeMap.end(), "error: myRoute::route::get_fileAddr()ʧ�� ==> " + url + "������");
		return res->second;  // res�ĵڶ���ֵ��������keyֵ�󶨵�ֵ
	}

	void  set_routeElement(const std::string& url, RouteABS* routeElement) {
		auto res = routeMap.find(url);
		myLog::log(res, routeMap.end(), "error: myRoute::route::set_fileAddr()ʧ�� ==> " + url + "������");
		// res->second = newFileAddr;  // res�ĵڶ���ֵ��������keyֵ�󶨵�ֵ
		routeMap[url] = routeElement;
	}

	bool add_routeElement(const std::string & url, RouteABS* routeElement) {
		auto res = routeMap.insert({ url, routeElement });
		return res.second;    // res�ĵڶ���ֵ��bool,��ʾ�Ƿ�ɹ�
	}

};





}

#endif // !_ROUTE_H_

