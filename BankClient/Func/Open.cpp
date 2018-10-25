#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <QDebug>
#include "Open.h"
#include "mark.h"
#include "MD5.h"

Open * Open::m_pOpen = NULL;
std::mutex Open::m_OpenLock;

void open_ask::Init(char *name, char *id, char *passwd, char *money)
{
    m_sName.clear();
    m_sID.clear();
    m_sMoney.clear();
    m_sPasswd.clear();

    m_sName = name;
    m_sID = id;
    m_sMoney = money;
    m_sPasswd = MD5(passwd).hexdigest();

    m_iNameLen = m_sName.size();
    m_iMoneyLen = m_sMoney.size();
}

void open_ask::Func(char **outstr)
{
    int max_len = m_iNameLen + m_iMoneyLen + 47;
    // 4个行为,4个namelen,18个给ID,16个给密码,4个给moneylen,1个给\0
    // 4+4+18+16+4+1=47
    *outstr = new char[max_len];
    memset(*outstr, 0, max_len);

    snprintf(*outstr,max_len,"%s%04d%s%04d%s%s%s",OPEN,m_iNameLen,m_sName.c_str(),m_iMoneyLen,m_sMoney.c_str(),m_sID.c_str(),m_sPasswd.c_str());
    //%04d 按照4个位子的空间左对齐 多余的位子用0代替

    qDebug() << "open::outstr:" << *outstr;
}

Open *Open::GetOpen()
{
    std::unique_lock<std::mutex> lock(m_OpenLock);
    if(m_pOpen == NULL)
    {
        m_pOpen = new Open();
    }
    return m_pOpen;
}

void Open::FreeOpen()
{
    std::unique_lock<std::mutex> lock(m_OpenLock);
    if(m_pOpen != NULL)
    {
        delete m_pOpen;
        m_pOpen = NULL;
    }
}

int Open::Function(int client_cfd, char *name, char *passwd, char *id, char *money)
{
    char *str=NULL;
    m_cOpenAsk.Init(name,id,passwd,money);
    m_cOpenAsk.Func(&str);

    send(client_cfd,str,strlen(str),0);

    recv(client_cfd,&m_iResult,sizeof(m_iResult),0);
    if(m_iResult == REGIST_SUCCESS)
    {

        recv(client_cfd, &m_iID, sizeof(m_iID),0);
        qDebug() << "Open::ID:" << m_iID;
    }

    delete []str;

    return m_iResult;
}

int Open::GetID()
{
    return m_iID;
}

