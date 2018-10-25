#ifndef PASS_H
#define PASS_H

#include "Abstract.h"
#include <mutex>

class Pass : public AB_Function
{
public:
    static Pass* GetPass();
    static void FreePass();

    virtual int Function(int cfd);
private:
    Pass();
    static Pass *m_pPass;
    static std::mutex m_PassLock;

    char *m_pName;
    char *m_pPasswd;

    int DataBase();  //数据库处理函数
};

#endif // PASS_H
