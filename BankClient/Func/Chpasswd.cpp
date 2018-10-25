#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <QDebug>
#include "Chpasswd.h"
#include "mark.h"
#include "MD5.h"

Chpasswd * Chpasswd::m_pChpasswd = NULL;
std::mutex Chpasswd::m_ChpasswdLock;

void chpasswd_ask::Init(char *id, char *passwd, char *newpasswd)
{
    m_sID.clear();
    m_sPasswd.clear();
    m_sNewPasswd.clear();

    m_sID = id;

    m_iIdLen = m_sID.size();
    m_sPasswd = MD5(passwd).hexdigest();
    m_sNewPasswd = MD5(newpasswd).hexdigest();
}

void chpasswd_ask::Func(char **outstr)
{
    int max_len = m_iIdLen + 41;
    //4个给行为,4个给id_len,32个给密码,1个给\0

    *outstr = new char[max_len];
    memset(*outstr, 0, max_len);
    sprintf(*outstr, "%s%04d%s", CHPW, m_iIdLen, m_sID.c_str());
    strncat(*outstr, m_sPasswd.c_str(), 16);
    strncat(*outstr, m_sNewPasswd.c_str(), 16);

    qDebug() << "chPasswd *outstr" << *outstr;
}

Chpasswd * Chpasswd::GetChpasswd()
{
    std::unique_lock<std::mutex> lock(m_ChpasswdLock);
    if(m_pChpasswd == NULL)
    {
        m_pChpasswd = new Chpasswd();
    }

    return m_pChpasswd;
}

void Chpasswd::FreeChpasswd()
{
    std::unique_lock<std::mutex> lock(m_ChpasswdLock);
    if(m_pChpasswd != NULL)
    {
        delete m_pChpasswd;
        m_pChpasswd = NULL;
    }
}

int Chpasswd::Function(int client_sock, char *id, char *passwd, char *newpasswd)
{
    char *str;
    m_cChpasswdAsk.Init(id,passwd,newpasswd);
    m_cChpasswdAsk.Func(&str);

    send(client_sock, str, strlen(str), 0);

    recv(client_sock,&m_iResult,sizeof(m_iResult),0);

    delete []str;
    return m_iResult;
}

