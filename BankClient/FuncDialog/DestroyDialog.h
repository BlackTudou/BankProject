#ifndef DESTROYDIALOG_H
#define DESTROYDIALOG_H

#include <QDialog>
#include "Func/Destroy.h"

namespace Ui {
class DestroyDialog;
}

class DestroyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DestroyDialog(QWidget *parent = 0, int client_sock = 0);
    ~DestroyDialog();

public slots:
    void Confirm();
    void Cancel();

private:
    Ui::DestroyDialog *ui;

    int m_iClientSfd; //连接服务器的套接字
    Destroy * m_pDestroy;  //销户操作

private:
    void Result(int action);
    int Warning();
};

#endif // DESTROYDIALOG_H
