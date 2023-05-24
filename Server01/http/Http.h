

/*// 说明
* (1) HTTP是一种应用层协议，它使用 ASCII 码作为消息格式标准来传输数据。
* (2) 各个函数还没有写异常处理相关的函数，因此在书写时请确保
* 
* 
* 
*/



#pragma once
#ifndef _HTTP_H_
#define _HTTP_H_

//#include<iostream>
#include<string>
#include <unordered_map>
#include <fstream> 

#include "../json/Json.h"

namespace myHttp
{

static std::unordered_map<std::string, std::string> contentType = {
	{"html", "text/html"},
	{"css", "text/css"},
	{"js", "text/javascript"},
	{"jpg", "image/jpeg"},
	{"jpeg", "image/jpeg"},
	{"png", "image/png"},
	{"bmp", "image/bmp"},
	{"ico", "image/x-icon"},
	{"json","application/json"} };    // 压缩格式？？？


class HttpRequest
{
private:
	char* httpRequest;
	char* url;
private:
	void setUrlByHttpRequest() {
		// 结构：请求类型 url HTTP/1.1
		char* start = strstr(httpRequest, " ");
		myLog::log(start, (char*)nullptr, "error: myHttp::getUrl()失败");
		char* end = strstr(httpRequest, "HTTP/1.1");
		myLog::log(end, (char*)nullptr, "error: myHttp::getUrl()失败");
		int len = end - start - 2;
		strncpy(url, start + 1, len);
		url[len] = '\0';
	}

public:
	HttpRequest(const char* httpRequestbuf)
		: httpRequest(new char[strlen(httpRequestbuf) + 1]()), url(new char[1024]())   // 多加1留给最后的'\0'
	{
		strcpy(httpRequest, httpRequestbuf);
		setUrlByHttpRequest();
	}
	~HttpRequest() {
		delete[] httpRequest;
		delete[] url;
	}

	const char* getUrl() {
		return url;
	}

	void setUrl(const char* url) {
		strcpy(this->url, url);
	}


	const std::string getContentType()
	{
		std::string Suffix;
		char* c = std::strstr(url, ".");
		if (c == nullptr) {
			return "text/html";   // 无后缀名默认返回 text/html
		}
		// 有后缀的情况
		++c;    // 去掉'.'
		while ((*c >= 48 && *c <= 57) || (*c >= 65 && *c <= 90) || (*c >= 97 && *c <= 122)) {
			Suffix += *c;
			++c;
		}
		auto res = contentType.find(Suffix);
		// myLog::log(res, contentType.end(), "error: myHttp::getContentType()失败 ==> 不存在以" + Suffix + "为后缀名的文件");
		if (res == contentType.end()) {
			std::cout << "warning: myHttp::getContentType()失败 ==> 不存在以" + Suffix + "为后缀名的文件" << std::endl;
			return "text/html";   // 无匹配的后缀名默认返回 text/html
		}
		return res->second;  // res的第二个值存的是与该key值绑定的值
	}

	std::string getPageName()
	{
		std::string pageName;
		pageName += '/';
		char* c = strstr(url + 1, "/");  // 尝试寻找第二个"/"的位置
		if (c == nullptr && url[1] == '\0') {
			return "/login";  // 只有一个"/"直接交给login页面处理
		}
		else {
			for (int i = 1; url[i] != '/' && url[i] != '\0'; i++) {
				if (url[i] == '.') {   // 说明说明是浏览器的文件请求如，http://local:8888/favicon.ico favicon并不是pageName
					return "/default";  // 交由default页面处理
				}
				else {
					pageName += url[i];
				}
			}
		}
		return pageName;
	}

	std::string getFileName()
	{
		std::string fileName;
		char* c = strstr(url, ".");
		if (c == nullptr) {   // url中没有显示的指定文件，则默认传pageName.html	
			return getPageName() + ".html";
		}
		else {
			while (*c != '/')
				--c;  // 从'.'移到'/'的位置
			++c;
			for (; *c != '?' && *c != '\0' && *c != '/'; ++c)
				fileName += *c;
			return "/" + fileName;   // 文件名和PageName一样由'/'开头
		}
	}

	std::string getValuebyCookie(const char* attributeName) {
		std::string resValue;
		char* c = strstr(httpRequest, "Cookie");   // 先移到cookie属性所在的位置，防止前面出现一样的参数名
		myLog::log(c, (char*)nullptr, "error: myHttp::getValuebyCookie()失败" + std::string(attributeName) + "未找到");
		c = strstr(c, attributeName);
		myLog::log(c, (char*)nullptr, "error: myHttp::getValuebyCookie()失败" + std::string(attributeName) + "未找到");
		c += strlen(attributeName) + 1;  // 多加1是因为后面还有一个"="号	
		for (; *c != ' ' && *c != ';' && *c != '\r'; ++c) {   // 多个cookie值之间用";"分隔
			resValue += *c;
		}
		return resValue;
	}

	std::string getValuebyPost(const char* attributeName) {
		std::string resValue;
		char* c = strstr(httpRequest, "{\"");   // 先移到cookie属性所在的位置，防止前面出现一样的参数名
		if (c == nullptr) return "";
		c = strstr(c, attributeName);
		if (c == nullptr) return "";
		c += strlen(attributeName) + 3;  // 多加3是因为后面还有一个":"
		for (; *c != '"'; ++c) {   // 多个cookie值之间用"; "分隔
			resValue += *c;
		}
		return resValue;
	}

	bool findAttribute(const char* attributeName) {
		char* c = strstr(httpRequest, attributeName);
		return c != nullptr ? true : false;
	}
};



class HttpResponce
{
private:
	std::string responseHeader;
	std::string cookie;
	std::string contentType;
	myJson::Json json;
	bool isPrepared = false;
public:
	HttpResponce(HttpRequest& httpRequest)
		:responseHeader("HTTP/1.1 200 OK\r\n"), contentType(httpRequest.getContentType())
	{}

	~HttpResponce() = default;

	std::string& getHttpResponce() {
		return responseHeader;
	}

	std::string& getContentType() {
		return contentType;
	}

	myJson::Json& getJson() {
		return json;
	}

	void setJson(myJson::Json json) {
		this->json = json;
	}

	void CombinatHttpResponce() 
	{
		if (isPrepared == true) return;
		isPrepared = true;

		responseHeader += "Content-Type: " + contentType + "\r\n";
	
		if (cookie.empty() == false) {
			responseHeader += "Set-Cookie: " + cookie + "path=/;expires=Mon, 21 Sep 2037 00:00:01 GMT\r\n";
		}

		responseHeader += "\r\n";
	}

	void addCookie(const std::string& attributeName, const std::string& strValue) {
		cookie += attributeName + "=" + strValue + ";";
	}

};




}







#endif  // !_HTTP_H_





