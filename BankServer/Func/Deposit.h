#ifndef DEPOSIT_H
#define DEPOSIT_H

#include "Abstract.h"
#include <mutex>

class Deposit : public AB_Function
{
public :
    static Deposit * GetDeposit();
    static void FreeDeposit();

    virtual int Function(int cfd);
    ~Deposit();
private :
    Deposit();
    static std::mutex m_DepositLock;
    static Deposit * m_pDeposit;

    int DataBase();
private:
    char *m_pID;
    char *m_pMoney;
};


#endif // DEPOSIT_H
