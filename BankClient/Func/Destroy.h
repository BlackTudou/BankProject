#ifndef DESTROY_H
#define DESTROY_H

#include <mutex>
#include <string>

using namespace std;

class destroy_ask
{
public :
    void Init(char *id, char *passwd);
    void Func(char **outstr);

private :
    int m_iIdLen;

    string m_sId;
    string m_sPasswd;

};


class Destroy
{
public :
    int Function(int client_sock, char *id, char *passwd);
    static Destroy * GetDestroy();
    static void FreeDestroy();

private :
    Destroy();
    static std::mutex m_DestroyLock;
    static Destroy * m_pDestroy;

    destroy_ask m_DestroyAsk;
    int m_iResult;
};

#endif // DESTROY_H
