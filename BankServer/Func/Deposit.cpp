#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <vector>
#include "Deposit.h"
#include "mark.h"
#include "My_Sql.h"

using namespace std;

Deposit * Deposit::m_pDeposit = NULL;
std::mutex Deposit::m_DepositLock;

Deposit * Deposit::GetDeposit()
{
    std::unique_lock<std::mutex> lock(m_DepositLock);
    if(m_pDeposit == NULL)
    {
        m_pDeposit = new Deposit();
    }

    return m_pDeposit;
}

void Deposit::FreeDeposit()
{
    std::unique_lock<std::mutex> lock(m_DepositLock);
    if(m_pDeposit != NULL)
    {
        delete m_pDeposit;
        m_pDeposit = NULL;
    }
}

Deposit::Deposit() : m_pID(NULL),m_pMoney(NULL)
{

}

Deposit::~Deposit()
{
    if(m_pID != NULL)
    {
        delete []m_pID;
    }

    if(m_pMoney != NULL)
    {
        delete []m_pMoney;
    }
}

int Deposit::Function(int cfd)
{
    char get_len[5];
    int len;
    int rev;

    memset(get_len, 0, sizeof(get_len));

    rev = recv(cfd, get_len, 4, 0);
    if(rev < 0)
    {
        perror("Deposit::id_len : ");
        return -1;
    }

    len = atoi(get_len);
    m_pID = new char[len + 1];
    memset(m_pID, 0, len + 1);
    rev = recv(cfd, m_pID, len, 0);
    if(rev < 0)
    {
        perror("Deposit::ID : ");
        return -1;
    }

    rev = recv(cfd, get_len, 4, 0);
    if(rev < 0)
    {
        perror("Deposit::money_len : ");
        return -1;
    }
    len = atoi(get_len);

    m_pMoney = new char[len + 1];
    memset(m_pMoney, 0, len+ 1);
    rev = recv(cfd, m_pMoney, len, 0);
    if(rev < 0)
    {
        perror("Deposit::money : ");
        return -1;
    }

    printf("deposit::id:%s\n",m_pID);
    printf("deposit::money:%s\n\n",m_pMoney);

    //数据读取完毕,进行数据库处理,并把处理的结果返回给客户端
    int s = DataBase();
    send(cfd, &s, sizeof(s), 0);

    return 0;
}


int Deposit::DataBase()
{
    int result = 0;
    char buf[1024];
    double money = 0.0;
    vector<string> row;
    My_Sql sql("BankData");

    sprintf(buf,"select *from UserInformation where ID= %s;",m_pID);
    sql.MySql_exec(buf);
    //sql.GetFieldsName(data);
    printf("Deposit::row number:%d\n",sql.m_iNumRows);
    if(sql.m_iNumRows == 0) //没有该用户
    {
        result = NO_ACCOUNT;
    }
    else
    {
        sprintf(buf,"select balance from _%s where number = (select max(number) from _%s);",m_pID, m_pID);
        sql.MySql_exec(buf);
        sql.GetRowInformation(row);
        std::cout << "deposit::balance:" << row[0] << std::endl;

        money = atof(row[0].c_str()) + atof(m_pMoney);

        sprintf(buf, "insert into _%s(deposit, balance, action) values(%s,%f,\"%s\");", m_pID,m_pMoney,money,DPST);
        sql.MySql_exec(buf);

        result = DPST_SUCCESS;
    }

    return result;
}


