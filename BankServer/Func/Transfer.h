#ifndef TRANSFER_H
#define TRANSFER_H

#include "Abstract.h"
#include <mutex>

class Transfer : public AB_Function
{
public :
    static Transfer * GetTransfer();
    static void FreeTransfer();

    virtual int Function(int cfd);
    ~Transfer();
private :
    Transfer();
    static std::mutex m_TransferLock;
    static Transfer * m_pTransfer;

    int DataBase();
private:
    char *m_pID;
    char *m_pPasswd;
    char *m_pMoney;
    char *m_pOtherId;
};

#endif // TRANSFER_H
