#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "Withdraw.h"
#include "mark.h"
#include "My_Sql.h"

std::mutex Withdraw::m_WithdrawLock;
Withdraw * Withdraw::m_pWithdraw = NULL;

Withdraw::Withdraw() : m_pID(NULL),m_pPasswd(NULL),m_pMoney(NULL)
{

}

Withdraw::~Withdraw()
{
    if(m_pID != NULL)
    {
        delete []m_pID;
    }

    if(m_pPasswd != NULL)
    {
        delete []m_pPasswd;
    }

    if(m_pMoney != NULL)
    {
        delete []m_pMoney;
    }
}

Withdraw * Withdraw::GetWithdraw()
{
    std::unique_lock<std::mutex> lock(m_WithdrawLock);
    if(m_pWithdraw == NULL)
    {
        m_pWithdraw = new Withdraw();
    }
    return m_pWithdraw;
}

void Withdraw::FreeWithdraw()
{
    std::unique_lock<std::mutex> lock(m_WithdrawLock);
    if(m_pWithdraw != NULL)
    {
        delete m_pWithdraw;
        m_pWithdraw = NULL;
    }
}

int Withdraw::Function(int cfd)
{
    char get_len[5];
    int len;
    int rev;

    memset(get_len, 0, sizeof(get_len));

    rev = recv(cfd, get_len, 4, 0);
    if(rev < 0)
    {
        perror("Withdraw::id_len : ");
        return -1;
    }

    len = atoi(get_len);
    m_pID = new char[len + 1];
    memset(m_pID, 0, len + 1);
    rev = recv(cfd, m_pID, len, 0);
    if(rev < 0)
    {
        perror("Withdraw::ID : ");
        return -1;
    }

    rev = recv(cfd, get_len, 4, 0);
    if(rev < 0)
    {
        perror("Withdraw::money_len : ");
        return -1;
    }
    len = atoi(get_len);

    m_pMoney = new char[len + 1];
    memset(m_pMoney, 0, len+ 1);
    rev = recv(cfd, m_pMoney, len, 0);
    if(rev < 0)
    {
        perror("Withdraw::money : ");
        return -1;
    }

    m_pPasswd = new char[17];
    memset(m_pPasswd, 0, 17);
    rev = recv(cfd, m_pPasswd, 16, 0);
    if(rev < 0)
    {
        perror("Withdraw::password : ");
        return -1;
    }

    printf("Withdraw::id:%s\n",m_pID);
    printf("Withdraw::money:%s\n",m_pMoney);
    printf("Withdraw::password:%s\n\n",m_pPasswd);

    //数据读取完毕,进行数据库处理,并把处理的结果返回给客户端
    int s = DataBase();
    send(cfd, &s, sizeof(s), 0);

    return 0;
}

int Withdraw::DataBase()
{
    int result = 0;
    char buf[1024];
    double money = 0.0;
    vector<string> row;
    My_Sql sql("BankData");

    sprintf(buf,"select *from UserInformation where ID= %s;",m_pID);
    sql.MySql_exec(buf);
    //sql.GetFieldsName(data);
    printf("Withdraw::row number:%d\n",sql.m_iNumRows);
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
            std::cout << "balance:" << row[0] << std::endl;

            money = atof(row[0].c_str()) - atof(m_pMoney);

            if(money < 0)  //余额不足
            {
                result = NO_MONEY;
            }
            else
            {
                sprintf(buf, "insert into _%s(draw, balance, action) values(%s,%f,\"%s\");", m_pID,m_pMoney,money,DRAW);
                sql.MySql_exec(buf);

                result = DRAW_SUCCESS;
            }
        }
    }

    return result;
}
