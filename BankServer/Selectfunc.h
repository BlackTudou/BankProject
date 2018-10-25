#ifndef SELECTFUNC_H
#define SELECTFUNC_H

#include "Abstract.h"
#include <mutex>

class Selectfunc
{
public:
    static Selectfunc *GetSel();
    static void FreeSel();

    int Func(char *action, int cfd);
private:
    Selectfunc();

    static Selectfunc *m_pSel;
    static std::mutex m_SelLock;

    AB_Factory *m_pFact;
};


#endif // SELECTFUNC_H
