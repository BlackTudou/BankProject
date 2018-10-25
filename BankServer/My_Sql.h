#ifndef MY_SQL_H
#define MY_SQL_H

#include <stdio.h>
#include <mysql/mysql.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
using namespace std;

class My_Sql
{
public:
    My_Sql(const char *db_name);
    virtual ~My_Sql();

    void errorIntoMySQL();
    int MySql_exec(const char *sql);

    bool getDatafromDB(string queryStr, std::vector<std::vector<std::string> >& data);

    void GetFieldsName(string &data);
    void GetRowInformation(vector<string> &data);
    //string GetLineInformation2();

    //vector<string> GetRowInformation2();

private:
    MYSQL m_Mysql;
    MYSQL_RES *m_pMysqlRes;   //执行结果
public:
    unsigned int m_iNumFields;   //列数
    unsigned int m_iNumRows;
};

#endif // MY_SQL_H

