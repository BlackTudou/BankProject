#include "My_Sql.h"
#include <mysql/mysql.h>
#include <iostream>
#include <vector>

using namespace std;

#define USERNAME "root"
#define DATAPASSWD "123456"

My_Sql::My_Sql(const char *db_name)
{
    mysql_library_init(0,NULL,NULL);
    mysql_init(&m_Mysql);
    if(&m_Mysql == NULL)
    {
        printf ("mysql_init error: %s\n", mysql_error(&m_Mysql));
        exit(-1);
    }

    mysql_options(&m_Mysql,MYSQL_SET_CHARSET_NAME,"gbk");
    // 连接服务器
    MYSQL * conn = mysql_real_connect(&m_Mysql, "localhost", USERNAME, DATAPASSWD, db_name, 0, NULL, 0);
    if(conn == NULL)
    {
        printf ("mysql_init error: %s\n", mysql_error(&m_Mysql));
        exit(-1);
    }

    // 设置中文显示
    mysql_query(&m_Mysql, "set names utf8");
    printf("连上my_db......\n");
}

My_Sql::~My_Sql()
{
    mysql_free_result(m_pMysqlRes);
    //mysql_close(&m_Mysql);
}

// 成功返回0， 失败返回-1
int My_Sql::MySql_exec(const char * sql)
{
        //  mysql语句执行
    int ret = mysql_query(&m_Mysql, sql);
    if (ret != 0)
    {
        printf ("mysql_query error: %s\n", mysql_error(&m_Mysql));
        mysql_close(&m_Mysql);
        return -1;
    }

    // 从服务器下载数据到本地
    //mysql_store_result()将查询的全部结果读取到客户端，分配1个MYSQL_RES结构，并将结果置于该结构中。
    m_pMysqlRes = mysql_store_result(&m_Mysql);
    if (m_pMysqlRes == NULL)
    {
        // 并没有发生错误，只是没有取到值而已
        if (mysql_errno(&m_Mysql) == 0)
        {
            printf ("Query OK!\n");
            return 0;
        }
        else
        {
            printf ("mysql_store_result error: %s\n", mysql_error(&m_Mysql));
            mysql_close(&m_Mysql);
            return -1;
        }
    }

    m_iNumFields =  mysql_num_fields(m_pMysqlRes);  // 获取列数
    m_iNumRows = mysql_num_rows(m_pMysqlRes); //

    return 0;
}

//读取数据
bool My_Sql::getDatafromDB(string queryStr, std::vector<std::vector<std::string> >& data)
{
    if(0!=mysql_query(&m_Mysql,queryStr.c_str()))
    {
        printf ("mysql_query error: %s\n", mysql_error(&m_Mysql));
        return false;
    }

    m_pMysqlRes=mysql_store_result(&m_Mysql);

    int row=mysql_num_rows(m_pMysqlRes);
    int field=mysql_num_fields(m_pMysqlRes);  
    std::string temp;

    MYSQL_ROW line=NULL;
    line=mysql_fetch_row(m_pMysqlRes);

    MYSQL_FIELD *fieldInformation = mysql_fetch_fields(m_pMysqlRes);
    vector<string> rowdata;

    for (unsigned int i = 0; i < field; i++)
    {

        temp = fieldInformation[i].name;
        rowdata.push_back(temp);
    }
    data.push_back(rowdata);

    while(NULL!=line)
    {
        std::vector<std::string> linedata;
        for(int i=0; i<field;i++)
        {
            if(line[i])
            {
                temp = line[i];
                std::cout << "temp= " << temp << std::endl;
                linedata.push_back(temp);
            }
            else
            {
                temp = "";
                std::cout << "temp= " << temp << std::endl;
                linedata.push_back(temp);
            }
        }
        line=mysql_fetch_row(m_pMysqlRes);
        data.push_back(linedata);
    }
    return true;
}

void My_Sql::GetFieldsName(string & data)
{
    data.clear();

    MYSQL_FIELD *field = mysql_fetch_fields(m_pMysqlRes);

    for (unsigned int i = 0; i < m_iNumFields; i++)
    {
        data = data + field[i].name + " ";
    }

    std::cout << "data: " << data << std::endl;
}

void My_Sql::GetRowInformation(vector<string> &data)
{
    MYSQL_ROW row;
    string temp;

    data.clear();

    while((row = mysql_fetch_row(m_pMysqlRes)) != NULL)
    {
        temp.clear();

        for (unsigned int i = 0; i < m_iNumFields; i++)
        {
           temp = temp + row[i];
        }
        std::cout << "data: " << temp << std::endl;

        data.push_back(temp);
    }
}
