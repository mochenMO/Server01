#pragma once
#ifndef _TOOL_H_
#define _TOOL_H_  

#include<iostream>
#include<string>
#include "../sql/MySql.h"    // �����⣿������

namespace myTool
{
	bool operator== (std::string s1, SQLCHAR* s2)   // Ӧ�÷���MySql.h��
	{
		return s1 == std::string(reinterpret_cast<const char*>(s2));
	}


	std::string getValuebyKey(const char* src, const char* attributeName) {
		std::string resValue;
		const char* c = strstr(src, attributeName);   // ���Ƶ�cookie�������ڵ�λ�ã���ֹǰ�����һ���Ĳ�����
		if (c == nullptr) return "";
		c += strlen(attributeName) + 3;  // ���3����Ϊ���滹��һ��":"
		for (; *c != '"'; ++c) {   // ���cookieֵ֮����"; "�ָ�
			resValue += *c;
		}
		return resValue;
	}


}







#endif  // !_TOOL_H_





