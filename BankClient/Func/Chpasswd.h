#ifndef CHPASSWD_H
#define CHPASSWD_H

#include <string>
#include <mutex>

using namespace std;

class chpasswd_ask
{
public:
    void Init(char *id, char *passwd, char *newpasswd);
    void Func(char **outstr);

private:
    int m_iIdLen;

    string m_sID;
    string m_sPasswd;
    string m_sNewPasswd;
};

class Chpasswd
{
public:
    static Chpasswd * GetChpasswd();
    static void FreeChpasswd();

    int Function(int client_sock, char *id, char *passwd, char *newpasswd);

    ~Chpasswd(){}

private:
    Chpasswd(){}
    static Chpasswd *m_pChpasswd;
    static std::mutex m_ChpasswdLock;

    chpasswd_ask m_cChpasswdAsk;   //发送
    int m_iResult;  //接收
};

#endif // CHPASSWD_H
