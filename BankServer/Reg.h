#ifndef REG_H
#define REG_H

#include "Abstract.h"
#include <mutex>

class Reg : public AB_Function
{
public:
    static Reg * GetReg();
    static void FreeReg();
    virtual int Function(int cfd);
private:
    Reg(){}
    static Reg *m_pReg;
    static std::mutex m_RegLock;

    char *m_pName;
    char *m_pPasswd;
    int DataBase();
};

#endif // REG_H
