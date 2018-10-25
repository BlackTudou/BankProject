#ifndef NETWORK_H
#define NETWORK_H

#include "Selectfunc.h"

struct fds
{
    int epollfd;
    int sockfd;
};

class Socket
{
public:
    Socket(char *ip, int port);
    void Work();
    static void reset_oneshot( int epollfd, int fd );

    static Selectfunc *m_pSelect;
private:
    char * m_pIP;
    int m_iPort;

    int setnonblocking( int fd );
    void addfd(int epollfd, int fd, bool oneshot);

};

#endif // NETWORK_H
