#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include "Inquiry.h"
#include "mark.h"
#include "My_Sql.h"

std::mutex Inquiry::m_InquiryLock;
Inquiry * Inquiry::m_pInquiry = NULL;

Inquiry::Inquiry() : m_pID(NULL) ,m_pPasswd(NULL)
{

}

Inquiry::~Inquiry()
{
    if(m_pID != NULL)
    {
        delete []m_pID;
    }
    if(m_pPasswd != NULL)
    {
        delete []m_pPasswd;
    }
}

Inquiry * Inquiry::GetInquiry()
{
    std::unique_lock<std::mutex> lock(m_InquiryLock);
    if(m_pInquiry == NULL)
    {
        m_pInquiry = new Inquiry();
    }

    return m_pInquiry;
}

void Inquiry::FreeInquiry()
{
    std::unique_lock<std::mutex> lock(m_InquiryLock);
    if(m_pInquiry != NULL)
    {
        delete m_pInquiry;
        m_pInquiry = NULL;
    }
}

int Inquiry::Function(int cfd)
{
    char get_len[5];
    int len;
    int rev;

    memset(get_len, 0, sizeof(get_len));

    rev = recv(cfd, get_len, 4, 0);
    if(rev < 0)
    {
        perror("Inquiry::id_len : ");
        return -1;
    }

    len = atoi(get_len);
    m_pID = new char[len + 1];
    memset(m_pID, 0, len + 1);
    rev = recv(cfd, m_pID, len, 0);
    if(rev < 0)
    {
        perror("Inquiry::ID : ");
        return -1;
    }

    m_pPasswd = new char[17];
    memset(m_pPasswd, 0, 17);
    rev = recv(cfd, m_pPasswd, 16, 0);
    if(rev < 0)
    {
        perror("Inquiry::password : ");
        return -1;
    }

    printf("Inquiry::id:%s\n",m_pID);
    printf("Inquiry::password:%s\n\n",m_pPasswd);

    //数据读取完毕,进行数据库处理,并把处理的结果返回给客户端
    int s = DataBase();
    send(cfd, &s, sizeof(s), 0);
    if(s == INQU_SUCCESS)
    {
        char temp[30];
        strcpy(temp,m_sMoney.c_str());
        send(cfd, temp, sizeof(temp), 0);
    }

    return 0;
}

int Inquiry::DataBase()
{
    int result = 0;
    char buf[1024];
    vector<string> row;
    My_Sql sql("BankData");

    sprintf(buf,"select *from UserInformation where ID= %s;",m_pID);
    sql.MySql_exec(buf);
    //sql.GetFieldsName(data);
    printf("Inquiry::row number:%d\n",sql.m_iNumRows);
    if(sql.m_iNumRows == 0) //没有该用户
    {
        result = NO_ACCOUNT;
    }
    else
    {
        sprintf(buf,"select passwd from UserInformation where ID= %s AND passwd=\"%s\";",m_pID,m_pPasswd);
        sql.MySql_exec(buf);
        if(sql.m_iNumRows == 0) //密码错误
        {
            result = PASSWD_ERROR;
        }
        else
        {
            sprintf(buf,"select balance from _%s where number = (select max(number) from _%s);",m_pID, m_pID);
            sql.MySql_exec(buf);
            sql.GetRowInformation(row);
            std::cout << "Inquiry::balance:" << row[0] << std::endl;

            m_sMoney = row[0];

            result = INQU_SUCCESS;
        }

    }

    return result;
}
