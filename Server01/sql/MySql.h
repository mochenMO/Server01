#pragma once
#ifndef _MYSQL_H_
#define _MYSQL_H_  

#include <iostream>
#include <string>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <odbcss.h>

#include <string>
#include <vector>

#include "../log/Log.h"



namespace MySql
{

class SqlData {
private:
    struct Data{
        SQLCHAR data[200];
        SQLLEN dataLen;
    };
    Data* data;
    int col;
    int row;
public:
    SqlData(int _col, int _row)
        :col(_col),
        row(_row),
        data(new Data[col * row]())
    {};

    SqlData(int _col)
        :col(_col),
        row(20),
        data(new Data[col * row]())
    {};

    ~SqlData() {
        delete[] data;
    }
    
    Data& getDataByRow(int _row)
    {
        return data[_row * col];
    }


    Data& operator[] (int i)
    {
        return data[i];
    }

    int getRow() {
        return row;
    }

    void setRow(int _row) {
        row = _row;
    }

    int getCol() {
        return col;
    }

    void setCol(int _col) {
        col = _col;
    }

    void printData(){
        for (int i = 0; i < row; i++) {
            for (int j = 0; j <col; j++) {
                std::cout << data[i * col + j].data << " ";
            }
            std::cout << "\n";
        }
        
    }
};

class SqlObject
{

private:
    // 定义需要的句柄和参数
    SQLHENV env;
    SQLHDBC dbc;
    SQLHSTMT stmt;
    SQLRETURN retcode;

    wchar_t ODBCName[50] = { 0 };
    wchar_t userName[50] = { 0 };
    wchar_t password[50] = { 0 };
public:
	SqlObject(const char* _ODBCName, const char* _userName, const char* _password)
    {
        // char 转成 wchat_t
        mbstowcs(ODBCName, _ODBCName, 50);
        mbstowcs(userName, _userName, 50);
        mbstowcs(password, _password, 50);
        // 建立环境 
        SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
        SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
        // 建立连接 
        SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);

        SQLRETURN ret = SQLConnect(dbc, ODBCName, SQL_NTS, userName, SQL_NTS, password, SQL_NTS);
        if (!(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)) {
            myLog::log(0, 0, "error: MySql::SqlObject()失败,未能连接到数据库");
        }

        retcode = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    };

	~SqlObject() 
    {
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        SQLDisconnect(dbc);
        SQLFreeHandle(SQL_HANDLE_DBC, dbc);
        SQLFreeHandle(SQL_HANDLE_ENV, env);
    };



    bool select_sql(const char* _commend, SqlData &data)
    {
        wchar_t commend[1024] = { 0 };
        mbstowcs(commend, _commend, 1024);


        retcode = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);   // 每次执行一条新的 SQL 语句时,都需要通过 SQLAllocHandle() 函数分配一个新的语句句柄,因为当执行完一条 SQL 语句后，ODBC 驱动程序将自动清除语句的状态
        retcode = SQLExecDirect(stmt, commend, SQL_NTS);
        if (retcode != SQL_SUCCESS) return false;


        //while (SQLFetch(stmt) == SQL_SUCCESS) {
        //    /* 处理结果 */
        //    SQLCHAR  id[50], name[50], pass[50];
        //    SQLLEN len1, len2, len3;

        //    SQLGetData(stmt, 1, SQL_C_CHAR, id, 50, &len1);
        //    SQLGetData(stmt, 2, SQL_C_CHAR, name, 50, &len2);
        //    SQLGetData(stmt, 3, SQL_C_CHAR, pass, 50, &len3);

        //    printf("%s %s %s\n", id, name, pass);
        //}


        int j = 0;
        while (SQLFetch(stmt) == SQL_SUCCESS) {
            /* 处理结果 */
            for(int i = 0; i < data.getCol(); i++){
                int k = i + j * data.getCol();
                SQLGetData(stmt, i+1, SQL_C_CHAR, data[k].data, 200, &data[k].dataLen);
                printf("%s \n", data[k].data);
            }
            j++;
            
            //SQLGetData(stmt, 2, SQL_C_CHAR, name, 50, &len2);
            //SQLGetData(stmt, 3, SQL_C_CHAR, pass, 50, &len3);

            //printf("%s %s %s\n", id, name, pass);
        }
        data.setRow(j);

        return true;
    }


    bool insert_sql(const char* _commend)
    {
        wchar_t commend[1024] = { 0 };
        mbstowcs(commend, _commend, 1024);

        retcode = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
        retcode = SQLExecDirect(stmt, commend, SQL_NTS);

        return retcode != SQL_SUCCESS ? false : true;
    }

    bool update_sql(const char* _commend)
    {
        wchar_t commend[1024] = { 0 };
        mbstowcs(commend, _commend, 1024);

        retcode = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
        retcode = SQLExecDirect(stmt, commend, SQL_NTS);

        return retcode != SQL_SUCCESS ? false : true;
    }

    bool delete_sql(const char* _commend)
    {
        wchar_t commend[1024] = { 0 };
        mbstowcs(commend, _commend, 1024);

        retcode = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
        retcode = SQLExecDirect(stmt, commend, SQL_NTS);

        return retcode != SQL_SUCCESS ? false : true;
    }
};



/*// 例子
*   sqlObject.select_sql("select * FRom user_tb");
*   sqlObject.insert_sql("insert into user_tb values(4,'tom','12345') ");
*   sqlObject.update_sql("update user_tb set username='tonn' where username = 'tom'");
*   sqlObject.delete_sql("delete from user_tb where username = 'tonn'");
*/



}







#endif  // !_MYSQL_H_





