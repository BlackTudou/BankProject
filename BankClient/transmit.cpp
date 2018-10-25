#include "transmit.h"
#include "MD5.h"

#include <string.h>
#include <unistd.h>
#include <QDebug>
#include <sys/types.h>
#include <sys/socket.h>

#include "mark.h"

transmit_ask::transmit_ask():m_iLen(0)
{
    m_sName.clear();
    m_sPasswd.clear();
}

void transmit_ask::Init(char *name , char *passwd)
{
    m_sName = name;
    qDebug()<< "m_sPasswd:" << passwd;
    m_sPasswd = MD5(passwd).hexdigest();
    qDebug()<< "After m_sPasswd:" << m_sPasswd.c_str();
    m_iLen = m_sName.size();

    qDebug() << "m_iLen=" << m_iLen; // size 当前字符串大小;length 当前字符串长度,测试结果这俩返回结果一毛一样
}

void transmit_ask::Func(char **outstr, int flag)
{
    int max_len = m_sName.size()+22;
    //22中，1个字符给_len，16个给密码，还有1个给\0,4个给行为
    qDebug() << "max_len:" << max_len;

    *outstr = new char[max_len];
    qDebug() << sizeof(outstr);
    memset(*outstr,0,sizeof(*outstr));

    if(flag == 1)   //登录
    {
        snprintf(*outstr,max_len,"%s%d%s%s",LOG,m_iLen,m_sName.c_str(),m_sPasswd.c_str());
    }
    else
    {
        snprintf(*outstr,max_len,"%s%d%s%s",REGIST,m_iLen,m_sName.c_str(),m_sPasswd.c_str());
    }
    qDebug() << *outstr;
}

Transmit::Transmit(int client,char *name,char *passwd):m_iSfd(client)
{
    m_cAsk.Init(name,passwd);
}

int Transmit::Action(int flag)
{
    char *str;
    m_cAsk.Func(&str,flag);

    if(send(m_iSfd,str,strlen(str),0)<0)//发送给服务器进行处理
    {
        qDebug()<<"send error";
    }

    qDebug() << "str:" << str;
    qDebug() << "strlen(str)" << strlen(str);
    recv(m_iSfd,&m_iReply,sizeof(m_iReply),0);//读取服务器处理后的结果

    delete []str;

    return m_iReply;
}



