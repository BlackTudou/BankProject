#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "ChPasswd.h"
#include "mark.h"
#include "My_Sql.h"

ChPasswd * ChPasswd::m_pChPasswd = NULL;
std::mutex ChPasswd::m_ChPasswdLock;

ChPasswd::ChPasswd() : m_pID(NULL),m_pPasswd(NULL),m_pNewpasswd(NULL)
{

}

ChPasswd::~ChPasswd()
{
    if(m_pID != NULL)
    {
        delete []m_pID;
    }

    if(m_pPasswd != NULL)
    {
        delete []m_pPasswd;
    }

    if(m_pNewpasswd != NULL)
    {
        delete []m_pNewpasswd;
    }
}

ChPasswd * ChPasswd::GetChPasswd()
{
    std::unique_lock<std::mutex> lock(m_ChPasswdLock);
    if(m_pChPasswd == NULL)
    {
        m_pChPasswd = new ChPasswd();
    }

    return m_pChPasswd;
}

void ChPasswd::FreeChPasswd()
{
    std::unique_lock<std::mutex> lock(m_ChPasswdLock);
    if(m_pChPasswd != NULL)
    {
        delete m_pChPasswd;
        m_pChPasswd = NULL;
    }
}

int ChPasswd::Function(int cfd)
{
    char get_len[5];
    int len;
    int rev;

    memset(get_len, 0, sizeof(get_len));

    rev = recv(cfd, get_len, 4, 0);
    if(rev < 0)
    {
        perror("ChPasswd::id_len : ");
        return -1;
    }

    len = atoi(get_len);
    m_pID = new char[len + 1];
    memset(m_pID, 0, len + 1);
    rev = recv(cfd, m_pID, len, 0);
    if(rev < 0)
    {
        perror("ChPasswd::ID : ");
        return -1;
    }

    m_pPasswd = new char[17];
    memset(m_pPasswd, 0, 17);
    rev = recv(cfd, m_pPasswd, 16, 0);
    if(rev < 0)
    {
        perror("ChPasswd::password : ");
        return -1;
    }

    m_pNewpasswd = new char[17];
    memset(m_pNewpasswd, 0, 17);
    rev = recv(cfd, m_pNewpasswd, 16, 0);
    if(rev < 0)
    {
        perror("ChPasswd::Newpasswd : ");
        return -1;
    }

    printf("ChPasswd::id:%s\n",m_pID);
    printf("ChPasswd::password:%s\n\n",m_pPasswd);
    printf("ChPasswd::newpassword:%s\n",m_pNewpasswd);

    //数据读取完毕,进行数据库处理,并把处理的结果返回给客户端
    int s = DataBase();
    send(cfd, &s, sizeof(s), 0);

    return 0;
}


int ChPasswd::DataBase()
{
    int result = 0;
    char buf[1024];

    My_Sql sql("BankData");

    sprintf(buf,"select *from UserInformation where ID= %s;",m_pID);
    sql.MySql_exec(buf);
    printf("ChPasswd::row number:%d\n",sql.m_iNumRows);

    if(sql.m_iNumRows == 0)  //没有该用户
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
        else   //修改密码
        {
            sprintf(buf,"update UserInformation set passwd = '%s' where ID = %s;",m_pNewpasswd,m_pID);
            sql.MySql_exec(buf);

            result = CHPW_SUCCESS;
        }
    }

    return result;
}

