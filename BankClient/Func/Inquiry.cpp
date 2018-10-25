#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "Inquiry.h"
#include "mark.h"
#include "MD5.h"

Inquiry *Inquiry::m_pInquiry = NULL;
std::mutex Inquiry::m_InquiryLock;

void inquiry_ask::Init(char *id, char *passwd)
{
    m_sID.clear();
    m_sPasswd.clear();

    m_sID = id;
    m_sPasswd = MD5(passwd).hexdigest();

    m_iIdLen = m_sID.size();
}

void inquiry_ask::Func(char **outstr)
{
    int max_len = m_iIdLen + 25;
    //4个给操作命令,4个给idlen,1个给\0,16个给密码
    //4+4+1+16 = 25

    *outstr = new char[max_len];

    snprintf(*outstr, max_len, "%s%04d%s%s",
             INQU,m_iIdLen,m_sID.c_str(),m_sPasswd.c_str());
}

Inquiry * Inquiry::GetInquiry()
{
    std::unique_lock<std::mutex> lock(m_InquiryLock);
    if(m_pInquiry == NULL)
    {
        m_pInquiry = new Inquiry();
    }

    return m_pInquiry;
}

void Inquiry::FreeInquiry()
{
    std::unique_lock<std::mutex> lock(m_InquiryLock);
    if(m_pInquiry != NULL)
    {
        delete m_pInquiry;
        m_pInquiry = NULL;
    }
}


int Inquiry::Function(int client_sock, char *id, char *passwd)
{
    char *str;

    m_cInquiryAsk.Init(id,passwd);
    m_cInquiryAsk.Func(&str);

    send(client_sock, str, strlen(str), 0);
    recv(client_sock, &m_iResult, sizeof(m_iResult), 0);

    if(m_iResult == INQU_SUCCESS)
    {
        m_pMoney = new char[30];
        memset(m_pMoney, 0, 30);
        recv(client_sock, m_pMoney, 30, 0);
    }

    delete []str;
    return m_iResult;
}


char *Inquiry::GetMoney()
{
    return m_pMoney;
}

Inquiry::Inquiry() : m_pMoney(NULL)
{

}

Inquiry::~Inquiry()
{
    if(m_pMoney != NULL)
    {
        delete []m_pMoney;
    }
}



