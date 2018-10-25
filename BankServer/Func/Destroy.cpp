#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <vector>
#include "My_Sql.h"
#include "Destroy.h"
#include "mark.h"

Destroy * Destroy::m_pDestroy = NULL;
std::mutex Destroy::m_DestroyLock;

Destroy::Destroy() : m_pId(NULL),m_pPasswd(NULL)
{

}

Destroy::~Destroy()
{
    if(m_pPasswd != NULL)
    {
        delete []m_pPasswd;
    }

    if(m_pId != NULL)
    {
        delete []m_pId;
    }
}

Destroy * Destroy::GetDestroy()
{
    std::unique_lock<std::mutex> lock(m_DestroyLock);
    if(m_pDestroy == NULL)
    {
        m_pDestroy = new Destroy();
    }

    return m_pDestroy;
}

void Destroy::FreeDestroy()
{
    std::unique_lock<std::mutex> lock(m_DestroyLock);
    if(m_pDestroy != NULL)
    {
        delete m_pDestroy;
        m_pDestroy = NULL;
    }
}

int Destroy::Function(int cfd)
{
    //printf("Destroy function!!!!!!\n");
    char get_len[5];
    int len;
    int rev;
    memset(get_len,0,sizeof(get_len));

    rev = recv(cfd, get_len, 4, 0);
    if(rev < 0)
    {
        perror("Destroy::id_len : ");
        return -1;
    }

    len = atoi(get_len);
    m_pId = new char[len + 1];
    memset(m_pId, 0, len + 1);
    rev = recv(cfd, m_pId, len, 0);
    if(rev < 0)
    {
        perror("Destroy::ID : ");
        return -1;
    }

    m_pPasswd = new char[ 17 ];
    memset(m_pPasswd, 0, 17);
    rev = recv(cfd, m_pPasswd, 16, 0);
    if(rev < 0)
    {
        perror("Destroy::passwd : ");
        return -1;
    }

    printf("Destroy::ID:%s\n",m_pId);
    printf("Destroy::passwd:%s\n",m_pPasswd);

    int s = DataBase();
    send(cfd,&s,sizeof(s),0);

    return 0;
}

int Destroy::DataBase()
{
    int result = 0;
    char buf[1024];
    //double money = 0.0;
    vector<string> row;
    My_Sql sql("BankData");

    sprintf(buf,"select *from UserInformation where ID= %s;",m_pId);
    sql.MySql_exec(buf);

    if(sql.m_iNumRows == 0) //没有该用户
    {
        result = NO_ACCOUNT;
    }
    else
    {
        sprintf(buf,"select passwd from UserInformation where ID= %s AND passwd=\"%s\";",m_pId,m_pPasswd);
        sql.MySql_exec(buf);
        if(sql.m_iNumRows == 0) //密码错误
        {
            result = PASSWD_ERROR;
        }
        else  //销户
        {
            sprintf(buf,"delete from UserInformation where id = %s;",m_pId);
            sql.MySql_exec(buf);

            //sprintf(buf,"drop table _%s;", m_pId.c_str());
            //sql.MySql_exec(buf);
            result = DEST_SUCCESS;
        }
    }

    return result;
}
