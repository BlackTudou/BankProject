#ifndef TRANSFERDIALOG_H
#define TRANSFERDIALOG_H

#include <QDialog>
#include "Func/Transfer.h"

namespace Ui {
class TransferDialog;
}

class TransferDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TransferDialog(QWidget *parent = 0,int client_sock = 0);
    ~TransferDialog();

public slots:
    void Confirm();
    void Cancel();

private:
    Ui::TransferDialog *ui;

    int m_iClientSfd; //连接服务器的套接字
    Transfer * m_pTransfer;  //转账操作

private:
    void Result(int action);
    int Warning();
};

#endif // TRANSFERDIALOG_H
