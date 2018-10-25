#ifndef SOCK_H
#define SOCK_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <mutex>

class Sock
{
public:
    static Sock *GetSock();
    static void FreeSock();

    int my_connect();
private:
    Sock();

    static Sock *m_pSock;
    static std::mutex m_ClientLock;

    int m_iClientSfd;
    struct sockaddr_in m_sServerAddr;
    struct sockaddr_in m_sClientAddr;
    static const int PORT = 8000; //端口号
};

#endif // SOCK_H
