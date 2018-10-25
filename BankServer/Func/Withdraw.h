#ifndef WITHDRAW_H
#define WITHDRAW_H

#include "Abstract.h"
#include <mutex>

class Withdraw : public AB_Function
{
public :
    static Withdraw * GetWithdraw();
    static void FreeWithdraw();

    virtual int Function(int cfd);
    ~Withdraw();
private :
    Withdraw();
    static std::mutex m_WithdrawLock;
    static Withdraw * m_pWithdraw;

    int DataBase();
private:
    char *m_pID;
    char *m_pPasswd;
    char *m_pMoney;
};

#endif // WITHDRAW_H
