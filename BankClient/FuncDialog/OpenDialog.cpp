#include "OpenDialog.h"
#include "ui_OpenDialog.h"
#include "Func/Open.h"
#include "mark.h"
#include <QByteArray>
#include <QMessageBox>

OpenDialog::OpenDialog(QWidget *parent,int client_sock) :
    QDialog(parent),
    ui(new Ui::OpenDialog),m_iClientSfd(client_sock)
{
    ui->setupUi(this);
    this->setWindowTitle("OpenDialog");
    //connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(Confirm()));
    connect(ui->cancel,SIGNAL(clicked(bool)),this,SLOT(Cancel()));
    connect(ui->confirm,SIGNAL(clicked(bool)),this,SLOT(Confirm()));
}

OpenDialog::~OpenDialog()
{
    delete ui;
}

void OpenDialog::Confirm()
{
    if(Warning())
    {
        return ;
    }

    QByteArray username = ui->username->text().toLatin1();
    QByteArray passwd = ui->passwd->text().toLatin1();
    QByteArray id = ui->ID->text().toLatin1();
    QByteArray money = ui->money->text().toLatin1();

    m_pOpen = Open::GetOpen();

    this->Result(m_pOpen->Function(m_iClientSfd,username.data(),passwd.data(),id.data(),money.data()));
}

void OpenDialog::Cancel()
{
    reject();
}

int OpenDialog::Warning()
{
   // qDebug() << _username->text() ;
   // qDebug() << _passwd->text() ;
    //qDebug() << _passagin->text();
   // qDebug() << _ID->text() ;
    //qDebug() << _money->text();


    if(ui->username->text().length() < 3)  //用户名过短
    {
        QMessageBox::warning(this,"warning","usename is too short");
        return 1;
    }

    if(ui->username->text().length() > 10) //用户名过长
    {
        QMessageBox::warning(this,"warning","usename is too long");
        return 1;
    }

    if(ui->passwd->text().length() < 6 )   //密码过短
    {
        QMessageBox::warning(this,"warning","passwd is too short");
        return 1;
    }

    if(ui->passwd->text().length() > 8 )   //密码过长
    {
        QMessageBox::warning(this,"warning","passwd is too long");
        return 1;
    }

    //qDebug() <<_passwd->text().compare(_passagin->text());


    if(ui->ID->text().length() < 18 )   //密码长度错误
    {
        QMessageBox::warning(this,"warning","ID is short");
        return 1;
    }

    if(ui->ID->text().length() > 18 )   //密码长度错误
    {
        QMessageBox::warning(this,"warning","ID is long");
        return 1;
    }

    if(!(QString::compare(ui->money->text(),"0"))) //初始金额为0
    {
        QMessageBox::warning(this,"warning","No money");
        return 1;
    }

    if(QString::compare(ui->passwd->text(),ui->passwdagain->text())) //两次密码不一致
    {
        QMessageBox::warning(this,"warning","the two passwords are not the same");
        return 1;
    }

    //身份证全为数字
    QByteArray ID = ui->ID->text().toLatin1();
    char *str = ID.data();
    while(*str != '\0')
    {
        if(!(*str >= '0' && *str <= '9'))
        {
            QMessageBox::warning(this,"warning","usename is not all letters");
            return 1;
        }
        str++;
    }

    //用户名全为字母
    QByteArray name = ui->username->text().toLatin1();
    str = name.data();
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

void OpenDialog::Result(int action)
{
    switch(action)
    {
        case HAS_ACCOUNT: //帐号已注册
        {
            QMessageBox::critical(this,"error","Account already exists");
        }break;

        case REGIST_SUCCESS: //注册成功，显示ID号
        {
            accept();
            char t[30];
            sprintf(t,"Account Success!!\nID:%d",m_pOpen->GetID());
            QMessageBox::about(this,"success",t);
        }break;

    }
}
