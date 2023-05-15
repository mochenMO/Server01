


/*// 说明
* 目前我认知里的log函数如下：
*	template<class T>
*	void log(T val1, T val2, infoType& info, int rank, std::funtion<> func);
* 
*   class infoType
*	{
*		fileName   // 文件名，包括文件路径
*       line       // 出错的行号
*       functonName    // 出错的函数名
*		info       // 提示信息	
*	}  
* 
*   (1) val1和val2可以是任意两个类型相同的数据,同时根据类型,进行相应的类型模板特化
*   (2) infoType是信息类，其成员如上
*   (3) rank是错误等级，log函数根据错误等级，选择停止，不处理或其他操作
*	(4) func是任意可调用对象，可以拥有任意参数，用于释放资源
*	(5) 输出格式：error[rank]:filename line functonName 其他信息
*	(6) 补：程序出错为啥要释放资源，直接关闭不就行了？
*       a.程序出错不一定要关闭，有其他处理手段，如重新创建等
*       b.在多线程中一个线程被迫停止，但它没有释放的内存资源，则该内存会一直被占用，直到整个程序停止时，才由操作系统自己回收，
*         即发生了内存泄漏，当然如果是单线程程序，则影响不大。
*   (7) 目前能力有限，写不出来啊！！！！！！！！！只能先把想法写下来，再实现一个简单的log。
*/



#pragma once
#ifndef _LOG_H_
#define _LOG_H_  // 

#include<iostream>
#include<string>

namespace myLog
{
	template<class T>
	inline void log(T val1, T val2,const std::string& info) {   // 用string是因为它重载了+号运算符，便于字符串的连接
		if (val1 == val2) {
			std::cout << info << std::endl;
			std::exit(-1);
		}
	}
}







#endif  // !_LOG_H_





