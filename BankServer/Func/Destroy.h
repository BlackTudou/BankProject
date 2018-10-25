#ifndef DESTROY_H
#define DESTROY_H

#include "Abstract.h"
#include <mutex>

class Destroy : public AB_Function
{
public :    
    static Destroy * GetDestroy();
    static void FreeDestroy();

    virtual int Function(int cfd);
    ~Destroy();

private :
    Destroy();
    static std::mutex m_DestroyLock;
    static Destroy * m_pDestroy;

    char *m_pId;
    char *m_pPasswd;

    int DataBase();
};


#endif // DESTROY_H
