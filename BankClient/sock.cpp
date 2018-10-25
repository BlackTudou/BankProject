#include "sock.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <QMessageBox>

Sock *Sock::m_pSock = NULL;
std::mutex Sock::m_ClientLock;

Sock::Sock()
{
    m_iClientSfd = socket(AF_INET, SOCK_STREAM, 0);	//创建客户端的套接字
    if(m_iClientSfd < 0)
    {
        perror("socket error");
        exit(-1);
    }

    bzero(&m_sServerAddr,sizeof(m_sServerAddr));
    m_sServerAddr.sin_family = AF_INET;
    m_sServerAddr.sin_port = htons(PORT);
    //m_sServerAddr.sin_addr.s_addr = inet_addr("172.20.10.4");
    m_sServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
}


Sock *Sock::GetSock()
{
    std::unique_lock<std::mutex> lock(m_ClientLock);
    if(m_pSock == NULL)
    {
        m_pSock = new Sock();
    }
    return m_pSock;
}

void Sock::FreeSock()
{
    std::unique_lock<std::mutex> lock(m_ClientLock);
    if(m_pSock != NULL)
    {
        delete m_pSock;
        m_pSock = NULL;
    }
}

int Sock::my_connect()
{
    int ret;

    while(connect(m_iClientSfd,(struct sockaddr *)&m_sServerAddr,sizeof(struct sockaddr_in)) < 0)
    {
        perror("connect error");
        exit(-1);
    }

    return m_iClientSfd;
}
