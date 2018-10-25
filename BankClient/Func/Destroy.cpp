#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "Destroy.h"
#include "mark.h"
#include "MD5.h"

Destroy * Destroy::m_pDestroy = NULL;
std::mutex Destroy::m_DestroyLock;

void destroy_ask::Init(char *id, char *passwd)
{
    m_sId.clear();
    m_sPasswd.clear();

    m_sId = id;
    m_sPasswd = MD5(passwd).hexdigest();

    m_iIdLen = m_sId.size();

}

void destroy_ask::Func(char **outstr)
{
    int max_len = m_iIdLen + 25;
    //25个中，4个长度，4个操作，1个\0,16个密码
    *outstr = new char [max_len];
    snprintf(*outstr,max_len,"%s%04d%s%s",
             DEST,m_iIdLen,m_sId.c_str(),m_sPasswd.c_str());

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

int Destroy::Function(int client_sock, char *id,char *passwd)
{
    char *str;
    m_DestroyAsk.Init(id,passwd);
    m_DestroyAsk.Func(&str);
   // qDebug() << str;
    send(client_sock,str,strlen(str),0);
    recv(client_sock,&m_iResult,sizeof(m_iResult),0);

    delete []str;

    return m_iResult;
}

Destroy::Destroy()
{

}
