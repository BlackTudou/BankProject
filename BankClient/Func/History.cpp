#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <QDebug>
#include "History.h"
#include "mark.h"
#include "MD5.h"

History * History::m_pHistory = NULL;
std::mutex History::m_HistoryLock;

void history_ask::Init(char *id, char *passwd, char *str1, char *str2)
{
    m_sID.clear();
    m_sPasswd.clear();
    m_sStr1.clear();
    m_sStr2.clear();

    m_sID = id;
    m_sStr1 = str1;
    m_sStr2 = str2;
    m_sPasswd = MD5(passwd).hexdigest();

    m_iIdLen = m_sID.size();
    m_iStr1Len = m_sStr1.size();
    m_iStr2Len = m_sStr2.size();
}

void history_ask::Func(char **outstr)
{
    int max_len = m_iStr1Len + m_iStr2Len + m_iIdLen + 33;
    //33中，4个给行为，4个给_str1_len，4个给_str2_len, 4个idlen
    //1个给\0，16个给密码

    *outstr = new char[max_len];
    memset(*outstr,0,max_len);
    snprintf(*outstr,max_len,"%s%04d%s%04d%s%04d%s%s",HIST,m_iIdLen,
             m_sID.c_str(),m_iStr1Len,
             m_sStr1.c_str(),m_iStr2Len,
             m_sStr2.c_str(),m_sPasswd.c_str());

    //qDebug() << *outstr;
}

History * History::GetHistory()
{
    std::unique_lock<std::mutex> lock(m_HistoryLock);
    if(m_pHistory == NULL)
    {
        m_pHistory = new History();
    }

    return m_pHistory;
}

void History::FreeHistory()
{
    std::unique_lock<std::mutex> lock(m_HistoryLock);
    if(m_pHistory != NULL)
    {
        delete m_pHistory;
        m_pHistory = NULL;
    }
}

int History::Function(int client_sock, char *id, char *passwd, char *str1, char*str2)
{
    //printf("Hist funccccc!!!!!!\n");
    char *str;
    m_cHistoryAsk.Init(id,passwd,str1,str2);
    m_cHistoryAsk.Func(&str);

    send(client_sock,str,strlen(str),0);

    recv(client_sock,&m_iResult,sizeof(m_iResult),0);

    if(m_iResult == HIST_SUCCESS)
    {
        HandleInformation(client_sock);
    }

    delete []str;
    return m_iResult;
}

string History::GetInformation()
{
    return m_sInformation;
}

void History::HandleInformation(int client_sock)
{
    char *buf;
    int len;
    m_sInformation.clear();

    recv(client_sock, &len, sizeof(len),0);
    while(len != 0)
    {
        buf = new char[len + 1];
        recv(client_sock,buf,len,0);
        buf[len+1] = '\n';
        m_sInformation.append(buf);
        delete []buf;
        recv(client_sock,&len,sizeof(len),0);
    }
}







