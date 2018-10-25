#ifndef HISTORYDIALOG_H
#define HISTORYDIALOG_H

#include <QDialog>
#include "Func/History.h"

namespace Ui {
class HistoryDialog;
}

class HistoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HistoryDialog(QWidget *parent = 0,int client_sock = 0);
    ~HistoryDialog();

public slots:
    void Confirm();
    void Cancel();

private:
    Ui::HistoryDialog *ui;
    int m_iClientSfd; //连接服务器的套接字
    History * m_pHistory;  //查看明细操作

private:
    void Result(int action);
    int Warning();
};

#endif // HISTORYDIALOG_H
