

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
	
	/*// ˵��
	* ��Ŀǰ�ҵ�����ˮƽ����Ҫʵ����routeͳһ������Ҫ������Դ�Ĺ������������3�ַ���
	* (1) std::vector<void*>���κ���Դ��������ת����Ҫ����Դ���ŵ����ٶȿ죬������Դ���ʷ��㣬ȱ����Ҫ�û��Լ��¸����±��Ӧ�����ͣ������Ͳ���ȫ��
	* (2) ��SQL��Ϊÿ���û���һ��������Դ���ŵ㰲ȫ��ȱ���ȡ�ٶ���
	* (3) ҳ�����httpЭ�鴫����Դ��ȱ���ȡ�ٶ�����Ҫ�����ַ�����ƥ�䣬����дһЩ������Դ����ĺ���
	* (*4) Ŀǰ����ʵ�ַ���(1)
	*/
};




}

#endif // !_ROUTEABS_H_






