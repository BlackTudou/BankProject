#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "FuncDialog/AllDialog.h"
#include <QMessageBox>
#include "FuncDialog/DestroyDialog.h"
#include <QDebug>
#include <QByteArray>

MainWindow::MainWindow(QWidget *parent, int client_cfd) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_iClientCfd(client_cfd)
{
    ui->setupUi(this);

    LoginDialog log;
    QByteArray temp = log.GetClerkName().toLatin1();

    qDebug() << "LoginDia.GetClerkName()::" << temp.data();

    QString mystr(temp.data());

    //QString mystr = QString("职工：%1  姓名：%2  正在服务中....").arg(WLparent->message.work_num).arg(WLparent->message.work_name);
    //ui->clerk->setText(mystr);
    ui->clerk->setText("lpc");

    connect(ui->open,SIGNAL(clicked(bool)),this,SLOT(Open()));
    connect(ui->deposit,SIGNAL(clicked(bool)),this,SLOT(Deposit()));
    connect(ui->withdraw,SIGNAL(clicked(bool)),this,SLOT(Withdraw()));
    connect(ui->transfer,SIGNAL(clicked(bool)),this,SLOT(Transfer()));
    connect(ui->inquiry,SIGNAL(clicked(bool)),this,SLOT(Inquiry()));
    connect(ui->chpasswd,SIGNAL(clicked(bool)),this,SLOT(Chpasswd()));
    connect(ui->history,SIGNAL(clicked(bool)),this,SLOT(History()));
    connect(ui->destroy,SIGNAL(clicked(bool)),this,SLOT(Destroy()));
    connect(ui->exit,SIGNAL(clicked(bool)),this,SLOT(Exit()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::Open() // 开户
{
    OpenDialog OpenDia(0,m_iClientCfd);
    OpenDia.exec();

    return 0;
}

int MainWindow::Deposit()   //存款
{
    DepositDialog DepositDia(0, m_iClientCfd);
    DepositDia.exec();

    return 0;
}

int MainWindow::Withdraw()   //取款
{
    WithdrawDialog WithdrawDia(0, m_iClientCfd);
    WithdrawDia.exec();

    return 0;
}

int MainWindow::Transfer() //转账
{
    TransferDialog TransferDia(0, m_iClientCfd);
    TransferDia.exec();

    return 0;
}

int MainWindow::Inquiry() //余额查询
{
    InquiryDialog InquiryDia(0, m_iClientCfd);
    InquiryDia.exec();

    return 0;
}

int MainWindow::Chpasswd() //修改密码
{
    ChpasswdDialog ChpasswdDia(0, m_iClientCfd);
    ChpasswdDia.exec();

    return 0;
}

int MainWindow::Destroy() //销户
{
    DestroyDialog DestroyDia(0, m_iClientCfd);
    DestroyDia.exec();

    return 0;
}

int MainWindow::History() //账户明细
{
    HistoryDialog HistoryDia(0, m_iClientCfd);
    HistoryDia.exec();

    return 0;
}

int MainWindow::Exit()  //退出
{
    int ret = QMessageBox::question(this, "Exit","Are you sure you want to exit?",
                                    QMessageBox::No|QMessageBox::Yes, QMessageBox::Yes);
    if(ret == QMessageBox::Yes)
    {
        close();
    }

    return 0;
}
