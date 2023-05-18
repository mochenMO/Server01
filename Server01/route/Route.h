#pragma once
#ifndef _ROUTE_H_
#define _ROUTE_H_

#include <unordered_map>
#include <vector>
// #include<hash_map>  // ������

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
	std::unordered_map<std::string, RouteABS*> routeMap;   // ע�⣺RouteABS*��ָ��
public:

	Route() :routeMap() {
		routeMap.insert({ "/default", new myDefault::Default });
		routeMap.insert({ "/login", new myLogin::Login });
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

	RouteABS* get_routeElement(const std::string& pageName) {
		auto res = routeMap.find(pageName);
		// myLog::log(res, routeMap.end(), "error: myRoute::route::get_routeElement()ʧ�� ==> " + pageName + "������");	
		if (res == routeMap.end()){
			res = routeMap.find("/error");  // 404ҳ��
		}
		return res->second;  // res�ĵڶ���ֵ��������keyֵ�󶨵�ֵ
	}

	void  set_routeElement(const std::string& pageName, RouteABS* routeElement) {
		auto res = routeMap.find(pageName);
		myLog::log(res, routeMap.end(), "error: myRoute::route::set_routeElement()ʧ�� ==> " + pageName + "������");
		// res->second = newFileAddr;  // res�ĵڶ���ֵ��������keyֵ�󶨵�ֵ
		routeMap[pageName] = routeElement;
	}

	bool add_routeElement(const std::string& pageName, RouteABS* routeElement) {
		auto res = routeMap.insert({ pageName, routeElement });
		return res.second;    // res�ĵڶ���ֵ��bool,��ʾ�Ƿ�ɹ�
	}

	void distribute_route(myServer::SocketItem* socketItem, myHttp::HttpRequest& httpRequest, std::vector<void*>& publicRes)
	{
		
		RouteABS* pageRoute;
		// ���pageName
		std::string pageName = httpRequest.getPageName();

		if (httpRequest.findAttribute("Cookie") == false) {   // ��ҳ��û��Cookie,��û�г�ʼ��
			pageRoute = get_routeElement("/login");
		}
		else {   // ��ҳ����Cookie
			if (httpRequest.getValuebyCookie("userName") == "nulluser") {   // username==nulluser���û�δ��¼
				pageRoute = get_routeElement("/login");
			}
			else {  // �û��ѵ�¼ 
				pageRoute = get_routeElement(pageName);
			}
		}
		// ����ָ����ҳ����д���
		pageRoute->dealRequest(socketItem, httpRequest, publicRes);
	}

};





}

#endif // !_ROUTE_H_




