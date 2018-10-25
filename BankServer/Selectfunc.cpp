#include <stdio.h>

#include "Selectfunc.h"
#include "DynBase.h"

Selectfunc *Selectfunc::m_pSel = NULL;
std::mutex Selectfunc::m_SelLock;

Selectfunc::Selectfunc(){}

Selectfunc *Selectfunc::GetSel()
{
    std::unique_lock<std::mutex> lock(m_SelLock);
    if(m_pSel == NULL)
    {
        m_pSel = new Selectfunc();
    }
    return m_pSel;
}

void Selectfunc::FreeSel()
{
    std::unique_lock<std::mutex> lock(m_SelLock);

    if(m_pSel != NULL)
    {
        delete m_pSel;
        m_pSel = NULL;
    }
}

int Selectfunc::Func(char *action, int cfd)
{
    printf("action=%s\n",action);
    m_pFact =reinterpret_cast<AB_Factory *>(DynObjectFactory::CreateObject(action));
    //printf("SelectFunc::func2\n");

    return m_pFact->CreateFunc()->Function(cfd);

}
