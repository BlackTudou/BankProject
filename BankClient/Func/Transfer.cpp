#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "Transfer.h"
#include "mark.h"
#include "MD5.h"

Transfer *Transfer::m_pTransfer = NULL;
std::mutex Transfer::m_TransferLock;

void transfer_ask::Init(char *id, char *passwd, char *money, char *other_id)
{
    m_sID.clear();
    m_sPasswd.clear();
    m_sMoney.clear();
    m_sOtherId.clear();

    m_sID = id;
    m_sPasswd = MD5(passwd).hexdigest();
    m_sMoney = money;
    m_sOtherId = other_id;

    m_iMoneyLen = m_sMoney.size();
    m_iIdLen = m_sID.size();
    m_iOtherIdLen = m_sOtherId.size();
}


void transfer_ask::Func(char **outstr)
{
    int max_len = m_iMoneyLen + m_iIdLen + m_iOtherIdLen + 33;
    //4个给操作命令,4个给idlen,4个给moneylen,1个给\0,16个给密码,4个other_idLen
    //4+4+4+1=13
    //13+16 = 29
    // 29+4 = 33

    *outstr = new char[max_len];

    snprintf(*outstr, max_len, "%s%04d%s%04d%s%04d%s%s",
             TRAN,m_iIdLen,m_sID.c_str(),m_iMoneyLen,m_sMoney.c_str(),m_iOtherIdLen,m_sOtherId.c_str(),m_sPasswd.c_str());
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


int Transfer::Function(int client_sock, char *id, char *passwd, char *money,char *other_id)
{
    char *str;

    m_cTransferAsk.Init(id,passwd,money,other_id);
    m_cTransferAsk.Func(&str);

    send(client_sock, str, strlen(str), 0);
    recv(client_sock, &m_iResult, sizeof(m_iResult), 0);

    delete []str;
    return m_iResult;
}
