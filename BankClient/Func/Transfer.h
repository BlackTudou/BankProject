#ifndef TRANSFER_H
#define TRANSFER_H

#include <string>
#include <mutex>

using namespace std;

class transfer_ask
{
public:
    void Init(char *id, char *passwd, char *money, char *other_id);
    void Func(char **outstr);

private:
    int m_iMoneyLen;
    int m_iIdLen;
    int m_iOtherIdLen;

    string m_sID;
    string m_sPasswd;
    string m_sMoney;
    string m_sOtherId;
};

class Transfer
{
public:
    static Transfer * GetTransfer();
    static void FreeTransfer();

    int Function(int client_sock, char *id, char *passwd, char *money,char *other_id);

private:
    Transfer(){}
    static Transfer *m_pTransfer;
    static std::mutex m_TransferLock;

    transfer_ask m_cTransferAsk;   //发送

    int m_iResult;  //接收
};


#endif // TRANSFER_H
