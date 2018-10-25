#ifndef OPENDIALOG_H
#define OPENDIALOG_H

#include <QDialog>
#include "Func/Open.h"

namespace Ui {
class OpenDialog;
}

class OpenDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OpenDialog(QWidget *parent = 0,int client_sock = 0);
    ~OpenDialog();

public slots:
    void Confirm();
    void Cancel();

private:
    Ui::OpenDialog *ui;

    int m_iClientSfd; // 连接服务器的套接字
    Open *m_pOpen;
private:
    void Result(int action);
    int Warning();
};

#endif // OPENDIALOG_H
