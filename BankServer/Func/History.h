#ifndef HISTORY_H
#define HISTORY_H

#include "Abstract.h"
#include <mutex>
#include <string>
#include <vector>
using namespace std;

class History : public AB_Function
{
public :
    static History * GetHistory();
    static void FreeHistory();

    virtual int Function(int cfd);
    ~History();
private :
    History();
    static std::mutex m_HistoryLock;
    static History * m_pHistory;

    char *m_pId;
    char *m_pPasswd;
    char *m_pStr1;
    char *m_pStr2;

    int DataBase();

    vector<string> m_vRow;
    string m_sFieldsName;
    vector<vector<string>> m_vData;
};


#endif // HISTORY_H
