#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "LoginDialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, int client_sfd = 0);
    ~MainWindow();

public slots:
    int Open();     // 开户
    int Deposit();  //存款
    int Withdraw();  //取款
    int Transfer();  //转账
    int Inquiry();   //余额查询
    int Chpasswd();  //修改密码
    int History();  //账户明细
    int Destroy();    //销户
    int Exit();  //退出

private:
    Ui::MainWindow *ui;

    int m_iClientCfd;

    //LoginDialog LoginDia;
};

#endif // MAINWINDOW_H
