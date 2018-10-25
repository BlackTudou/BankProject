//#include "stdafx.h"
#include "MySQLInterface.h"


//构造函数 初始化各个变量和数据
MySQLInterface::MySQLInterface():
    errorNum(0),errorInfo("ok")
{
    mysql_library_init(0,NULL,NULL);
    mysql_init(&mysqlInstance);
    mysql_options(&mysqlInstance,MYSQL_SET_CHARSET_NAME,"gbk");
}

MySQLInterface::~MySQLInterface()
{

}

//连接MySQL
bool MySQLInterface::connectMySQL(char* server, char* username, char* password, char* database,int port)
{
    if(mysql_real_connect(&mysqlInstance,server,username,password,database,port,0,0) != NULL)
        return true;
    else
        errorIntoMySQL();
    return false;
}

//判断数据库是否存在，不存在则创建数据库，并打开
bool MySQLInterface::createDatabase(std::string& dbname)
{
    std::string queryStr = "create database if not exists ";
    queryStr += dbname;
    if (0 == mysql_query(&mysqlInstance,queryStr.c_str()))
    {
        queryStr = "use ";
        queryStr += dbname;
        if (0 == mysql_query(&mysqlInstance,queryStr.c_str()))
        {
            return true;
        }

    }
    errorIntoMySQL();
    return false;
}
//判断数据库中是否存在相应表，不存在则创建表
bool MySQLInterface::createdbTable(const std::string& query)
{
    if (0 == mysql_query(&mysqlInstance,query.c_str()))
    {
        return true;
    }
    errorIntoMySQL();
    return false;
}

//写入数据
bool MySQLInterface::writeDataToDB(string queryStr)
{
    if(0==mysql_query(&mysqlInstance,queryStr.c_str()))
        return true;
    else
        errorIntoMySQL();
    return false;
}
//读取数据
bool MySQLInterface::getDatafromDB(string queryStr, std::vector<std::vector<std::string> >& data)
{
    if(0!=mysql_query(&mysqlInstance,queryStr.c_str()))
    {
        errorIntoMySQL();
        return false;
    }

    result=mysql_store_result(&mysqlInstance);

    int row=mysql_num_rows(result);
    int field=mysql_num_fields(result);

    MYSQL_ROW line=NULL;
    line=mysql_fetch_row(result);

    int j=0;
    std::string temp;
    while(NULL!=line)
    {
        std::vector<std::string> linedata;
        for(int i=0; i<field;i++)
        {
            if(line[i])
            {
                temp = line[i];
                linedata.push_back(temp);
            }
            else
            {
                temp = "";
                linedata.push_back(temp);
            }
        }
        line=mysql_fetch_row(result);
        data.push_back(linedata);
    }
    return true;
}

//错误信息
void MySQLInterface::errorIntoMySQL()
{
    errorNum=mysql_errno(&mysqlInstance);
    errorInfo=mysql_error(&mysqlInstance);
}

//断开连接
void MySQLInterface::closeMySQL()
{
    mysql_close(&mysqlInstance);
}
