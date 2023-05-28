#pragma once
#ifndef _TOOL_H_
#define _TOOL_H_  

#include<iostream>
#include<string>
#include "../sql/MySql.h"    // 有问题？？？？

namespace myTool
{
	bool operator== (std::string s1, SQLCHAR* s2)   // 应该放在MySql.h中
	{
		return s1 == std::string(reinterpret_cast<const char*>(s2));
	}


	std::string getValuebyKey(const char* src, const char* attributeName) {
		std::string resValue;
		const char* c = strstr(src, attributeName);   // 先移到cookie属性所在的位置，防止前面出现一样的参数名
		if (c == nullptr) return "";
		c += strlen(attributeName) + 3;  // 多加3是因为后面还有一个":"
		for (; *c != '"'; ++c) {   // 多个cookie值之间用"; "分隔
			resValue += *c;
		}
		return resValue;
	}


}







#endif  // !_TOOL_H_





