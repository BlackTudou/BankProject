#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>

#include "Reg.h"
#include "My_Sql.h"
#include "Pass.h"
#include "mark.h"

Reg *Reg::m_pReg = NULL;
std::mutex Reg::m_RegLock;

Reg* Reg::GetReg()
{
    std::unique_lock<std::mutex> lock(m_RegLock);
    if(m_pReg == NULL)
    {
        m_pReg = new Reg();
    }
    return m_pReg;
}

void Reg::FreeReg()
{
    std::unique_lock<std::mutex> lock(m_RegLock);
    if(m_pReg != NULL)
    {
        delete m_pReg;
        m_pReg = NULL;
    }
}

int Reg::Function(int cfd)
{
    printf("I am in reg\n");
    char len = 0;
    int rev = -1;
    while(rev < 0)
    {
        rev = recv(cfd, &len, sizeof(char),0); //由于sock是非阻塞，因此要循环读取数据
    }
    printf("len=%c\n",len);

    m_pName = new char[len-'0'+1];
    memset(m_pName,0,len-'0'+1);
    rev = recv(cfd, m_pName, len-'0', 0);
    if(rev < 0)
    {
        perror("Reg->name:");
        return -1;
    }

    m_pPasswd = new char[17];
    memset(m_pPasswd, 0, 17);
    rev = recv(cfd, m_pPasswd, 16, 0);
    if(rev < 0)
    {
        perror("Reg->password:");
        return -1;
    }
    std::cout << "m_pPasswd:" << m_pPasswd << std::endl;

 //数据读取完毕,进行数据库处理,并把处理的结果返回给客户端
    int s = DataBase();
    send(cfd, &s, sizeof(s), 0);

    return 0;
}

int Reg::DataBase()
{
    //printf("DataBase deal\n");
    char buf[100];
    My_Sql sql("BankData");
    int result = 0;

    sql.MySql_exec("create table if not exists Clerk(id int unsigned not null auto_increment primary key,name char(10) not null,password char(16) not null);");

    sprintf(buf,"select *from Clerk where name=\"%s\";",m_pName);
    sql.MySql_exec(buf);
    printf("Reg::row number:%d\n",sql.m_iNumRows);
    if(sql.m_iNumRows == 0)  //没有该用户,可以注册
    {
        sprintf(buf,"insert into Clerk values(NULL,\"%s\",\"%s\");",m_pName,m_pPasswd);
        sql.MySql_exec(buf);
        result = REGIST_SUCCESS;
    }
    else
    {
        result = HAS_ACCOUNT;
    }
    return result;
}
