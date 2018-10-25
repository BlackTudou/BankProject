#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "Pass.h"
#include "My_Sql.h"
#include "mark.h"

Pass *Pass::m_pPass = NULL;
std::mutex Pass::m_PassLock;

Pass::Pass(){}

Pass* Pass::GetPass()
{
    std::unique_lock<std::mutex> lock(m_PassLock);
    if(m_pPass == NULL)
    {
        m_pPass = new Pass();
    }
    return m_pPass;
}


void Pass::FreePass()
{
    std::unique_lock<std::mutex> lock(m_PassLock);
    if(m_pPass != NULL)
    {
        delete m_pPass;
        m_pPass = NULL;
    }
}


int Pass::Function(int cfd)
{
    printf("I am in pass\n");
    char len = 0;
    int rev = -1;
    while(rev < 0)
    {
        rev = recv(cfd, &len, sizeof(char),0);
    }
    printf("len=%c\n",len);

    m_pName = new char[len-'0'+1];
    memset(m_pName, 0, len-'0'+1);
    if(recv(cfd,m_pName,len-'0',0) < 0)
    {
        perror("Pass->name:");
        return -1;
    }

    m_pPasswd = new char[17];
    memset(m_pPasswd, 0, 17);
    rev = recv(cfd, m_pPasswd, 16, 0);
    if(rev < 0)
    {
        perror("Pass->passwd:");
    }

    //数据读取完毕,进行数据库处理,并把处理的结果返回给客户端
    int s = DataBase();
    send(cfd, &s, sizeof(s), 0);

    return 0;
}

int Pass::DataBase()
{
    //printf("DataBase deal\n");
    char buf[100];
    My_Sql sql("BankData");
    int result = 0;

    sql.MySql_exec("create table if not exists Clerk(id int unsigned not null auto_increment primary key,name char(10) not null,password char(16) not null);");

    sprintf(buf,"select *from Clerk where name=\"%s\";",m_pName);
    sql.MySql_exec(buf);
    printf("Pass:: row number:%d\n",sql.m_iNumRows);
    if(sql.m_iNumRows == 0)  //没有该用户,需要先注册
    {
        result = NO_ACCOUNT;
    }
    else
    {
        sprintf(buf,"select *from Clerk where name=\"%s\" AND password=\"%s\";",m_pName,m_pPasswd);
        sql.MySql_exec(buf);
        printf("Pass::row number:%d\n",sql.m_iNumRows);
        if(sql.m_iNumRows == 0)
        {
            result = PASSWD_ERROR;
        }
        else
        {
            result = LOG_SUCCESS;
        }
    }

    return result;
}
