#include <QMessageBox>
#include <QDebug>

#include "LoginDialog.h"
#include "ui_LoginDialog.h"
#include "mainwindow.h"
#include "RegDialog.h"
#include "sock.h"
#include "transmit.h"
#include "mark.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog),m_bFlag(false)
{
    ui->setupUi(this);
    ui->password->setPlaceholderText("密码");
    ui->username->setPlaceholderText("用户名");
    connect(ui->log,SIGNAL(clicked(bool)),this,SLOT(Login()));
    connect(ui->reg,SIGNAL(clicked(bool)),this,SLOT(Reg()));
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

int LoginDialog::Warning()
{
    if(ui->username->text().length() < 3)
    {
        QMessageBox::warning(this,"warning","username is too short");
        return 1;
    }

    if(ui->username->text().length() > 10)
    {
        QMessageBox::warning(this,"warning","username is too long");
        return 1;
    }

    if(ui->password->text().length() < 6 )
    {
        QMessageBox::warning(this,"warning","passwd is too short");
        return 1;
    }

    if(ui->password->text().length() > 8 )
    {
        QMessageBox::warning(this,"warning","passwd is too long");
        return 1;
    }

    QByteArray name = ui->username->text().toLatin1();
    char *str = name.data();
    while(*str != '\0')
    {
        if(!((*str >= 'a' && *str <= 'z')||(*str >= 'A' && *str <='Z')))
        {
            QMessageBox::warning(this,"warning","usename is not all letters");
            return 1;
        }
        str++;
    }

    return 0;
}

void LoginDialog::Result(int action)
{
    switch(action)
    {
        case LOG_SUCCESS: //登录成功
        {
            accept();
        }break;

        case PASSWD_ERROR: //密码错误
        {
            QMessageBox::critical(this,"error","Passwd error!");
        }break;

        case NO_ACCOUNT: //帐号未注册
        {
            QMessageBox::critical(this,"error","Account is not registed");
        }break;

        case HAS_ACCOUNT: //帐号已注册
        {
            QMessageBox::critical(this,"error","Account already exists");
        }break;

        case REGIST_SUCCESS: //注册成功，显示ID号
        {
            QMessageBox::about(this,"success","Register Success!!");
        }break;

        case DOUBLE_LOG: //请勿重复登录
        {
            QMessageBox::critical(this,"error","Don't log again");
        }break;
    }
}

void LoginDialog::Reg()
{
    RegDialog reg;
    if(reg.exec() == QDialog::Accepted)//要想使reg对话框变成模态对话框,调用exec
    {
        qDebug() << "reg confirm";
        if(! m_bFlag) //如果未连接
        {
            Sock *my_client = Sock::GetSock();
            m_iClientSfd = my_client->my_connect();
            m_bFlag = true;
        }

        QByteArray name = reg.GetUserName().toLatin1();
        QByteArray passwd = reg.GetPassWd().toLatin1();

        m_pTransmit = new Transmit(m_iClientSfd,name.data(),passwd.data());
        this->Result(m_pTransmit->Action(0)); //0:注册
        delete m_pTransmit;
    }
}

void LoginDialog::Login()
{
    if(Warning())
    {
        return ;
    }
    if(! m_bFlag) //如果未连接
    {
        Sock *my_client = Sock::GetSock();
        m_iClientSfd = my_client->my_connect();
        m_bFlag = true;
    }
    QByteArray username = ui->username->text().toLatin1();
    QByteArray password = ui->password->text().toLatin1();
    m_pTransmit = new Transmit(m_iClientSfd,username.data(),password.data());

    this->Result(m_pTransmit->Action(1)); //1:登录

    delete m_pTransmit;
}

int LoginDialog::GetSock()
{
    return m_iClientSfd;
}

QString LoginDialog::GetClerkName()
{
    qDebug() << "userlineEdit username:" << ui->username->text();

    return ui->username->text();
}

