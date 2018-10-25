#ifndef INQUIRY_H
#define INQUIRY_H

#include "Abstract.h"
#include <mutex>
#include <string>

using namespace std;

class Inquiry : public AB_Function
{
public :
    static Inquiry * GetInquiry();
    static void FreeInquiry();

    virtual int Function(int cfd);
    ~Inquiry();
private :
    Inquiry();
    static std::mutex m_InquiryLock;
    static Inquiry * m_pInquiry;

    char *m_pID;
    char *m_pPasswd;

    int DataBase();

    string m_sMoney;
};

#endif // INQUIRY_H
