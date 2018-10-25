#ifndef HISTORY_H
#define HISTORY_H

#include <mutex>
#include <string>

using namespace std;

class history_ask
{
public :
    void Init(char *id, char *passwd, char *str1,char *str2);
    void Func(char **outstr);

private:
    int m_iIdLen;
    int m_iStr1Len;
    int m_iStr2Len;

    string m_sID;
    string m_sPasswd;
    string m_sStr1;
    string m_sStr2;
};

class History
{
public :
    int Function(int client_sock, char *id,char *passwd, char *str1, char *str2);
    static History * GetHistory();
    static void FreeHistory();

    string GetInformation();
private :
    History(){}
    static std::mutex m_HistoryLock;
    static History * m_pHistory;

    history_ask m_cHistoryAsk;
    int m_iResult;

    void HandleInformation(int client_sock);

    string m_sInformation;
};


#endif // HISTORY_H
