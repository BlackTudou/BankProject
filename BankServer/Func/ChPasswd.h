#ifndef CHPASSWD_H
#define CHPASSWD_H

#include "Abstract.h"
#include <mutex>

class ChPasswd : public AB_Function
{
public :
    static ChPasswd * GetChPasswd();
    static void FreeChPasswd();

    virtual int Function(int cfd);
    ~ChPasswd();
private :
    ChPasswd();
    static std::mutex m_ChPasswdLock;
    static ChPasswd * m_pChPasswd;

    char *m_pID;
    char *m_pPasswd;
    char *m_pNewpasswd;

    int DataBase();
};

#endif // CHPASSWD_H
