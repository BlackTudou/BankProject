#ifndef DEPOSITDIALOG_H
#define DEPOSITDIALOG_H

#include <QDialog>
#include "Func/Deposit.h"

namespace Ui {
class DepositDialog;
}

class DepositDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DepositDialog(QWidget *parent = 0, int client_sock = 0);
    ~DepositDialog();

public slots:
    void Confirm();
    void Cancel();

private:
    Ui::DepositDialog *ui;

    int m_iClientSfd; //连接服务器的套接字
    Deposit * m_pDeposit;  //存款操作
private:
    void Result(int action);
    int Warning();
};

#endif // DEPOSITDIALOG_H
