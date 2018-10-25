#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "Withdraw.h"
#include "mark.h"
#include "MD5.h"

Withdraw *Withdraw::m_pWithdraw = NULL;
std::mutex Withdraw::m_WithdrawLock;

void withdraw_ask::Init(char *id, char *passwd, char *money)
{
    m_sID.clear();
    m_sPasswd.clear();
    m_sMoney.clear();

    m_sID = id;
    m_sPasswd = MD5(passwd).hexdigest();
    m_sMoney = money;

    m_iMoneyLen = m_sMoney.size();
    m_iIdLen = m_sID.size();
}


void withdraw_ask::Func(char **outstr)
{
    int max_len = m_iMoneyLen + m_iIdLen + 29;
    //4个给操作命令,4个给idlen,4个给moneylen,1个给\0,16个给密码
    //4+4+4+1=13
    //13+16 = 29

    *outstr = new char[max_len];

    snprintf(*outstr, max_len, "%s%04d%s%04d%s%s",DRAW,m_iIdLen,m_sID.c_str(),m_iMoneyLen,m_sMoney.c_str(),m_sPasswd.c_str());
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


int Withdraw::Function(int client_sock, char *id, char *passwd, char *money)
{
    char *str;

    m_cWithdrawAsk.Init(id,passwd,money);
    m_cWithdrawAsk.Func(&str);

    send(client_sock, str, strlen(str), 0);
    recv(client_sock, &m_iResult, sizeof(m_iResult), 0);

    delete []str;
    return m_iResult;
}
