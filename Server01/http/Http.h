

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
    {"json","application/json"}};    // 压缩格式？？？


void getUrl(char* httpRequest, char* url)
{
    char* start = strstr(httpRequest, " ");
    myLog::log(start, (char*)nullptr, "error: myHttp::getUrl()失败");
    char* end = strstr(httpRequest, "HTTP/1.1");
    myLog::log(end, (char*)nullptr, "error: myHttp::getUrl()失败");
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
        return "text/html";   // 无后缀名默认返回 text/html
    }
    // 有后缀的情况
    ++c;    // 去掉'.'
    while ((*c >= 48 && *c <= 57) || (*c >= 65 && *c <= 90) || (*c >= 97 && *c <= 122) || *c == '_') {
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

void getPageName(char* httpRequest, char* pageName)
{
    char* c = strstr(httpRequest, "/");
    myLog::log(c, (char*)nullptr, "error: myHttp::getPageName()失败");
    pageName[0] = '/';
    ++c; 
    int i = 1;
    for (; *c != ' ' && *c != '/' && *c != '.'; ++i, ++c) {  // 注意：i从1开始
        pageName[i] = *c;
    }
    pageName[i] = '\0';
}


void getValuebyCookie(char* httpRequest, const char* attributeName, char* resValue) {
    char* c = strstr(httpRequest, "Cookie");   // 先移到cookie属性所在的位置，防止前面出现一样的参数名
    myLog::log(c, (char*)nullptr, "error: myHttp::getValuebyCookie()失败" + std::string(attributeName) + "未找到");
    c = strstr(c, attributeName);
    myLog::log(c, (char*)nullptr, "error: myHttp::getValuebyCookie()失败" + std::string(attributeName) + "未找到");
    c += strlen(attributeName) + 1;  // 多加1是因为后面还有一个"="号
    int i = 0;
    for (; *c != ' ' && *c != ';'; ++i, ++c) {   // 多个cookie值之间用";"分隔
        resValue[i] = *c;
    }
    resValue[i] = '\0';
}


void getValuebyPost(char* httpRequest, const char* attributeName, char* resValue) {
    char* c = strstr(httpRequest, "{\"");   // 先移到cookie属性所在的位置，防止前面出现一样的参数名
    myLog::log(c, (char*)nullptr, "error: myHttp::getValuebyPost()失败" + std::string(attributeName) + "未找到");
    c = strstr(c, attributeName);
    myLog::log(c, (char*)nullptr, "error: myHttp::getValuebyPost()失败" + std::string(attributeName) + "未找到");
    c += strlen(attributeName) + 3;  // 多加3是因为后面还有一个":"
    int i = 0;
    for (; *c != '"' ; ++i, ++c) {   // 多个cookie值之间用"; "分隔
        resValue[i] = *c;
    }
    resValue[i] = '\0';
}

void getValuebyGet(char* httpRequest, const char* attributeName, char* resValue) {
    char* c = strstr(httpRequest, "?");
    myLog::log(c, (char*)nullptr, "error: myHttp::getValuebyPost()失败" + std::string(attributeName) + "未找到");
    c = strstr(httpRequest, attributeName);
    myLog::log(c, (char*)nullptr, "error: myHttp::getValuebyPost()失败" + std::string(attributeName) + "未找到");
    c += strlen(attributeName) + 1;  // 多加1是因为后面还有一个"="号
    int i = 0;
    for (; *c != ' ' && *c != '&'; ++i, ++c) {   // 多个cookie值之间用";"分隔
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
        file.open("res" + std::string(fileName), std::ios::binary);   // image资源
        if (file.fail()) {
            // myLog::log(0, 0, "读取文件失败:" + std::string(fileName));

            std::cout << "读取文件失败:" << fileName << std::endl;
            return;
        }
    }
    
    char buffer[4096];
    while (!file.eof()) {
        file.read(buffer, sizeof(buffer));
        send(socketItem.socket, buffer, file.gcount(), 0);
    }

    std::cout << "已发送" << fileName << std::endl;
}







}







#endif  // !_HTTP_H_


