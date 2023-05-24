

/*// ˵��
* (1) HTTP��һ��Ӧ�ò�Э�飬��ʹ�� ASCII ����Ϊ��Ϣ��ʽ��׼���������ݡ�
* (2) ����������û��д�쳣������صĺ������������дʱ��ȷ��
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
	{"json","application/json"} };    // ѹ����ʽ������


class HttpRequest
{
private:
	char* httpRequest;
	char* url;
private:
	void setUrlByHttpRequest() {
		// �ṹ���������� url HTTP/1.1
		char* start = strstr(httpRequest, " ");
		myLog::log(start, (char*)nullptr, "error: myHttp::getUrl()ʧ��");
		char* end = strstr(httpRequest, "HTTP/1.1");
		myLog::log(end, (char*)nullptr, "error: myHttp::getUrl()ʧ��");
		int len = end - start - 2;
		strncpy(url, start + 1, len);
		url[len] = '\0';
	}

public:
	HttpRequest(const char* httpRequestbuf)
		: httpRequest(new char[strlen(httpRequestbuf) + 1]()), url(new char[1024]())   // ���1��������'\0'
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
			return "text/html";   // �޺�׺��Ĭ�Ϸ��� text/html
		}
		// �к�׺�����
		++c;    // ȥ��'.'
		while ((*c >= 48 && *c <= 57) || (*c >= 65 && *c <= 90) || (*c >= 97 && *c <= 122)) {
			Suffix += *c;
			++c;
		}
		auto res = contentType.find(Suffix);
		// myLog::log(res, contentType.end(), "error: myHttp::getContentType()ʧ�� ==> ��������" + Suffix + "Ϊ��׺�����ļ�");
		if (res == contentType.end()) {
			std::cout << "warning: myHttp::getContentType()ʧ�� ==> ��������" + Suffix + "Ϊ��׺�����ļ�" << std::endl;
			return "text/html";   // ��ƥ��ĺ�׺��Ĭ�Ϸ��� text/html
		}
		return res->second;  // res�ĵڶ���ֵ��������keyֵ�󶨵�ֵ
	}

	std::string getPageName()
	{
		std::string pageName;
		pageName += '/';
		char* c = strstr(url + 1, "/");  // ����Ѱ�ҵڶ���"/"��λ��
		if (c == nullptr && url[1] == '\0') {
			return "/login";  // ֻ��һ��"/"ֱ�ӽ���loginҳ�洦��
		}
		else {
			for (int i = 1; url[i] != '/' && url[i] != '\0'; i++) {
				if (url[i] == '.') {   // ˵��˵������������ļ������磬http://local:8888/favicon.ico favicon������pageName
					return "/default";  // ����defaultҳ�洦��
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
		if (c == nullptr) {   // url��û����ʾ��ָ���ļ�����Ĭ�ϴ�pageName.html	
			return getPageName() + ".html";
		}
		else {
			while (*c != '/')
				--c;  // ��'.'�Ƶ�'/'��λ��
			++c;
			for (; *c != '?' && *c != '\0' && *c != '/'; ++c)
				fileName += *c;
			return "/" + fileName;   // �ļ�����PageNameһ����'/'��ͷ
		}
	}

	std::string getValuebyCookie(const char* attributeName) {
		std::string resValue;
		char* c = strstr(httpRequest, "Cookie");   // ���Ƶ�cookie�������ڵ�λ�ã���ֹǰ�����һ���Ĳ�����
		myLog::log(c, (char*)nullptr, "error: myHttp::getValuebyCookie()ʧ��" + std::string(attributeName) + "δ�ҵ�");
		c = strstr(c, attributeName);
		myLog::log(c, (char*)nullptr, "error: myHttp::getValuebyCookie()ʧ��" + std::string(attributeName) + "δ�ҵ�");
		c += strlen(attributeName) + 1;  // ���1����Ϊ���滹��һ��"="��	
		for (; *c != ' ' && *c != ';' && *c != '\r'; ++c) {   // ���cookieֵ֮����";"�ָ�
			resValue += *c;
		}
		return resValue;
	}

	std::string getValuebyPost(const char* attributeName) {
		std::string resValue;
		char* c = strstr(httpRequest, "{\"");   // ���Ƶ�cookie�������ڵ�λ�ã���ֹǰ�����һ���Ĳ�����
		if (c == nullptr) return "";
		c = strstr(c, attributeName);
		if (c == nullptr) return "";
		c += strlen(attributeName) + 3;  // ���3����Ϊ���滹��һ��":"
		for (; *c != '"'; ++c) {   // ���cookieֵ֮����"; "�ָ�
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





