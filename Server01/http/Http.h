

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

#include "../server/Server.h"

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
    {"json","application/json"}};    // ѹ����ʽ������


void getUrl(char* httpRequest, char* url)
{
    char* start = strstr(httpRequest, " ");
    myLog::log(start, (char*)nullptr, "error: myHttp::getUrl()ʧ��");
    char* end = strstr(httpRequest, "HTTP/1.1");
    myLog::log(end, (char*)nullptr, "error: myHttp::getUrl()ʧ��");
    int len = end - start - 2;
    strncpy(url, start + 1, len);
    url[len] = '\0';
}

const std::string getContentType(char* httpRequest)
{
    std::string Suffix;
    char url[256] = { 0 };
    myHttp::getUrl(httpRequest, url);
    char* c = std::strstr(url, ".");
    if (c == nullptr) {
        return "text/html";   // �޺�׺��Ĭ�Ϸ��� text/html
    }
    // �к�׺�����
    ++c;    // ȥ��'.'
    while ((*c >= 48 && *c <= 57) || (*c >= 65 && *c <= 90) || (*c >= 97 && *c <= 122) || *c == '_') {
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

void getPageName(char* httpRequest, char* pageName)
{
    char* c = strstr(httpRequest, "/");
    myLog::log(c, (char*)nullptr, "error: myHttp::getPageName()ʧ��");
    pageName[0] = '/';
    ++c; 
    int i = 1;
    for (; *c != ' ' && *c != '/' && *c != '.'; ++i, ++c) {  // ע�⣺i��1��ʼ
        pageName[i] = *c;
    }
    pageName[i] = '\0';
}


void getValuebyCookie(char* httpRequest, const char* attributeName, char* resValue) {
    char* c = strstr(httpRequest, "Cookie");   // ���Ƶ�cookie�������ڵ�λ�ã���ֹǰ�����һ���Ĳ�����
    myLog::log(c, (char*)nullptr, "error: myHttp::getValuebyCookie()ʧ��" + std::string(attributeName) + "δ�ҵ�");
    c = strstr(c, attributeName);
    myLog::log(c, (char*)nullptr, "error: myHttp::getValuebyCookie()ʧ��" + std::string(attributeName) + "δ�ҵ�");
    c += strlen(attributeName) + 1;  // ���1����Ϊ���滹��һ��"="��
    int i = 0;
    for (; *c != ' ' && *c != ';'; ++i, ++c) {   // ���cookieֵ֮����";"�ָ�
        resValue[i] = *c;
    }
    resValue[i] = '\0';
}


void getValuebyPost(char* httpRequest, const char* attributeName, char* resValue) {
    char* c = strstr(httpRequest, "{\"");   // ���Ƶ�cookie�������ڵ�λ�ã���ֹǰ�����һ���Ĳ�����
    myLog::log(c, (char*)nullptr, "error: myHttp::getValuebyPost()ʧ��" + std::string(attributeName) + "δ�ҵ�");
    c = strstr(c, attributeName);
    myLog::log(c, (char*)nullptr, "error: myHttp::getValuebyPost()ʧ��" + std::string(attributeName) + "δ�ҵ�");
    c += strlen(attributeName) + 3;  // ���3����Ϊ���滹��һ��":"
    int i = 0;
    for (; *c != '"' ; ++i, ++c) {   // ���cookieֵ֮����"; "�ָ�
        resValue[i] = *c;
    }
    resValue[i] = '\0';
}

void getValuebyGet(char* httpRequest, const char* attributeName, char* resValue) {
    char* c = strstr(httpRequest, "?");
    myLog::log(c, (char*)nullptr, "error: myHttp::getValuebyPost()ʧ��" + std::string(attributeName) + "δ�ҵ�");
    c = strstr(httpRequest, attributeName);
    myLog::log(c, (char*)nullptr, "error: myHttp::getValuebyPost()ʧ��" + std::string(attributeName) + "δ�ҵ�");
    c += strlen(attributeName) + 1;  // ���1����Ϊ���滹��һ��"="��
    int i = 0;
    for (; *c != ' ' && *c != '&'; ++i, ++c) {   // ���cookieֵ֮����";"�ָ�
        resValue[i] = *c;
    }
    resValue[i] = '\0';
}


void getFileName(char* httpRequest, char* resFileName)
{
    char url[256] = { 0 };
    myHttp::getUrl(httpRequest, url);
    int len = strlen(url);
    char* c = strstr(url, "?");
    if (c != nullptr) {
        len = c - url;
    }
    strncpy(resFileName, url, len);
    resFileName[len] = '\0';
}


bool findAttribute(char* httpRequest, const char* attributeName) {
    char* c = strstr(httpRequest, attributeName);
    return c != nullptr ? true : false;
}


void SendhttpRequest(myServer::SocketItem& socketItem,const char* httpRequest, int requestLen,const char* fileName)
{
    // send(socketItem.socket, httpRequest, requestLen, 0);
    //FILE* fp;
    //fopen_s(&fp, fileName, "rb");
    //char buffer[1024];
    //int bytesRead;
    //while ((bytesRead = fread(buffer, sizeof(char), sizeof(buffer), fp)) > 0){
    //    send(socketItem.socket, buffer, bytesRead, 0);
    //}
    //fclose(fp);

    // ifstream file("www" + path, ios::binary);

    // std::cout << httpRequest << std::endl;

    send(socketItem.socket, httpRequest, requestLen, 0);
    std::ifstream file("web" + std::string(fileName), std::ios::binary);
    if (file.fail()) {
        file.open("res" + std::string(fileName), std::ios::binary);   // image��Դ
        if (file.fail()) {
            // myLog::log(0, 0, "��ȡ�ļ�ʧ��:" + std::string(fileName));

            std::cout << "��ȡ�ļ�ʧ��:" << fileName << std::endl;
            return;
        }
    }
    
    char buffer[4096];
    while (!file.eof()) {
        file.read(buffer, sizeof(buffer));
        send(socketItem.socket, buffer, file.gcount(), 0);
    }

    std::cout << "�ѷ���" << fileName << std::endl;
}







}







#endif  // !_HTTP_H_


