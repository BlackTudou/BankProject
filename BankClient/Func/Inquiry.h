#ifndef INQUIRY_H
#define INQUIRY_H

#include <string>
#include <mutex>

using namespace std;

class inquiry_ask
{
public:
    void Init(char *id, char *passwd);
    void Func(char **outstr);

private:
    int m_iIdLen;

    string m_sID;
    string m_sPasswd;
};

class Inquiry
{
public:
    static Inquiry * GetInquiry();
    static void FreeInquiry();

    int Function(int client_sock, char *id, char *passwd);

    char *GetMoney();
    ~Inquiry();

private:
    Inquiry();
    static Inquiry *m_pInquiry;
    static std::mutex m_InquiryLock;

    inquiry_ask m_cInquiryAsk;   //发送

    int m_iResult;  //接收
    char *m_pMoney;
};

#endif // INQUIRY_H
