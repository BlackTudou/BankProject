#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include "Transfer.h"
#include "mark.h"
#include "My_Sql.h"

std::mutex Transfer::m_TransferLock;
Transfer * Transfer::m_pTransfer = NULL;

Transfer::Transfer() : m_pID(NULL),m_pPasswd(NULL),m_pMoney(NULL),m_pOtherId(NULL)
{

}

Transfer::~Transfer()
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


    if(m_pOtherId != NULL)
    {
        delete []m_pOtherId;
    }
}

Transfer * Transfer::GetTransfer()
{
    std::unique_lock<std::mutex> lock(m_TransferLock);
    if(m_pTransfer == NULL)
    {
        m_pTransfer = new Transfer();
    }
    return m_pTransfer;
}

void Transfer::FreeTransfer()
{
    std::unique_lock<std::mutex> lock(m_TransferLock);
    if(m_pTransfer != NULL)
    {
        delete m_pTransfer;
        m_pTransfer = NULL;
    }
}

int Transfer::Function(int cfd)
{
    char get_len[5];
    int len;
    int rev;

    memset(get_len, 0, sizeof(get_len));

    rev = recv(cfd, get_len, 4, 0);
    if(rev < 0)
    {
        perror("Transfer::id_len : ");
        return -1;
    }

    len = atoi(get_len);
    m_pID = new char[len + 1];
    memset(m_pID, 0, len + 1);
    rev = recv(cfd, m_pID, len, 0);
    if(rev < 0)
    {
        perror("Transfer::ID : ");
        return -1;
    }

    rev = recv(cfd, get_len, 4, 0);
    if(rev < 0)
    {
        perror("Transfer::money_len : ");
        return -1;
    }
    len = atoi(get_len);

    m_pMoney = new char[len + 1];
    memset(m_pMoney, 0, len+ 1);
    rev = recv(cfd, m_pMoney, len, 0);
    if(rev < 0)
    {
        perror("Transfer::money : ");
        return -1;
    }

    rev = recv(cfd, get_len, 4, 0);
    if(rev < 0)
    {
        perror("Transfer::otherID_len : ");
        return -1;
    }
    len = atoi(get_len);

    m_pOtherId = new char[len + 1];
    memset(m_pOtherId, 0, len+ 1);
    rev = recv(cfd, m_pOtherId, len, 0);
    if(rev < 0)
    {
        perror("Transfer::OtherId : ");
        return -1;
    }

    m_pPasswd = new char[17];
    memset(m_pPasswd, 0, 17);
    rev = recv(cfd, m_pPasswd, 16, 0);
    if(rev < 0)
    {
        perror("Transfer::password : ");
        return -1;
    }

    printf("Transfer::id:%s\n",m_pID);
    printf("Transfer::money:%s\n",m_pMoney);
    printf("Transfer::other_id:%s\n",m_pOtherId);
    printf("Transfer::password:%s\n\n",m_pPasswd);

    //数据读取完毕,进行数据库处理,并把处理的结果返回给客户端
    int s = DataBase();
    send(cfd, &s, sizeof(s), 0);

    return 0;
}

int Transfer::DataBase()
{
    int result = 0;
    char buf[1024];
    double money = 0.0;
    vector<string> row;
    My_Sql sql("BankData");

    sprintf(buf,"select *from UserInformation where ID= %s;",m_pID);
    sql.MySql_exec(buf);
    //sql.GetFieldsName(data);
    printf("Transfer::row number:%d\n",sql.m_iNumRows);
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
            sprintf(buf,"select *from UserInformation where ID= %s;",m_pOtherId);
            sql.MySql_exec(buf);
            if(sql.m_iNumRows == 0)
            {
                result = NO_OTHER_ACCOUNT;   //无对方账户
            }
            else
            {
                sprintf(buf,"select balance from _%s where number = (select max(number) from _%s);",m_pID, m_pID);
                sql.MySql_exec(buf);
                sql.GetRowInformation(row);
                std::cout << "Transfer::id  balance:" << row[0] << std::endl;

                money = atof(row[0].c_str()) - atof(m_pMoney);

                if(money < 0)  //余额不足
                {
                    result = NO_MONEY;
                }
                else
                {
                    // 己方减钱
                    sprintf(buf, "insert into _%s(draw, balance, other_account_id, action) values(%s, %f, %s, \"%s\");",
                            m_pID, m_pMoney, money, m_pOtherId, TRAN);
                    sql.MySql_exec(buf);

                    //对方加钱
                    sprintf(buf,"select balance from _%s where number = (select max(number) from _%s);",m_pOtherId, m_pOtherId);
                    sql.MySql_exec(buf);
                    sql.GetRowInformation(row);
                    std::cout << "Transfer::other_id balance:" << row[0] << std::endl;
                    money = atof(row[0].c_str()) + atof(m_pMoney);
                    sprintf(buf, "insert into _%s(deposit, balance, other_account_id, action) values(%s, %f, %s, \"%s\");",
                            m_pOtherId, m_pMoney, money, m_pID, TRAN);
                    sql.MySql_exec(buf);

                    result = DRAW_SUCCESS;
                }
            }
        }
    }

    return result;
}
