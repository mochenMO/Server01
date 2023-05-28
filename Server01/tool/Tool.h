#pragma once
#ifndef _TOOL_H_
#define _TOOL_H_  

#include<iostream>
#include<string>
#include "../sql/MySql.h"

namespace myTool
{
	bool operator== (std::string s1, SQLCHAR* s2)
	{
		return s1 == std::string(reinterpret_cast<const char*>(s2));
	}
}







#endif  // !_TOOL_H_





