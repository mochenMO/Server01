

#ifndef _ROUTE_H_
#define _ROUTE_H_

#include <unordered_map>
#include <vector>
// #include<hash_map>  // ������

#include "RouteABS.h"
#include "../log/Log.h"
#include "../web/login/Login.h"
#include "../http/Http.h"
#include "../server/Server.h"

namespace myRoute
{


class Route
{
private:
	std::unordered_map<std::string, RouteABS*> routeMap;   // ע�⣺RouteABS*��ָ��
public:

	Route() :routeMap() {
		routeMap.insert({ "/", new myLogin::Login });
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
		myLog::log(res, routeMap.end(), "error: myRoute::route::get_fileAddr()ʧ�� ==> " + pageName + "������");	
		return res->second;  // res�ĵڶ���ֵ��������keyֵ�󶨵�ֵ
	}

	void  set_routeElement(const std::string& pageName, RouteABS* routeElement) {
		auto res = routeMap.find(pageName);
		myLog::log(res, routeMap.end(), "error: myRoute::route::set_fileAddr()ʧ�� ==> " + pageName + "������");
		// res->second = newFileAddr;  // res�ĵڶ���ֵ��������keyֵ�󶨵�ֵ
		routeMap[pageName] = routeElement;
	}

	bool add_routeElement(const std::string& pageName, RouteABS* routeElement) {
		auto res = routeMap.insert({ pageName, routeElement });
		return res.second;    // res�ĵڶ���ֵ��bool,��ʾ�Ƿ�ɹ�
	}

	void distribute_route(myServer::SocketItem socketItem, char* httpRequest, std::vector<void*>& publicRes) 
	{
		char pageName[256] = { 0 };
		RouteABS* pageRoute;
		
		// ���pageName
		myHttp::getPageName(httpRequest, pageName);
		
		// ���������cookie��ʵ��ҳ��ĸ���
		char userName[128] = { 0 };
		if (myHttp::findAttribute(httpRequest,"Cookie")== false) {   // ��ҳ��û��Cookie
			pageRoute = get_routeElement("/login");
		}
		else {   // ��ҳ����Cookie
			myHttp::getValuebyCookie(httpRequest, "userName", userName);
			if (strcmp(userName, "nulluser") == 0) {     // username==null���û�δ��¼
				pageRoute = get_routeElement("/login");
			}
			else {  // �û��ѵ�¼
				pageRoute = get_routeElement(pageName);
			}
			//else if (strcmp(pageName, "/login") == 0) {  // �û��ѵ�¼�����û���Ҫ�ֶ���url������¼ҳ��
			//	closesocket(socketItem.socket);
			//	return;
			//}
		}

		// ����ָ����ҳ����д���
		pageRoute->dealRequest(socketItem, httpRequest, publicRes);
	}
	
};





}

#endif // !_ROUTE_H_

