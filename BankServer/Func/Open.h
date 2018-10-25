#ifndef OPEN_H
#define OPEN_H

#include "Abstract.h"
#include <mutex>

class Open : public AB_Function
{
public :
    static Open * GetOpen();
    static void FreeOpen();

    virtual int Function(int cfd);
    ~Open();
private :
    Open();
    static std::mutex m_OpenLock;
    static Open * m_pOpen;

    char *m_pName;
    char *m_pPasswd;
    char *m_pMoney;
    char *m_pID;

    int DataBase();

    //char m_aID[7];    //返回的ID号码
    int m_iID;
};

#endif // OPEN_H
