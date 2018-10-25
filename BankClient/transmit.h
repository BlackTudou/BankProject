#ifndef TRANSMIT_H
#define TRANSMIT_H

#include <string>

class transmit_ask
{
public :
    transmit_ask();
    void Init(char *name , char *passwd);
    void Func(char **outstr, int flag);

private:
    int m_iLen;   //姓名的长度
    std::string m_sName;
    std::string m_sPasswd;
};


class Transmit
{
public:
    Transmit(int client,char *name,char *passwd);
    int Action(int flag);

private:
    int m_iSfd;
    transmit_ask m_cAsk;  //请求信号
    int m_iReply;      //应答标志
};

#endif // TRANSMIT_H
