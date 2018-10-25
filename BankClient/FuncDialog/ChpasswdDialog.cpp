#include "ChpasswdDialog.h"
#include "ui_ChpasswdDialog.h"
#include "mark.h"
#include <QMessageBox>

ChpasswdDialog::ChpasswdDialog(QWidget *parent, int client_sock) :
    QDialog(parent),
    ui(new Ui::ChpasswdDialog),m_iClientSfd(client_sock)
{
    ui->setupUi(this);

    this->setWindowTitle("ChpasswdDialog");

    connect(ui->confirm,SIGNAL(clicked(bool)),this,SLOT(Confirm()));
    connect(ui->cancel,SIGNAL(clicked(bool)),this,SLOT(Cancel()));
}

ChpasswdDialog::~ChpasswdDialog()
{
    delete ui;
}

void ChpasswdDialog::Confirm()
{
    if(Warning())
    {
        return ;
    }

    QByteArray id = ui->id->text().toLatin1();
    QByteArray passwd = ui->passwd->text().toLatin1();
    QByteArray newpasswd = ui->newpd->text().toLatin1();

    m_pChpasswd = Chpasswd::GetChpasswd();
    this->Result(m_pChpasswd->Function(m_iClientSfd,id.data(),passwd.data(),newpasswd.data()));
}

void ChpasswdDialog::Cancel()
{
    reject();
}

void ChpasswdDialog::Result(int action)
{
    switch(action)
    {
        case NO_ACCOUNT: //无帐号
        {
            QMessageBox::critical(this,"error","No Account !!");
        }break;


        case PASSWD_ERROR: //密码错误
        {
            QMessageBox::critical(this,"error","PassWord ERROR !!");
        }break;

        case CHPW_SUCCESS: //修改密码成功
        {
            accept();
            QMessageBox::about(this,"success","Change Passwd successfully");
        }break;

    }
}


int ChpasswdDialog::Warning()
{
    if(ui->id->text().length() == 0)  //无ID
    {
        QMessageBox::warning(this,"warning","Please input ID !");
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

    if(ui->newpd->text().length() < 6 )   //新密码过短
    {
        QMessageBox::warning(this,"warning","new passwd is too short");
        return 1;
    }

    if(ui->newpd->text().length() > 8 )   //新密码过长
    {
        QMessageBox::warning(this,"warning","new passwd is too long");
        return 1;
    }

    if(QString::compare(ui->newpd->text(),ui->newpdagain->text()))
    {
        QMessageBox::warning(this,"warning","the two passwords are not the same");
        return 1;
    }

    return 0;
}
