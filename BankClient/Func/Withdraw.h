#ifndef WITHDRAW_H
#define WITHDRAW_H

#include <string>
#include <mutex>

using namespace std;

class withdraw_ask
{
public:
    void Init(char *id, char *passwd, char *money);
    void Func(char **outstr);

private:
    int m_iMoneyLen;
    int m_iIdLen;

    string m_sID;
    string m_sPasswd;
    string m_sMoney;
};

class Withdraw
{
public:
    static Withdraw * GetWithdraw();
    static void FreeWithdraw();

    int Function(int client_sock, char *id, char *passwd, char *money);

private:
    Withdraw(){}
    static Withdraw *m_pWithdraw;
    static std::mutex m_WithdrawLock;

    withdraw_ask m_cWithdrawAsk;   //发送

    int m_iResult;  //接收
};

#endif // WITHDRAW_H
