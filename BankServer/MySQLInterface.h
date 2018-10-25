#ifndef MYSQLINTERFACE_H
#define MYSQLINTERFACE_H

#include <iostream>
#include <string>
#include "mysql/mysql.h"
#include <vector>
#include <string>

using namespace std;

class MySQLInterface
{
public:
    MySQLInterface();
    virtual ~MySQLInterface();

    bool connectMySQL(char* server, char* username, char* password, char* database,int port);
    bool createDatabase(std::string& dbname);
    bool createdbTable(const std::string& query);

    void errorIntoMySQL();
    bool writeDataToDB(string queryStr);
    bool getDatafromDB(string queryStr, std::vector<std::vector<std::string> >& data);
    void closeMySQL();

public:
    int errorNum;                    //错误代号
    const char* errorInfo;             //错误提示

private:
    MYSQL mysqlInstance;                      //MySQL对象，必备的一个数据结构
    MYSQL_RES *result;                 //用于存放结果 建议用char* 数组将此结果转存
};

#endif // MYSQLINTERFACE_H
