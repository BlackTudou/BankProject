#ifndef WITHDRAWDIALOG_H
#define WITHDRAWDIALOG_H

#include <QDialog>
#include "Func/Withdraw.h"

namespace Ui {
class WithdrawDialog;
}

class WithdrawDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WithdrawDialog(QWidget *parent = 0, int client_sock = 0);
    ~WithdrawDialog();

public slots:
    void Confirm();
    void Cancel();

private:
    Ui::WithdrawDialog *ui;

    int m_iClientSfd; //连接服务器的套接字
    Withdraw * m_pWithdraw;  //取款操作
private:
    void Result(int action);
    int Warning();
};

#endif // WITHDRAWDIALOG_H
