


/*// ˵��
* Ŀǰ����֪���log�������£�
*	template<class T>
*	void log(T val1, T val2, infoType& info, int rank, std::funtion<> func);
* 
*   class infoType
*	{
*		fileName   // �ļ����������ļ�·��
*       line       // ������к�
*       functonName    // ����ĺ�����
*		info       // ��ʾ��Ϣ	
*	}  
* 
*   (1) val1��val2��������������������ͬ������,ͬʱ��������,������Ӧ������ģ���ػ�
*   (2) infoType����Ϣ�࣬���Ա����
*   (3) rank�Ǵ���ȼ���log�������ݴ���ȼ���ѡ��ֹͣ�����������������
*	(4) func������ɵ��ö��󣬿���ӵ����������������ͷ���Դ
*	(5) �����ʽ��error[rank]:filename line functonName ������Ϣ
*	(6) �����������ΪɶҪ�ͷ���Դ��ֱ�ӹرղ������ˣ�
*       a.�������һ��Ҫ�رգ������������ֶΣ������´�����
*       b.�ڶ��߳���һ���̱߳���ֹͣ������û���ͷŵ��ڴ���Դ������ڴ��һֱ��ռ�ã�ֱ����������ֹͣʱ�����ɲ���ϵͳ�Լ����գ�
*         ���������ڴ�й©����Ȼ����ǵ��̳߳�����Ӱ�첻��
*   (7) Ŀǰ�������ޣ�д��������������������������ֻ���Ȱ��뷨д��������ʵ��һ���򵥵�log��
*/



#pragma once
#ifndef _LOG_H_
#define _LOG_H_  // 

#include<iostream>
#include<string>

namespace myLog
{
	template<class T>
	inline void log(T val1, T val2,const std::string& info) {   // ��string����Ϊ��������+��������������ַ���������
		if (val1 == val2) {
			std::cout << info << std::endl;
			std::exit(-1);
		}
	}
}







#endif  // !_LOG_H_





