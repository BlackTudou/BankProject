#include "mainwindow.h"
#include <QApplication>
#include <QtWidgets>
#include "LoginDialog.h"
#include "sock.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    //MainWindow w;
    //w.show();

    //Sock *pSock = Sock::GetInstance();
    //pSock->my_connect();

    LoginDialog logindia;
    logindia.setWindowTitle("LoginDialog");
    if(logindia.exec() == QDialog::Accepted)
    {
        MainWindow w(0,logindia.GetSock());
        w.show();
        return app.exec();
    }
    else
    {
        return 0;
    }
}
