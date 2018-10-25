#ifndef DEPOSIT_H
#define DEPOSIT_H

#include <string>
#include <mutex>

using namespace std;

class deposit_ask
{
public:
    void Init(char *id, char *money);
    void Func(char **outstr);

private:
    int m_iMoneyLen;
    int m_iIdLen;

    string m_sID;
    string m_sMoney;
};

class Deposit
{
public:
    static Deposit * GetDeposit();
    static void FreeDeposit();

    int Function(int client_sock, char *id, char *money);

private:
    Deposit(){}
    static Deposit *m_pDeposit;
    static std::mutex m_DepositLock;

    deposit_ask m_cDepositAsk;   //发送

    int m_iResult;  //接收
};

#endif // DEPOSIT_H
