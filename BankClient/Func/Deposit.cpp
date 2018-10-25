#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "Deposit.h"
#include "mark.h"

Deposit *Deposit::m_pDeposit = NULL;
std::mutex Deposit::m_DepositLock;

void deposit_ask::Init(char *id, char *money)
{
    m_sID.clear();
    m_sMoney.clear();

    m_sID = id;
    m_sMoney = money;

    m_iMoneyLen = m_sMoney.size();
    m_iIdLen = m_sID.size();
}


void deposit_ask::Func(char **outstr)
{
    int max_len = m_iMoneyLen + m_iIdLen + 13;
    //4个给操作命令,4个给idlen,4个给moneylen,1个给\0
    //4+4+4+1=13

    *outstr = new char[max_len];

    snprintf(*outstr, max_len, "%s%04d%s%04d%s",DPST,m_iIdLen,m_sID.c_str(),m_iMoneyLen,m_sMoney.c_str());
}

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


int Deposit::Function(int client_sock, char *id, char *money)
{
    char *str;

    m_cDepositAsk.Init(id,money);
    m_cDepositAsk.Func(&str);

    send(client_sock, str, strlen(str), 0);
    recv(client_sock, &m_iResult, sizeof(m_iResult), 0);

    delete []str;
    return m_iResult;
}
