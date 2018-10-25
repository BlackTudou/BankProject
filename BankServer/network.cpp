#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <iostream>

#include "network.h"

#define MAX_EVENT_NUMBER 1024
#define BUFFER_SIZE 1024

Selectfunc *Socket::m_pSelect = NULL;

Socket::Socket(char *ip, int port):m_pIP(ip),m_iPort(port)
{
    std::cout << "constructor success" << std::endl;
    m_pSelect = Selectfunc::GetSel();
}

/* 将文件描述符设置成非阻塞的 */
int Socket::setnonblocking( int fd )
{
    int old_option = fcntl( fd, F_GETFL );		/* 获取文件描述符旧的状态标志 */
    int new_option = old_option | O_NONBLOCK;	/* 设置非阻塞标志 */
    fcntl( fd, F_SETFL, new_option );
    return old_option;							/* 返回文件描述符旧的状态标志，以便日后恢复该状态标志 */
}

void Socket::addfd( int epollfd, int fd, bool oneshot )
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    if( oneshot )
    {
        event.events |= EPOLLONESHOT;
    }
    epoll_ctl( epollfd, EPOLL_CTL_ADD, fd, &event );
    setnonblocking( fd );  // 将这个文件描述符设成非阻塞的
}

void Socket::reset_oneshot(int epollfd, int fd)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
    epoll_ctl( epollfd, EPOLL_CTL_MOD, fd, &event );
}

void* worker( void* arg )
{
    int sockfd = ( (fds*)arg )->sockfd;
    int epollfd = ( (fds*)arg )->epollfd;
    printf( "start new thread to receive data on fd: %d\n", sockfd );
    char action[5]; //行为标志
    memset(action, 0, sizeof(action));
    while( 1 )
    {
        int ret = recv( sockfd, action, sizeof(action)-1, 0 );
        //printf("ret=%d\n",ret);
        if( ret == 0 )
        {
            close( sockfd );
            printf( "foreiner closed the connection\n" );
            break;
        }
        else if( ret < 0 )
        {
            if( errno == EAGAIN )
            {
                Socket::reset_oneshot( epollfd, sockfd ); //将套接字再加进到队列中
                printf( "read later\n" );
                break;
            }
        }
        else
        {
            //printf("action=%s\n",action);
            printf("get content:action = %s\n",action);
            Socket::m_pSelect->Func(action,sockfd);

            //sleep( 5 );
        }
    }
    printf( "end thread receiving data on fd: %d\n", sockfd );

    return NULL;
}

void Socket::Work()
{
    int ret = 0;

    int listenfd = socket( PF_INET, SOCK_STREAM, 0 );
    assert( listenfd >= 0 );

    struct sockaddr_in address;
    bzero( &address, sizeof( address ) );
    address.sin_family = AF_INET;
    inet_pton( AF_INET, m_pIP, &address.sin_addr );
    address.sin_port = htons( m_iPort );

    //使服务器可以多次绑定
    int on = 1;
    if(setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)) < 0)
    {
        perror("Setsockopt error !");
        exit(-1);
    }

    ret = bind( listenfd, ( struct sockaddr* )&address, sizeof( address ) );
    //assert( ret != -1 );
    if(ret == -1)
    {
        perror("bind error:");
    }

    ret = listen( listenfd, 5 );
    assert( ret != -1 );

    epoll_event events[MAX_EVENT_NUMBER];
    int epollfd = epoll_create( 5 );
    assert( epollfd != -1 );
    addfd( epollfd, listenfd, false );

    while( 1 )
    {
        int ret = epoll_wait( epollfd, events, MAX_EVENT_NUMBER, -1 );
        //printf("epoll wait success\n");
        if ( ret < 0 )
        {
            std::cout<<  "epoll failure" << std::endl;
            break;
        }

        for ( int i = 0; i < ret; i++ )
        {
            int sockfd = events[i].data.fd;
            if ( sockfd == listenfd )
            {
                struct sockaddr_in client_address;
                socklen_t client_addrlength = sizeof( client_address );
                int connfd = accept( listenfd, ( struct sockaddr* )&client_address, &client_addrlength );
                addfd( epollfd, connfd, true );
                printf("sockfd == listenfd\n");
            }
            else if ( events[i].events & EPOLLIN )
            {
                pthread_t thread;
                fds fds_for_new_worker;
                fds_for_new_worker.epollfd = epollfd;
                fds_for_new_worker.sockfd = sockfd;
                pthread_create( &thread, NULL, worker, ( void* )&fds_for_new_worker );
                printf("read message\n");
            }
            else
            {
                printf( "something else happened \n" );
            }
        }

    }

    close( listenfd );
}






