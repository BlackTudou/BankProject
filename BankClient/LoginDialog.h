#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QString>
#include "transmit.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();
    int GetSock();
    QString GetClerkName();

private:
    Ui::LoginDialog *ui;

    Transmit *m_pTransmit; //传输登录注册包
    bool m_bFlag;    //是否连接上服务器标志位
    int m_iClientSfd;      //链接服务器的套结字

    void Result(int action);
    int Warning();

public slots:
    void Login();
    void Reg();
};

#endif // LOGINDIALOG_H
