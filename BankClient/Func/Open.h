#ifndef OPEN_H
#define OPEN_H

#include <mutex>
#include <string>
using namespace std;

class open_ask
{
public:
    void Init(char *name, char *id, char *passwd, char *money);
    void Func(char **outstr);

private:
    int m_iNameLen;
    int m_iMoneyLen;
    string m_sName;
    string m_sID;
    string m_sPasswd;
    string m_sMoney;
};

class Open
{
public:
    int Function(int client_cfd, char *name, char *passwd, char *id, char *money);
    //char *GetID();
    int GetID();

    static Open *GetOpen();
    static void FreeOpen();
private:
    Open(){}
    static std::mutex m_OpenLock;
    static Open * m_pOpen;

    open_ask m_cOpenAsk;
    int m_iResult;
    //char m_aID[7];
    int m_iID;
};

#endif // OPEN_H
