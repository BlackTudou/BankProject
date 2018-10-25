#ifndef CHPASSWDDIALOG_H
#define CHPASSWDDIALOG_H

#include <QDialog>
#include "Func/Chpasswd.h"

namespace Ui {
class ChpasswdDialog;
}

class ChpasswdDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChpasswdDialog(QWidget *parent = 0, int client_sock = 0);
    ~ChpasswdDialog();

public slots:
    void Confirm();   //确认
    void Cancel();    //取消

private:
    Ui::ChpasswdDialog *ui;
    int m_iClientSfd; //连接服务器的套接字
    Chpasswd * m_pChpasswd;  //修改密码操作

private:
    void Result(int action);
    int Warning();
};

#endif // CHPASSWDDIALOG_H
